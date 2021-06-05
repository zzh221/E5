
#include "server.h"

server::server()
{
	init();
}

void server::init()
{
	WSADATA wsaData;
	int port = 1234;						//服务器端口
	std::string serverip;	//服务器IP

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) //调用Windows Sockets DLL
	{
		printf("Winsock未能初始化!\n");
		WSACleanup();
		exit(-1);
	}

	printf("服务器开始创建SOCKET。\n");
	std::cout << "请输入服务器IP:\n";
	std::cin >> serverip;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);				///监听端口
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); ///IP地址设定为本机IP
	socket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/*将结构体绑定到当前的套接字*/
	bind(socket1,						  //套接字标识符
		 (struct sockaddr*) & serverAddr, //套接字地址结构体
		  sizeof(serverAddr)			  //结构体长度
	);			  

}

void server::sendToClient(sockaddr_in* clientAddr, std::string data)
{
	sendto(socket1, data.c_str(), data.size(), 0, (struct sockaddr*) clientAddr, sizeof(*clientAddr));//向客户端送出数据
}

void server::receieveFromClient()
{
	printf("开始接收信息...\n");
	while (true) {
		char buffer[128] = "\0";		//接收数据用的缓冲区字符串
		sockaddr_in clientAddr;			//客户端地址结构体
		int msgLen=sizeof(clientAddr);	//地址结构体长度
		if (recvfrom(socket1, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr,&msgLen) != SOCKET_ERROR) {
			msgBuffer.push(messageData(std::string(buffer), clientAddr));//数据存入队列
		}
	}
}
messageData::messageData(std::string m, sockaddr_in a):msg(m),addr(a){}

clientData::clientData()
{
	clientAddr = sockaddr_in();
	username = "";
	seqNum = "";
	corrThread = nullptr;
}

clientData::clientData(sockaddr_in&Addr, std::string name="",
	 std::string seqNum="",std::thread*thr=nullptr) {
	clientAddr = Addr;
	username = name;
	this->seqNum = seqNum;
	corrThread = thr;
};

updateTuple::updateTuple(sockaddr_in addr,bool flag):clientAddr(addr),statusFlag(flag){};


