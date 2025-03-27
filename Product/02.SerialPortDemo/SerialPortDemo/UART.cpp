#include "pch.h"
#include "UART.h"
#include <process.h>
#include <iostream>

using namespace std;

/*********************************************************************************************************
* ��������: CUART
* ��������:	���캯��
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
CUART::CUART()
	: mCommHandle(INVALID_HANDLE_VALUE),
	mRecvMsgCallFunc(NULL),
	mRegisterPoint(NULL),
	mRecvMode(Call_Func_Mode),
	mRecvMsgEvent(NULL),
	mRecvThread(NULL),
	mRunThread(FALSE)
{
	ZeroMemory(&mLSend, sizeof(OVERLAPPED));
	ZeroMemory(&mLRecv, sizeof(OVERLAPPED));
}

/*********************************************************************************************************
* ��������: CUART
* ��������:	��������
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
CUART::~CUART()
{
	ClosePort(); // �رմ���

	if(mRecvMsgEvent != NULL)
	{
		CloseHandle(mRecvMsgEvent); // �رս�����Ϣ֪ͨ�¼��߳̾��
		mRecvMsgEvent = NULL;
	}

	mRunThread = FALSE; // �����������б�־ΪFALSE

	if(mRecvThread != NULL)
	{
		WaitForSingleObject(mRecvThread, INFINITE); // �ȴ������̺߳����˳�
		CloseHandle(mRecvThread);                   // �رս����߳̾��
		mRecvThread = NULL;
	}

	mRecvMsgCallFunc = NULL; // ���ջص�����ָ��ΪNULL
	mRegisterPoint = NULL;   // ע��ģ��ָ��ΪNULL
}

/*********************************************************************************************************
* ��������: OpenPort
* ��������:	��һ���˿ڣ�����ʼ��
* �������:	iPort-�˿ںţ�iRecvMode-����ģʽ
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2021��01��22��
* ע    ��: ������һ�������̣߳����ǲ�������ִ�У����ǹ���mRecvMsgEvent�¼��ϣ�
*           ����ȵ���ConfigPort�����ִ�У�
*********************************************************************************************************/
int CUART::OpenPort(int iPort, int iRecvMode)
{
	CString sPort;

	if (iPort < 10)
	{
		sPort.Format(_T("COM%d"), iPort);
	}
	else // ɨ�贮�ںų���10�Ĵ���
	{
		sPort.Format(_T("\\\\.\\COM%d"), iPort);
	}

	//�������ھ�����ص���ʽ
	mCommHandle = CreateFile(sPort,    // ��������
		GENERIC_WRITE | GENERIC_READ,  // ʹ�ö�/д��ʽ
		0,                             // �����������豸
		NULL,                          // ��ȫ��������
		OPEN_EXISTING,                 // ������ʽ��ֻ�����д���
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // ʹ���ص���ʽ
		NULL                           // ������ʱ��ָ��ģ���ļ����
	);

	if(mCommHandle == INVALID_HANDLE_VALUE) // ���ھ����Ч�ľ��ֵ
	{
		return ERR_DK_FAILURE;
	}

	// Ϊ�ص�д�����¼������ֹ����ã���ʼ��Ϊ���źŵ�
	ZeroMemory(&mLSend, sizeof(OVERLAPPED)); // ��ָ�����ڴ����㣬һ����ʹ�ýṹǰ����
	mLSend.hEvent = CreateEvent(NULL,        // �þ�����ܱ��̳�
		TRUE,                                // �˹�����
		FALSE,                               // �¼���ʼ��״̬Ϊ���ź�״̬
		NULL                                 // ָ���¼���
	);
	if (mLSend.hEvent == INVALID_HANDLE_VALUE) // mLSend��hEventΪ��Ч�ľ��ֵ
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// Ϊ�ص��������¼������ֹ����ã���ʼ��Ϊ���źŵ�
	ZeroMemory(&mLRecv, sizeof(OVERLAPPED)); // ��ָ�����ڴ����㣬һ����ʹ�ýṹǰ����
	mLRecv.hEvent = CreateEvent(NULL,        // �þ�����ܱ��̳�
		TRUE,                                // �˹�����
		FALSE,                               // �¼���ʼ��״̬Ϊ���ź�״̬
		NULL                                 // ָ���¼���
	);
	if(mLRecv.hEvent == INVALID_HANDLE_VALUE) // mLRecv��hEventΪ��Ч�ľ��ֵ
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// �����¼�����
	// SetCommMask-ָ������������Ӧ���¼�ΪEV_RXCHAR,�����յ�һ���ַ�����������ջ�����
	if(!SetCommMask(mCommHandle, EV_RXCHAR))
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// ���ô����շ�������
	if(!SetupComm(mCommHandle, MAXRECVBUF, MAXSENDBUF)) // ��ʼ�����ڵ�ͨ�Ų�����0-ʧ�ܣ�����-�ɹ�
	{
		// �رմ���
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// ��մ��ڻ�����
	if(!PurgeComm(mCommHandle, PURGE_RXABORT | PURGE_TXABORT |
		PURGE_RXCLEAR | PURGE_TXCLEAR))
	{
		// �رմ���
		ClosePort();
		return ERR_DK_FAILURE;
	}

	COMMTIMEOUTS commtimeouts;

	// �Ѽ���ʱ��Ϊ��󣬰��ܳ�ʱ��Ϊ0������ReadFile�������ز���ɲ���
	commtimeouts.ReadIntervalTimeout = MAXDWORD; // ������ʱ����Ϊ���
	commtimeouts.ReadTotalTimeoutMultiplier = 0; // ��ʱ��ϵ��Ϊ0
	commtimeouts.ReadTotalTimeoutConstant = 0;   // ��ʱ�䳣��Ϊ0

	// ����д��ʱ��ָ��WriteComm��Ա�����е�GetOverlappedResult�����ĵȴ�ʱ��
	commtimeouts.WriteTotalTimeoutMultiplier = 50; // дʱ��ϵ��Ϊ50
	commtimeouts.WriteTotalTimeoutConstant = 200;  // дʱ�䳣��Ϊ200
	SetCommTimeouts(mCommHandle, &commtimeouts);   // ���ô����豸��дʱ�ĳ�ʱ����

	// ���ý���ģʽ
	mRecvMode = iRecvMode;

	switch(mRecvMode)
	{
	case Call_Func_Mode:
		mRecvMsgEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // ����������Ϣʱ����,�Զ�����
		if(mRecvMsgEvent == INVALID_HANDLE_VALUE)
		{
			ClosePort();
			return ERR_DK_FAILURE;
		}

		mRunThread = TRUE; // �����������б�־ΪTRUE

		mRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, this, 0, NULL); //���������̣߳����ؽ����߳̾��
		if(mRecvThread == NULL)
		{
			ClosePort();
			return ERR_DK_FAILURE;
		}
		break;

	case Event_Mode:
		break;
	default:
		break;
	}

	return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: ConfigPort
* ��������:	���ö˿ڣ�����������߳�
* �������:	���Ǹ�����ComboBox����ѡ���˳���ţ���Ŵ�0��ʼ
			baudRate-�����ʣ� parity-��żУ�飬dataBits-�˿�����λ
*           stopBits-�˿�ֹͣλ����fc-�˿ڿ�������
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int CUART::ConfigPort(int baudRate, int parity, int dataBits,
	int stopBits, EnumFlowControl fc)
{
	// ��ö˿���ǰ��״̬
	DCB dcb; // ����һ��dcb�ṹ��Ŵ���ͨ�Ų���
	dcb.DCBlength = sizeof(DCB); // DCB�ṹ�Ĵ�С
	if(!GetCommState(mCommHandle, &dcb)) // �򿪴��ں󣬻�ȡ���ڵĳ�ʼ������
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// ͨ���޸�DCB�ṹ�������ô��ڵĸ������
	// ���ö�����ģʽ
	dcb.fBinary = TRUE;

	// ���ò�����
	switch(baudRate)
	{
	case 0: // ������4800
		dcb.BaudRate = 4800;
		break;
	case 1: // ������9600
		dcb.BaudRate = 9600;
		break;
	case 2: // ������14400
		dcb.BaudRate = 14400;
		break;
	case 3: // ������19200
		dcb.BaudRate = 19200;
		break;
	case 4: // ������38400
		dcb.BaudRate = 38400;
		break;
	case 5: // ������57600
		dcb.BaudRate = 57600;
		break;
	case 6: // ������115200
		dcb.BaudRate = 115200;
		break;
	default:
		break;
	}

	// ����������żУ��
	dcb.fParity = TRUE;

	// ������żУ��
	switch(parity)
	{
	case 0: // ��У��
		dcb.Parity = NOPARITY;
		break;
	case 1: // ����У��
		dcb.Parity = ODDPARITY;
		break;
	case 2: // żУ��
		dcb.Parity = EVENPARITY;
		break;
	case 3: // ��־У��
		dcb.Parity = MARKPARITY;
		break;
	case 4: // �ո�У��
		dcb.Parity = SPACEPARITY;
		break;
	default:
		break;
	}

	// ���ö˿ڵ�ǰʹ�õ�����λ
	// dcb.ByteSize = byDataBits;

	switch(dataBits)
	{
	case 0:
		dcb.ByteSize = 5;
		break;
	case 1:
		dcb.ByteSize = 6;
		break;
	case 2:
		dcb.ByteSize = 7;
		break;
	case 3:
		dcb.ByteSize = 8;
		break;
	default:
		break;
	}

	// ���ö˿ڵ�ǰʹ�õ�ֹͣ����λ
	switch(stopBits)
	{
	case 0: // 1λ
		dcb.StopBits = ONESTOPBIT;
		break;
	case 1: // 1.5λ
		dcb.StopBits = ONE5STOPBITS;
		break;
	case 2: // 2λ
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
		break;
	}

	// �趨��DTR�ź����Ƿ���У����ΪTRUE,��ôDSRΪOFF�����յ��κ��ֽڽ�������
	dcb.fDsrSensitivity = FALSE;
	switch(fc)
	{
	case No_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Cts_Rts_FlowControl:
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Cts_Dtr_FlowControl:
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Dsr_Rts_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Dsr_Dtr_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = TRUE;
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
		break;
	}
	case Xon_Xoff_FlowControl:
	{
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fOutX = TRUE;
		dcb.fInX = TRUE;
		dcb.XonChar = 0x11;
		dcb.XoffChar = 0x13;
		dcb.XoffLim = 100;
		dcb.XonLim = 100;
		break;
	}
	default:
		break;
	}

	// �������õ�ǰ�˿�״̬
	if(!SetCommState(mCommHandle, &dcb))
	{
		ClosePort();
		return ERR_DK_FAILURE;
	}

	// ��������¼�
	if(Call_Func_Mode == mRecvMode)
	{
		SetEvent(mRecvMsgEvent); // ��Event��״̬����Ϊ���ź�״̬
	}
	return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: RegisterCallFunc
* ��������:	�����ش���ģ��ע�ắ����ע����û����ڴ���������ݵ��̺߳���
* �������:	callFunc-ע�ắ��ָ�룬pThis-ע�����ָ��
* �������:	void
* �� �� ֵ: void
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
void CUART::RegisterCallFunc(CALLFUNC callFunc, void* pThis)
{
	if (NULL == callFunc || NULL == pThis)
	{
		return;
	}

	mRecvMsgCallFunc = callFunc; // ���ջص�����ָ��Ϊ�β�callFunc
	mRegisterPoint = pThis;      // ע��ģ�庯����λ��Ϊ�β�pThis
}

/*********************************************************************************************************
* ��������: RecvThread
* ��������:	���������͵��̵߳���ִ�еĺ�����ַ�����ƾͱ�ʾ��ַ
* �������:	pParam-��ǰ����ָ��,
* �������:	void
* �� �� ֵ: DK_SUCCESS
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
unsigned _stdcall CUART::RecvThread(void* param)
{
	CUART* myComm = NULL;

	if (NULL != param)
	{
		myComm = (CUART*)param;
		(void)myComm->RecvTaskProc();
	}
	return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: RecvTaskProc
* ��������:	�����¼���Ϣ����
* �������:	void
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int CUART::RecvTaskProc()
{
	COMSTAT ComStat; // ͨѶ�豸�����ṹ��
	DWORD dwErrorFlags = 0;
	int nError;
	HANDLE hMyComm = NULL;

	// ��ȡ��̨������Ĳ������
	WaitForSingleObject(mRecvMsgEvent, INFINITE); // ʹ���߳���Ը����ȴ�״̬��ֱ����Ϊ��֪ͨ״̬
	CloseHandle(mRecvMsgEvent);
	mRecvMsgEvent = NULL;

	while(mRunThread) // �����������б�־ΪTRUE
	{
		if(mCommHandle == INVALID_HANDLE_VALUE)
		{
			break;
		}

		ClearCommError(mCommHandle, &dwErrorFlags, &ComStat); // ��������־

		if(ComStat.cbInQue)
		{
			char szRecvBuf[MAXRECVBUF + 1] = { 0 };

			int len = 0;
			len = RecvData((void*)szRecvBuf, MAXRECVBUF); // ���ڽ������ݣ�����szRecvBuf��

			if(len > 0)
			{
				mRecvMsgCallFunc(szRecvBuf, len, mRegisterPoint); // �ص�����
			}
			continue;
		}

		nError = WaitComm();
		if(ERR_DK_FAILURE == nError)
		{
			break;
		}
	}
	return ERR_DK_FAILURE;
}

/*********************************************************************************************************
* ��������: SendData
* ��������:	��������
* �������:	pData-���͵�����ָ�룬dwLen-�������ݳ���
* �������:	void
* �� �� ֵ: �ɹ����ط��ͳɹ����ֽ���, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int CUART::SendData(LPCVOID pData, DWORD dwLen)
{
	// �ж���ڲ���
	if (pData == NULL)
	{
		return ERR_DK_FAILURE;
	}

	BOOL bSuccess;
	DWORD dwErrorFlags;
	COMSTAT Comstat;
	DWORD dwSendCount = 0;
	DWORD dwError;

	ClearCommError(mCommHandle, &dwErrorFlags, &Comstat);

	// ��������
	bSuccess = WriteFile(mCommHandle, pData, dwLen, &dwSendCount, &mLSend);
	/*WriteFile������
	mCommHandle : д���ļ��ľ��
	pData   : д���ļ������ݻ�����ָ��
	dwLen   : д����ֽ���
	dwSendCount : �ѷ��͵��ֽ���
	mLSend: �ص���ʽʹ�õĽṹ��
	*/

	if (!bSuccess)
	{
		// ���IO������
		dwError = GetLastError();
		if (ERROR_IO_PENDING == dwError) // ��ʾ������δ�������
		{
			// һֱ�ȴ����ͳɹ�
			bSuccess = GetOverlappedResult(mCommHandle, &mLSend, &dwSendCount, TRUE);
			if (!bSuccess)
			{
				return ERR_DK_FAILURE;
			}
		}
		else
		{
			return ERR_DK_FAILURE;
		}
	}
	return dwSendCount;
}

/*********************************************************************************************************
* ��������: RecvData
* ��������:	��������
* �������:	pData - ���յ�����ָ�룬dwLen - �������ݳ���
* �������:	void
* �� �� ֵ: �ɹ����ؽ��յ��ֽ���, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int CUART::RecvData(LPVOID  pData, DWORD dwLen)
{
	// �ж���ڲ���
	if (pData == NULL)
	{
		return ERR_DK_FAILURE;
	}

	BOOL bSuccess = FALSE;
	DWORD dwErrorFlags;
	COMSTAT Comstat;
	DWORD dwRecvCount = 0;
	DWORD dwError;

	ClearCommError(mCommHandle, &dwErrorFlags, &Comstat);
	dwRecvCount = min(dwLen, Comstat.cbInQue); // ȡʵ�ʽ��յ����ݳ�������ջ��峤��֮�����Сֵ

	// ���¼�����
	bSuccess = ReadFile(mCommHandle, pData, dwLen, &dwRecvCount, &mLRecv);

	if (!bSuccess)
	{
		// ���IO������
		dwError = GetLastError();
		if (ERROR_IO_PENDING == dwError) // ��ʾ������δ�������
		{
			// һֱ�ȴ����ճɹ�
			bSuccess = GetOverlappedResult(mCommHandle, &mLRecv, &dwRecvCount, TRUE);
			if (!bSuccess)
			{
				return ERR_DK_FAILURE;
			}
		}
		else
		{
			return ERR_DK_FAILURE;
		}
	}
	return dwRecvCount;
}

/*********************************************************************************************************
* ��������: WaitComm
* ��������:	�ȴ������¼�
* �������:	void
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
int CUART::WaitComm()
{
	OVERLAPPED os;
	DWORD dwMark = 0;
	DWORD dwTrans = 0;

	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if(os.hEvent == NULL)
	{
		return ERR_DK_FAILURE;
	}

	if(!WaitCommEvent(mCommHandle, &dwMark, &os)) // �ص�����
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			// ���޵ȴ��ص��������
			GetOverlappedResult(mCommHandle, &os, &dwTrans, TRUE);
		}
		else
		{
			CloseHandle(os.hEvent);
			return ERR_DK_FAILURE;
		}
	}

	CloseHandle(os.hEvent);
	return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: ClosePort
* ��������:	�رն˿�,ͬʱ,���ڹرս����������߳̽���
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
void CUART::ClosePort()
{
	BOOL open = FALSE;

	try
	{
		open = IsOpen();
		mRunThread = FALSE;
		if (TRUE == open)
		{
			// �رյ�ǰ�򿪵Ķ˿ھ��
			(void)CloseHandle(mCommHandle);
			mCommHandle = INVALID_HANDLE_VALUE;

			// �رյ�ǰ�¼����
			if(mLSend.hEvent != NULL)
			{
				CloseHandle(mLSend.hEvent);
			}

			if(mLRecv.hEvent != NULL)
			{
				CloseHandle(mLRecv.hEvent);
			}

			ZeroMemory(&mLSend, sizeof(OVERLAPPED));
			ZeroMemory(&mLRecv, sizeof(OVERLAPPED));
		}
	}
	catch (...)
	{
		cout << "close port failed!" << endl;
	}
}

/*********************************************************************************************************
* ��������: IsOpen
* ��������:	����Ƿ��Ѿ���һ���˿�
* �������:	void
* �������:	void
* �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
* ��������: 2021��01��22��
* ע    ��:
*********************************************************************************************************/
BOOL CUART::IsOpen() const
{
	return (INVALID_HANDLE_VALUE != mCommHandle);
}



