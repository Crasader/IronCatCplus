#ifndef LIU_BYTE_BUFFER_H_
#define LIU_BYTE_BUFFER_H_
#include <iostream>
#include <stdlib.h>
#include <vector>
//#include "cocos2d.h"
typedef   unsigned   char   byte;
using namespace std;
//using namespace cocos2d;
//stream动态缓冲区的初始化大小
#define BYTE_BUFFER_SIZE 256

class ByteBuffer
{
public:
	//ByteBuffer(void);
	~ByteBuffer(void);
	//构建缓冲池大小
	ByteBuffer();
	ByteBuffer(char* buffer, int len);
	void addBuffer(char* buffer, int len);
	//得到容量
	int getCapacity();
	//得到缓冲池当前位置
	int getPostion();
	/**
	* 判断在当前位置和限制之间是否有任何元素
	* @return 当且仅当此缓冲区中至少还有一个元素时返回 true
	*/
	bool hasRemaining();
	/**
	* 返回当前位置与限制之间的元素数量
	* @return 此缓冲区中的剩余元素数量
	*/
	int remaining();
	ByteBuffer* setPosition(int newPosition);
	ByteBuffer* flip();
	//void release();
	
	/***
	*读取1字节的byte
	*/
	byte readByte();
	ByteBuffer* writeByte(byte value);

	/**
	
	*/
	bool readBool();
	ByteBuffer* writeBool(bool val);

	/**
	*读取2字节的short
	*/
	short readShort();
	ByteBuffer* writeShort(short value);

	/***
	*读取4字节的int
	*/
	int readInt();
	ByteBuffer* writeInt(int value);
	ByteBuffer* writeInt(int index, int value);

	/***
	*读取8字节的long
	*/
	long long readLong();
	ByteBuffer* writeLong(long long value);

	/**
	*读取4字节的float
	*/
	float readFloat();
	ByteBuffer* writeFloat(float value);

	/**
	*读取8字节的double
	*/
	double readDouble();
	ByteBuffer* writeDouble(double value);

	/**
	*字符串
	*/
	char* readString();
	ByteBuffer* writeString(const char* str);
	char* flush();
	int size();
	//缓冲池
	vector<char> data;
private:
	///构成的字符串 
	char* m_pData;
	//偏移量
	int position;
	//容量
	int capacity;
	
	//需要释放的
	vector<char*> m_DeleteCharList;
};

#endif // !LIU_BYTE_BUFFER_H_
