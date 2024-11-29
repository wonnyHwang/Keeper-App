#include "pch.h"
#include "CDataSocket.h"
#include "framework.h"
#include "JH_TCP_Server.h"
#include "JH_TCP_ServerDlg.h"
CDataSocket::CDataSocket(CJHTCPServerDlg* pDlg)
{
	m_pDlg = pDlg;
}

CDataSocket::~CDataSocket()
{
}


// CDataSocket ��� �Լ�


void CDataSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnReceive(nErrorCode);
	m_pDlg->ProcessReceive(this, nErrorCode);
}


void CDataSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CSocket::OnClose(nErrorCode);
	m_pDlg->ProcessClose(this, nErrorCode);
}