#include<iostream>
#include<Winsock2.h>
#include<windows.h>

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
		exit(-1);
	}
	
	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	const size_t BufMaxSize = 1000;
	char buf[BufMaxSize] = {};
	//修改为服务器IP
	sockaddr toAddr = GetSockAddr(127, 0, 0, 1, 50002); 
	int toAddrLen = sizeof(sockaddr);
	std::cout << ">>> ";
	while(true){
		if(std::cin >> buf){
			char *buf_UTF8 = G2U(buf);
			sendto(udpSocket, buf_UTF8, strlen(buf_UTF8), 0, &toAddr, toAddrLen);
			std::cout << "已发送!" <<std::endl;
			std::cout << ">>> ";
			memset(buf, 0, sizeof(buf));
		}
	}
	
	//shutdown(udpSocket, SB_BOTH);
	closesocket(udpSocket);
	
	WSACleanup();
	
	return 0;
}
