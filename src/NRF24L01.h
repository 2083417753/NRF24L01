#ifndef __NRF24L01_H__
#define __NRF24L01_H__

#include "stdint.h"

/**********  NRF24L01 接收模式  ***********/
void NRF24L01_RX_Mode(void);                        //将 NRF24L01 配置为接收模式，若配置失败则循环配置
uint8_t NRF24L01_RX_FIFO_FULL(void);                //查询 NRF24L01 接收 FIFO 寄存器是否已满，若已满则返回 1 ，反之返回 0
uint8_t NRF24L01_GetRxBuf(uint8_t *Buf);            //读出 NRF24L01 接收到数据

/**********  NRF24L01 发送模式  ***********/
void NRF24L01_TX_Mode(void);                        //将 NRF24L01 配置为发送模式，若配置失败则循环配置
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf, ...);			//写入 NRF24L01 发送的数据

/**********  读取 IRQ 引脚电位  ***********/
uint8_t NRF24L01_Get_Value(void);										//在接收模式下当接收到数据时间为高电位，读取数据后由硬件重新置为低点位

#endif
