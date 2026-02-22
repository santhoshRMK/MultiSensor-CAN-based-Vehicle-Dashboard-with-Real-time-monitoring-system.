/* Main_node and Speed Control*/
#include <lpc21xx.h>
#include "header.h"
#include "lcd.h"
#define sw1 1<<12 // switch assign to the PWM ADC VALUE
int speed,A=0;
void pwm_init(void)
{
	PINSEL1 |=1<<10;
	PWMPR =60-1;
	PWMMR0=0xfff;
	PWMMCR=(0X02);
	PWMPCR=(1<<13);
	PWMTCR=(0X09);
}

void pwm_update(void)
{
	PWMMR5=speed;
	PWMLER=1<<5;
}

int main()
{	
	CAN2_MSG m1;
	lcd_init();
	can2_init();
	lcd_str("Vehicle Dashboard");

	pwm_init();
	speed = 0;
	pwm_update();
	while(1)
	{		
		can2_rx(&m1);
		
		if (!(IOPIN0 & sw1))    // Switch pressed (active LOW)
    {
				lcd_cmd(0x80);
        delay_ms(100);       // Debounce
        if (!(IOPIN0 & sw1))
        {
						A++;
						IOSET0=1<<21;
						lcd_cmd(0xC0);
            switch(A)
            {
               case 1:
									lcd_str("Vehicle Started");
									speed = 600;
									break;
               case 2:
					
									lcd_str("Low Speed      ");
									speed = 1500; 
									break;
               case 3: 
									lcd_str("Medium Speed   ");
									speed = 2500;
									break;
               default:
									lcd_str("High Speed     ");
									speed = 4000;
                  A = 0;
									break;
						}
          }
          pwm_update();
          while (!(IOPIN0 & sw1));  // Wait until button release
      }
   }
	 if(m1.id == 0x1EF)
	 {	
			if(m1.byteA <=8)
			{
				lcd_cmd(0x94);
				lcd_str("Fuel Level :100%    ");
			}
			else if(m1.byteA > 8 && m1.byteA <=15)
			{
					lcd_cmd(0x94);
					lcd_str("Fuel Level :80%     ");
			}
			else if(m1.byteA > 15 && m1.byteA <=22)
			{
					lcd_cmd(0x94);
					lcd_str("Fuel Level :60%     ");
			}
			else if(m1.byteA > 22 && m1.byteA <=30)
			{
					lcd_cmd(0x94);
					lcd_str("Fuel Level :40%     ");
			}
			else if(m1.byteA > 30 && m1.byteA <=35)
			{
					lcd_cmd(0x94);
					lcd_str("Fuel Level :20%     ");
			}
			else if(m1.byteA > 35)
			{
					lcd_cmd(0x94);
					lcd_str("Fuel Level :NO FUEL ");
			}
		}
    if(m1.id == 0x11)
		{
			if(m1.byteA > 0)
			{
				lcd_cmd(0xD4);
				lcd_str("Low Temp");
			}
		}
		delay_ms(100);
}	

