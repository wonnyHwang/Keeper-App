/* ����������ռ��� ������ǰ - ��ǻ�� ��Ʈ��ũ ���� */
/* 21812207 ������, 21812227 Ȳ����*/
/* Server : ����/ȣ�� ����Ʈ
   Client : ����/ȣ�� �� ����     */
#pragma comment(lib,"ws2_32.lib");
#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <Windows.h>
#include <iostream>
using namespace std;

#define MAX_BUF 1000//Packet Size ����

int main(void)
{
	WSADATA wsa;//���� ���̺귯���� ����ϱ� ���� ����
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//Socket�� ������� ��Ȳ�� �ƴ϶�� Error���� ���
	{
		printf("Error in starting up Winsock\n");
		return -1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//Socket ����
	if (s == INVALID_SOCKET)//Socket ���� ������ Error ���� ���
	{
		printf("Error in socket(), Error code : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	SOCKADDR_IN serverAddress;//Server �ּ�(����/ȣ�� ����Ʈ)
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(50000);//Server ��Ʈ �ּ�(Host endian�� network endian���� ��ȯ)
	inet_pton(AF_INET, "172.20.10.2", &(serverAddress.sin_addr.s_addr));//���� �ּҸ� endian�� �°� �ڵ���ȯ
	if (connect(s, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)//Server���� ������ ��û�Ѵ�. / ��û���н� Error���� ���
	{
		printf("Error in connect(), Error code : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	while (1)//�ݺ��ؼ� �����Ѵ�.
	{
		char chTxBuf[MAX_BUF] = { 0 };
		printf("�Է� �޼��� : ");
		gets_s(chTxBuf, MAX_BUF);//Text�޼��� ���
		char chTxBuf2[MAX_BUF] = "101: ";
		name(chTxBuf2, chTxBuf);//���ڿ��� �̾� ���δ�.
		int iLength = send(s, chTxBuf2, strlen(chTxBuf2), 0);//������ textMessage ���ڿ� ���� / Server���� ���� ����
		printf("�߼� �޼���: ");
		printf(chTxBuf2, &s);
		printf("\n");
		int iAddressLength = sizeof(serverAddress);//�ּ� ����
		char chRxBuf[MAX_BUF + 1] = { 0 };
		int iLength2 = recv(s, chRxBuf, MAX_BUF, 0); // �����κ��� �޽����� �����մϴ�.
		if (iLength2 <= 0) {
			printf("�������� ������ ���������ϴ�.\n");
			break;
		}
		chRxBuf[iLength2] = '\0'; // ���ڿ��� ���� ǥ���մϴ�.
		printf("���� �޼���: %s\n", chRxBuf);
	}
	closesocket(s);//���� ����
	WSACleanup();
	return 0;
}
void name(char buffer[], char buffer2[])//���ڿ��� �̾� �ٿ��ش�.
{
	strcat(buffer, buffer2);//Buffer�� Buffer2 ���ڿ��� ���� �̾� �ٿ��ش�.
}