	/*  ECU-2 Fuel Monitoring node  */
#include <lpc21xx.h>
#include "lcd.h"
#include "header.h"

#define TRIG (1<<10)
#define ECHO (1<<11)

unsigned int count, distance;

int main(){	

	CAN2_MSG m1;
	can2_init(); // CAN Initialization
	lcd_init();  // LCD Initialization
	
	/*sending data frame*/
	m1.id=0x1EF;
	m1.rtr=0;//data frame
	m1.dlc=4;

    IODIR0 |= TRIG;
    IODIR0 &= ~ECHO;

    while(1)
    {
        /* Trigger pulse */
        IOSET0 = TRIG;
        delay(10);
        IOCLR0 = TRIG;

        /* Wait for echo */
			
        while(!(IOPIN0 & ECHO));
        count = 0;

				/* Measure Echo Duration	*/
			
        while(IOPIN0 & ECHO)
            count++;

        distance = count / 58;	  //Convert to Distance

				/* CAN Frame */
				m1.byteA=distance;
				m1.byteB=0;	
			
				can2_tx(m1);	//Transmit CAN Message


			/* Fuel Level Logic	*/
		  if(m1.byteA <=8)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :95%    ");
			}
			else if(m1.byteA > 8 && m1.byteA <=15)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :70%     ");
			}
			else if(m1.byteA > 15 && m1.byteA <=22)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :50%     ");
			}
			else if(m1.byteA > 22 && m1.byteA <=30)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :30%     ");
			}
			else if(m1.byteA > 30 && m1.byteA <=35)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :20%     ");
			}
			else if(m1.byteA > 35)
			{
					lcd_cmd(0x80);
					lcd_str("Fuel Level :NO FUEL ");
			}

		delay_ms(500);
		}
}

