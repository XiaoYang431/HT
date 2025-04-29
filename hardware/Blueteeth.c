#include "ht32f5xxxx_01.h"
#include "Blueteeth.h"
#include "GPS.h"
#include "BM7701_00_1.h"
#include <string.h> // 用于 memcpy

#include <stdio.h>
#include <stdlib.h>

//获取gps信息
extern _SaveData Save_Data;
void moveDecimalPoint(const char *input, char *output, int maxlen);
uint8_t BM7701_00_1_bleProcess(void);
void _blueteech_delay(vu32 count);
//蓝牙连接状态
bool board_connect = false;
//rx状态
bool board_receive = false;
bool board_conIntv = false;

uint8_t flag=0;
uint8_t count=0;
uint8_t sel = 1;
uint8_t receiveBuf[40] = {0};
uint8_t BDAddress[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66}; //Device address
uint8_t BDName[] = {'B', 'M', 'C', '7', '7', 'M', '0', '0', '1'};//Device name
uint8_t Adata[] = {0x02, 0x01, 0x06}; 					//advertising data
uint8_t Sdata[] = {0x03, 0x02, 0x0f, 0x18};     //scan response data
//获取蓝牙状态
uint8_t Status;         
//------------------------------------------------------------------------------------------
 
char gpsdata_receive =0;
uint8_t blueteech_avaible_flag=0;
void processBluetoothGPS(_SaveData *saveData)
{
    gpsdata_receive = 0;
    BM7701_00_1_readData(receiveBuf, 0);

   
        char gps_str[40] = {0};
        

        // 拷贝整个GPS字符串，从[4]开始
        memcpy(gps_str, &receiveBuf[4], 33);
        

        // 检查是否以 T: 开头
if (gps_str[0] == 'T' && gps_str[1] == ':')
        {
            char *lat_start = gps_str + 2;  // 跳过 "T:"
            char *comma_pos = strchr(lat_start, ',');

            if (comma_pos != NULL)
            {
                char lat_buf[20] = {0};
                char lon_buf[20] = {0};

                // 拷贝纬度部分
                strncpy(lat_buf, lat_start, comma_pos - lat_start);
                lat_buf[comma_pos - lat_start] = '\0';

                // 拷贝经度部分
                strcpy(lon_buf, comma_pos + 1);

                // --- 手动移动小数点 ---
                moveDecimalPoint(lat_buf, saveData->latitude, latitude_Length);
                moveDecimalPoint(lon_buf, saveData->longitude, longitude_Length);

                saveData->isParseData = 1;
                gpsdata_receive = 1;
            }
            else
            {
                saveData->isParseData = 0;  // 没找到逗号
				return ;
            }
        }
        else
        {
            saveData->isParseData = 0;  // 格式错误
			return;
        }
    
}

// 将字符串小数点向右移动2位
void moveDecimalPoint(const char *input, char *output, int maxlen)
{
    int i = 0, j = 0;
    int decimal_pos = -1;
    int len = strlen(input);

    // 找小数点位置
    for (i = 0; i < len; i++)
    {
        if (input[i] == '.')
        {
            decimal_pos = i;
            break;
        }
    }

    if (decimal_pos == -1)
    {
        // 没找到小数点，直接拷贝
        strncpy(output, input, maxlen - 1);
        output[maxlen - 1] = '\0';
        return;
    }

    // 开始移动小数点
    j = 0;
    for (i = 0; i < len && j < maxlen - 1; i++)
    {
        if (i == decimal_pos)
            continue; // 跳过原本的小数点

        output[j++] = input[i];

        if (i == decimal_pos + 2 && j < maxlen - 1)
        {
            output[j++] = '.';  // 在新位置插入小数点（向右移2位）
        }
    }

    output[j] = '\0'; // 字符串结尾
}




//蓝牙（串口0）初始化
void BLUETEETH_Init(void)
{
    BM7701_00_1_Init(BM7701_00_1_BAUD_115200);
	BM7701_00_1_reset();
	
	while (sel != 10)
  {
    switch (sel)
    {
      case 1: if (BM7701_00_1_setAddress(BDAddress) == true) sel++;                        //Set address
        else sel = 0xFF; break;
      case 2: if (BM7701_00_1_setName(sizeof(BDName), BDName) == true) sel++;              //Set Bluetooth Name  (max:31)
        else sel = 0xFF; break;
      case 3: if (BM7701_00_1_setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7) == true) sel++; //Set AdvIntv
        else sel = 0xFF; break;
      case 4: if (BM7701_00_1_setAdvData(BM7701_00_1_APPEND_NAME, sizeof(Adata), Adata) == true) sel++; //Set AdvData
        else sel = 0xFF; break;
      case 5: if (BM7701_00_1_setScanData(sizeof(Sdata), Sdata) == true) sel++;            //Set ScanData
        else sel = 0xFF; break;
      case 6: if (BM7701_00_1_setTXpower(TX_POWER) == true) sel++;                         //Set TX power
        else sel = 0xFF; break;
      case 7: if (BM7701_00_1_setCrystalOffset(XTAL_CLOAD) == true) sel++;                     //Set Xtal Cload
        else sel = 0xFF; break;
      case 8: if (BM7701_00_1_setFeature(BM7701_00_1_FEATURE_DIR,BM7701_00_1_AUTO_SEND_SATUS) == true) sel++;            //Auto Send Satus
        else sel = 0xFF; break;
      case 9: if (BM7701_00_1_setAdvCtrl(BM7701_00_1_ENABLE) == true) {sel++;  blueteech_avaible_flag = 1;}               //Turn on broadcast
        else sel = 0xFF; break; 
      case 0xFF:   
      
        break;//Configure fail
		}
		//blueteech_delay(650);
	}
}
//------------------------------------------------------------------------------------------
//蓝牙（串口0） 发生数据空中断函数
//void USART0_IRQHandler(void)
//{
//  u8 data;
//  if( USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR) ) // 接收器 FIFO 就绪标志位
//    {                                                                                                                                                                                    
//     data = USART_ReceiveData(HT_USART0); // 接收数据时已经自动清除中断标志位了，不用手动清除。      
//    }             
//}
//------------------------------------------------------------------------------------------

//根据蓝牙状态执行相应动作
void Get_blueteech_status(void)
{
Status = BM7701_00_1_bleProcess();                                   //Read BLE status
  if (Status)
  {
    switch (Status)
    {
      case BM7701_00_1_API_CONNECTED:
        if (board_connect == false)
        {
          board_connect = true;
          board_receive = false;
        }
        break;
      case BM7701_00_1_API_DISCONNECTED:
        board_connect = false;
        board_receive = false;
        board_conIntv = false;
        break;
      case BM7701_00_1_DATA_RECEIVED:
        if (board_connect == true)
        {					
          board_receive = true;
        }
        break;
      case BM7701_00_1_API_ERROR:
					
        break;
    }		
  }
}


//蓝牙（串口0） 接收数据就绪函数
/*
void BLUETEETH_USART_Tx(uint8_t *TxBuffer, uint8_t length)
{
  int i;
 
  for (i = 0; i < length; i++)
  {
    while (USART_GetFlagStatus(HT_USART1, USART_FLAG_TXC) == RESET); // 判断是否 发送完成

	BM7701_00_1_writeData( TxBuffer[i],length);  
    //while (USART_GetFlagStatus(HT_USARTx, USART_FLAG_TXDE) == RESET); // 判断是否 发送完成
  }
}
*/
void GPS_To_BlueTooth(void)
{
    if (Save_Data.isParseData && Save_Data.isUsefull)
	{
		//复位标志位
	  Save_Data.isUsefull = false;
      Save_Data.isParseData = false;

	uint8_t buffer[64]; // 根据打包字段总长度估算
    uint8_t index = 0;

    // 拷贝字段到发送缓冲区
    memcpy(&buffer[index], (uint8_t*)Save_Data.UTCTime, sizeof(Save_Data.UTCTime));
    index += sizeof(Save_Data.UTCTime);

    memcpy(&buffer[index], (uint8_t*)Save_Data.latitude, sizeof(Save_Data.latitude));
    index += sizeof(Save_Data.latitude);

    memcpy(&buffer[index], (uint8_t*)Save_Data.N_S, sizeof(Save_Data.N_S));
    index += sizeof(Save_Data.N_S);

    memcpy(&buffer[index], (uint8_t*)Save_Data.longitude, sizeof(Save_Data.longitude));
    index += sizeof(Save_Data.longitude);

    memcpy(&buffer[index], (uint8_t*)Save_Data.E_W, sizeof(Save_Data.E_W));
    index += sizeof(Save_Data.E_W);

    buffer[index++] = Save_Data.isUsefull;

    // 发送数据
    BM7701_00_1_writeData(buffer, index);
    }

}

//获取蓝牙状态
uint8_t BM7701_00_1_bleProcess(void)
{
  uint8_t st = BM7701_00_1_API_ERROR;
  uint8_t lenth = 0;
  
  BM7701_00_1_readData(receiveBuf, lenth);
  switch (receiveBuf[1])
    {
      case 0x00:
        if (receiveBuf[0] == 0x00)
        {
          if ((receiveBuf[3] & 0x01) == 0x01)
          {
            st = BM7701_00_1_API_CONNECTED;

          }
          else st = BM7701_00_1_API_DISCONNECTED;

        }
        break;
      case 0xF2:
        if ((receiveBuf[0] == 0x00) && (receiveBuf[2] == 0xFF))
        {
          st = BM7701_00_1_DATA_RECEIVED;
        }
        break;
      default: break;
    }
  return st;
}

void blueteech_delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}



