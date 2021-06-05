#pragma warning(disable:4996)
#include"public.h"
#include"server.h"
server myServer;
std::queue<updateTuple> updateBuffer;	//�ݴ�����еĸ���
std::map<sockaddr_in, clientData> clientInfo;		//ʹ�ÿͻ��˵�ַ���ҿͻ�����Ϣ

bool operator<(const sockaddr_in& l, const sockaddr_in& r) {//����С�ں�
	return l.sin_port < r.sin_port;
}

void readLicenceData() {	//��ȡ֤���е���Ϣ

}

void connectClient(sockaddr_in clientAddr) {
	//���Ӧ�Ŀͻ��˽���
	//�趨һ����ʱ��һ��ʱ��û�յ�check��ر��߳�
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == CK_MSG) {
				//���ü�ʱ��
			}
			if (msg.message == WM_QUIT) {
				return;
			}

		}
	}


}

bool checkInfo(std::string name,std::string seqnum){
	//ȷ�����֤
	if (seqnum == "100")
		return true;
	else return false;
}

void updateLicence(updateTuple tuple) {
	//��֤���ļ����ݽ��и���
	std::string state;
	if (tuple.statusFlag == true)
		state = "login";
	else state = "logout";
	std::cout << "user " << inet_ntoa(tuple.clientAddr.sin_addr) << "\t" << state << std::endl;
}


void handleMessage(messageData data) {
	//�������Կͻ��˵���Ϣ
	std::cout << "������ϢΪIP��ַ������"<<std::endl;
	std::cout << inet_ntoa(data.addr.sin_addr) << ":\t" << data.msg << std::endl;
	std::stringstream ss;
	ss.str(data.msg);
	std::string ins;
	ss >> ins;

	//������Ϣָ��,login��¼,check��ʱ�㱨,quit�˳�
	if (ins == "login") {
		std::string username, seqNum;	//�û����������к�
		ss >> username  >> seqNum;

		if (checkInfo(username, seqNum)) {
			//ȷ����Ϣ������ͻ��˷��������¼ָ��
			std::cout << "�����¼\n";
			//�����µ��̹߳�����ͻ��˵����ӣ��������Ӧ�ͻ��˵���Ϣ,����¶�����Ӹ���ָ��
			myServer.sendToClient(&(data.addr), "permit");
			std::thread* thr = new std::thread(connectClient, data.addr);
			clientInfo[data.addr] = clientData(data.addr,username,seqNum,thr);
			updateBuffer.push(updateTuple(data.addr, true));

		}
		else {
			std::cout << "δ�����¼\n";
			myServer.sendToClient(&(data.addr), "inhibit");
		}
	}
	else if (ins == "check") {
		//���ݵ�ַ�ҵ���Ӧ���̣߳������߳�ʱ��
		std::thread*tr = clientInfo.at(data.addr).corrThread;
		DWORD tid = GetThreadId(tr->native_handle());
		while (!PostThreadMessage(tid,CK_MSG,0,0));
	}
	else if (ins == "quit") {
		//�رն�Ӧ�̣߳�����¶�����Ӹ���ָ��
		std::thread* tr = clientInfo.at(data.addr).corrThread;
		DWORD tid = GetThreadId(tr->native_handle());
		while (!PostThreadMessage(tid, WM_QUIT, 0, 0));
		clientInfo.erase(data.addr);
		updateBuffer.push(updateTuple(data.addr, false));
	}
	else myServer.sendToClient(&(data.addr), "��������ȷ��ָ�");
}

int main()
{
	std::stringstream ss;
	readLicenceData();
	std::thread recvThread(&server::receieveFromClient, &myServer);

	while (true) {
		if (!myServer.msgBuffer.empty()) {
			//�����Ϣ���в�Ϊ�գ�����ȡ��һ����Ϣ���д���
			handleMessage(myServer.msgBuffer.front());
			myServer.msgBuffer.pop();
		}

		if (!updateBuffer.empty()) {
			//������¶��в�Ϊ�գ��ͽ���һ�θ���
			updateLicence(updateBuffer.front());
			updateBuffer.pop();
		}


	}
	recvThread.join();
	return 0;
}