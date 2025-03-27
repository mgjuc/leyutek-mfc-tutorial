#include "pch.h"

#include  "Uart.h"
#include  <process.h>   //�õ�_beginthreadex����
#include  <iostream>

using namespace std;

/*********************************************************************************************************
* ��������: CUart
* ��������:	���캯��
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
CUart::CUart()
    : m_hComm(INVALID_HANDLE_VALUE),
    m_RecvMsgCallFunc(NULL),
    m_pRegisterPoint(NULL),
    m_nRecvMode(Call_Func_Mode),
    m_hRecvMsgEvent(NULL),
    m_hRecvThread(NULL),
    m_bRunThread(FALSE)
{
    ZeroMemory(&m_olSend, sizeof(OVERLAPPED));
    ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));

}

/*********************************************************************************************************
* ��������: CUart
* ��������:	��������
* �������:	void
* �������:	void
* �� �� ֵ: void
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
CUart::~CUart()
{
    ClosePort();   //�رմ���

    if (m_hRecvMsgEvent != NULL)
    {
        CloseHandle(m_hRecvMsgEvent);  //�رս�����Ϣ֪ͨ�¼��߳̾��
        m_hRecvMsgEvent = NULL;
    }

    m_bRunThread = FALSE;  //�����������б�־ΪFALSE

    if (m_hRecvThread != NULL)
    {
        WaitForSingleObject(m_hRecvThread, INFINITE);  //�ȴ������̺߳����˳�
        CloseHandle(m_hRecvThread);                    //�رս����߳̾��
        m_hRecvThread = NULL;
    }

    m_RecvMsgCallFunc = NULL;   //���ջص�����ָ��ΪNULL
    m_pRegisterPoint = NULL;   //ע��ģ��ָ��ΪNULL
}

/*********************************************************************************************************
* ��������: OpenPort
* ��������:	��һ���˿ڣ�����ʼ��
* �������:	iPort-�˿ںţ�iRecvMode-����ģʽ
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2017��08��29��
* ע    ��: ������һ�������̣߳����ǲ�������ִ�У����ǹ���m_hRecvMsgEvent�¼��ϣ�
*           ����ȵ���ConfigPort�����ִ�У�
*********************************************************************************************************/
int CUart::OpenPort(int iPort, int iRecvMode)
{
    CString sPort;

    if (iPort < 10)
    {
        sPort.Format(_T("COM%d"), iPort);
    }
    else  //ɨ�贮�ںų���10�Ĵ���
    {
        sPort.Format(_T("\\\\.\\COM%d"), iPort);
    }

    //�������ھ�����ص���ʽ
    m_hComm = CreateFile(sPort,              //��������
        GENERIC_WRITE | GENERIC_READ,  //ʹ�ö�/д��ʽ
        0,                             //����������豸
        NULL,                          //��ȫ��������
        OPEN_EXISTING,                 //������ʽ��ֻ�����д���
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,   //ʹ���ص���ʽ
        NULL                           //������ʱ��ָ��ģ���ļ����
    );

    if (m_hComm == INVALID_HANDLE_VALUE)     //���ھ����Ч�ľ��ֵ
    {
        return ERR_DK_FAILURE;
    }

    //Ϊ�ص�д�����¼������ֹ����ã���ʼ��Ϊ���źŵ�
    ZeroMemory(&m_olSend, sizeof(OVERLAPPED));            //��ָ�����ڴ����㣬һ����ʹ�ýṹǰ����
    m_olSend.hEvent = CreateEvent(NULL,                   //�þ�����ܱ��̳�
        TRUE,                   //�˹�����
        FALSE,                  //�¼���ʼ��״̬Ϊ���ź�״̬
        NULL                    //ָ���¼���
    );
    if (m_olSend.hEvent == INVALID_HANDLE_VALUE)           //m_olSend��hEventΪ��Ч�ľ��ֵ
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //Ϊ�ص��������¼������ֹ����ã���ʼ��Ϊ���źŵ�
    ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));            //��ָ�����ڴ����㣬һ����ʹ�ýṹǰ����
    m_olRecv.hEvent = CreateEvent(NULL,                   //�þ�����ܱ��̳�
        TRUE,                   //�˹�����
        FALSE,                  //�¼���ʼ��״̬Ϊ���ź�״̬
        NULL                    //ָ���¼���
    );
    if (m_olRecv.hEvent == INVALID_HANDLE_VALUE)           //m_olRecv��hEventΪ��Ч�ľ��ֵ
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //�����¼�����
    //SetCommMask-ָ������������Ӧ���¼�ΪEV_RXCHAR,�����յ�һ���ַ�����������ջ�����
    if (!SetCommMask(m_hComm, EV_RXCHAR))
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //���ô����շ�������
    if (!SetupComm(m_hComm, MAXRECVBUF, MAXSENDBUF))   //��ʼ�����ڵ�ͨ�Ų�����0-ʧ�ܣ�����-�ɹ�
    {
        //�رմ���
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //��մ��ڻ�����
    if (!PurgeComm(m_hComm, PURGE_RXABORT | PURGE_TXABORT |
        PURGE_RXCLEAR | PURGE_TXCLEAR))
    {
        //�رմ���
        ClosePort();
        return ERR_DK_FAILURE;
    }

    COMMTIMEOUTS commtimeouts;

    //�Ѽ����ʱ��Ϊ��󣬰��ܳ�ʱ��Ϊ0������ReadFile�������ز���ɲ���
    commtimeouts.ReadIntervalTimeout = MAXDWORD;       //�������ʱ����Ϊ���
    commtimeouts.ReadTotalTimeoutMultiplier = 0;              //��ʱ��ϵ��Ϊ0
    commtimeouts.ReadTotalTimeoutConstant = 0;              //��ʱ�䳣��Ϊ0

    //����д��ʱ��ָ��WriteComm��Ա�����е�GetOverlappedResult�����ĵȴ�ʱ��
    commtimeouts.WriteTotalTimeoutMultiplier = 50;            //дʱ��ϵ��Ϊ50
    commtimeouts.WriteTotalTimeoutConstant = 200;           //дʱ�䳣��Ϊ200
    SetCommTimeouts(m_hComm, &commtimeouts);                  //���ô����豸��дʱ�ĳ�ʱ����

    //���ý���ģʽ
    m_nRecvMode = iRecvMode;

    switch (m_nRecvMode)
    {
    case Call_Func_Mode:

        m_hRecvMsgEvent = CreateEvent(NULL, FALSE, FALSE, NULL);    //����������Ϣʱ����,�Զ�����
        if (m_hRecvMsgEvent == INVALID_HANDLE_VALUE)
        {
            ClosePort();
            return ERR_DK_FAILURE;
        }

        m_bRunThread = TRUE;   //�����������б�־ΪTRUE

        m_hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvThread, this, 0, NULL); //���������̣߳����ؽ����߳̾��
        if (m_hRecvThread == NULL)
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
* ��������:	���ö˿ڣ������������߳�
* �������:	���Ǹ�����Combo����ѡ���˳���ţ���Ŵ�0��ʼ
            baud - �����ʣ� parity - ��żУ�飬dataBits - �˿�����λ
*           stopbits - �˿�ֹͣλ����fc - �˿ڿ�������
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
int CUart::ConfigPort(int nbaud, int nparity, int ndatabits,
    int nstopbits, EnumFlowControl fc)
{
    //��ö˿���ǰ��״̬
    DCB dcb;          //����һ��dcb�ṹ��Ŵ���ͨ�Ų���
    dcb.DCBlength = sizeof(DCB);      //DCB�ṹ�Ĵ�С
    if (!GetCommState(m_hComm, &dcb))  //�򿪴��ں󣬻�ȡ���ڵĳ�ʼ������
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //ͨ���޸�DCB�ṹ�������ô��ڵĸ������
    //���ö�����ģʽ
    dcb.fBinary = TRUE;

    //���ò�����
    switch (nbaud)
    {
    case 0:        //������4800
        dcb.BaudRate = 4800;
        break;
    case 1:       //������9600
        dcb.BaudRate = 9600;
        break;
    case 2:       //������14400
        dcb.BaudRate = 14400;
        break;
    case 3:       //������19200
        dcb.BaudRate = 19200;
        break;
    case 4:       //������38400
        dcb.BaudRate = 38400;
        break;
    case 5:       //������57600
        dcb.BaudRate = 57600;
        break;
    case 6:       //������115200
        dcb.BaudRate = 115200;
        break;
    default:
        break;
    }

    //����������żУ��
    dcb.fParity = TRUE;

    //������żУ��
    switch (nparity)
    {
    case 0:        //��У��
        dcb.Parity = NOPARITY;
        break;
    case 1:       //����У��
        dcb.Parity = ODDPARITY;
        break;
    case 2:       //żУ��
        dcb.Parity = EVENPARITY;
        break;
    case 3:       //��־У��
        dcb.Parity = MARKPARITY;
        break;
    case 4:       //�ո�У��
        dcb.Parity = SPACEPARITY;
        break;
    default:
        break;
    }

    //���ö˿ڵ�ǰʹ�õ�����λ
    //dcb.ByteSize = byDataBits;

    switch (ndatabits)
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

    //���ö˿ڵ�ǰʹ�õ�ֹͣ����λ
    switch (nstopbits)
    {
    case 0:          //1λ
        dcb.StopBits = ONESTOPBIT;
        break;
    case 1:          //1.5λ
        dcb.StopBits = ONE5STOPBITS;
        break;
    case 2:          //2λ
        dcb.StopBits = TWOSTOPBITS;
        break;
    default:
        break;
    }


    //�趨��DTR�ź����Ƿ����У����ΪTRUE,��ôDSRΪOFF�����յ��κ��ֽڽ�������
    dcb.fDsrSensitivity = FALSE;
    switch (fc)
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

    //�������õ�ǰ�˿�״̬
    if (!SetCommState(m_hComm, &dcb))
    {
        ClosePort();
        return ERR_DK_FAILURE;
    }

    //���������¼�
    if (Call_Func_Mode == m_nRecvMode)
    {
        SetEvent(m_hRecvMsgEvent);   //��Event��״̬����Ϊ���ź�״̬
    }
    return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: RegisterCallFunc
* ��������:	�����ش���ģ��ע�ắ����ע����û����ڴ���������ݵ��̺߳���
* �������:	CallFunc - ע�ắ��ָ�룬pThis - ע�����ָ��
* �������:	void
* �� �� ֵ: void
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
void CUart::RegisterCallFunc(CALLFUNC CallFunc, void* pThis)
{
    if (NULL == CallFunc || NULL == pThis)
    {
        return;
    }

    m_RecvMsgCallFunc = CallFunc;     //���ջص�����ָ��Ϊ�β�CallFunc
    m_pRegisterPoint = pThis;        //ע��ģ�庯����λ��Ϊ�β�pThis
}

/*********************************************************************************************************
* ��������: RecvThread
* ��������:	���������͵��̵߳���ִ�еĺ�����ַ�����ƾͱ�ʾ��ַ
* �������:	pParam - ��ǰ����ָ��,
* �������:	void
* �� �� ֵ: DK_SUCCESS
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
unsigned _stdcall CUart::RecvThread(void* pParam)
{
    CUart* pMyComm = NULL;

    if (NULL != pParam)
    {
        pMyComm = (CUart*)pParam;
        (void)pMyComm->RecvTaskProc();
    }
    return DK_SUCCESS;
}

/*********************************************************************************************************
* ��������: RecvTaskProc
* ��������:	�����¼���Ϣ����
* �������:	void
* �������:	void
* �� �� ֵ: �ɹ�����DK_SUCCESS, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
int CUart::RecvTaskProc()
{
    COMSTAT ComStat;  //ͨѶ�豸�����ṹ��
    DWORD dwErrorFlags = 0;
    int nError;
    HANDLE hMyComm = NULL;

    //��ȡ��̨������Ĳ������
    WaitForSingleObject(m_hRecvMsgEvent, INFINITE);   //ʹ���߳���Ը����ȴ�״̬��ֱ����Ϊ��֪ͨ״̬
    CloseHandle(m_hRecvMsgEvent);
    m_hRecvMsgEvent = NULL;

    while (m_bRunThread)   //�����������б�־ΪTRUE
    {
        if (m_hComm == INVALID_HANDLE_VALUE)
        {
            break;
        }

        ClearCommError(m_hComm, &dwErrorFlags, &ComStat);  //��������־

        if (ComStat.cbInQue)
        {
            char szRecvBuf[MAXRECVBUF + 1] = { 0 };

            int nLen = 0;
            nLen = RecvData((void*)szRecvBuf, MAXRECVBUF);   //���ڽ������ݣ�����szRecvBuf��

            if (nLen > 0)
            {
                m_RecvMsgCallFunc(szRecvBuf, nLen, m_pRegisterPoint); //�ص�����
            }
            continue;
        }

        nError = WaitComm();
        if (ERR_DK_FAILURE == nError)
        {
            break;
        }
    }
    return ERR_DK_FAILURE;
}

/*********************************************************************************************************
* ��������: SendData
* ��������:	��������
* �������:	pData - ���͵�����ָ�룬dwLen - �������ݳ���
* �������:	void
* �� �� ֵ: �ɹ����ط��ͳɹ����ֽ���, ʧ�ܷ���ERR_DK_FAILURE
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
int CUart::SendData(LPCVOID pData, DWORD dwLen)
{
    //�ж���ڲ���
    if (pData == NULL)
    {
        return ERR_DK_FAILURE;
    }

    BOOL bSuccess;
    DWORD dwErrorFlags;
    COMSTAT Comstat;
    DWORD dwSendCount = 0;
    DWORD dwError;

    ClearCommError(m_hComm, &dwErrorFlags, &Comstat);

    //��������
    bSuccess = WriteFile(m_hComm, pData, dwLen, &dwSendCount, &m_olSend);
    /*WriteFile������
    m_hComm : д���ļ��ľ��
    pData   : д���ļ������ݻ�����ָ��
    dwLen   : д����ֽ���
    dwSendCount : �ѷ��͵��ֽ���
    m_olSend: �ص���ʽʹ�õĽṹ��
    */

    if (!bSuccess)
    {
        //���IO������
        dwError = GetLastError();
        if (ERROR_IO_PENDING == dwError)  //��ʾ������δ�������
        {
            //һֱ�ȴ����ͳɹ�
            bSuccess = GetOverlappedResult(m_hComm, &m_olSend, &dwSendCount, TRUE);
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
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
int CUart::RecvData(LPVOID  pData, DWORD dwLen)
{
    //�ж���ڲ���
    if (pData == NULL)
    {
        return ERR_DK_FAILURE;
    }

    BOOL bSuccess = FALSE;
    DWORD dwErrorFlags;
    COMSTAT Comstat;
    DWORD dwRecvCount = 0;
    DWORD dwError;

    ClearCommError(m_hComm, &dwErrorFlags, &Comstat);
    dwRecvCount = min(dwLen, Comstat.cbInQue);  //ȡʵ�ʽ��յ����ݳ�������ջ��峤��֮�����Сֵ

    //���¼�����
    bSuccess = ReadFile(m_hComm, pData, dwLen, &dwRecvCount, &m_olRecv);

    if (!bSuccess)
    {
        //���IO������
        dwError = GetLastError();
        if (ERROR_IO_PENDING == dwError)  //��ʾ������δ�������
        {
            //һֱ�ȴ����ճɹ�
            bSuccess = GetOverlappedResult(m_hComm, &m_olRecv, &dwRecvCount, TRUE);
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
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
int CUart::WaitComm()
{
    OVERLAPPED os;
    DWORD dwMark = 0;
    DWORD dwTrans = 0;

    os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    if (os.hEvent == NULL)
    {
        return ERR_DK_FAILURE;
    }

    if (!WaitCommEvent(m_hComm, &dwMark, &os))   //�ص�����
    {
        if (GetLastError() == ERROR_IO_PENDING)
        {
            //���޵ȴ��ص��������
            GetOverlappedResult(m_hComm, &os, &dwTrans, TRUE);
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
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
void CUart::ClosePort()
{
    BOOL bOpen = FALSE;

    try
    {
        bOpen = IsOpen();
        m_bRunThread = FALSE;
        if (TRUE == bOpen)
        {
            //�رյ�ǰ�򿪵Ķ˿ھ��
            (void)CloseHandle(m_hComm);
            m_hComm = INVALID_HANDLE_VALUE;

            //�رյ�ǰ�¼����
            if (m_olSend.hEvent != NULL)
            {
                CloseHandle(m_olSend.hEvent);
            }

            if (m_olRecv.hEvent != NULL)
            {
                CloseHandle(m_olRecv.hEvent);
            }

            ZeroMemory(&m_olSend, sizeof(OVERLAPPED));
            ZeroMemory(&m_olRecv, sizeof(OVERLAPPED));
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
* ��������: 2017��08��29��
* ע    ��:
*********************************************************************************************************/
BOOL CUart::IsOpen() const
{
    return  (INVALID_HANDLE_VALUE != m_hComm);
}



