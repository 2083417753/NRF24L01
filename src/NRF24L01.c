#include "NRF24L01_Ins.h"
#include "NRF24L01.h"

#include "string.h"
#include "stdio.h"
#include "stdarg.h"

#include "spi.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x01,0x23,0x45,0x67,0x89}; //NRF24L01 数据发送地址，需于接收地址相同
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x01,0x23,0x45,0x67,0x89}; //NRF24L01 数据接收地址，需于发送地址相同

/*------------------------------------------------
作用		: 设置 CSN 引脚电位
参数		: State: 设置的 CSN 引脚电位
返回值	: 无
------------------------------------------------*/
void NRF24L01_CSN(uint8_t State)
{
	HAL_GPIO_WritePin(CSN_GPIOX,CSN_Pin_X,(GPIO_PinState)State);													//设置 CSN 引脚电位
}

/*------------------------------------------------
作用		: 设置 CE 引脚电位
参数		: State: 设置的 CE 引脚电位
返回值	: 无
------------------------------------------------*/
void NRF24L01_CE(uint8_t State)
{
	HAL_GPIO_WritePin(CE_GPIOX,CE_Pin_X,(GPIO_PinState)State);														//设置 CE 引脚电位
}

/*------------------------------------------------
作用		: 设置 SCK 引脚电位
参数		: State: 设置的 SCK 引脚电位
返回值	: 无
------------------------------------------------*/
//void NRF24L01_SCK(uint8_t State)
//{
//	HAL_GPIO_WritePin(SCK_GPIOX,SCK_Pin_X,(GPIO_PinState)State);													//设置 SCK 引脚电位
//}

/*------------------------------------------------
作用		: 设置 MOSI 引脚电位
参数		: State: 设置的 MOSI 引脚电位
返回值	: 无
------------------------------------------------*/
//void NRF24L01_MOSI(uint8_t State)
//{
//	HAL_GPIO_WritePin(MOSI_GPIOX,MOSI_Pin_X,(GPIO_PinState)State);													//设置 MOSI 引脚电位
//}

/*------------------------------------------------
作用		: 获取 IRQ 引脚电位
参数		: 无
返回值	: IRQ 引脚电位
------------------------------------------------*/
uint8_t NRF24L01_IRQ()
{
	return HAL_GPIO_ReadPin(IRQ_GPIOX,IRQ_Pin_X);																					//返回 IRQ 引脚电位
}

/*------------------------------------------------
作用		: 获取 MISO 引脚电位
参数		: 无
返回值	: IRQ 引脚电位
------------------------------------------------*/
//uint8_t NRF24L01_MISO()
//{
//	return HAL_GPIO_ReadPin(MISO_GPIOX,MISO_Pin_X);																					//返回 MISO 引脚电位
//}

/*------------------------------------------------
作用		: 使用 HAL 库硬件 SPI2 或软件模拟交换一个字节数据
参数		: Dat_Tx: 待发送的数据
返回值	: 接收到的数据
------------------------------------------------*/
uint8_t SPI2_SwapByte(uint8_t Dat_Tx)
{
	uint8_t Dat_Rx = 0;
	
	/* 使用 HAL 库硬件 SPI2 交换一个字节数据 */
	HAL_SPI_TransmitReceive(NRF_SPI_Handle,&Dat_Tx,&Dat_Rx,1,1000);
	
	/* 使用软件模拟交换一个字节数据 */
//	for (uint8_t i = 0; i < 8; i++)
//	{
//		HAL_GPIO_WritePin(SCK_GPIOX, SCK_Pin_X, GPIO_PIN_RESET);

//		if (Dat_Tx&0x80)
//		{
//			HAL_GPIO_WritePin(MOSI_GPIOX, MOSI_Pin_X, GPIO_PIN_SET);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(MOSI_GPIOX, MOSI_Pin_X, GPIO_PIN_RESET);
//		}

//		Dat_Tx <<= 1;
//		Dat_Rx <<= 1;
//		
//		HAL_GPIO_WritePin(SCK_GPIOX, SCK_Pin_X, GPIO_PIN_SET);
//		
//		if(HAL_GPIO_ReadPin(MISO_GPIOX, MISO_Pin_X))
//		{
//			Dat_Rx++;
//		}
//		
//		HAL_GPIO_WritePin(SCK_GPIOX, SCK_Pin_X, GPIO_PIN_RESET);
//	}
	
	return Dat_Rx;                                                                        //返回接收到的数据
}

/*------------------------------------------------
作用		: 读 NRF24L01 寄存器
参数		: Reg: 指定寄存器地址
返回值	: 寄存器值
------------------------------------------------*/
uint8_t NRF24L01_Read_Reg(uint8_t Reg)
{
 	uint8_t Value;

	NRF24L01_CSN(0);                                                                      //使能 SPI 通讯
  SPI2_SwapByte(Reg);	                                                					        //发送寄存器值(位置)
	Value = SPI2_SwapByte(NOP);																														//获取寄存器值
	NRF24L01_CSN(1);                                                                      //失能 SPI 通讯

	return Value;                                                                  	      //返回寄存器值
}

/*------------------------------------------------
作用		: 写 NRF24L01 寄存器
参数		: Reg: 指定寄存器地址
参数		: Value: 待写入的值
返回值	: 状态值
------------------------------------------------*/
uint8_t NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value)
{
	uint8_t Status;

	NRF24L01_CSN(0);                                                                      //使能 SPI 通讯
  Status = SPI2_SwapByte(Reg);	                                                        //发送寄存器值(位置)，并读取状态值
	SPI2_SwapByte(Value);																																	//写入指定的值
	NRF24L01_CSN(1);                                                                      //失能 SPI 通讯

	return Status;                                                                        //返回状态值
}

/*------------------------------------------------
作用		: 在 NRF24L01 指定地址读出指定个数的字节数据
参数		: Reg: 指定寄存器地址
参数		: Buf: 存储所读出的数据的数组地址
参数		: Len: 需读出的字节个数
返回值	: 状态值
------------------------------------------------*/
uint8_t NRF24L01_Read_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
	uint8_t Status, i;
	
	NRF24L01_CSN(0);                                                                      //使能 SPI 通讯
	Status = SPI2_SwapByte(Reg);	                                                        //发送寄存器值(位置)，并读取状态值	
 	for(i = 0; i < Len; i ++)                                                             //循环指定次数
	{
		Buf[i] = SPI2_SwapByte(NOP);	                                                      //读出数据
	}
	NRF24L01_CSN(1);                                                                      //失能 SPI 通讯
	
	return Status;                                                                        //返回状态值
}

/*------------------------------------------------
作用		: 在 NRF24L01 指定地址写入指定个数的字节数据
参数		: Reg: 指定寄存器地址
参数		: Buf: 存储所读出的数据的数组地址
参数		: Len: 需读出的字节个数
返回值	: 状态值
------------------------------------------------*/
uint8_t NRF24L01_Write_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
	uint8_t Status, i;
	
	NRF24L01_CSN(0);                                                                      //使能 SPI 通讯
	Status = SPI2_SwapByte(Reg);                                                          //发送寄存器值(位置)，并读取状态值
	for(i = 0; i < Len; i ++)                                                             //循环指定次数
	{
		SPI2_SwapByte(*Buf ++);                                                             //写入数据
	}
	NRF24L01_CSN(1);                                                                      //失能 SPI 通讯
	
	return Status;                                                                        //返回状态值
}

/*------------------------------------------------
作用		: 检验 NRF24L01 是否通讯正常
参数		: 无
返回值	:  NRF24L01 通讯正常返回 0 ，反之返回 1
------------------------------------------------*/
uint8_t NRF24L01_Check(void)
{
	uint8_t check_in_buf[5] = {0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t check_out_buf[5] = {0x00};
	uint8_t i;
	NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR,check_in_buf,5);		     									  //写入 5 个字节的地址
	NRF24L01_Read_Buf(NRF_READ_REG + TX_ADDR,check_out_buf,5); 								            //读出写入的地址
	
	check_out_buf[0] = check_out_buf[0];
	check_out_buf[1] = check_out_buf[1];
	check_out_buf[2] = check_out_buf[2];
	check_out_buf[3] = check_out_buf[3];
	check_out_buf[4] = check_out_buf[4];
	
	for(i = 0;i < 5;i ++)
		if(check_out_buf[i]!=0XA5)                                                          //判断读出的地址是否为写入的地址
			break;
	if(i!=5)
		return 1;                                                           							 	//未检测到 NRF24L01 ，返回 1
	return 0;									                                                            //检测到 NRF24L01 ，返回 0
}

/*------------------------------------------------
作用		: 查询 NRF24L01 接收 FIFO 寄存器是否已满，若已满则返回 1 ，反之返回 0
参数		: 无
返回值	: 若接收 FIFO 寄存器已满返回 1 ，反之返回 0
------------------------------------------------*/
uint8_t NRF24L01_RX_FIFO_FULL(void)
{
	uint8_t State;
	State = NRF24L01_Read_Reg(NRF_READ_REG + STATUS);                                     //读取状态寄存器的值
	if((State & 0x0E) != 0x0E)                                                            //判断接收 FIFO 寄存器是否已满
		return 1;                                                                           //返回 1
	else
		return 0;                                                                           //返回 0
}

/*------------------------------------------------
作用		: 读出 NRF24L01 接收到数据
参数		: Buf: 存储所读出的数据的数组地址
返回值	: 接收成功返回 0 ，反之返回 1
------------------------------------------------*/
uint8_t NRF24L01_GetRxBuf(uint8_t *Buf)
{
	uint8_t State;
	State = NRF24L01_Read_Reg(NRF_READ_REG + STATUS);                                     //读取状态寄存器的值
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, State);                                    //清除 RX_RD 中断标志
	if(State & RX_OK)								                                                      //判断是否接收到数据
	{
		NRF24L01_CE(1);                                                                     //拉高 CE ，接收数据
		NRF24L01_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH);                                //读取接收到的数据，并存储到指定的数组里
		NRF24L01_Write_Reg(FLUSH_RX, NOP);                                                  //清除 RX_FIFO 寄存器 
		return 0;                                                                           //返回 0 
	}
	return 1;                                                                             //返回 1
}

/*------------------------------------------------
作用		: 写入 NRF24L01 发送的数据
参数		: Buf: 待发送的数据的数组地址
返回值	: 发送完成返回 TX_OK ，达到最大发送次数返回 MAX_TX ，发送失败返回 0
------------------------------------------------*/
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf, ...)
{
	char string[62] = {0};
	string[0] = 31;
	va_list arg;
	va_start(arg, Buf);
	vsprintf(string + 1, (char *)Buf, arg);
	va_end(arg);
	
	uint8_t State;
	NRF24L01_CE(0);												                                                //CE 置低进入待机模式，写入指令
  NRF24L01_Write_Buf(WR_TX_PLOAD, (uint8_t *)string, TX_PLOAD_WIDTH);                   //写入待发送的数据
 	NRF24L01_CE(1);                                                                       //CE 拉高，启动发送
	while(NRF24L01_IRQ() == 1);                                                           //等待发送完成
	State = NRF24L01_Read_Reg(NRF_READ_REG + STATUS);                                     //读取状态寄存器的值
	NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, State); 		                                //清除 TX_D S或 MAX_RT 中断标志
	if(State & TX_OK)	                                                                    //判断是否为发送成功中断
		return TX_OK;                                                                       //返回 TX_OK
	if(State & MAX_TX)                                                                    //判断是否为达到最大重发次数中断
	{
		NRF24L01_Write_Reg(FLUSH_TX, NOP);					                                        //清除 TX_FIFO 寄存器
		return MAX_TX;                                                                      //返回 MAX_TX
	}
	return 0;                                                                        	    //返回 0
}

/*------------------------------------------------
作用		: 将 NRF24L01 配置为接收模式（内部检测 NRF24L01 是否存在）
					建议: 接收模式下的 NRF24L01 可通过模块本身中断拉低 IRQ 引脚电位，故可配置外部中断实现接收数据
参数		: 无
返回值	: 无
------------------------------------------------*/
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CSN(1);																																			//设置 CSN 默认电平，禁止 SPI 传输
	NRF24L01_CE(0);																																				//CE 置低进入待机模式 1 ，配置寄存器要处于掉电或待机模式
	HAL_Delay(10);
	while(NRF24L01_Check()){}																															//检测 NRF24L01 是否存在，不存在则进入死循环
	
  NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0,(uint8_t *)RX_ADDRESS,RX_ADR_WIDTH);		//设置数据通道 0 接收地址
 	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA,0x01);    																		//使能通道 0 自动应答功能
 	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR,0x01);																		//使能通道 0 接收地址
 	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH,40);	    																		//设置工作通道频率，需与发送方相同
 	NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0,RX_PLOAD_WIDTH);													//设置接收数据通道 0 有效数据宽度
 	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP,0x0F);																		//设置数据传输率，发射功率，开启低噪声增益
	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0F);																			//配置基本工作模式的参数 PWR_UP,EN_CRC,16BIT_CRC,接收模式
  NRF24L01_CE(1); 																																			//CE 为高，进入接收模式
}

/*------------------------------------------------
作用		: 将 NRF24L01 配置为发送模式（内部检测 NRF24L01 是否存在）
参数		: 无
返回值	: 无
------------------------------------------------*/
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CSN(1);																																			//设置 CSN 默认电平，禁止 SPI 传输
	NRF24L01_CE(0);																																				//CE 置低进入待机模式 1 ，配置寄存器要处于掉电或待机模式
	HAL_Delay(10);
	while(NRF24L01_Check()){}																															//检测 NRF24L01 是否存在，不存在则进入死循环
	
  NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);				//设置数据发送地址 
 	NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); 		//设置数据通道 0 接收地址，配合接收方自动应答功能
 	NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA,0x01);     																	//使能 AUTO ACK 
  NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR,0x01); 																	//使能通道 0 接收地址
 	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR,0x1a);																	//设置自动重发间隔时间 586us ，自动重发次数 10 次
 	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH,40);     																	  //设置工作通道频率，需与接收方相同
  NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP,0x0f);  																	//设置数据传输率，发射功率，开启低噪声增益
 	NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG,0x0e);    																		//配置基本工作模式的参数 PWR_UP,EN_CRC,16BIT_CRC,发送模式,开启所有中断
	NRF24L01_CE(1);																																				//CE 为高大于 10us 后进入发送模式
	HAL_Delay(1);
}

/*------------------------------------------------
作用		: 读取 NRF24L01 的 IRQ 引脚电位
					在接收模式下当接收到数据时间为高电位，读取数据后由硬件重新置为低点位
参数		: 无
返回值	: IRQ 引脚电位
------------------------------------------------*/
uint8_t NRF24L01_Get_Value(void)
{
	return NRF24L01_IRQ();																																//返回 IRQ 引脚电位
}
