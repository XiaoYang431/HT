#include "sim900a.h"
#include "Motor.h"
#include <stdio.h>
//坑逝警报
void SendForceAlert(void) 
{

    //SIM900A_SendENGSMS("OK", "13322749246");	//坑短信给手机
	SIM900A_SendSms("17707592248", "fall over detected!");
	//SIM900A_MakeCall("13322749246"); 
	SIM900A_makeCall( "17707592248");
	//打电话给手机
    	
	                                       
	                                         //尝车坜止违动代砝	
}
