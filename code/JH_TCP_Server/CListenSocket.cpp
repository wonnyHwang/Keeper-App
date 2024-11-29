#include "pch.h"
#include "CListenSocket.h"
#include "framework.h"
#include "JH_TCP_Server.h"
#include "JH_TCP_ServerDlg.h"

CListenSocket::CListenSocket(CJHTCPServerDlg* pDlg)
{
	m_pDlg = pDlg;
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket ��� �Լ�


void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnAccept(nErrorCode);
	m_pDlg->ProcessAccept(nErrorCode);
}