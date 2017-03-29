#pragma once
#include "cocos2d.h"
#include "ODSocket.h"
#include "Singleton.h"
#include "ByteBuffer.h"

USING_NS_CC;


class SocketTest:public Singleton<SocketTest>
{
public:
	SocketTest();
	~SocketTest();


	void init();
	void receiveData();
	void send(ByteBuffer* buff);

	void login();
private:
	ODSocket* socket;
	bool _isStart;
};

