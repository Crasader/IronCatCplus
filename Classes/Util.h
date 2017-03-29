#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#else 
#include "iconv/include/iconv.h"
#endif
#define UTIL_HOUR 3600
#define UTIL_MIN 60

USING_NS_CC;
using namespace std;


	//转化为字符串
inline string ConvertToString(int value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

inline int convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t iconvH = iconv_open(to_charset, from_charset);
	if (!iconvH)   return -1;
	memset(outbuf, 0, outlen);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	char* pTemp = inbuf;
	char** pIn = &pTemp;
	char** pOut = &outbuf;
	if (iconv(iconvH, pIn, &inlen, pOut, &outlen) == -1)
	{
		iconv_close(iconvH);
		return -1;
	}
#else
	if (iconv(iconvH, &inbuf, &inlen, &outbuf, &outlen) == -1)
	{
		iconv_close(iconvH);
		return -1;
	}
#endif
	iconv_close(iconvH);
	return 0;
}


inline string rand_str(int len)
{
	string p;
	for (int q = 0; q < len; q++)
	{
		if (rand() & 0x1)
			p += char(rand() % 26 + 'a');
		else
			p += char(rand() % 10 + '0');
	}
	return p;
}

inline string toUtf8(const char *inbuf)
{
	size_t inleng = strlen(inbuf);
	char* outbuf = new char[inleng * 2 + 2];
	std::string strRet;
	if (convert("gb2312", "utf-8", (char*)inbuf, inleng, outbuf, inleng * 2 + 2) == 0)
		strRet = outbuf;
	else
		return inbuf;
	delete[] outbuf;
	return strRet;
}

inline string toGbk(const char *inbuf)
{
	size_t inleng = strlen(inbuf);
	char* outbuf = new char[inleng * 2 + 2];
	std::string strRet;
	if (convert("utf-8", "gb2312", (char*)inbuf, inleng, outbuf, inleng * 2 + 2) == 0)
		strRet = outbuf;
	else
		return inbuf;
	delete[] outbuf;
	return strRet;
}


inline vector<string> parseUTF8(const string &str)
{
	int l = str.length();
	vector<string> ret;
	ret.clear();
	for (int p = 0; p < l;) {
		int size;
		unsigned char c = str[p];
		if (c < 0x80) {
			size = 1;
		}
		else if (c < 0xc2) {
		}
		else if (c < 0xe0) {
			size = 2;
		}
		else if (c < 0xf0) {
			size = 3;
		}
		else if (c < 0xf8) {
			size = 4;
		}
		else if (c < 0xfc) {
			size = 5;
		}
		else if (c < 0xfe) {
			size = 6;
		}
		std::string temp = "";
		temp = str.substr(p, size);
		ret.push_back(temp);
		p += size;
	}
	return ret;
}
//把字符串拆分，按照UTF8
inline string wrap(string str, int start, int end)
{
	vector<string> test = parseUTF8(str);
	if ((int)test.size() <= end) return str;
	std::string res;
	for (vector<string>::iterator iter = (test.begin() + start); iter != (test.begin() + end); iter++)
	{
		res += *iter;
	}
	return res;
}
	//格式化时间 1:10:10 时分秒
inline string formatTime(int firstTime, int scendTime)
{
	int len = firstTime > scendTime ? firstTime - scendTime : -(firstTime - scendTime);
	string result;
	if (len >= UTIL_HOUR)
	{ //有小时
		result.append(ConvertToString(len / UTIL_HOUR) + ":");
		len = len % UTIL_HOUR;
	}

	if (len >= UTIL_MIN)
	{
		result.append(ConvertToString(len / UTIL_MIN) + ":");
		len = len % UTIL_MIN;
	}
	else
		result.append("0:");

	result.append(ConvertToString(len));
	return result;
}


	//把时间转化为 2012年10月1日  
inline string forTimeToString(int time)
{
	time_t t = time;
	struct tm *tm;
	tm = gmtime(&t);
	string result;
	int year = tm->tm_year + 1900;
	result.append(ConvertToString(year) + ".");
	int mon = tm->tm_mon + 1;
	result.append(ConvertToString(mon) + ".");
	int day = tm->tm_mday;
	result.append(ConvertToString(day) + " ");
	int hour = tm->tm_hour + 8;
	result.append(ConvertToString(hour) + ":");
	int min = tm->tm_min;
	result.append(ConvertToString(min) + ":");
	int sec = tm->tm_sec;
	result.append(ConvertToString(sec));
	return result;
}

	//得到statr->end中的随机值
inline int getRand(int start, int end)
{
	return CCRANDOM_0_1()*(end - start + 1) + start;
}

	//12345 -> 12,345
inline string changeNumber(int number)
{
	string temp = ConvertToString(number);
	int len = temp.length();
	for (int index = (int)len - 3; index > 0; index -= 3)
	{
		temp.insert(index, ",");
	}
	return temp;
}
inline vector<string> splits(string str, string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作  
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

inline vector<string> splitsOnce(string str, string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			result.push_back(str.substr(pos - i+1,size-pos+i-pattern.size()-1));
			break;
			//i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//得到当前时间秒
inline unsigned int getSecNow()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//win32平台
	struct tm *tm;
	time_t timep;
	time(&timep);
	tm = localtime(&timep);
	return timep;
#else  
	//android、ios平台
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
#endif 
}

//判断两个时间相差几天
inline int getDisDay(int time1, int time2)
{
	 int oneDay = 24 * 3600;
	 int changeTime = 8 * 3600;
	 time1 = time1 + changeTime;
	 time2 = time2 + changeTime;
	 int day1 = time1 / oneDay + 1;
	 int day2 = time2 / oneDay + 1;
	 return abs(day1 - day2);
}
//判断是否是同一天
inline bool isInSameDay(int time1, int time2)
{
	 return getDisDay(time1, time2) == 0;
}
//得到char*的长度
inline int getCharLen(const char* c)
{
	 int i = 0;
	 for (const char *p = c; *p != '\0'; ++p) ++i;
	 return i;
}

//将vector<int>转化为一个用度号分隔的字符串
inline std::string vecIntStr(const vector<int>&vec)
{
	std::stringstream _buf;
	bool _noFirst = false;
	for (auto e : vec)
	{
		if (_noFirst)
		{
			_buf << ",";
		}

		_noFirst = true;
		_buf << e;
	}

	return _buf.str();
}

//将一个用度号分隔的字符串转化为vector<int>
inline void strVecInt(const std::string& str, std::vector<int>& vecInt)
{
	if (!str.empty())
	{
		auto _vecStr = splits(str, ",");
		int j = _vecStr.size();

		if (j > vecInt.size())
			vecInt.resize(j);

		for (int i = 0; i < j; ++i)
		{
			vecInt[i] = atoi(_vecStr[i].c_str());
		}
	}

}

inline std::string moneyToString(int money)  //将钱转化成字符串，并将大于10万的转化为以k为单位
{
	char _buff[32] = { 0 };
	if (money > 100000)
	{
		float fmoney = (float)money / 1000.0f;
		sprintf(_buff, "%.2fK", fmoney);
	}
	else
	{
		sprintf(_buff, "%d", money);
	}
	return _buff;
}

inline std::string intToString(int num)
{
	char _buff[16] = { 0 };
	sprintf(_buff, "%d", num);
	return _buff;
}


inline bool isnum(string s)
{
	stringstream sin(s);
	double t;
	char p;
	if (!(sin >> t))
		return false;
	if (sin >> p)
		return false;
	else
		return true;
}


#endif // !_UTIL_H_


