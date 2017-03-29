#pragma once
//”Œœ∑÷ß∏∂¥˙¬Î
enum PayCodeConstant
{
	PayCode_Empty,
	PayCode_Fish1, //
	PayCode_Fish2, //
	PayCode_Fish3, //π∫¬Ú¿Ò∞¸id=3 
	PayCode_Fish4, //
	PayCode_Fish5, //”„±“100		6.99‘™
	PayCode_Fish6, //”„±“500		9.99‘™
	PayCode_Fish7, //”„±“15000		29.99‘™
	PayCode_Fish8, //”„±“40000		49.99‘™
	PayCode_Fish9, //
	PayCode_Fish10, //
	PayCode_Fish11, //”„±“
	PayCode_Fish12, //”„±“
	PayCode_Fish13, //”„±“
	PayCode_Fish14, //”„±“
	PayCode_Fish15, //”„±“
	PayCode_Fish16, //”„±“
	PayCode_Fish17, //”„±“
	PayCode_Fish18, //”„±“
};

struct payInfo
{
    std::map<int,std::string> m_payCode;
    payInfo()
    {
        m_payCode[PayCode_Fish5] = "com.ninekey.ironcat.yb01";
         m_payCode[PayCode_Fish6] = "com.ninekey.ironcat.yb02";
         m_payCode[PayCode_Fish7] = "com.ninekey.ironcat.yb03";
         m_payCode[PayCode_Fish8] = "com.ninekey.ironcat.yb04";
         m_payCode[PayCode_Fish3] = "com.ninekey.ironcat.yb05";
    }
    std::string& getStringCode(int paycode)
    {
        return m_payCode[paycode];
    }
    static payInfo* getInstance()
    {
        static payInfo info;
        return &info;
    }
};

