#ifndef __NRF24L01_Ins_H__
#define __NRF24L01_Ins_H__


/**********  NRF24L01 引脚定义  ***********/
/**********  	 该引脚定义仅为控制时引用，需使用 STM32CubeMX 初始化下列引脚   ***********/
#define CSN_GPIOX 												NRF_CS_GPIO_Port
#define CSN_Pin_X 												NRF_CS_Pin										//需配置为推挽输出模式，默认高电平

#define CE_GPIOX 													NRF_CE_GPIO_Port
#define CE_Pin_X 													NRF_CE_Pin										//需配置为推挽输出模式，默认低电平

#define SCK_GPIOX 												SPI2_SCK_GPIO_Port
#define SCK_Pin_X 												SPI2_SCK_Pin											//需配置为推挽输出模式，默认低电平

#define MOSI_GPIOX 												SPI2_MOSI_GPIO_Port
#define MOSI_Pin_X 												SPI2_MOSI_Pin											//需配置为推挽输出模式，默认高电平

#define IRQ_GPIOX 												NRF_IRQ_GPIO_Port
#define IRQ_Pin_X 												NRF_IRQ_Pin										//需配置为上拉输入模式，默认高电平

#define MISO_GPIOX 												SPI2_MISO_GPIO_Port
#define MISO_Pin_X 												SPI2_MISO_Pin											//需配置为上拉输入模式，默认高电平

/*硬件 SPI 句柄地址*/
#define NRF_SPI_Handle										&hspi1

/**********  NRF24L01 发送接收数据宽度定义  ***********/
#define TX_ADR_WIDTH   										 5   													//5 字节的地址宽度
#define RX_ADR_WIDTH   										 5   													//5 字节的地址宽度
#define TX_PLOAD_WIDTH 										 32  													//32 字节的用户数据宽度
#define RX_PLOAD_WIDTH 										 32  													//32 字节的用户数据宽度

/**********  NRF24L01 寄存器操作指令  ***********/
#define NRF_READ_REG   										 0x00 												//读配置寄存器，低5位为寄存器地址
#define NRF_WRITE_REG  										 0x20 												//写配置寄存器，低5位为寄存器地址
#define RD_RX_PLOAD    										 0x61 												//读 RX 有效数据， 1 ~ 32 字节
#define WR_TX_PLOAD    										 0xA0 												//写 TX 有效数据， 1 ~ 32 字节
#define FLUSH_TX       										 0xE1 												//清除 TX FIFO 寄存器，用于发射模式
#define FLUSH_RX       										 0xE2 												//清除 RX FIFO 寄存器，用于接收模式
#define REUSE_TX_PL    										 0xE3 												//重新发送上一包数据， CE 为高，数据包被不断发送
#define NOP            										 0xFF 												//空操作，用于读状态寄存器
	 
/**********  SPI 通讯(NRF24L01)寄存器地址  ***********/
#define CONFIG          									 0x00 												/*	配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选 
																																						择;bit2:CRC模式;bit3:CRC使能;
																																						bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使 
																																						能;bit6:中断RX_DR使能		*/
#define EN_AA           									 0x01 												//使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       									 0x02 												//接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        									 0x03 												//设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      									 0x04 												//建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时250*x+86us
#define RF_CH           									 0x05 												//RF通道,bit6:0,工作通道频率;
#define RF_SETUP        									 0x06 												//RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          									 0x07 												/*	状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最 
																																						大:6);bit4,达到最多次重发
																																						bit5:数据发送完成中断;bit6:接收数据中断		*/
#define MAX_TX  													 0x10 												//达到最大发送次数中断
#define TX_OK   													 0x20 												//TX发送完成中断
#define RX_OK   													 0x40 												//接收到数据中断
#define OBSERVE_TX      									 0x08 												//发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              									 0x09 												//载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      									 0x0A 												//数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      									 0x0B 												//数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      									 0x0C 												//数据通道2接收地址,最低字节可设置，高字节,必须同 RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      									 0x0D 												//数据通道3接收地址,最低字节可设置，高字节,必须同 RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      									 0x0E 												//数据通道4接收地址,最低字节可设置，高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      									 0x0F 												//数据通道5接收地址,最低字节可设置，高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         									 0x10 												//发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        									 0x11 												//接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        									 0x12 												//接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        									 0x13 												//接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        									 0x14 												//接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        									 0x15 												//接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        									 0x16 												//接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS 									 0x17 												/*FIFO状态寄存器;bit0,RX FIFO寄存器空标志;
																																					bit1,RX FIFO满标志;bit2,3,保留
																																					bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一 数据包.0,不循环		*/

#endif
