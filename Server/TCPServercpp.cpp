#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    1024

int main(int argc, char* argv[])
{
	int retval;
	FILE* file;
	size_t filesize, recvsize = 0;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");


	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");


	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");


	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	int len;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));


		while (1) {
			// 파일 이름 크기 받기
			retval = recv(client_sock, (char*)&len, sizeof(int), MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;
			char* filename = new char[len + 1];
			// 파일 이름 받기
			retval = recv(client_sock, filename, len, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;
			filename[retval] = '\0';
			//받은 파일 이름의 파일 생성 및 열기
			file = fopen(filename, "wb");
			// 파일 크기 받기
			retval = recv(client_sock, (char*)&filesize, sizeof(size_t), 0);
			if (retval == SOCKET_ERROR) {
				// 잘못 받으면 파일 닫기
				err_display("recv()");
				fclose(file);
				break;
			}
			// 파일 받기
			while (true) {
				retval = recv(client_sock, buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				if (retval == 0) {
					// 전송이 모두 끝나고 정상 종료시 진행상황 표시 후 빠져나가기
					printf("%zd byte/%zd byte %.0f %% \n", recvsize, filesize, (float)recvsize / (float)filesize * 100.0);
					break;
				}
				recvsize += retval;
				fwrite(buf, sizeof(char), retval, file);
				printf("%zd byte/%zd byte %.0f %% \r", recvsize, filesize, (float)recvsize / (float)filesize * 100.0);
			}
			fclose(file);
			recvsize = 0;
		}


		closesocket(client_sock);

		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}


	closesocket(listen_sock);

	WSACleanup();
	return 0;
}