#include<iostream>
#include<cstring>
#ifdef _WIN32

#include<winsock2.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
typedef int SOCKET;

#endif

using namespace std;

//UTF-8转GB2312
char* U2G(const char* utf8){
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}
//GB2312转UTF-8
char* G2U(const char* gb2312){
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

sockaddr GetSockAddr(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint16_t inPort){
	sockaddr addr;
	sockaddr_in *addrin = reinterpret_cast<sockaddr_in*>(&addr);
	addrin->sin_family = AF_INET;
	addrin->sin_addr.S_un.S_un_b.s_b1 = b1;
	addrin->sin_addr.S_un.S_un_b.s_b2 = b2;
	addrin->sin_addr.S_un.S_un_b.s_b3 = b3;
	addrin->sin_addr.S_un.S_un_b.s_b4 = b4;
	addrin->sin_port = htons(inPort);
	
	return addr;
}


int main(){
	WSADATA wsd;
	if(WSAStartup(MAKEWORD(2, 2), &wsd)){
		std::cout << "WSAStartup Error" << std::endl;
		return -1;
	}
	SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	//修改为服务器IP
	sockaddr serverAddr = GetSockAddr(127, 0, 0, 1, 50002);
	int serverAddrLen = sizeof(sockaddr);
	if(connect(tcpSocket, &serverAddr, serverAddrLen) == -1){
		std::cout << "connect Error!" << std::endl;
		return -1;
	}
	std::cout << "已成功连接到服务器" << std::endl;
	//客户端的socket就是用于连接的socket
	const int BufMaxLen = 1000;
	char sendBuf[BufMaxLen] = {};
	char buf[BufMaxLen] = {};
	std::cout << ">>> ";
	while(true){
		if(std::cin >> sendBuf){
			if(strcmp(sendBuf, "exit") == 0){
				std::cout << "正在关闭连接..." << std::endl;
				break;
			}
			char *sendBuf_UTF8 = G2U(sendBuf);
			send(tcpSocket, sendBuf_UTF8, strlen(sendBuf_UTF8), 0);
			memset(sendBuf, 0, BufMaxLen);
			int ret = recv(tcpSocket, buf, BufMaxLen, 0);
			if(ret > 0){
				std::cout << "从服务器接收到回应：" << U2G(buf) << std::endl;
				memset(buf, 0, BufMaxLen);
			}
			std::cout << ">>> ";
		}
	}
	
	
	shutdown(tcpSocket, SB_BOTH);
	closesocket(tcpSocket);
	WSACleanup();
	std::cout << "已关闭客户端Socket..." << std::endl;
	
	return 0;
}
