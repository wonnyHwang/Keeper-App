#include "pch.h"
#include "CDataSocket.h"
#include "framework.h"
#include "JH_TCP_ServerAndClient.h"
#include "JH_TCP_ServerAndClientDlg.h"

// CDataSocket

//m_pDlg 멤버 변수를 이용한 대화상자 객체에 접근
CDataSocket::CDataSocket(CJHTCPServerAndClientDlg* pDlg)
{
	m_pDlg = pDlg;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket 멤버 함수


void CDataSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}


void CDataSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose(this, nErrorCode);
}