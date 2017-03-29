//
//  UMSocialUIObject.h
//  UmengGame
//
//  Created by yeahugo on 14-3-28.
//
//
#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import <Foundation/Foundation.h>
#import "UMSocial.h"
#include "CCUMTypeDef.h"

using namespace umeng::social;

@interface UMSocialUIObject : NSObject<UMSocialUIDelegate>
{
    std::function<void(int,int,const string&)> handler;
}

-(id)initWithCallback:(std::function<void(int,int,const string&)>)callBack;

@end

#endif