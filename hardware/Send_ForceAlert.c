#include "sim900a.h"
#include "Motor.h"
#include "BMV31T001.h"
//发送警报
void SendForceAlert(s32 force) 
{
    char msg[64];
    snprintf(msg, sizeof(msg), "Force Alert: %dN", force);
    SIM900A_SendSMS("18318187023", msg);     //发短信给手机
	SIM900A_MakeCall("18318187023");         //打电话给手机
	                                         //小车停止运动代码
	if(SIM900A_MakeCall("PHONE_NUMBER") == 0) //如果电话接通了
	{
	      BMV31T001_playVoice(0x01, 0);  // 播放语音编号1
	}
}