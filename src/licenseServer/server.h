#pragma once
#include"public.h"

//����ӿͻ��˽��յ���Ϣ��������Ϣ�͵�ַ
struct messageData {
	std::string msg;
	sockaddr_in addr;
	messageData(std::string, sockaddr_in);
};

//����ͻ�����Ϣ
struct clientData {
	sockaddr_in clientAddr;
	std::string username;
	std::string seqNum;
	std::thread* corrThread;
	clientData();
	clientData(sockaddr_in&Addr, std::string name,
		 std::string seqNum, std::thread* thr);
};

//���¶��е�Ԫ��
struct updateTuple {
	sockaddr_in clientAddr;
	bool statusFlag;
	updateTuple(sockaddr_in addr, bool flag);
};

class server
{
	SOCKET socket1;
	sockaddr_in serverAddr;
	void init();
public:
	server();
	void sendToClient(sockaddr_in* clientAddr, std::string msg);
	void receieveFromClient();
	std::queue<messageData> msgBuffer;		//�ݴ��յ�����Ϣ
};

