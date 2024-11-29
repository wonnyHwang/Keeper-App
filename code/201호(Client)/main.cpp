/* 정보통신종합설계 졸업작품 - 컴퓨터 네트워크 구현 */
/* 21812207 한종헌, 21812227 황종원*/
/* Server : 모텔/호텔 프론트
   Client : 모텔/호텔 내 객실     */
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

#define MAX_BUF 1000//Packet Size 지정

int main(void)
{
	WSADATA wsa;//소켓 라이브러리를 사용하기 위해 선언
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)//Socket이 만들어질 상황이 아니라면 Error구문 출력
	{
		printf("Error in starting up Winsock\n");
		return -1;
	}
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//Socket 생성
	if (s == INVALID_SOCKET)//Socket 생성 오류시 Error 구문 출력
	{
		printf("Error in socket(), Error code : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	SOCKADDR_IN serverAddress;//Server 주소(모텔/호텔 프론트)
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(50000);//Server 포트 주소(Host endian을 network endian으로 변환)
	inet_pton(AF_INET, "172.20.10.2", &(serverAddress.sin_addr.s_addr));//서버 주소를 endian에 맞게 자동변환
	if (connect(s, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)//Server에게 연결을 요청한다. / 요청실패시 Error구문 출력
	{
		printf("Error in connect(), Error code : %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}
	while (1)//반복해서 전송한다.
	{
		char chTxBuf[MAX_BUF] = { 0 };
		printf("입력 메세지 : ");
		gets_s(chTxBuf, MAX_BUF);//Text메세지 출력
		char chTxBuf2[MAX_BUF] = "101: ";
		name(chTxBuf2, chTxBuf);//문자열을 이어 붙인다.
		int iLength = send(s, chTxBuf2, strlen(chTxBuf2), 0);//보내는 textMessage 문자열 길이 / Server에게 문자 전송
		printf("발송 메세지: ");
		printf(chTxBuf2, &s);
		printf("\n");
		int iAddressLength = sizeof(serverAddress);//주소 길이
		char chRxBuf[MAX_BUF + 1] = { 0 };
		int iLength2 = recv(s, chRxBuf, MAX_BUF, 0); // 서버로부터 메시지를 수신합니다.
		if (iLength2 <= 0) {
			printf("서버와의 연결이 끊어졌습니다.\n");
			break;
		}
		chRxBuf[iLength2] = '\0'; // 문자열의 끝을 표시합니다.
		printf("수신 메세지: %s\n", chRxBuf);
	}
	closesocket(s);//소켓 종료
	WSACleanup();
	return 0;
}
void name(char buffer[], char buffer2[])//문자열을 이어 붙여준다.
{
	strcat(buffer, buffer2);//Buffer와 Buffer2 문자열을 서로 이어 붙여준다.
}