/*
================================================================================
Copyright(c)  Yihe Embedded Studio, All rights reserved.
              www.yhmcu.com
              http://yhmcu.taobao.com

File Name   : nRF24L01.c
Description : nRF24L01 low level operations and configurations.
Author      : ShawZhang,LiYong
Date        : 2012-4-17 9:08:36
Version     : 2.0
Compiler    : Any
Note        : None
================================================================================
*/
//#define _nRF24L01_C_
#include "nRF24L01.h"
#include <avr/io.h>
#include "nRF24L01_Reg.h"
#include "macr.h"
typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;
#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <util/delay.h>
typedef enum
{
	BOOL_FALSE = 0,
	BOOL_TRUE  = 1
}BOOLEAN;
unsigned char CE_Status = 0;

unsigned char a=0;




void L01_CSN_LOW(void){csn_L;}
void L01_CSN_HIGH(void){csn_H;}
void L01_CE_LOW(void){ce_L;}
void L01_CE_HIGH(void){ce_H;}

void Init_SPI(void)
{
     SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);   // 主机模式，fck/16, SPI方式0
	 DDR_PB6=0;
//	DDR_PB4=1;
}
void Init_MCU(void)
{
     DDRA=0XFF;
	 PORTA=0XFF;
	 
	 DDRB=0XFf;             
     PORTB=0XFF;              //数码管位控制
	 
	 DDRC=0XFF;               //数码管段控制
	 PORTC=0Xff;             
	 
	 DDRD=0XfF;               //键盘接口
     PORTD=0XFf;   
	             //锁存关闭LED显
	
	
}

unsigned char SPI_ExchangeByte( unsigned char add )
{
   /* SPDR = input;
//	a=input;
	while ( !(SPSR & (1<<SPIF)) );
//	a=SPSR; 
	return SPDR;*/
	unsigned char clear;
SPDR=add;
while(!(SPSR&(1<<SPIF)));
clear=SPSR;  
return SPDR ;
}
/*
================================================================================
Function : L01_GetCEStatus( )
Description : Get the status of the CE PIN
Input : NONE
Output: 1:CE=1, 0:CE=0
================================================================================
*/
unsigned char L01_GetCEStatus( void )
{
	return _PB2;
}
/*
================================================================================
Function : L01_SetCE( )
Description : Set the CE PIN as 1 or 0
Input : -status, 1: CE=1, 0: CE=0
Output: None
================================================================================
*/
void L01_SetCE( unsigned char status )
{
	CE_Status = status;
	if( status == 0 )  { L01_CE_LOW( ); }
	else               { L01_CE_HIGH( ); }
}
/*
================================================================================
Function : L01_ReadSingleReg( )
Description : Read a single register of nRF24L01
Input : -Addr, The address of the register
Output: The value read from the register
================================================================================
*/
unsigned char L01_ReadSingleReg( unsigned char Addr )
{
    unsigned char btmp;
    L01_CSN_LOW( );
    btmp = SPI_ExchangeByte( R_REGISTER | Addr );
    btmp = SPI_ExchangeByte( 0xFF );
    L01_CSN_HIGH( );
    return btmp;
}
/*
================================================================================
Function : L01_ReadMultiReg( )
Description : Read several registers of nRF24L01
Input : -StartAddr, The start address of the registers
        -nBytes, How many registers do you want to read
        -pBuff, The buffer to save the values
Output: None
================================================================================
*/
/*void L01_ReadMultiReg( unsigned char StartAddr, unsigned char nBytes, unsigned char *pBuff )
{
    unsigned char btmp;
    L01_CSN_LOW( );
    SPI_ExchangeByte( R_REGISTER | StartAddr );
    for( btmp = 0; btmp < nBytes; btmp ++ )
    {
        *( pBuff + btmp ) = SPI_ExchangeByte( 0xFF );
    }
    L01_CSN_HIGH( );
}

================================================================================
Function : L01_WriteSingleReg( )
Description : Write a single byte to a register
Input : -Addr, The address of the register
        -Value, The value to be written
Output: None
================================================================================
*/
void L01_WriteSingleReg( unsigned char Addr, unsigned char Value )
{
	unsigned char tmp = L01_GetCEStatus( );
	L01_SetCE( 0 );
    L01_CSN_LOW( );
    SPI_ExchangeByte( W_REGISTER | Addr );
    SPI_ExchangeByte( Value );
    L01_CSN_HIGH( );
	L01_SetCE( tmp );
}
/*
================================================================================
Function : L01_WriteMultiReg( )
Description : Read several registers of nRF24L01
Input : -StartAddr, The start address of the registers
        -pBuff, The buffer store the values
        -Length, How many registers do you want to write
Output: None
================================================================================
*/
void L01_WriteMultiReg( unsigned char StartAddr, unsigned char *pBuff, unsigned char Length )
{
    unsigned char i;
	unsigned char tmp = L01_GetCEStatus( );
	L01_SetCE( 0 );
    L01_CSN_LOW( );
    SPI_ExchangeByte( W_REGISTER | StartAddr );
    for( i = 0; i < Length; i ++ )
    {
        SPI_ExchangeByte( *( pBuff + i ) );
    }
    L01_CSN_HIGH( );
	L01_SetCE( tmp );
}
/*
================================================================================
Function : L01_FlushTX( )
Description : Flush the TX buffer
Input : None
Output: None
================================================================================
*/
void L01_FlushTX( void )
{
    L01_CSN_LOW( );
    SPI_ExchangeByte( FLUSH_TX );
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_FlushRX( )
Description : Flush the RX buffer
Input : None
Output: None
================================================================================
*/
void L01_FlushRX( void )
{
    L01_CSN_LOW( );
    SPI_ExchangeByte( FLUSH_RX );
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_ReuseTXPayload( )
Description : Reuse the last transmitted payload
Input : None
Output: None
================================================================================
*/
void L01_ReuseTXPayload( void )
{
    L01_CSN_LOW( );
    SPI_ExchangeByte( REUSE_TX_PL );
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_Nop( )
Description : nop operation of nRF24L01
Input : None
Output: None
================================================================================
*/
void L01_Nop( void )
{
    L01_CSN_LOW( );
    SPI_ExchangeByte( L01_NOP );
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_ReadStatusReg( )
Description : Read statu register of nRF24L01
Input : None
Output: Statu register of nRF24L01
================================================================================
*/
unsigned char L01_ReadStatusReg( void )
{
    unsigned char Status;
    L01_CSN_LOW( );
    Status = SPI_ExchangeByte( R_REGISTER + L01REG_STATUS );
    L01_CSN_HIGH( );
    return Status;
}
/*
================================================================================
Function : L01_ClearIRQ( )
Description : Clear IRQ cuased by nRF24L01
Input : None
Output: None
================================================================================
*/
void L01_ClearIRQ( unsigned char IRQ_Source )
{
    unsigned char btmp = 0;

    IRQ_Source &= ( 1<<RX_DR ) | ( 1<<TX_DS ) | ( 1<<MAX_RT );
    btmp = L01_ReadStatusReg( );
    L01_CSN_LOW( );
	L01_WriteSingleReg( L01REG_STATUS, IRQ_Source | btmp );
    L01_CSN_HIGH( );
    L01_ReadStatusReg( );
}
/*
================================================================================
Function : L01_ReadIRQSource( )
Description : Read the IRQ source of nRF24L01+
Input : None
Output: IRQ source mask code
================================================================================
*/
unsigned char L01_ReadIRQSource( void )
{
    return ( L01_ReadStatusReg( ) & ( ( 1<<RX_DR ) | ( 1<<TX_DS ) | ( 1<<MAX_RT ) ) );
}
/*
================================================================================
Function : L01_ReadTopFIFOWidth( )
Description : Read the payload width of the top buffer of FIFO
Input : None
Output: The width of the pipe buffer
================================================================================
*/
unsigned char L01_ReadTopFIFOWidth( void )
{
    unsigned char btmp;
    L01_CSN_LOW( );
    SPI_ExchangeByte( R_RX_PL_WID );
    btmp = SPI_ExchangeByte( 0xFF );
    L01_CSN_HIGH( );
    return btmp;
}
/*
================================================================================
Function : L01_ReadRXPayload( )
Description : Read the RX payload from internal buffer
Input : -pBuff, buffer to store the data
Output: The length of data read
================================================================================
*/
unsigned char L01_ReadRXPayload( unsigned char *pBuff )
{
    unsigned char width, PipeNum;
    PipeNum = ( L01_ReadSingleReg( L01REG_STATUS ) >> 1 ) & 0x07;
    width = L01_ReadTopFIFOWidth( );

    L01_CSN_LOW( );
    SPI_ExchangeByte( R_RX_PAYLOAD );
    for( PipeNum = 0; PipeNum < width; PipeNum ++ )
    {
        *( pBuff + PipeNum ) = SPI_ExchangeByte( 0xFF );
    }
    L01_CSN_HIGH( );
    L01_FlushRX( );
    return width;
}
/*
================================================================================
Function : L01_WriteTXPayload( )
Description : Write TX payload to a pipe and prx will return ack back
Input : -PipeNum, number of the pipe
        -pBuff, A buffer stores the data
        -nBytes, How many bytes to be wrote to
Output: None
================================================================================
*/
void L01_WriteTXPayload_Ack( unsigned char *pBuff, unsigned char nBytes )
{
    unsigned char btmp;
    unsigned char length = ( nBytes > 32 ) ? 32 : nBytes;

    L01_FlushTX( );
    L01_CSN_LOW( );
    SPI_ExchangeByte( W_TX_PAYLOAD );
    for( btmp = 0; btmp < length; btmp ++ )
    {
        SPI_ExchangeByte( *( pBuff + btmp ) );
    }
   
	L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_WritePayload_NoAck( )
Description : write data in tx mode, and prx won't return ack back
Input : -Data, A buffer stores the address data
        -Data_Length, How many bytes of the data buff
Output: None
================================================================================
*/
void L01_WriteTXPayload_NoAck( unsigned char *Data, unsigned char Data_Length )
{
    if( Data_Length > 32 || Data_Length == 0 )
    {
        return ;
    }
    L01_CSN_LOW( );
    SPI_ExchangeByte( W_TX_PAYLOAD_NOACK );
    while( Data_Length-- )
    {
        SPI_ExchangeByte( *Data++ );
    }
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_WritePayload_InAck( )
Description : write data in tx fifo when rx mode
Input : -Data, A buffer stores the address data
        -Data_Length, How many bytes of the data buff
Output: None
================================================================================
*/
void L01_WriteRXPayload_InAck( unsigned char *pData, unsigned char Data_Length )
{
    unsigned char length = ( Data_Length > 32 ) ? 32 : Data_Length;
    unsigned char btmp;

    L01_CSN_LOW( );
    SPI_ExchangeByte( W_ACK_PAYLOAD );
    for( btmp = 0; btmp < length; btmp ++ )
    {
        SPI_ExchangeByte( *( pData + btmp ) );
    }
    L01_CSN_HIGH( );
}
/*
================================================================================
Function : L01_SetTXAddr( )
Description : Write address for the own device
Input : -pAddr, A buffer stores the address data
        -Addr_Length, How many bytes of the address
Output: None
================================================================================
*/
void L01_SetTXAddr( unsigned char *pAddr, unsigned char Addr_Length )
{
    unsigned char Length = ( Addr_Length > 5 ) ? 5 : Addr_Length;
    L01_WriteMultiReg( L01REG_TX_ADDR, pAddr, Length );
}
/*
================================================================================
Function : L01_SetRXAddr( )
Description : Write address for a RX pipe
Input : -PipeNum, number of the pipe
        -pAddr, A buffer stores the address data
        -Addr_Length, How many bytes of the address
Output: None
================================================================================
*/
void L01_SetRXAddr( unsigned char PipeNum, unsigned char *pAddr, unsigned char Addr_Length )
{
    unsigned char Length = ( Addr_Length > 5 ) ? 5 : Addr_Length;
    unsigned char pipe = ( PipeNum > 5 ) ? 5 : PipeNum;

    L01_WriteMultiReg( L01REG_RX_ADDR_P0 + pipe, pAddr, Length );
}
/*
================================================================================
Function : L01_SetSpeed )
Description : Send the communication speed of the RF device
Input :    speed,
Output: None
================================================================================
*/
void L01_SetSpeed( L01SPD speed )
{
	unsigned char btmp = L01_ReadSingleReg( L01REG_RF_SETUP );

	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	if( speed == SPD_250K )		//250K
	{
		btmp |= ( 1<<5 );
	}
	else if( speed == SPD_1M )   //1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( speed == SPD_2M )   //2M
	{
		btmp |= ( 1<<3 );
	}

	L01_WriteSingleReg( L01REG_RF_SETUP, btmp );
}
/*
================================================================================
Function : L01_SetPower )
Description : Send the transmitt power of the device
Input :    power
Output: None
================================================================================
*/
void L01_SetPower( L01PWR power )
{
    unsigned char btmp = L01_ReadSingleReg( L01REG_RF_SETUP ) & ~0x07;
    switch( power )
    {
        case P_F18DBM:
            btmp |= PWR_18DB;
            break;
        case P_F12DBM:
            btmp |= PWR_12DB;
            break;
        case P_F6DBM:
            btmp |= PWR_6DB;
            break;
        case P_0DBM:
            btmp |= PWR_0DB;
            break;
        default:
            break;
    }
    L01_WriteSingleReg( L01REG_RF_SETUP, btmp );
}
/*
================================================================================
Function : L01_WriteHoppingPoint( )
Description : Set frequency for the device,must be 0-125
Input : -FreqPoint, The hopping point
Output: None
================================================================================
*/
void L01_WriteHoppingPoint( unsigned char FreqPoint )
{
    L01_WriteSingleReg( L01REG_RF_CH, FreqPoint & 0x7F );
}
/*
================================================================================
Function : L01_IsTXEmpty( )
Description : Check the TX FIFO if is empty,1:empty, 0:not empty
Input : -FreqPoint, The hopping point
Output: 1:empty, 0:not empty
================================================================================
*/
unsigned char L01_IsTXEmpty( void )
{
	return L01_ReadSingleReg( L01REG_FIFO_STATUS ) & ( 1<<TX_EMPTY );
}

/*
================================================================================
Function : L01_SetTRMode( )
Description : Set the device as TX or RX mode
Input : -mode, the mode to be set
            @TX_MODE, TX mode
            @RX_MODE, RX mode
Output: None
================================================================================
*/
void L01_SetTRMode( L01MD mode )
{
    unsigned char controlreg = L01_ReadSingleReg( L01REG_CONFIG );
    if( mode == TX_MODE )       { controlreg &= ~( 1<<PRIM_RX ); L01_SetCE( 0 ); }
    else if( mode == RX_MODE )  { controlreg |= ( 1<<PRIM_RX ); L01_SetCE( 1 );}

    L01_WriteSingleReg( L01REG_CONFIG, controlreg );
}
/*
================================================================================
Function : L01_SetPowerDown( )
Description : Set the nRF24L01 into PowerDown mode
Input : None
Output: None
================================================================================
*/
void L01_SetPowerDown( void )
{
	unsigned char controlreg = L01_ReadSingleReg( L01REG_CONFIG );
    L01_WriteSingleReg( L01REG_CONFIG, controlreg & ( ~( 1<<PWR_UP ) ) );
}
/*
================================================================================
Function : L01_SetPowerUp( )
Description : Set the nRF24L01 into Powerup mode
Input : None
Output: None
================================================================================
*/
void L01_SetPowerUp( void )
{
	unsigned char controlreg = L01_ReadSingleReg( L01REG_CONFIG );
    L01_WriteSingleReg( L01REG_CONFIG, controlreg | ( 1<<PWR_UP ) );
}
/*
================================================================================
Function : L01_Init( )
Description : Initialize the nRF24L01
Input : None
Output: None
================================================================================
*/
void L01_Init( void )
{
    unsigned char addr[5] = {INIT_ADDR};

	volatile unsigned char tmp = L01_GetCEStatus( );
    L01_SetCE( 0 );
	L01_SetPowerDown( );
    L01_ClearIRQ( IRQ_ALL );
#if DYNAMIC_PACKET == 1
    //dynamic payload length
    L01_WriteSingleReg( L01REG_DYNPD, ( 1<<0 ) );//Enable pipe 0 dynamic payload length
    L01_WriteSingleReg( L01REG_FEATRUE, 0x06 );
    L01_ReadSingleReg(L01REG_DYNPD);//L01REG_DYNPD
    L01_ReadSingleReg(L01REG_FEATRUE);
	 a=L01_ReadSingleReg(L01REG_DYNPD);//L01REG_DYNPD
    a=L01_ReadSingleReg(L01REG_FEATRUE);
#elif DYNAMIC_PACKET == 0
    //Fixed packet length
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN );
#endif//DYNAMIC_PACKET

    L01_WriteSingleReg( L01REG_CONFIG,/* ( 1<<MASK_TX_DS ) |*////receive interrupt
                                      ( 1<<EN_CRC ) );     //Enable CRC, 1 byte
    L01_WriteSingleReg( L01REG_EN_AA, ( 1<<ENAA_P0 ) );   //Auto ack in pipe 0
    L01_WriteSingleReg( L01REG_EN_RXADDR, ( 1<<ERX_P0 ) );//Enable pipe 0 receive
    L01_WriteSingleReg( L01REG_SETUP_AW, AW_5BYTES );     //Address width : 5Byte
    L01_WriteSingleReg( L01REG_RETR, (((REPEAT_TIME/250)&0X0F)<<4) |
                        ( REPEAT_CNT & 0x0F ) );          //repeat SETTING
//						SPI_ExchangeByte( W_TX_PAYLOAD_NOACK );
    L01_WriteSingleReg( L01REG_RF_CH, 0x0 );             //Initial channel
    L01_WriteSingleReg( L01REG_RF_SETUP, 0x27 );		//0x27 0db  21 -18db
    L01_SetTXAddr( &addr[0], 5 );                          //Set TX address
    L01_SetRXAddr( 0, &addr[0], 5 );                       //Set RX address
	L01_SetPowerUp( );
	a=L01_ReadSingleReg(L01REG_RF_SETUP);
	irq=0;
}
unsigned char send_int(int tx_couter)
{
	uart_sendB(tx_couter);
/*unsigned char testbuffer[6]={"123"};
unsigned int itmp;
volatile unsigned char tmp=0;

            itmp = tx_couter;
            testbuffer[0] = ( itmp / 10000 ) + '0'-16;
            itmp %= 10000;
            testbuffer[1] = ( itmp / 1000 ) + '0'-16;
            itmp %= 1000;
            testbuffer[2] = ( itmp / 100 ) + '0'-16;
            itmp %= 100;
            testbuffer[3] = ( itmp / 10 ) + '0'-16;
            itmp %= 10;
            testbuffer[4] = itmp + '0'-16;
            testbuffer[5] = 0;
		L01_FlushRX( );
        L01_FlushTX( );
        L01_WriteTXPayload_Ack(testbuffer,5);//(INT8U*)"len", strlen( "len" )
        L01_CE_HIGH( );	// CE = 1,启动发射
		
	   while( ( tmp = L01_ReadIRQSource( ) ) == 0 );//itcmp>=10且brk=0时跳出
		L01_CE_LOW( );


	 if( tmp & ( 1<<MASK_TX_DS ) )
        {
		 L01_CE_LOW( );
		L01_ClearIRQ(112);
		return 1;
        }
        else if( tmp & ( 1<<MASK_MAX_RT ) )
        {
		 L01_CE_LOW( );
		L01_ClearIRQ(112);
		return 0;
        }
        else if( tmp & ( 1<<MASK_RX_DR )  )
        {
			
	
        }
		
        L01_CE_LOW( );	// 发射完毕，CE = 0，省电
	L01_ClearIRQ(112);
	  L01_CE_LOW( );	// 发射完毕，CE = 0，省电*/

}

/*
================================================================================
====================================End of file=================================
================================================================================
*/
