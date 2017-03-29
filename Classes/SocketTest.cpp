#include "SocketTest.h"
#define GAME_SERVER_IP "192.168.10.114"
#define  GAME_SOCKET_PORT 9001

SocketTest::SocketTest() :socket(nullptr), _isStart(false)
{
	socket = new ODSocket;
}


SocketTest::~SocketTest()
{
}

void SocketTest::init()
{
	
	socket->Clean();
	socket->Init();
	socket->Create(AF_INET, SOCK_STREAM, 0);
	_isStart = socket->Connect(GAME_SERVER_IP, Value(GAME_SOCKET_PORT).asInt());
	std::thread(&SocketTest::receiveData,this).detach();
	
}
void SocketTest::send(ByteBuffer* buff)
{
	if (_isStart)
	{
		int len = socket->Send(buff);
		log("socket.Send(data) success_____________%d",len);
	}
	else
	{
		log("socket.Send(data) error_____________");
	
	}

}

void SocketTest::login()
{
	ByteBuffer* buff = new ByteBuffer;
	buff->writeShort(1001);
	buff->writeString("test");
	buff->writeString("test");
	buff->writeString("1.0.0");
	buff->writeString("321");

	//int len = buff->size();
	//buff->writeInt(0, buff->size());

	//int nBufferSize = buff->data.size();
	//string s;
	//if (nBufferSize != 0)
	//{
	//	int nIdx = 0;
	//	//动态分配的堆内存，注意释放！
	//	auto m_pData = new char[nBufferSize];
	//	
	//	s.assign(buff->data.begin(), buff->data.end());

	//}
	//
	//log("%s",s.c_str());
	this->send(buff);

}


void SocketTest::receiveData()
{
	if (_isStart)
	{
		
	}
	while (_isStart)
	{
		char recvBuf[4] = "";// 获取请求头的 数据
		log("i am waitting______");
		int i = socket->Recv(recvBuf, 4, 0);
		log("i am waitting ok______");

		if (i == 4)
		{
			log("message comming_________");
			char dc1[4] = { recvBuf[3], recvBuf[2], recvBuf[1], recvBuf[0] };
			//结构体的长度
			int len = *(unsigned int*)&dc1;

			log("message comming 1 %d_________", len);
			char* messbody = new char[len];
			int recvLen = socket->Recv(messbody, len, 0);
			ByteBuffer* buffer = new ByteBuffer(messbody, recvLen);
		

			log("i do post buffer  %x_____________________", buffer);
			
			log("i do post  2_____________________");
		}
		else
		{
			log("close socket  2_____________________");
			_isStart = false;
			socket->Close();
		}
		if (!_isStart) return;
	}

}




