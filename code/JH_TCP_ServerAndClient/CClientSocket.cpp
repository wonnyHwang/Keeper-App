#include "pch.h"
#include "CClientSocket.h"
#include "framework.h"
#include "JH_TCP_ServerAndClient.h"
#include "JH_TCP_ServerAndClientDlg.h"

CClientSocket::CClientSocket(CJHTCPServerAndClientDlg* pDlg)
{
	m_pDlg = pDlg;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose2(this, nErrorCode);
}
void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive2(this, nErrorCode);
}