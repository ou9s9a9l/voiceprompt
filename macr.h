#ifndef MACRO_H
#define MACRO_H
#endif
/********************
*   系 统 宏 定 义  *
********************/
//    软件spi定义



//--------------------------SPI:SDO  in--------------------------//B1
#define    SDO_H         READ_PA5
//#define    SDO_L         READ_PA5=0
//--------------------------SPI:SDI out--------------------------//B0
#define    SDI_SET       _PA2=1
#define    SDI_CLR       _PA2=0
//--------------------------SPI:SCLK out--------------------------//A0
#define    SCLK_SET      _PA1=1
#define    SCLK_CLR      _PA1=0
//--------------------------SPI:CS  out--------------------------//A1
#define    nSET_SET      _PA0=1
#define    nSET_CLR      _PA0=0
//--------------------------IRQ  in--------------------------//A2
#define   SPI_CS     _PB4


/*------------------*
*   常 数 宏 定 义  *
*------------------*/
    /*Data type definations*/  


/*------------------*
*   功 能 宏 定 义  *
*------------------*/
# define SET_BIT8_FORMAT(Addr)     (*((volatile BYTEBIT *)&Addr))
# define SET_BYTE_DIV_2(Addr)      (*((volatile BYTE2HBYTE *)&Addr))

# define SET_BIT16_FORMAT(Addr)    (*((volatile WORDBIT *)&Addr))
# define SET_WORD_DIV_2(Addr)      (*((volatile WORD2BYTE *)&Addr))
# define SET_WORD_DIV_4(Addr)      (*((volatile WORD2HBYTE *)&Addr))

# define SET_BIT32_FORMAT(Addr)    (*((volatile DWORDBIT *)&Addr))
# define SET_DWORD_DIV_2(Addr)     (*((volatile DWORD2WORD *)&Addr))
# define SET_DWORD_DIV_4(Addr)     (*((volatile DWORD2BYTE *)&Addr))
# define SET_DWORD_DIV_8(Addr)     (*((volatile DWORD2HBYTE *)&Addr))

/********************
* 结 构 体 定 义 区 *
********************/

/*------------------*
*   8位变量位映射   *
*------------------*/
typedef struct BYTE_BIT
{
    unsigned BIT0:1;
    unsigned BIT1:1;
    unsigned BIT2:1;
    unsigned BIT3:1;
    unsigned BIT4:1;
    unsigned BIT5:1;
    unsigned BIT6:1;
    unsigned BIT7:1;
}BYTEBIT,*BYTEBIT1;



/*------------------*
*   常 数 宏 定 义  *
*------------------*/
#ifndef PDIR_OUT
    # define PDIR_OUT            0x01
#endif
#ifndef PDIR_IN             
    # define PDIR_IN             0x00
#endif



/*------------------*
* 硬 件 连 接 定 义 *
*------------------*/

/*---------------- PORT口绑定 ---------------*/
#ifdef PORTA
    # define PORTA_BIT     SET_BIT8_FORMAT(PORTA)
#endif
#ifdef PORTB
    # define PORTB_BIT     SET_BIT8_FORMAT(PORTB)
#endif
#ifdef PORTC
    # define PORTC_BIT     SET_BIT8_FORMAT(PORTC)
#endif
#ifdef PORTD
    # define PORTD_BIT     SET_BIT8_FORMAT(PORTD)
#endif
#ifdef PORTE
    # define PORTE_BIT     SET_BIT8_FORMAT(PORTE)
#endif
#ifdef PORTF
    # define PORTF_BIT     SET_BIT8_FORMAT(PORTF)
#endif
#ifdef PORTG
    # define PORTG_BIT     SET_BIT8_FORMAT(PORTG)
#endif


/*---------------- DDR 口绑定 ---------------*/
#ifdef DDRA
    # define DDRA_BIT     SET_BIT8_FORMAT(DDRA)
#endif
#ifdef DDRB
    # define DDRB_BIT     SET_BIT8_FORMAT(DDRB)
#endif
#ifdef DDRC
    # define DDRC_BIT     SET_BIT8_FORMAT(DDRC)
#endif
#ifdef DDRD
    # define DDRD_BIT     SET_BIT8_FORMAT(DDRD)
#endif
#ifdef DDRE
    # define DDRE_BIT     SET_BIT8_FORMAT(DDRE)
#endif
#ifdef DDRF
    # define DDRF_BIT     SET_BIT8_FORMAT(DDRF)
#endif
#ifdef DDRG
    # define DDRG_BIT     SET_BIT8_FORMAT(DDRG)
#endif

/*---------------- PIN 口绑定 ---------------*/
#ifdef PINA
    # define PINA_BIT     SET_BIT8_FORMAT(PINA)
#endif
#ifdef PINB
    # define PINB_BIT     SET_BIT8_FORMAT(PINB)
#endif
#ifdef PINC
    # define PINC_BIT     SET_BIT8_FORMAT(PINC)
#endif
#ifdef PIND
    # define PIND_BIT     SET_BIT8_FORMAT(PIND)
#endif
#ifdef PINE
    # define PINE_BIT     SET_BIT8_FORMAT(PINE)
#endif
#ifdef PINF
    # define PINF_BIT     SET_BIT8_FORMAT(PINF)
#endif
#ifdef PING
    # define PING_BIT     SET_BIT8_FORMAT(PING)
#endif

/*------------------*
*   端 口 宏 定 义  *
*------------------*/

/*-------------PORT位操作定义--------------*/
#ifdef PORTA
    # define _PA0        PORTA_BIT.BIT0
    # define _PA1        PORTA_BIT.BIT1
    # define _PA2        PORTA_BIT.BIT2
    # define _PA3        PORTA_BIT.BIT3
    # define _PA4        PORTA_BIT.BIT4
    # define _PA5        PORTA_BIT.BIT5
    # define _PA6        PORTA_BIT.BIT6
    # define _PA7        PORTA_BIT.BIT7
#endif
#ifdef PORTB
    # define _PB0        PORTB_BIT.BIT0
    # define _PB1        PORTB_BIT.BIT1
    # define _PB2        PORTB_BIT.BIT2
    # define _PB3        PORTB_BIT.BIT3
    # define _PB4        PORTB_BIT.BIT4
    # define _PB5        PORTB_BIT.BIT5
    # define _PB6        PORTB_BIT.BIT6
    # define _PB7        PORTB_BIT.BIT7
#endif
#ifdef PORTC
    # define _PC0        PORTC_BIT.BIT0
    # define _PC1        PORTC_BIT.BIT1
    # define _PC2        PORTC_BIT.BIT2
    # define _PC3        PORTC_BIT.BIT3
    # define _PC4        PORTC_BIT.BIT4
    # define _PC5        PORTC_BIT.BIT5
    # define _PC6        PORTC_BIT.BIT6
    # define _PC7        PORTC_BIT.BIT7
#endif
#ifdef PORTD
    # define _PD0        PORTD_BIT.BIT0
    # define _PD1        PORTD_BIT.BIT1
    # define _PD2        PORTD_BIT.BIT2
    # define _PD3        PORTD_BIT.BIT3
    # define _PD4        PORTD_BIT.BIT4
    # define _PD5        PORTD_BIT.BIT5
    # define _PD6        PORTD_BIT.BIT6
    # define _PD7        PORTD_BIT.BIT7
#endif
#ifdef PORTE
    # define _PE0        PORTE_BIT.BIT0
    # define _PE1        PORTE_BIT.BIT1
    # define _PE2        PORTE_BIT.BIT2
    # define _PE3        PORTE_BIT.BIT3
    # define _PE4        PORTE_BIT.BIT4
    # define _PE5        PORTE_BIT.BIT5
    # define _PE6        PORTE_BIT.BIT6
    # define _PE7        PORTE_BIT.BIT7
#endif
#ifdef PORTF
    # define _PF0        PORTF_BIT.BIT0
    # define _PF1        PORTF_BIT.BIT1
    # define _PF2        PORTF_BIT.BIT2
    # define _PF3        PORTF_BIT.BIT3
    # define _PF4        PORTF_BIT.BIT4
    # define _PF5        PORTF_BIT.BIT5
    # define _PF6        PORTF_BIT.BIT6
    # define _PF7        PORTF_BIT.BIT7
#endif
#ifdef PORTG
    # define _PG0        PORTG_BIT.BIT0
    # define _PG1        PORTG_BIT.BIT1
    # define _PG2        PORTG_BIT.BIT2
    # define _PG3        PORTG_BIT.BIT3
    # define _PG4        PORTG_BIT.BIT4
    # define _PG5        PORTG_BIT.BIT5
    # define _PG6        PORTG_BIT.BIT6
    # define _PG7        PORTG_BIT.BIT7
#endif


/*------------- DDR 位操作定义 -------------*/
#ifdef DDRA
    # define DDR_PA0     DDRA_BIT.BIT0
    # define DDR_PA1     DDRA_BIT.BIT1
    # define DDR_PA2     DDRA_BIT.BIT2
    # define DDR_PA3     DDRA_BIT.BIT3
    # define DDR_PA4     DDRA_BIT.BIT4
    # define DDR_PA5     DDRA_BIT.BIT5
    # define DDR_PA6     DDRA_BIT.BIT6
    # define DDR_PA7     DDRA_BIT.BIT7
#endif
#ifdef DDRB
    # define DDR_PB0     DDRB_BIT.BIT0
    # define DDR_PB1     DDRB_BIT.BIT1
    # define DDR_PB2     DDRB_BIT.BIT2
    # define DDR_PB3     DDRB_BIT.BIT3
    # define DDR_PB4     DDRB_BIT.BIT4
    # define DDR_PB5     DDRB_BIT.BIT5
    # define DDR_PB6     DDRB_BIT.BIT6
    # define DDR_PB7     DDRB_BIT.BIT7
#endif
#ifdef DDRC
    # define DDR_PC0     DDRC_BIT.BIT0
    # define DDR_PC1     DDRC_BIT.BIT1
    # define DDR_PC2     DDRC_BIT.BIT2
    # define DDR_PC3     DDRC_BIT.BIT3
    # define DDR_PC4     DDRC_BIT.BIT4
    # define DDR_PC5     DDRC_BIT.BIT5
    # define DDR_PC6     DDRC_BIT.BIT6
    # define DDR_PC7     DDRC_BIT.BIT7
#endif
#ifdef DDRD
    # define DDR_PD0     DDRD_BIT.BIT0
    # define DDR_PD1     DDRD_BIT.BIT1
    # define DDR_PD2     DDRD_BIT.BIT2
    # define DDR_PD3     DDRD_BIT.BIT3
    # define DDR_PD4     DDRD_BIT.BIT4
    # define DDR_PD5     DDRD_BIT.BIT5
    # define DDR_PD6     DDRD_BIT.BIT6
    # define DDR_PD7     DDRD_BIT.BIT7
#endif
#ifdef DDRE
    # define DDR_PE0     DDRE_BIT.BIT0
    # define DDR_PE1     DDRE_BIT.BIT1
    # define DDR_PE2     DDRE_BIT.BIT2
    # define DDR_PE3     DDRE_BIT.BIT3
    # define DDR_PE4     DDRE_BIT.BIT4
    # define DDR_PE5     DDRE_BIT.BIT5
    # define DDR_PE6     DDRE_BIT.BIT6
    # define DDR_PE7     DDRE_BIT.BIT7
#endif
#ifdef DDRF
    # define DDR_PF0     DDRF_BIT.BIT0
    # define DDR_PF1     DDRF_BIT.BIT1
    # define DDR_PF2     DDRF_BIT.BIT2
    # define DDR_PF3     DDRF_BIT.BIT3
    # define DDR_PF4     DDRF_BIT.BIT4
    # define DDR_PF5     DDRF_BIT.BIT5
    # define DDR_PF6     DDRF_BIT.BIT6
    # define DDR_PF7     DDRF_BIT.BIT7
#endif
#ifdef DDRG
    # define DDR_PG0     DDRG_BIT.BIT0
    # define DDR_PG1     DDRG_BIT.BIT1
    # define DDR_PG2     DDRG_BIT.BIT2
    # define DDR_PG3     DDRG_BIT.BIT3
    # define DDR_PG4     DDRG_BIT.BIT4
    # define DDR_PG5     DDRG_BIT.BIT5
    # define DDR_PG6     DDRG_BIT.BIT6
    # define DDR_PG7     DDRG_BIT.BIT7
#endif

/*------------- PIN 位操作定义 -------------*/
#ifdef PINA
    # define READ_PA0     PINA_BIT.BIT0
    # define READ_PA1     PINA_BIT.BIT1
    # define READ_PA2     PINA_BIT.BIT2
    # define READ_PA3     PINA_BIT.BIT3
    # define READ_PA4     PINA_BIT.BIT4
    # define READ_PA5     PINA_BIT.BIT5
    # define READ_PA6     PINA_BIT.BIT6
    # define READ_PA7     PINA_BIT.BIT7
    
    # define PIN_PA0      PINA_BIT.BIT0
    # define PIN_PA1      PINA_BIT.BIT1
    # define PIN_PA2      PINA_BIT.BIT2
    # define PIN_PA3      PINA_BIT.BIT3
    # define PIN_PA4      PINA_BIT.BIT4
    # define PIN_PA5      PINA_BIT.BIT5
    # define PIN_PA6      PINA_BIT.BIT6
    # define PIN_PA7      PINA_BIT.BIT7
#endif
#ifdef PINB
    # define READ_PB0     PINB_BIT.BIT0
    # define READ_PB1     PINB_BIT.BIT1
    # define READ_PB2     PINB_BIT.BIT2
    # define READ_PB3     PINB_BIT.BIT3
    # define READ_PB4     PINB_BIT.BIT4
    # define READ_PB5     PINB_BIT.BIT5
    # define READ_PB6     PINB_BIT.BIT6
    # define READ_PB7     PINB_BIT.BIT7
    
    # define PIN_PB0      PINB_BIT.BIT0
    # define PIN_PB1      PINB_BIT.BIT1
    # define PIN_PB2      PINB_BIT.BIT2
    # define PIN_PB3      PINB_BIT.BIT3
    # define PIN_PB4      PINB_BIT.BIT4
    # define PIN_PB5      PINB_BIT.BIT5
    # define PIN_PB6      PINB_BIT.BIT6
    # define PIN_PB7      PINB_BIT.BIT7
#endif
#ifdef PINC
    # define READ_PC0     PINC_BIT.BIT0
    # define READ_PC1     PINC_BIT.BIT1
    # define READ_PC2     PINC_BIT.BIT2
    # define READ_PC3     PINC_BIT.BIT3
    # define READ_PC4     PINC_BIT.BIT4
    # define READ_PC5     PINC_BIT.BIT5
    # define READ_PC6     PINC_BIT.BIT6
    # define READ_PC7     PINC_BIT.BIT7
    
    # define PIN_PC0      PINC_BIT.BIT0
    # define PIN_PC1      PINC_BIT.BIT1
    # define PIN_PC2      PINC_BIT.BIT2
    # define PIN_PC3      PINC_BIT.BIT3
    # define PIN_PC4      PINC_BIT.BIT4
    # define PIN_PC5      PINC_BIT.BIT5
    # define PIN_PC6      PINC_BIT.BIT6
    # define PIN_PC7      PINC_BIT.BIT7
#endif
#ifdef PIND
    # define READ_PD0     PIND_BIT.BIT0
    # define READ_PD1     PIND_BIT.BIT1
    # define READ_PD2     PIND_BIT.BIT2
    # define READ_PD3     PIND_BIT.BIT3
    # define READ_PD4     PIND_BIT.BIT4
    # define READ_PD5     PIND_BIT.BIT5
    # define READ_PD6     PIND_BIT.BIT6
    # define READ_PD7     PIND_BIT.BIT7
    
    # define PIN_PD0      PIND_BIT.BIT0
    # define PIN_PD1      PIND_BIT.BIT1
    # define PIN_PD2      PIND_BIT.BIT2
    # define PIN_PD3      PIND_BIT.BIT3
    # define PIN_PD4      PIND_BIT.BIT4
    # define PIN_PD5      PIND_BIT.BIT5
    # define PIN_PD6      PIND_BIT.BIT6
    # define PIN_PD7      PIND_BIT.BIT7
#endif
#ifdef PINE
    # define READ_PE0     PINE_BIT.BIT0
    # define READ_PE1     PINE_BIT.BIT1
    # define READ_PE2     PINE_BIT.BIT2
    # define READ_PE3     PINE_BIT.BIT3
    # define READ_PE4     PINE_BIT.BIT4
    # define READ_PE5     PINE_BIT.BIT5
    # define READ_PE6     PINE_BIT.BIT6
    # define READ_PE7     PINE_BIT.BIT7
    
    # define PIN_PE0      PINE_BIT.BIT0
    # define PIN_PE1      PINE_BIT.BIT1
    # define PIN_PE2      PINE_BIT.BIT2
    # define PIN_PE3      PINE_BIT.BIT3
    # define PIN_PE4      PINE_BIT.BIT4
    # define PIN_PE5      PINE_BIT.BIT5
    # define PIN_PE6      PINE_BIT.BIT6
    # define PIN_PE7      PINE_BIT.BIT7
#endif
#ifdef PINF
    # define READ_PF0     PINF_BIT.BIT0
    # define READ_PF1     PINF_BIT.BIT1
    # define READ_PF2     PINF_BIT.BIT2
    # define READ_PF3     PINF_BIT.BIT3
    # define READ_PF4     PINF_BIT.BIT4
    # define READ_PF5     PINF_BIT.BIT5
    # define READ_PF6     PINF_BIT.BIT6
    # define READ_PF7     PINF_BIT.BIT7
    
    # define PIN_PF0      PINF_BIT.BIT0
    # define PIN_PF1      PINF_BIT.BIT1
    # define PIN_PF2      PINF_BIT.BIT2
    # define PIN_PF3      PINF_BIT.BIT3
    # define PIN_PF4      PINF_BIT.BIT4
    # define PIN_PF5      PINF_BIT.BIT5
    # define PIN_PF6      PINF_BIT.BIT6
    # define PIN_PF7      PINF_BIT.BIT7
#endif
#ifdef PING
    # define READ_PG0     PING_BIT.BIT0
    # define READ_PG1     PING_BIT.BIT1
    # define READ_PG2     PING_BIT.BIT2
    # define READ_PG3     PING_BIT.BIT3
    # define READ_PG4     PING_BIT.BIT4
    # define READ_PG5     PING_BIT.BIT5
    # define READ_PG6     PING_BIT.BIT6
    # define READ_PG7     PING_BIT.BIT7
    
    # define PIN_PG0      PING_BIT.BIT0
    # define PIN_PG1      PING_BIT.BIT1
    # define PIN_PG2      PING_BIT.BIT2
    # define PIN_PG3      PING_BIT.BIT3
    # define PIN_PG4      PING_BIT.BIT4
    # define PIN_PG5      PING_BIT.BIT5
    # define PIN_PG6      PING_BIT.BIT6
    # define PIN_PG7      PING_BIT.BIT7
#endif

//#endif
