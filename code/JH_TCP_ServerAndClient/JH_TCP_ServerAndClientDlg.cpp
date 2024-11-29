
// JH_TCP_ServerAndClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "JH_TCP_ServerAndClient.h"
#include "JH_TCP_ServerAndClientDlg.h"
#include "afxdialogex.h"
#include "CListenSocket.h"
#include "CDataSocket.h"
#include "CClientSocket.h"
#include "windows.h"
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MAX_BUF 1000
CCriticalSection tx_cs;
CCriticalSection rx_cs;
CCriticalSection flag_cs;
int flag = -1;// 119 출동 : 2, 위험 : 1, 안전 : 0

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CJHTCPServerAndClientDlg 대화 상자



CJHTCPServerAndClientDlg::CJHTCPServerAndClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JH_TCP_SERVERANDCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJHTCPServerAndClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_rx_edit);
	DDX_Control(pDX, IDC_EDIT2, m_tx_edit);
	
}

BEGIN_MESSAGE_MAP(CJHTCPServerAndClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CJHTCPServerAndClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CJHTCPServerAndClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CJHTCPServerAndClientDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

UINT RXThread(LPVOID arg)	//입력한 내용을 Server의 Sender Text Box에 출력
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CStringList* plist = pArg->pList;
	CJHTCPServerAndClientDlg* pDlg = (CJHTCPServerAndClientDlg*)pArg->pDlg;
	int count = 0;
	while (pArg->Thread_run) {
		POSITION pos = plist->GetHeadPosition();
		POSITION current_pos;
		while (pos != NULL) {
			current_pos = pos;
			rx_cs.Lock();	//SenderTextBox의 임계구역을 Lock
			CString str = plist->GetNext(pos);
			rx_cs.Unlock();
			char txbuf[MAX_BUF]= "";
			strcpy_s(txbuf, CT2A(str.GetBuffer()));
			std::string strBuf(txbuf);
			std::istringstream iss(strBuf);
			std::string text;//방 호수를 읽는다.
			std::string message2;//상황을 읽는다.
			getline(iss, text, ':');
			
			getline(iss, message2);
			message2.erase(std::remove_if(message2.begin(), message2.end(), ::isspace), message2.end());//공백 제거
			//char* text = strtok(txbuf, ": ");//방 호수가 담긴다.
			//flag_cs.Lock();
			if (message2.find("Danger") != std::string::npos)//Danger라는 신호가 오게 된다면
			{
				flag = 1;//위험
				CString message;
				message.Format(_T("%hs호 위급상황"), text.c_str());
				pDlg->m_rx_edit.SetWindowText(message);
				pDlg->m_rx_edit.LineScroll(pDlg->m_rx_edit.GetLineCount());			
			}
			else if (message2.find("Emergency") != std::string::npos)
			{
				//pArg->flag = 2;//응급
				CString message;
				CTime currentTime = CTime::GetCurrentTime();
				message = currentTime.Format(_T("119 현 %H:%M:%S부로 출동", currentTime));
				pDlg->m_tx_edit.SetWindowText(message);
				pDlg->m_tx_edit.LineScroll(pDlg->m_rx_edit.GetLineCount());
			}
			//flag_cs.Unlock();
			rx_cs.Lock();
			plist->RemoveAt(current_pos);
			rx_cs.Unlock();
		}
		Sleep(10);
	}
	return 0;
}
UINT sendThread(LPVOID arg)
{
	ThreadArg* pArg = (ThreadArg*)arg;
	CJHTCPServerAndClientDlg* pDlg = (CJHTCPServerAndClientDlg*)pArg->pDlg;
	char* str2 = (char*)malloc(sizeof(char) * MAX_BUF);
	str2 = "영남호텔 위급상황 발생!!";
	while (pArg->Thread_run) 
	{
		//flag_cs.Lock();
		if (flag == 1) // 위급 상황
		{
			pDlg->m_pClientSocket->Send(str2, (int)strlen(str2));
			flag = -1;
		}

		Sleep(10);
	}
	return 0;
}
BOOL CJHTCPServerAndClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CStringList* newlist = new CStringList;
	arg1.pList2 = newlist;
	arg1.Thread_run = 1;	//스레드 현재 상태
	arg1.pDlg = this;
	//arg1.flag = 0;

	CStringList* newlist2 = new CStringList;
	arg2.pList = newlist2;
	arg2.Thread_run = 1;	//스레드 현재 상태
	arg2.pDlg = this;
	//arg2.flag = 0;

	arg3.Thread_run = 1;
	arg3.pDlg = this;

	WSADATA wsa;
	int error_code;
	if ((error_code = WSAStartup(MAKEWORD(2, 2), &wsa)) != 0) {
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}
	m_pListenSocket = NULL;
	ASSERT(m_pListenSocket == NULL);
	m_pListenSocket = new CListenSocket(this);
	if (m_pListenSocket->Create(50000)) {	//소켓 번호 설정
		if (m_pListenSocket->Listen()) {
			AfxMessageBox(_T("서버를 시작합니다."), MB_ICONINFORMATION);
			m_pClientSocket = new CClientSocket(this);
			m_pClientSocket->Create();
			if (m_pClientSocket->Connect(_T("172.20.10.9"), 51000) == FALSE) {
				AfxMessageBox(_T("ERROR : Failed to connect Server"));
				PostQuitMessage(0);
				return FALSE;
			}
			//TX, RX, send 스레드 실행
			//pThread1 = AfxBeginThread(TXThread, (LPVOID)&arg1);
			pThread2 = AfxBeginThread(RXThread, (LPVOID)&arg2);
			pThread3 = AfxBeginThread(sendThread, (LPVOID)&arg3);
			return TRUE;
		}
	}
	else {
		int err = m_pListenSocket->GetLastError();
		TCHAR buffer[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 256, NULL);
		AfxMessageBox(buffer, MB_ICONERROR);
	}
	
	AfxMessageBox(_T("이미 실행 중인 서버가 있습니다.") _T("\n프로그램을 종료합니다."), MB_ICONERROR);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CJHTCPServerAndClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CJHTCPServerAndClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CJHTCPServerAndClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CJHTCPServerAndClientDlg::ProcessAccept(int nErrorCode)
{
	CString PeerAddr;
	UINT PeerPort;
	CString str;

	m_pDataSocket = NULL;

	ASSERT(nErrorCode == 0);
	if (m_pDataSocket == NULL) {
		m_pDataSocket = new CDataSocket(this);
		if (m_pListenSocket->Accept(*m_pDataSocket)) {
			m_pDataSocket->GetPeerName(PeerAddr, PeerPort);
		}
		else {
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
	}
}

//프로세스를 받았을 경우
void CJHTCPServerAndClientDlg::ProcessReceive(CDataSocket* pSocket, int nErrorCode)//메세지를 받는 경우 위급상황임을 인지
{
	char pBuf[1024 + 1];
	CString strData;
	int nbytes;

	nbytes = pSocket->Receive(pBuf, 1024);
	pBuf[nbytes] = NULL;
	strData = (LPCSTR)pBuf;
	
	rx_cs.Lock();
	arg2.pList->AddTail(strData);
	rx_cs.Unlock();
}

//프로세스를 종료하였을 경우
void CJHTCPServerAndClientDlg::ProcessClose(CDataSocket* pSocket, int nErrorCode) {
	pSocket->Close();
	delete m_pDataSocket;
	m_pDataSocket = NULL;

	int len = m_rx_edit.GetWindowTextLengthW();
	CString message = _T("### 접속 종료 ###\r\n");
	m_rx_edit.SetSel(len, len);
	m_rx_edit.ReplaceSel(message);
}

//프로세스를 받았을 경우
void CJHTCPServerAndClientDlg::ProcessReceive2(CClientSocket* pSocket, int nErrorCode)
{
	char pBuf[1024 + 1];
	CString strData;
	int nbytes;

	nbytes = pSocket->Receive(pBuf, 1024);
	pBuf[nbytes] = NULL;
	strData = (LPCSTR)pBuf;

	rx_cs.Lock();
	arg2.pList->AddTail(strData);
	rx_cs.Unlock();
}

//프로세스를 종료하였을 경우
void CJHTCPServerAndClientDlg::ProcessClose2(CClientSocket* pSocket, int nErrorCode) {
	pSocket->Close();
	delete m_pClientSocket;
	m_pClientSocket = NULL;

	int len = m_tx_edit.GetWindowTextLengthW();
	CString message = _T("### 접속 종료 ###\r\n");
	m_tx_edit.SetSel(len, len);
	m_tx_edit.ReplaceSel(message);
}

void CJHTCPServerAndClientDlg::OnBnClickedButton1()//Pan On
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char* str2 = (char*)malloc(sizeof(char) * MAX_BUF);
	str2 = "PAN_ON";
	m_pDataSocket->Send(str2, (int)strlen(str2));
}

void CJHTCPServerAndClientDlg::OnBnClickedButton2()//Sensor ON
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char* str2 = (char*)malloc(sizeof(char) * MAX_BUF);
	str2 = "SENSOR_ON";
	m_pDataSocket->Send(str2, (int)strlen(str2));
}

void CJHTCPServerAndClientDlg::OnBnClickedButton3()//Sensor OFF
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char* str2 = (char*)malloc(sizeof(char) * MAX_BUF);
	str2 = "SENSOR_OFF";
	m_pDataSocket->Send(str2, (int)strlen(str2));
}
