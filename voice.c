//  精简rdata2
//free(tmp)
//0.81版本  精简了radata3 缩小了5倍提高了可适应性
//解决输出不正确
//关于0011变成1100会有四个纪录数的解决
//n=0即可跳出 不用继续下面的
//search中的b没初始化为0
//判断不影响UDR1的值 赋值才影响
//c mkii shiyan shiyan   笔记本  新安村txt


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <AVR/sleep.h>
#include "nRF24L01.h"
//#include "nRF24L01_Reg.h"
#include "macr.h"
//#include "nRF24L01.h"
unsigned char *p;//flag
unsigned char *j=((unsigned char *)0x000800);//数据存储位置
#define  LENGTH 11
const unsigned int x_reserve[15][LENGTH];

#define size  350
#define N  8
#define DELAY 50
#define DEVIDE 9

#ifndef F_CPU
#define F_CPU 8000000
#endif
#include <util/delay.h>
#define ConSize 7
#define SentSize 2
struct word
{
	unsigned int con[ConSize];
	unsigned char sent[SentSize];
	struct word *next;
}*head,*pthis;
struct DoWith
{
	unsigned char howmany;
	unsigned int  what;
}DoWithS[20],factor[20];
void send_char(unsigned char *testbuffer);
unsigned char DoWithTiaoJian(unsigned int *rdat);
void searchL(void);

BYTEBIT  Cdata1,Cdata2;
#define FORMAT(Addr) SET_BIT8_FORMAT(Addr)
//#ifdef Cdata1
	#define Cdat1_0   Cdata1.BIT0
	#define Cdat1_1   Cdata1.BIT1
	#define Cdat1_2   Cdata1.BIT2
	#define Cdat1_3   Cdata1.BIT3
	#define Cdat1_4   Cdata1.BIT4
	#define Cdat1_5   Cdata1.BIT5
	#define Cdat1_6   Cdata1.BIT6
	#define Cdat1_7   Cdata1.BIT7
//#endif
//#ifdef Cdata2
	#define Cdat2_0   Cdata2.BIT0
	#define Cdat2_1   Cdata2.BIT1
	#define Cdat2_2   Cdata2.BIT2
	#define Cdat2_3   Cdata2.BIT3
	#define Cdat2_4   Cdata2.BIT4
	#define Cdat2_5   Cdata2.BIT5
	#define Cdat2_6   Cdata2.BIT6
	#define Cdat2_7   Cdata2.BIT7
//#endif
#define LED_OFF  _PA3=1;
#define LED_ON  _PA3=0;
#define LED_TWINKEL {LED_ON;_delay_ms(200);LED_OFF}
int hei[3]; 
unsigned int m_freedelay=0;					////k是rdata3里面的条件数//x代表rdata123的第几个 //n代表与条件 如果是1则成立h为已经成立条件数
volatile unsigned int delay=0,rx0x12=0,rx0x11=0;
unsigned int m_save,m_crc,m_count;
volatile char rdata[size]={0};
volatile char rdata1[size]={0};
volatile char rdatatemp[size]={0};
volatile unsigned int DoWithArray[80]={0};
unsigned int rdata2[11]={115,117,119,121,123,125,127,129,131,133};
volatile unsigned int rdata3[150]={0};
unsigned int rdata4[DELAY];
unsigned char Bcast[6][3];


void EEPROM_write(unsigned int uiAddress, unsigned char  ucData)
{
/*  Wait for completion of previous write */
while(EECR & (1<<EEPE))
;
/* Set up address and Data Registers */
EEAR = uiAddress;
EEDR = ucData;
/*  Write logical one to EEMPE */
EECR |= (1<<EEMPE);
/* Start eeprom write by setting EEPE */
EECR |= (1<<EEPE);
}
unsigned char  EEPROM_read( unsigned int uiAddress)
{
/* Wait for completion of previous write */
while(EECR & (1<<EEPE))
;
/* Set up address register */
EEAR = uiAddress;
/*  Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from Data Register */
return EEDR;
}


//发送一个字节 usart
void uart_sendB(unsigned char data) 
{
   while ( !( UCSR0A & (1<<UDRE0)) ); 
    UDR0=data;
}
//延迟

//定时器初始化

//串口初始化
void USART0_Init( void )
{
     UBRR0L=8;         //12m 76  8m 51 1m  12 4800   
//	 UCSR0A =(1<<U2X0);         
     UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	 UCSR0C =0x0e;// (1<<USBS0)|(3<<UCSZ0);
}

///////////////////////////////////////接收并储存到ram里//////////////////////////////////////////////////////////
void input_usart()
{
	//设置p为初始指针 j用为操作指针
	int a=0,b=0;
	p=j;
	while(1)
	{
		while ( !(UCSR0A & (1<<RXC0)) );
		b=UDR0;
		hei[0]=b;
		if(b!=0)
		{m_crc=b;
		m_crc++;}
		if(0x3c==b)	
		{
		LED_ON
		*j++=b;
			while(a<2500)//全数据4000   接收半数据封闭
			{
			while ( !(UCSR0A & (1<<RXC0)) );
			b=UDR0;
			if(b==0x7c)return;
			else *j++=b;
			a++;
			}
		
		}
	}
}
//crc 校验
unsigned int CRC(char* pBuf,int Len)
{
	unsigned int CRC = 0;
unsigned char R;
	int i,j,k,m;
	CRC=0;
		if(Len<=0) return 0;
		for(i=1;i<Len;i++)    {
		R=pBuf[i];
			for(j=0;j<8;j++)	
			{
			if (R>127) k=1;else k=0;
			R<<=1;
			if(CRC>0x7fff) m=1;	else m=0;
			if (k+m==1) k=1; 	else k=0;
			CRC<<=1;
			if (k==1) CRC^=0x1021;
			}
		}
		return CRC; 
}


////////////////////////////////////////判断////////////////////////////////////////////////////////////////////////////////
unsigned char Quest_len_int(unsigned int *Par)
{
unsigned char b=0;
while(*Par++!=0&&b<size)b++;
return b;
}
unsigned char Quest_len_char(unsigned char *Par)
{
unsigned char b=0;
while(*Par++!=0&&b<size)b++;
return b;
}
//Cdata1,Cdata2;
//0xff查询长度
void cut(unsigned int Num)
{
volatile unsigned char a,b,c,temp_len,count=0;

	for(b=0;b<Quest_len_int(rdata3);b++)
	if(rdata3[b]==Num)
	{
	temp_len=Quest_len_int(rdata3);
	for(;b<temp_len;b++)rdata3[b]=rdata3[b+1];
	}

	pthis=head;
	if(head==NULL)
	{
	return;
	}
while(pthis!=NULL)
	{

        


		//////////////////////成功
		if(Num==pthis->con[1])
		{
		factor[count].what=0;
		factor[count].howmany=0;
				
		}
	pthis=pthis->next;
	count++;
	}
	for(c=0;c<DELAY-10;c++)
	{
	if(Num==rdata4[c])
		rdata4[c]=0;
	}

/*	for(c=DELAY-9;c<DELAY;c++)
	{
	if(Num==rdata4[c])
		for(;c<DELAY-1;c++)////////////////////////////////
		rdata4[c]=rdata4[c+1];
	}*/

}
unsigned char find(unsigned int *rdat,unsigned int source)
{
	unsigned char b=0,temp;
	temp=Quest_len_int(rdat);
	if (rdat[DELAY-9]!=0&&0==temp)
	temp=DELAY-1;
	for (unsigned char a=0;a<=temp;a++)
	{if(rdat[a]==source)return 1;}
		
	return 0;
}

void add(unsigned char Data2,unsigned char Data1,volatile unsigned int Ident)
{
	
	volatile unsigned int dat=0;
	Cdata1=FORMAT(Data1);
	Cdata2=FORMAT(Data2);
	dat=Ident*8-31;
	if(Cdat1_7>Cdat2_7&&!find(rdata3,dat+0))
	{rdata3[Quest_len_int(rdata3)]=dat+0;
	//if(find(rdata2,(dat+0))&&(!find(rdata4,(dat+0))))addto4_last(dat+0);
	}

	if(Cdat1_6>Cdat2_6&&!find(rdata3,dat+1))
	{rdata3[Quest_len_int(rdata3)]=dat+1;
	}

	if(Cdat1_5>Cdat2_5&&!find(rdata3,dat+2))
	{rdata3[Quest_len_int(rdata3)]=dat+2;
	//if(find(rdata2,(dat+2))&&(!find(rdata4,(dat+2))))addto4_last(dat+2);
	}

	if(Cdat1_4>Cdat2_4&&!find(rdata3,dat+3))
	{rdata3[Quest_len_int(rdata3)]=dat+3;
	}

	if(Cdat1_3>Cdat2_3&&!find(rdata3,dat+4))
	{rdata3[Quest_len_int(rdata3)]=dat+4;
	//if(find(rdata2,(dat+4))&&(!find(rdata4,(dat+4))))addto4_last(dat+4);
	}

	if(Cdat1_2>Cdat2_2&&!find(rdata3,dat+5))
	{rdata3[Quest_len_int(rdata3)]=dat+5;
	}

	if(Cdat1_1>Cdat2_1&&!find(rdata3,dat+6))
	{rdata3[Quest_len_int(rdata3)]=dat+6;
	//if(find(rdata2,(dat+6))&&(!find(rdata4,(dat+6))))addto4_last(dat+6);
	}

	if(Cdat1_0>Cdat2_0&&!find(rdata3,dat+7))
	{rdata3[Quest_len_int(rdata3)]=dat+7;
	}
	
	if(Cdat1_0<Cdat2_0){cut((dat+7));}
	if(Cdat1_1<Cdat2_1){cut((dat+6));}
	if(Cdat1_2<Cdat2_2){cut((dat+5));}
	if(Cdat1_3<Cdat2_3){cut((dat+4));}
	if(Cdat1_4<Cdat2_4){cut((dat+3));}
	if(Cdat1_5<Cdat2_5){cut((dat+2));}
	if(Cdat1_6<Cdat2_6){cut((dat+1));}
	if(Cdat1_7<Cdat2_7){cut((dat+0));}

	
 }

void addto4(unsigned int Num)
{
	volatile  char b;
	for(b=0;b<(DELAY-11);b++)
	rdata4[b]=rdata4[b+1];
	rdata4[DELAY-11]=Num;
}

void Judge(void)//传入一个接收到的rxdata数组 这个数组依次和前一个比较结果存到类似   05 51  这样的结果中第五位为51
{
	int b=0,a=0;
//		for (b=0;b<150;b++)
//		rdata3[b]=0;
		for (b=DELAY-9;b<DELAY;b++)
		{
			rdata4[b]=0;
		}
		rdata4[41]=133;
		for (a=4;a<33;a++)
		{
		add(rdata1[a],rdata[a],a);                             //处理程序  
		} 
		for (a=110;a<132;a++)
		{
		add(rdata1[a],rdata[a],a);                             //处理程序
		}
	for(a=0;a<size;a++)
	rdata1[a]=rdata[a];
}


////////////////////////////////////////////////////////////////////



void addx(unsigned int q,unsigned int *t,unsigned char r)
{
for(int i=0;i<Quest_len_int(t);i++)
if (t[i]==q)
{
	return;
}
*(t+r)=q;	
}
///////////////////////////////////传入一个当前条件的指针，无输出  弄出已经输出列表
//void cut(int *q,int *t,int *r,char s)//q是现有的  t为要消除的 r是q的最大条件数目 s为t最大的条件
///////////////////////////////////传入一个指针  判断是否为以输出

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void searchF(void)
{


	
	volatile unsigned int a=0,b=0,c=0,d=1,e,n=0,o_count=0,temp,tempa=0,tempb=0,tempc=0;//f是条件后几个不发
//	if(1==m)return;
//	if(1==i){i=0;return;}

	pthis=head;
	if(head==NULL)
	{
	return;
	}
	for (a=0;a<80;a++)
	DoWithArray[a]=0;
	for (a=0;a<20;a++)
	{DoWithS[a].howmany=0;DoWithS[a].what=0;}
	DoWithTiaoJian(rdata3);
while(pthis!=NULL)
	{
	
		if(DEVIDE<o_count)break;//前45个条件
		n=1;
		b=0;
		for(a=0;a<150;a++)//延迟80秒
		{
			{b|=(rdata3[a]==pthis->con[1]);}
		}
 		n&=b;//rdata3 01 42  01  43    
		 b=0;
		 for(a=0;a<150;a++)//延迟80秒
		{
			{b|=(rdata3[a]==pthis->con[2]);}
		}
 		n&=b;     
		//头一个为不符合条件选项
		if(pthis->con[0]!=255)
		{
		b=0;
			for(a=0;a<DELAY;a++)
			{
			{b|=(rdata3[a]==pthis->con[0]);}
			}
 		if(b==1)n=0;
		}

		//////////////////////成功//1进来factor1=0通过
		if(n==1)
		{
		
			for (a=0;a<8;a++)
			{
			d=0;
			if(factor[o_count].what)
				{d=1;}//轨道边
			if (tempb>0&&0==d)
			{
				factor[o_count].howmany=tempb;
				factor[o_count].what=tempc;
			}
			if(DoWithS[a].what!=0)
			for (c=0;c<20;c++)
				if(DoWithS[a].what==factor[c].what)
					if(DoWithS[a].howmany<=factor[c].howmany
					||(5==factor[c].howmany&&7==DoWithS[a].howmany))d=1;//轨道中报完5报7
				if(d==0&&DoWithS[a].howmany!=0)
 				{
				if (o_count==0||o_count==8)temp=0;
				else temp=1;
				if (1==temp&&DoWithS[a].howmany!=6&&find(rdata3,x_reserve[DoWithS[a].what][0])&&find(rdata3,x_reserve[DoWithS[a].what][2]))
				{
					
						tempb=DoWithS[a].howmany;
						tempc=DoWithS[a].what;
						uart_sendB(DoWithS[a].howmany);
						uart_sendB(DoWithS[a].what);
						uart_sendB(o_count+1);
						uart_sendB(temp);
						
						if (DoWithS[a].howmany==3||DoWithS[a].howmany==7)
						tempa+=50;
						else if(DoWithS[a].howmany==4)
						tempa+=10;
						else if(DoWithS[a].howmany==5)
						tempa+=70;
						if (DoWithS[a].what<8)
						{
							tempa+=DoWithS[a].what;
						} 
						else if(DoWithS[a].what==10)
						tempa+=8;
						else if(DoWithS[a].what==12)
						tempa+=9;
						else if(DoWithS[a].what==14)
						tempa+=10;
						//tempa+=50;
						if(DoWithS[a].howmany==4)
						add2Bcast(tempa,0,131);//131女
						else
						add2Bcast(tempa,0,132);//131女
						uart_sendB(tempa);
						uart_sendB(0xFF);
						factor[o_count]=DoWithS[a];
							for (e=0;e<15;e++)
							{
								if(factor[e].what==factor[o_count].what)
								factor[e].howmany=factor[o_count].howmany;
							}
				}
				if (0==temp&&DoWithS[a].howmany!=6&&find(rdata3,x_reserve[DoWithS[a].what][1])&&find(rdata3,x_reserve[DoWithS[a].what][3]))
				{
				//		4 1 1	5 1 0	3 1 0
				//		4 2 0	5 2 1	3 2 1
						tempb=DoWithS[a].howmany;
						tempc=DoWithS[a].what;
					uart_sendB(DoWithS[a].howmany);
					uart_sendB(DoWithS[a].what);
					uart_sendB(o_count+1);
					uart_sendB(temp);
						if (DoWithS[a].howmany==3||DoWithS[a].howmany==7)
						tempa+=0;
						else if(DoWithS[a].howmany==4)
						tempa+=60;
						else if(DoWithS[a].howmany==5)
						tempa+=20;
						if (DoWithS[a].what<8)
						{
							tempa+=DoWithS[a].what;
						}
						else if(DoWithS[a].what==10)
						tempa+=8;
						else if(DoWithS[a].what==12)
						tempa+=9;
						else if(DoWithS[a].what==14)
						tempa+=10;
						if(DoWithS[a].howmany==4)
						add2Bcast(tempa,0,132);//改成131
						else add2Bcast(tempa,0,131);
						uart_sendB(tempa);
						uart_sendB(0xFF);
					factor[o_count]=DoWithS[a];
					for (e=0;e<15;e++)
					{
						if(factor[e].what==factor[o_count].what)
						factor[e].howmany=factor[o_count].howmany;
					}
				}

				}	
			}						
		}
	if (factor[o_count].howmany==6)
	{
		factor[o_count].howmany=0;
		factor[o_count].what=0;
	}
	pthis=pthis->next;
	o_count++;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////	

void input_rom(void)
{
	volatile unsigned char sum=0;
	volatile int	a=0,b=0;
	struct word *tmp;
	char array[3]={0};
	
while(EEPROM_read(++b)!=0x2e)
{
	b--;
	tmp=(struct word*)malloc(sizeof(struct word));
	 if (NULL == tmp) 
    { 
        exit (1); 
    } 
	unsigned int *x=tmp->con;
	unsigned char *y=tmp->sent;
	for(a=0;a<ConSize;a++)
	tmp->con[a]=0;
	for(a=0;a<SentSize;a++)
	tmp->sent[a]=0;
if(EEPROM_read(b)==0x3C)
	{
	b++;
	while(1)
		{
    	if(EEPROM_read(b)==0x3e)break;
		else 
		for (a=0;a<3;a++)
		array[a]=EEPROM_read(b++);
		*x++=atoi(array);
		}
	}

if(EEPROM_read(b)==0x3e)
	{
	b++;
	while(1)
		{
		if(EEPROM_read(b)==0x3c)break;
		else 
		*y++=EEPROM_read(b++);
		}
	sum++;
	}
		if(head==NULL)
		{
		head=tmp;
		pthis=head;
		}
		else
		{
		pthis->next=tmp;
		pthis=pthis->next;
		}
}
pthis->next=NULL;
//uart_sendB(sum);
//str_send(luxi);
}
void qingling(unsigned char *rdat,unsigned char length)
{
	unsigned int a;
	for(a=0;a<length;a++)
	rdat[a]=0;
}
//////////////////////////////////
void add2Bcast(unsigned char Num1,unsigned char Num2,unsigned char Num3)
{
if (Bcast[5][0]==0){Bcast[5][0]=Num1;Bcast[5][1]=Num2;Bcast[5][2]=Num3;}
else if (Bcast[4][0]==0){Bcast[4][0]=Num1;Bcast[4][1]=Num2;Bcast[4][2]=Num3;}
else if	(Bcast[3][0]==0){Bcast[3][0]=Num1;Bcast[3][1]=Num2;Bcast[3][2]=Num3;}
else if	(Bcast[2][0]==0){Bcast[2][0]=Num1;Bcast[2][1]=Num2;Bcast[2][2]=Num3;}
else if	(Bcast[1][0]==0){Bcast[1][0]=Num1;Bcast[1][1]=Num2;Bcast[1][2]=Num3;}
/*else 
{{for(b=0;b<5;b++)
Bcast[b]=Bcast[b+1];}
Bcast[5]=Num;}*/

}
void sendcast(void)
{
	if(Bcast[0][0]!=0)qingling(Bcast[0],3);
	m_count++;m_freedelay++;
	if(m_count<11)return;
	if (m_freedelay>200)
		{
			add2Bcast(0,0,122);//正定 3分钟一次
			m_freedelay=0;
		}
	volatile unsigned char b=0;
	if(Bcast[5][0]!=0)
	{
	m_freedelay=0;
	LED_ON
	b=send_int(Bcast[5][0],Bcast[5][1],Bcast[5][2]);
	if(b)
	for(b=5;b!=0;b--)
	{
		Bcast[b][0]=Bcast[b-1][0];
		Bcast[b][1]=Bcast[b-1][1];
		Bcast[b][2]=Bcast[b-1][2];
	}
	else hei[2]=b;		
	m_count=0;
	_delay_ms(100);
	LED_OFF
	}

}


void Timer_Init(void)
{
	cli();
	TCCR1B=0x05;// 1024  5devide
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	TIMSK1=0x00;//1开启中断
	sei();
}
void setsleep(void)
{
	SMCR|=(1<<SE);	
}
ISR(TIMER1_OVF_vect)
{
	hei[2]=1;	
}
int rxflag=0,succflag=0,count=0;
ISR(USART0_RX_vect)
{	
	int temp,a=0;
	if(UCSR0A & (1<<RXC0))
	temp=UDR0;
	if(temp==0x01)succflag=1;
	if(1==count&&temp==0x14)
	{sendcast();}
	if(1==count&&(temp==0x14||temp==0x15))
	 {
		 succflag=0;count=0;return;
	 }
	 if(1==count&&(temp==0x11||temp==0x12))
	 LED_ON
	if (count>(size-10))
	{
	for (a=0;a<size;a++)
	rdatatemp[a]=0;
	succflag=0;
	count=0;
	}
/*if (count>340)
{
	hei[1]=count;
}*/
	if(succflag)
	{
	 rdatatemp[count]=temp;
	
	
	 if (rdatatemp[count]==0x03)
	 {
		  m_save=CRC(rdatatemp,count-2);
		  m_crc=rdatatemp[count-2];
		  m_crc<<=8;
		  m_crc|=rdatatemp[count-1];
		  if(m_save!=m_crc||m_crc==0)
		  {
			
			  for (a=0;a<size;a++)
			  rdatatemp[a]=0;
			  hei[0]=m_crc;
		  }
		 for (int a=0;a<size;a++)
		 {
			 rdata[a]=rdatatemp[a];
		 }
	 if(rdatatemp[1]==0x12)
	 rx0x12=0;
	  if(rdatatemp[1]==0x11)
	 rx0x11=0;	 
	 count=-1;
	 succflag=0;
	 LED_OFF
	 
	  for (a=0;a<size;a++)
	rdatatemp[a]=0;
	 }
	  if (rxflag==1)      				          //转译标志
	 switch(rdatatemp[count])
	 {
		 case 0x81: rdatatemp[count-1]=0x01;count--;rxflag=0;break;
		 case 0x83: rdatatemp[count-1]=0x03;count--;rxflag=0;break;
		 case 0x90: rdatatemp[count-1]=0x10;count--;rxflag=3;break;
		 default:rxflag=0;break;
	 }
	 if (rdatatemp[count]==0x10&&rxflag!=3)
	 rxflag=1;     //设置转译标志	 
	 else rxflag=0;
	 count++;
	 //辨别终止位
	 }	 
}

const unsigned int x_reserve[15][LENGTH]=
{ {0,0,0,0,0},	{987,1011,0,0,16,115,115,116,70,116,116},//1
				{917,1003,0,0,34,117,117,118,58,118,118},//2
				{909,951,988,1012,26,119,119,120,82,120,120},//3
				{916,950,0,0,32,121,121,122,80,122,122},//4
				{911,953,910,952,26,123,123,124,82,124,124},//5
				{915,949,914,946,32,125,125,126,80,126,126},//6
				{912,952,0,0,28,127,127,128,84,128,128},//7
				{0,0,0,0,0},	  
				{0,0,0,0,0},
				{913,945,904,940,30,129,129,130,76,130,130},//10
				{0,0,0,0,0},	
				{18,939,0,0,18,131,131,132,76,132,132},//12
				{0,0,0,0,0},
				{918,50,0,0,34,133,133,134,50,134,134}};//14

													  


unsigned char DoWithTiaoJian(unsigned int *rdat)//左1右0
{
	volatile unsigned char a,b,c=0,DoWithCount=1,DoWithvalue=0;
	for (a=1;a<15;a++)
		for ( b=4;b<LENGTH;b++)
			for (c=0;c<150;c++)
				if (x_reserve[a][b]==rdat[c]&&x_reserve[a][b]!=0)
					{
					if(b==4&&find(rdata3,x_reserve[a][0])&&find(rdata3,x_reserve[a][2]))
					{DoWithArray[DoWithCount++]=x_reserve[a][b];
					DoWithArray[DoWithCount++]=a;}
					else if(b==8&&find(rdata3,x_reserve[a][1])&&find(rdata3,x_reserve[a][3]))
					{DoWithArray[DoWithCount++]=x_reserve[a][b];
					DoWithArray[DoWithCount++]=a;}
					if (b!=8&&b!=4)
					{
					DoWithArray[DoWithCount++]=x_reserve[a][b];
					DoWithArray[DoWithCount++]=a;
					}
					}
	
	if (DoWithCount>1)
	{
		for (a=1;a<40;a++)
			if (DoWithArray[a*2]==DoWithArray[a*2+2]&&DoWithArray[a*2]==DoWithArray[a*2+4]&&DoWithArray[a*2]==DoWithArray[a*2+6]&&DoWithArray[a*2]==DoWithArray[a*2+8]&&DoWithArray[a*2]==DoWithArray[a*2+10]&&DoWithArray[a*2]==DoWithArray[a*2+12]
			&&DoWithS[DoWithvalue-1].what!=DoWithArray[a*2]&&DoWithArray[a*2]!=0)
			{DoWithS[DoWithvalue].howmany=7;DoWithS[DoWithvalue].what=DoWithArray[a*2];DoWithvalue++;}
			else if (DoWithArray[a*2]==DoWithArray[a*2+2]&&DoWithArray[a*2]==DoWithArray[a*2+4]&&DoWithArray[a*2]==DoWithArray[a*2+6]&&DoWithArray[a*2]==DoWithArray[a*2+8]&&DoWithArray[a*2]==DoWithArray[a*2+10]
			&&DoWithS[DoWithvalue-1].what!=DoWithArray[a*2]&&DoWithArray[a*2]!=0)
			{DoWithS[DoWithvalue].howmany=6;DoWithS[DoWithvalue].what=DoWithArray[a*2];DoWithvalue++;}
			else if (DoWithArray[a*2]==DoWithArray[a*2+2]&&DoWithArray[a*2]==DoWithArray[a*2+4]&&DoWithArray[a*2]==DoWithArray[a*2+6]&&DoWithArray[a*2]==DoWithArray[a*2+8]
			&&DoWithS[DoWithvalue-1].what!=DoWithArray[a*2]&&DoWithArray[a*2]!=0)
			{DoWithS[DoWithvalue].howmany=5;DoWithS[DoWithvalue].what=DoWithArray[a*2];DoWithvalue++;}
			else if (DoWithArray[a*2]==DoWithArray[a*2+2]&&DoWithArray[a*2]==DoWithArray[a*2+4]&&DoWithArray[a*2]==DoWithArray[a*2+6]
			&&DoWithS[DoWithvalue-1].what!=DoWithArray[a*2]&&DoWithArray[a*2]!=0)
			{DoWithS[DoWithvalue].howmany=4;DoWithS[DoWithvalue].what=DoWithArray[a*2];DoWithvalue++;}
			else if(DoWithArray[a*2]==DoWithArray[a*2+2]&&DoWithArray[a*2]==DoWithArray[a*2+4]
			&&DoWithS[DoWithvalue-1].what!=DoWithArray[a*2]&&DoWithArray[a*2]!=0)
			{
				DoWithS[DoWithvalue].what=DoWithArray[a*2];
				DoWithS[DoWithvalue].howmany=3;
				DoWithvalue++;
				}
	}
}
int DupliDoWith(int x,int y)
{
	int b=1;
	int a=0;
	for(a=0;a<x;a++)
 	b|=(DoWithArray[y*2]==DoWithArray[y*2+2+a*2]);
	return b;
}
//	CLKPR = 0x80;2分频
//    CLKPR = 0x02;
//	setsleep();
//    asm("sleep");
//const unsigned int x_reserve[15][7]={ {0,0,0,0,0},{115,115,16,116,70,116,116},{117,117,34,118,58,118,118},{119,119,26,120,82,120,120},{121,121,32,122,80,122,122},{123,123,26,124,82,124,124},
//												  {125,125,32,126,80,126,126},{127,127,28,128,84,128,128},{0,0,0,0,0},	  {0,0,0,0,0},	  {129,129,30,130,76,130,130},
//		
//									   	  {0,0,0,0,0},	  {131,131,18,132,76,132,132},{0,0,0,0,0},	  {133,133,34,134,50,134,134}};
void rx12(void)
{
	volatile unsigned int a,b=0,c=0,tempNum;
	for (a=0;a<rdata[4];a++)
	{
		c=0;
		tempNum=rdata[a*2+6];
		tempNum|=rdata[a*2+7]<<8;
		if ((tempNum&0x0fff)<240)
		{
			if (tempNum&0x8000)
			{for (b=0;b<80;b++)
				{
					if(((tempNum&0x0fff)+1)==rdata3[b])
					c=1;
				}
				if (c==0)
				{
					hei[0]=(tempNum&0x0fff)+1;
					rdata3[Quest_len_int(rdata3)]=(tempNum&0x0fff)+1;
				//	if((((tempNum&0x0fff)+1)%2)==0)addto4((tempNum&0x0fff)+1);
				//	 if(find(rdata2,(tempNum&0x0fff)+1))
				//		addto4_last((tempNum&0x0fff)+1);//不可能在一分钟内这个消失的，所以不用判断是否在rdata4有
				//	else if(((tempNum&0x0fff)+1)>230)addto4((tempNum&0x0fff)+1);
				}
			}
				else
				cut((tempNum&0x0fff)+1);                          //处理程序
			}
		else if((tempNum&0x0fff)>888&&(tempNum&0x0fff)<1020)
				{
					if (tempNum&0x8000)
					{
						for (b=0;b<150;b++)
						{
						if(((tempNum&0x0fff)+1)==rdata3[b])
						c=1;
						}
						if (c==0)
						{
							hei[0]=(tempNum&0x0fff)+1;
							rdata3[Quest_len_int(rdata3)]=(tempNum&0x0fff)+1;
						}
					}
					else
					cut((tempNum&0x0fff)+1);
				}
	}
}	

////////////////////////////////////////////////////////////////
#define TEST   0
//////////////////////////////////////////
//                                      //
//                main                  //
//                                      //
//////////////////////////////////////////	
int main(void)
{ 
	volatile unsigned int a,b=0,c=0,d=0,e;
	volatile unsigned char temp;
	Init_MCU( );
	Init_SPI( );
	Timer_Init();
	

//	EEPROM_write(0xcd,0x2e);
	USART0_Init(); 
	L01_CE_LOW( );
	L01_Init();
	L01_SetTRMode( TX_MODE );
	L01_WriteHoppingPoint( 0 );
	
	LED_ON
	if (EEPROM_read(1)==0xFF)
	_delay_ms(1000);
	if(UCSR0A&(1<<RXC0))
	{b=UDR0;
	if(b==0xff)
	for (e=0;e<2000;e++)  
	EEPROM_write(e,0xff);
	}
  	LED_OFF

//	str_send(luxj);
	#if TEST==2
	while(1)
	{LED_ON
		b=send_int(11,13,15);
		LED_OFF
	_delay_ms(1000);}
	#elif TEST==1
	while(1)
	{uart_sendB(0xff);}
	#endif
	if (EEPROM_read(1)==0xFF)
	{
	input_usart();
	j=0x800;
	while(*j!=0xED)
	EEPROM_write(d++,*j++);
	uart_sendB(0x2e);
	LED_OFF//上一个在input_usart里
	}
	else 
	{
	EEPROM_write(0,0x3c);
	input_rom();
	LED_TWINKEL//init complete
	}
UCSR0B |=(1<<RXCIE0);
hei[1]=head;
     while(1)
 {
	/*if (factor[1].what!=2)
	{
		factor[1].what=2;
	}*/
	if (hei[1]!=head)
	{
		head=hei[1];
	}
	if (rdata[1]==0x11&&rx0x11==0)
	{

			Judge();
			rx0x11=1;
			searchF();
			searchL();
	}
	if (rdata[1]==0x12&&rx0x12==0)
	{
	
	rx12();
	rx0x12=1;
	searchF();
	searchL();
	
//	sendcast();
	}
	
	
 }
	 return 0;
}
void searchL(void)
{
	unsigned int a,b;
	if (find(rdata3,167)&&(!find(rdata4,167)))
	{
		add2Bcast(105,0,131);
		addto4(167);
//		uart_sendB(115);
	}
	if (find(rdata3,161)&&(!find(rdata4,161)))
	{
		add2Bcast(106,0,131);
		addto4(161);
//		uart_sendB(116);
	}
	if (find(rdata3,163)&&(!find(rdata4,163)))
	{
		add2Bcast(107,0,131);
		addto4(163);
//		uart_sendB(117);
	}
	if (find(rdata3,225)&&(!find(rdata4,225)))
	{
		add2Bcast(115,0,132);
		addto4(225);
//		uart_sendB(105);
	}
	if (find(rdata3,221)&&(!find(rdata4,221)))
	{
		add2Bcast(116,0,132);
		addto4(221);
//		uart_sendB(106);
	}
	if (find(rdata3,213)&&(!find(rdata4,213)))
	{
		add2Bcast(117,0,132);
		addto4(213);
//		uart_sendB(107);
	}
	if (find(rdata3,169)&&(!find(rdata4,169)))//s1jg
	{
		a=30;b=0;
		if (factor[8].howmany==4)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+50;}
		if (factor[8].howmany==5)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+10+50;}
		if (factor[8].howmany==0)
		{add2Bcast(111,0,132);addto4(169);return;}
		add2Bcast(111,a,132);
		addto4(169);
	//	uart_sendB(101);
	}
	if (find(rdata3,171)&&(!find(rdata4,171)))//s2jg
	{
		a=30;b=0;
		if (factor[8].howmany==4)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+50;}
		if (factor[8].howmany==5)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+10+50;}
		if (factor[8].howmany==0)
		{add2Bcast(112,0,132);addto4(171);return;}
		add2Bcast(112,a,132);
		addto4(171);
	//	uart_sendB(102);
	}
	if (find(rdata3,165)&&(!find(rdata4,165)))//s3jg
	{
		a=30;b=0;
		if (factor[8].howmany==4)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+50;}
		if (factor[8].howmany==5)
		{if (factor[8].what==10)
		b=8;
		else if(factor[8].what==12)
		b=9;
		else if(factor[8].what==14)
		b=10;
		else b=factor[8].what;
		a+=b+10+50;}
		if (factor[8].howmany==0)
		{add2Bcast(113,0,132);addto4(165);return;}
		add2Bcast(113,a,132);
		addto4(165);
	//	uart_sendB(103);
	}
	if (find(rdata3,211)&&(!find(rdata4,211)))//x1jg
	{
			a=30;b=0;
		if (factor[3].howmany==4)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b;}
		if (factor[3].howmany==5)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b+10;}
		if (factor[3].howmany==0)
		{add2Bcast(101,0,131);addto4(211);return;}
		add2Bcast(101,a,131);
		addto4(211);
//		uart_sendB(111);
	}
	if (find(rdata3,215)&&(!find(rdata4,215)))//x2jg
	{
			a=30;b=0;
		if (factor[3].howmany==4)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b;}
		if (factor[3].howmany==5)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b+10;}
		if (factor[3].howmany==0)
		{add2Bcast(102,0,131);addto4(215);return;}
		add2Bcast(102,a,131);
		addto4(215);
	//	uart_sendB(112);
	}
	if (find(rdata3,219)&&(!find(rdata4,219)))//x3jg
	{
			a=30;b=0;
		if (factor[3].howmany==4)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b;}
		if (factor[3].howmany==5)
		{if (factor[3].what==10)
		b=8;
		else if(factor[3].what==12)
		b=9;
		else if(factor[3].what==14)
		b=10;
		else b=factor[3].what;
		a+=b+10;}
		if (factor[3].howmany==0)
		{add2Bcast(103,0,131);addto4(219);return;}
		add2Bcast(103,a,131);
		addto4(219);
//		uart_sendB(113);
	}
		if (find(rdata3,7)&&(!find(rdata4,7))&&find(rdata3,971))
	{
		add2Bcast(104,0,131);
		addto4(7);
//		uart_sendB(114);//下
	}
		if (find(rdata3,41)&&(!find(rdata4,41)))
	{
		add2Bcast(114,0,132);
		addto4(41);
	//	uart_sendB(104);//上
	}
	if (find(rdata3,179)&&(!find(rdata4,179)))//n1jg 379bg
	{
		add2Bcast(121,0,131);
		addto4(179);
		//		uart_sendB(105);
	}
	if (find(rdata3,191)&&(!find(rdata4,191)))//n2jg 393bg
	{
		add2Bcast(122,0,131);
		addto4(191);
		//		uart_sendB(105);
	}
	if (find(rdata3,199)&&(!find(rdata4,199)))//n3jg  407bg
	{
		add2Bcast(123,0,131);
		addto4(199);
		//		uart_sendB(105);
	}
	if (find(rdata3,87)&&(!find(rdata4,87)))//njinzhan
	{
		add2Bcast(124,0,131);
		addto4(87);
		//		uart_sendB(105);
	}
	if (find(rdata3,189)&&(!find(rdata4,189)))//n1lq 412
	{
		add2Bcast(125,0,132);
		addto4(189);
		//		uart_sendB(105);
	}
	if (find(rdata3,185)&&(!find(rdata4,185)))//n2lq 398
	{
		add2Bcast(126,0,132);
		addto4(185);
		//		uart_sendB(105);
	}
	if (find(rdata3,177)&&(!find(rdata4,177)))//n3lq 384
	{
		add2Bcast(127,0,132);
		addto4(177);
		//		uart_sendB(105);
	}
}
int duplicate(int* p)
{
	int temp,a,b;
	temp=Quest_len_int(p);
	if (temp==0)return 0;
	temp--;
	for (b=0;b<temp;b++)
	{
		for (a=b+1;a<temp;a++)
		{
			if (p[b]==p[a])
			{
				return 1;
			}
		}
	}
	return 0;
}	
	



//322 321 330 312 4fa
/*	TIMSK2&=~((1<<TOIE2)|(1<<OCIE2A)|(1<<OCIE2B));
	ASSR |= (1<<AS2); //set Timer/Counter0 to be asynchronous
	TIMSK2 |= (1<<TOIE2); 
//	MCUCR = 0x70;//16有区别 //entering sleeping mode: power savSM 2 51e mode
	TCCR2B=0x05; // Write dummy value to Control register//分频
	TCNT2=0x00;		*/
		

//	Timer_Init();