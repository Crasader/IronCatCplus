
#pragma once
#include <map>
#include <string>
using namespace std;


#ifdef __cplusplus

#define USING_NS_UM_SOCIAL      using namespace umeng::social
#else 
#define USING_NS_SOCIAL
#endif 
namespace umeng
{
	namespace social
	{
		enum SharePlatform
		{
			SINA,
			WEIXIN,
			WEIXIN_CIRCLE,
			QQ,
			QZONE,
			RENREN,
			DOUBAN,
			LAIWANG,
			LAIWANG_CIRCLE,
			YIXIN,
			YIXIN_CIRCLE,
			FACEBOOK,
			TWITTER,
			INSTAGRAM,
			SMS,
			EMAIL,
			TENCENT_WEIBO
		};
	}
}



