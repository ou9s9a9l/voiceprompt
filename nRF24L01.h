/*
================================================================================
Copyright(c)  Yihe Embedded Studio, All rights reserved.
              www.yh-mcu.com
              http://yh-mcu.taobao.com

File Name   : nRF24L01.c
Description : nRF24L01 low level operations and configurations.
Author      : ShawZhang,LiYong
Date        : 2012-11-07 
Version     : 2.0
Compiler    : Any
Note        : None
================================================================================
*/
#ifndef nRF24L01_H_
#define nRF24L01_H_
#endif



/*
================================================================================
============================Configurations and Options==========================
================================================================================
*/
#define DYNAMIC_PACKET      1 //1:DYNAMIC packet length, 0:fixed
#define FIXED_PACKET_LEN    32//Packet size in fixed size mode
#define REPEAT_CNT          15//0-15, repeat transmit count
#define REPEAT_TIME			2000//250, 500, 750......4000
#define INIT_ADDR           0X99, 0X88, 0X21, 0X36, 0XAB
#define CHOICE      0
extern void Init_MCU(void);
extern void Init_SPI(void);

#if CHOICE==2//新版子
#define irq DDR_PD5
#define irq_P READ_PD5

#define csn_L _PD6=0
#define csn_H _PD6=1
#define ce_L _PD7=0
#define ce_H _PD7=1

#elif CHOICE==1//旧板子
#define irq DDR_PD6
#define irq_P READ_PD6

#define csn_L _PD7=0
#define csn_H _PD7=1
#define ce_L _PD5=0
#define ce_H _PD5=1

#elif CHOICE==0//单片机直连   没通
#define irq DDR_PB3
#define irq_P READ_PB3

#define csn_L _PD5=0
#define csn_H _PD5=1
#define ce_L _PD4=0
#define ce_H _PD4=1
#endif

void L01_CE_LOW(void);
void L01_CE_HIGH(void);
void L01_CSN_LOW(void);
void L01_CSN_HIGH(void);
//unsigned char SPI_ExchangeByte( unsigned char input )


/*Read a single register of nRF24L01*/
extern unsigned char L01_ReadSingleReg( unsigned char Addr );

/*Read several registers of nRF24L01*/
extern void L01_ReadMultiReg( unsigned char StartAddr, unsigned char nBytes, unsigned char *pBuff );

/*Write a single byte to a register*/
extern void L01_WriteSingleReg( unsigned char Addr, unsigned char Value );

/*Read several registers of nRF24L01*/
extern void L01_WriteMultiReg( unsigned char StartAddr, unsigned char *pBuff, unsigned char Length );

/*Flush the TX buffer*/
extern void L01_FlushTX( void );

/*Flush the RX buffer*/
extern void L01_FlushRX( void );

/*reuse payload in tx fifo*/
extern void L01_ReuseTXPayload( void );

/* nop operation of nRF24L01 */
void L01_Nop( void );

/*Reuse the last transmitted payload*/
extern void L01_ReuseTXPayload( void );

/* read statu register of nRF24l01*/
extern unsigned char L01_ReadStatusReg( void );

/*L01 clear IRQ*/
#define IRQ_ALL  ( (1<<RX_DR) | (1<<TX_DS) | (1<<MAX_RT) )
extern void L01_ClearIRQ( unsigned char IRQ_Source );

/*Read the IRQ source of nRF24L01+*/
extern unsigned char L01_ReadIRQSource( void );

/*Read the payload width of the top buffer of FIFO*/
extern unsigned char L01_ReadTopFIFOWidth( void );

/*Read the RX payload from internal buffer*/
extern unsigned char L01_ReadRXPayload( unsigned char *pBuff );

/*Write TX payload to a pipe and prx will return ack back*/
extern void L01_WriteTXPayload_Ack( unsigned char *pBuff, unsigned char nBytes );

/*write data in tx mode, and prx won't return ack back*/
extern void L01_WriteTXPayload_NoAck( unsigned char *Data, unsigned char Data_Length );

/*write data in tx fifo when rx mode*/
extern void L01_WriteRXPayload_InAck( unsigned char *pData, unsigned char Data_Length );

/*Write address for the own device*/
extern void L01_SetTXAddr( unsigned char *pAddr, unsigned char Addr_Length );

/*Write address for a RX pipe*/
extern void L01_SetRXAddr( unsigned char PipeNum, unsigned char *pAddr, unsigned char Addr_Length );

/*Set a hopping point for the device,must be 0-125*/
extern void L01_WriteHoppingPoint( unsigned char FreqPoint );

/*Set the device as TX or RX mode*/
typedef enum{ TX_MODE, RX_MODE }L01MD;
void L01_SetTRMode( L01MD mode );

/*Send the communication speed of the RF device*/
typedef enum{ SPD_250K, SPD_1M, SPD_2M  }L01SPD;
extern void L01_SetSpeed( L01SPD speed );

/*Send the transmitt power of the device*/
typedef enum{ P_F18DBM, P_F12DBM, P_F6DBM, P_0DBM }L01PWR;
extern void L01_SetPower( L01PWR power );

/*Set the nRF24L01 into Powerup mode*/
extern void L01_SetPowerUp( void );

/*Set the nRF24L01 into PowerDown mode*/
extern void L01_SetPowerDown( void );

/*Check the TX FIFO if is empty,1:empty, 0:not empty*/
extern unsigned char L01_IsTXEmpty( void );

/*Initialize the nRF24L01*/
extern void L01_Init( void );


//_nRF24L01_H_
/*
================================================================================
====================================End of file=================================
================================================================================
*/
