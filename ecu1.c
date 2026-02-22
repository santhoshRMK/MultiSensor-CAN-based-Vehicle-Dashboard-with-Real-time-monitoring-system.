													 /*  Temperature Monitoring  */
#include <lpc21xx.h>
#include "lcd.h"

/*  typedef using  for  creating alias name of data types  */

typedef unsigned char u8;  
typedef signed char   s8;		
typedef unsigned short int  u16;
typedef signed short   int  s16;
typedef unsigned int  u32;
typedef signed   int  s32;
typedef float f32;
typedef double f64;

u8 stat;

/* SPI chip selection pin */

#define CS 7

/*structure for transfer the data through CAN */

struct st
{
	u32 id;
	u32 dlc;
	u32 rtr;
	u32 temp_val;
	u32 byteB;
}M1;

/* spi initialization */

void spi_init(void)
{
	PINSEL0 |=0X00001500;
	S0SPCCR  = 150;						  // to set 100kbps(The SPI rate may be
	S0SPCR   = 1<<5|1<<4|1<<3;  //spi module in master mode,and selecting mode 3...
	IODIR0  |= 1<<7;						// pin no 7 as output pin
	IOSET0  |=1<<7;							// set the pin no 7
}

u8 SPI0(u8 data)
{
    stat = S0SPSR;    //clear SPIF
    S0SPDR = data;   // load spi tx reg
    while(((S0SPSR>>7)&1)==0); // wait for transmission to complete
    return S0SPDR;    // read data from SPI data reg, place into buffer
}

f32 read_volt()
{
    u8 h,l;
    f32 adc=0;
    IOCLR0=1<<7;
    SPI0(0x06);
    h = SPI0(0X00);
    l = SPI0(0x00);
    IOSET0 = (1<<7);
    adc=((h&0x0f)<<8)|l; //upper 4 byte data only transfer
    adc=((adc*3.3)/4096); //analog value convert into digital value
    return adc;
}
void can_init(void)
{
	PINSEL1|=0X14000; // setting the pins 0.23 & 0.24 as RD2 And TD2...
	VPBDIV = 1;
	C2MOD=0X01; 			//Reset mode..
	AFMR=0X02;			  // Accepting the all msg without acceptance filtering..
	C2BTR=0X001C001D; // setting the BRP Value..
	C2MOD=0X00; 			//Normal mode...
}

void can_tx(struct st M1)
{
	C2TID1=M1.id;
	C2TFI1 &= ~(0xF << 16);
	C2TFI1|=(M1.dlc<<16);
	C2TFI1&=~(1<<30);
	C2TDA1=M1.temp_val;
	C2CMR=(1<<0)|(1<<5);// Start the transmission and selecting the transmit buffer 1...
	while(!(C2GSR&(1<<3))); // waiting for transmit buffer to complete..
}

int main()
{
	lcd_init();	//lcd initialization
	spi_init(); //spi initialization
	can_init(); // CAN initialization
	lcd_str("Temperature");
	M1.id=0x11; // id for spi node..
	M1.rtr=0;
	M1.dlc=4;
	while(1)
	{
		f32 temp = read_volt();
		temp=temp*100;  //that digital value convert into temprature
		M1.temp_val = temp; 
		lcd_cmd(0xc0);
		lcd_str("temp: ");
		lcd_float(temp);
		lcd_str("c");
		can_tx(M1);
		delay_ms(200);
	}
}
