#include "sim900a.h"
#include "Motor.h"
#include <stdio.h>
//发送警报
void SendForceAlert(s32 force) 
{
    char msg[64];
    snprintf(msg, sizeof(msg), "Force Alert: %dN", force);
    SIM900A_SendENGSMS("OK", "13322749246");	//发短信给手机
	SIM900A_delay_ms(100);
	SIM900A_MakeCall("13322749246");         //打电话给手机
    SIM900A_delay_ms(5000); 	
	SIM900A_HangUp();                                         
	                                         //小车停止运动代码	
}