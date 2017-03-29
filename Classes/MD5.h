#pragma once
//MD5.h 
#ifndef BZF_MD5_H
#define BZF_MD5_H

#include <string>
#include <iostream>
using namespace std;

// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class MD5
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD5();
	MD5(const std::string& text);
	void update(const unsigned char *buf, size_type length);
	void update(const char *buf, size_type length);
	MD5& finalize();
	std::string hexdigest() const;
	friend std::ostream& operator<<(std::ostream&, MD5 md5);

private:
	void init();
	typedef unsigned char uint1; //  8bit
	typedef unsigned int uint4;  // 32bit
	enum {blocksize = 64}; // VC6 won't eat a const static int here

	void transform(const uint1 block[blocksize]);
	static void decode(uint4 output[], const uint1 input[], size_type len);
	static void encode(uint1 output[], const uint4 input[], size_type len);

	bool finalized;
	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
	uint4 state[4];   // digest so far
	uint1 digest[16]; // the result

	// low level logic operations
	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};

std::string md5(const std::string str);


struct MD5_DATA
{
	unsigned long data[4];
	bool operator < (const MD5_DATA& p) const
	{
		return memcmp(data,p.data,4*sizeof(long))>0;
	}
};

class CMD5
{
private:
#define uint8  unsigned char
#define uint32 unsigned long int

	struct md5_context
	{
		uint32 total[2];
		uint32 state[4];
		uint8 buffer[64];
	};

	void md5_starts( struct md5_context *ctx );
	void md5_process( struct md5_context *ctx, uint8 data[64] );
	void md5_update( struct md5_context *ctx, uint8 *input, size_t length );
	void md5_finish( struct md5_context *ctx, uint8 digest[16] );

public:
	//! construct a CMD5 from any buffer
	void GenerateMD5(unsigned char* buffer,size_t bufferlen);

	//! construct a CMD5
	CMD5();

	//! construct a md5src from char *
	CMD5(const char * md5src);

	//! construct a CMD5 from a 16 bytes md5
	CMD5(unsigned long* md5src);

	//! add a other md5
	CMD5 operator +(CMD5 adder);

	//! just if equal
	bool operator ==(CMD5 cmper);

	//! give the value from equer
	// void operator =(CMD5 equer);

	//! to a string
	string ToString();


	unsigned long m_data[4];
};

class CDes
{
protected:
	bool SubKey[16][48];
	void F_func(bool In[32], const bool Ki[48]);// f 函数
	void S_func(bool Out[32], bool In[48]);// S 盒代替
	void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
	void Xor(bool *InA, const bool *InB, int len);// 异或
	void RotateL(bool *In, int len, int loop);// 循环左移
	void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
	void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组
public:
	//加密文本,产生base64加密结果
	string Des_EncryptText(string text,string key);
	//解密文本，由base64解密成原始字符串
	string Des_DecryptText(string text,string key);


	// 加/解密 一个连续内存块，要求len必须8倍数
	void Des_Full(unsigned char* pdata,size_t len, bool bENCRYPT);
	// 加/解密 一个8字节块
	void Des_Block(char* Out, char* In, bool bENCRYPT);
	// 设置密钥
	void Des_SetKey(const char Key[8]);
	void Des_SetKey(const string &Key);
	//当前使用的key,即常见的56bit密码
	char FullKey[8];
};
#endif


