#pragma warning(disable:4996)
#include "client.h"

void client::init()
{
	WSADATA wsaData;
	int port = 1234;						//端口
	std::string serverIp;					//服务器IP

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) //调用Windows Sockets初始化WinSock
	{
		printf("Winsock无法初始化!\n");
		WSACleanup();
		exit(-1);
	}
	printf("成功加载Winsock！\n");
	printf("正在创建SOCKET....\n");
	std::cout << "请输入服务器IP:\n";
	std::cin >> serverIp;
	/* 配置服务器IP、端口信息 */
	memset(&serverAddr, 0, sizeof(struct sockaddr));//先用0来填充
	serverAddr.sin_family = AF_INET;				//AF_INET是IPv4的Internet地址族格式。
	serverAddr.sin_port = htons(port);				//设置server的监听端口
	serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());	//设置server的地址
	socket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);			//创造数据报格式用于无连接的UDP传输的套接字
}

client::client()
{
	init();
}

void client::sendToServer(std::string msg)//发送数据
{
	sendto(socket1,	 //本地套接字
		msg.c_str(), //指向包含要传输的数据
		msg.size(),	 //数据的长度
		0,			 //标志
		(struct sockaddr*)&serverAddr,//服务器套接字地址
		sizeof(serverAddr));//地址长度
}

std::string client::receieveFromServer()
{
	while (true) {
		char buffer[128] = "\0";//用于接受数据的缓冲区字符串
		sockaddr_in ServerAddr;
		int msgLen=sizeof(ServerAddr);
		if (recvfrom(socket1,		//本地套接字
					buffer,			//用字符串接受数据
					sizeof(buffer), //字符串大小 
					0,				//标志
					(struct sockaddr*) & ServerAddr,//服务器套接字地址
					&msgLen							//地址长度
					) != SOCKET_ERROR) {//若未收到则返回错误字符串，循环接受等待，若收到则返回字节数，进入下一步
			return std::string(buffer);//收到信息则返回信息
		}
	}
}
