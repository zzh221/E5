
#include "server.h"

server::server()
{
	init();
}

void server::init()
{
	WSADATA wsaData;
	int port = 1234;						//�������˿�
	std::string serverip;	//������IP

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) //����Windows Sockets DLL
	{
		printf("Winsockδ�ܳ�ʼ��!\n");
		WSACleanup();
		exit(-1);
	}

	printf("��������ʼ����SOCKET��\n");
	std::cout << "�����������IP:\n";
	std::cin >> serverip;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);				///�����˿�
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); ///IP��ַ�趨Ϊ����IP
	socket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/*���ṹ��󶨵���ǰ���׽���*/
	bind(socket1,						  //�׽��ֱ�ʶ��
		 (struct sockaddr*) & serverAddr, //�׽��ֵ�ַ�ṹ��
		  sizeof(serverAddr)			  //�ṹ�峤��
	);			  

}

void server::sendToClient(sockaddr_in* clientAddr, std::string data)
{
	sendto(socket1, data.c_str(), data.size(), 0, (struct sockaddr*) clientAddr, sizeof(*clientAddr));//��ͻ����ͳ�����
}

void server::receieveFromClient()
{
	printf("��ʼ������Ϣ...\n");
	while (true) {
		char buffer[128] = "\0";		//���������õĻ������ַ���
		sockaddr_in clientAddr;			//�ͻ��˵�ַ�ṹ��
		int msgLen=sizeof(clientAddr);	//��ַ�ṹ�峤��
		if (recvfrom(socket1, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr,&msgLen) != SOCKET_ERROR) {
			msgBuffer.push(messageData(std::string(buffer), clientAddr));//���ݴ������
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


