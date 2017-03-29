#ifndef LIU_BYTE_BUFFER_H_
#define LIU_BYTE_BUFFER_H_
#include <iostream>
#include <stdlib.h>
#include <vector>
//#include "cocos2d.h"
typedef   unsigned   char   byte;
using namespace std;
//using namespace cocos2d;
//stream��̬�������ĳ�ʼ����С
#define BYTE_BUFFER_SIZE 256

class ByteBuffer
{
public:
	//ByteBuffer(void);
	~ByteBuffer(void);
	//��������ش�С
	ByteBuffer();
	ByteBuffer(char* buffer, int len);
	void addBuffer(char* buffer, int len);
	//�õ�����
	int getCapacity();
	//�õ�����ص�ǰλ��
	int getPostion();
	/**
	* �ж��ڵ�ǰλ�ú�����֮���Ƿ����κ�Ԫ��
	* @return ���ҽ����˻����������ٻ���һ��Ԫ��ʱ���� true
	*/
	bool hasRemaining();
	/**
	* ���ص�ǰλ��������֮���Ԫ������
	* @return �˻������е�ʣ��Ԫ������
	*/
	int remaining();
	ByteBuffer* setPosition(int newPosition);
	ByteBuffer* flip();
	//void release();
	
	/***
	*��ȡ1�ֽڵ�byte
	*/
	byte readByte();
	ByteBuffer* writeByte(byte value);

	/**
	
	*/
	bool readBool();
	ByteBuffer* writeBool(bool val);

	/**
	*��ȡ2�ֽڵ�short
	*/
	short readShort();
	ByteBuffer* writeShort(short value);

	/***
	*��ȡ4�ֽڵ�int
	*/
	int readInt();
	ByteBuffer* writeInt(int value);
	ByteBuffer* writeInt(int index, int value);

	/***
	*��ȡ8�ֽڵ�long
	*/
	long long readLong();
	ByteBuffer* writeLong(long long value);

	/**
	*��ȡ4�ֽڵ�float
	*/
	float readFloat();
	ByteBuffer* writeFloat(float value);

	/**
	*��ȡ8�ֽڵ�double
	*/
	double readDouble();
	ByteBuffer* writeDouble(double value);

	/**
	*�ַ���
	*/
	char* readString();
	ByteBuffer* writeString(const char* str);
	char* flush();
	int size();
	//�����
	vector<char> data;
private:
	///���ɵ��ַ��� 
	char* m_pData;
	//ƫ����
	int position;
	//����
	int capacity;
	
	//��Ҫ�ͷŵ�
	vector<char*> m_DeleteCharList;
};

#endif // !LIU_BYTE_BUFFER_H_
