#include "ByteBuffer.h"

//��������ش�С
ByteBuffer::ByteBuffer()
{
	this->m_pData = NULL;
	this->position = 0;
	this->capacity = BYTE_BUFFER_SIZE;
	this->data.reserve(BYTE_BUFFER_SIZE);
}

//ByteBuffer::ByteBuffer()
//{
//	this->m_pData = NULL;
//	this->position = 0;
//	this->capacity = 0;
//	this->data.reserve(0);
//}

ByteBuffer::ByteBuffer(char* buffer, int len)
{
	if (len < 0) return;
	this->m_pData = NULL;
	this->position = 0;
	this->data.reserve(len);
	for (int i = 0; i < len; ++i)
	{
		this->data.push_back(buffer[i]);
	}
}

void ByteBuffer::addBuffer(char* buffer, int len)
{
	for (int i = 0; i < len; ++i)
	{
		this->data.push_back(buffer[i]);
	}
}

ByteBuffer::~ByteBuffer(void)
{
	//log("ByteBuffer::~ByteBuffer__________________");
	if (m_pData != NULL) delete m_pData;
	m_pData = 0;
	if (!m_DeleteCharList.empty())
	{
		for (vector<char*>::iterator iter = m_DeleteCharList.begin(); iter != m_DeleteCharList.end(); iter++)
		{
			delete *iter;
		}
	}
}

//�õ�����
int ByteBuffer::getCapacity()
{
	return this->capacity;
}

//�õ�����ص�ǰλ��
int ByteBuffer::getPostion()
{
	return this->position;
}
/**
* �ж��ڵ�ǰλ�ú�����֮���Ƿ����κ�Ԫ��
* @return ���ҽ����˻����������ٻ���һ��Ԫ��ʱ���� true
*/
bool ByteBuffer::hasRemaining()
{
	return this->position < this->capacity;
}
/**
* ���ص�ǰλ��������֮���Ԫ������
* @return �˻������е�ʣ��Ԫ����
*/
int ByteBuffer::remaining()
{
	return this->capacity - this->position;
}

ByteBuffer* ByteBuffer::setPosition(int newPosition)
{
	this->position = newPosition;
	return this;
}

ByteBuffer* ByteBuffer::flip()
{
	this->capacity = this->position;
	this->position = 0;
	return this;
}

//void ByteBuffer::release()
//{
//	if (m_pData != NULL) delete m_pData;
//	m_pData = NULL;
//	if (!m_DeleteCharList.empty())
//	{
//		for (vector<char*>::iterator iter = m_DeleteCharList.begin(); iter != m_DeleteCharList.end(); iter++)
//		{
//			delete *iter;
//		}
//		m_DeleteCharList.clear();
//	}
//}

/***
*��ȡ1�ֽڵ�byte
*/
byte ByteBuffer::readByte()
{
	char result = this->data[position];
	++position;
	return result;
}

bool ByteBuffer::readBool()
{
	return this->readByte() == 1;
}

ByteBuffer* ByteBuffer::writeBool(bool val)
{
	this->writeByte(val ? 1 : 0);
	return this;
}

ByteBuffer* ByteBuffer::writeByte(byte value)
{
	this->data.push_back(value);
	++position;
	return this;
}

/**
*��ȡ2�ֽڵ�short
*/
short ByteBuffer::readShort()
{
	byte b1 = this->data[this->position];
	byte b2 = this->data[this->position + 1];
	this->position += 2;
	return (short)((b1 << 8) | (b2 & 0xff));
}

ByteBuffer* ByteBuffer::writeShort(short value)
{
	this->data.push_back((byte)(value >> 8));
	this->data.push_back((byte)value);
	this->position += 2;
	return this;
}

/***
*��ȡ4�ֽڵ�int
*/
int ByteBuffer::readInt()
{
	byte b1 = this->data[this->position];
	byte b2 = this->data[this->position + 1];
	byte b3 = this->data[this->position + 2];
	byte b4 = this->data[this->position + 3];
	this->position += 4;
	return (((b1) << 24) |((b2 & 0xff) << 16) |	((b3 & 0xff) << 8) | ((b4 & 0xff)));

}

ByteBuffer* ByteBuffer::writeInt(int value)
{
	this->data.push_back((byte)(value >> 24));
	this->data.push_back((byte)(value >> 16));
	this->data.push_back((byte)(value >> 8));
	this->data.push_back((byte)value);
	this->position += 4;
	return this;
}

ByteBuffer* ByteBuffer::writeInt(int index, int value)
{
	this->data.insert(this->data.begin() + index, (byte)(value >> 24));
	this->data.insert(this->data.begin() + index + 1, (byte)(value >> 16));
	this->data.insert(this->data.begin() + index + 2, (byte)(value >> 8));
	this->data.insert(this->data.begin() + index + 3, (byte)(value));
	this->position += 4;
	return this;
}

/***
*��ȡ8�ֽڵ�long
*/
long long ByteBuffer::readLong()
{
	byte b1 = this->data[this->position];
	byte b2 = this->data[this->position + 1];
	byte b3 = this->data[this->position + 2];
	byte b4 = this->data[this->position + 3];
	byte b5 = this->data[this->position + 4];
	byte b6 = this->data[this->position + 5];
	byte b7 = this->data[this->position + 6];
	byte b8 = this->data[this->position + 7];
	this->position += 8;

	return ((((long long)b1 & 0xff) << 56) + (((long long)b2 & 0xff) << 48) +
		   (((long long)b3 & 0xff) << 40) +(((long long)b4 & 0xff) << 32) +
		   (((long long)b5 & 0xff) << 24) + (((long long)b6 & 0xff) << 16) +
		   (((long long)b7 & 0xff) << 8) +(((long long)b8 & 0xff)));

	/*long long addr = ((long long)data[position + 7] & 255L)
		+ (((long long)data[position + 6] & 255L) << 8)
		+ (((long long)data[position + 5] & 255L) << 16)
		+ (((long long)data[position + 4] & 255L) << 24)
		+ (((long long)data[position + 3] & 255L) << 32)
		+ (((long long)data[position + 2] & 255L) << 40)
		+ (((long long)data[position + 1] & 255L) << 48)
		+ (((long long)data[position] & 255L) << 56);

	this->position += 8;

	return addr;*/
}

ByteBuffer* ByteBuffer::writeLong(long long value)
{
	this->data.push_back((byte)(value >> 56));
	this->data.push_back((byte)(value >> 48));
	this->data.push_back((byte)(value >> 40));
	this->data.push_back((byte)(value >> 32));
	this->data.push_back((byte)(value >> 24));
	this->data.push_back((byte)(value >> 16));
	this->data.push_back((byte)(value >> 8));
	this->data.push_back((byte)value);
	this->position += 8;
	return this;
}

/**
*��ȡ4�ֽڵ�float
*/
float ByteBuffer::readFloat()
{
	int temp = this->readInt();
	float result;
	memcpy(&result, &temp, 4);
	return result;
}

ByteBuffer* ByteBuffer::writeFloat(float value)
{
	int result;
	memcpy(&result, &value, 4);
	this->writeInt(result);
	return this;
}

/**
*��ȡ8�ֽڵ�double
*/
double ByteBuffer::readDouble()
{
	long long temp = this->readLong();
	double result;
	memcpy(&result, &temp, 8);
	return result;
}

ByteBuffer* ByteBuffer::writeDouble(double value)
{
	long long result;
	memcpy(&result, &value, 8);
	this->writeLong(result);
	return this;
}

char* ByteBuffer::readString()
{
	int length = this->readInt();
	char* pUTF = new char[length + 1];
	for (int i = 0; i < length; ++i, ++position)
	{
		pUTF[i] = data[position];
	}
	pUTF[length] = '\0';
	m_DeleteCharList.push_back(pUTF);
	return pUTF;
}

ByteBuffer* ByteBuffer::writeString(const char* str)
{
	int size = strlen(str);
	this->writeInt(size);
	for (int i = 0; i < size; ++i){
		this->data.push_back(str[i]);
	}
	return this;
}


char* ByteBuffer::flush()
{
	int nBufferSize = data.size();
	if (nBufferSize != 0)
	{
		int nIdx = 0;
		//��̬����Ķ��ڴ棬ע���ͷţ�
		this->m_pData = new char[nBufferSize];
		for (vector<char>::iterator iter = data.begin(); iter != data.end(); ++iter, ++nIdx)
		{
			this->m_pData[nIdx] = *iter;

		}
		return this->m_pData;
	}
	return NULL;
}

int ByteBuffer::size()
{
	return (this->data.size());
}

