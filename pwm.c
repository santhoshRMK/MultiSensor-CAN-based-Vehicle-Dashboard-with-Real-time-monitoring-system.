#include<lpc21xx.h>	
#define sw1 1<<10
#include"lcd.h"

int speed;
int A=0;
void pwm_init(void)
{
PINSEL1 |=1<<10;
PWMPR=60-1;
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
int main(){
pwm_init();
speed = 0;
pwm_update();
lcd_init();
   IOCLR0=1<<21;

   //IOSET0=sw1;

while(1)
    {
        if (!(IOPIN0 & sw1))    // Switch pressed (active LOW)
        {
		    lcd_cmd(0x80);
            delay_ms(20);       // Debounce

            if (!(IOPIN0 & sw1))
            {
			    A++;
				IOSET0=1<<21;
				lcd_cmd(0x01);
				lcd_cmd(0x80);

                switch(A)
                {
                    case 1:
					{
					 lcd_str("Vehicle Started");
					 speed = 600; break;
					}
                    case 2:{
					 lcd_str("Low Speed");
					 speed = 1500; break;
					}
                    case 3: {
					 lcd_str("Medium Speed");
					 speed = 2500; break;
					}

                    default:
					{
					    lcd_str("High Speed");
					    speed = 4000;
                        A = 0;
						break;
					}
                }

                pwm_update();

                while (!(IOPIN0 & sw1));
				lcd_cmd(0xC0);
				lcd_str("LOOP ENDS");  // Wait until button release
            }
        }
    }
}

