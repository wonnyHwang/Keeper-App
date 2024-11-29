#include "pch.h"
#include "CListenSocket.h"
#include "framework.h"
#include "JH_TCP_ServerAndClient.h"
#include "JH_TCP_ServerAndClientDlg.h"

CListenSocket::CListenSocket(CJHTCPServerAndClientDlg* pDlg)
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