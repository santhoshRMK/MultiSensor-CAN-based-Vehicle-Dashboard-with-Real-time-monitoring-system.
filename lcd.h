/* LCD header file */
#include <LPC21xx.h>
#include <stdio.h>
void delay_ms(unsigned int);

// LCD pins P0.14–P0.17 (D4–D7), P0.12 (RS), P0.13 (E)
#define LCD_D   (0xF << 14)
#define RS      (1 << 8)
#define E       (1 << 9)

//--------------------------------------
// Send command in 4-bit mode
//--------------------------------------
void 
lcd_cmd(unsigned char cmd)
{
    IOCLR0 = LCD_D;                       
    IOSET0 = ((cmd >> 4) & 0x0F) << 14;   // Upper nibble
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(3);
    IOCLR0 = E;

    IOCLR0 = LCD_D;
    IOSET0 = (cmd & 0x0F) << 14;          // Lower nibble
    IOCLR0 = RS;
    IOSET0 = E;
    delay_ms(3);
    IOCLR0 = E;
}

//--------------------------------------
// Send data (character)
//--------------------------------------
void lcd_data(unsigned char d)
{
    IOCLR0 = LCD_D;
    IOSET0 = ((d >> 4) & 0x0F) << 14;     // Upper nibble
    IOSET0 = RS;
    IOSET0 = E;
    delay_ms(3);
    IOCLR0 = E;


    IOCLR0 = LCD_D;
    IOSET0 = (d & 0x0F) << 14;            // Lower nibble
    IOSET0 = RS;
    IOSET0 = E;
    delay_ms(3);
    IOCLR0 = E;
}

//--------------------------------------
// LCD Initialization
//--------------------------------------
void lcd_init(void)
{
IODIR0 |= LCD_D | RS | E;
lcd_cmd(0x01); //Clear the lc display
lcd_cmd(0x02); //Move the cursor to the home position
lcd_cmd(0x0c); //display on cursor off
lcd_cmd(0x28); //two row display
lcd_cmd(0x80);  // Clear
    //delay_ms(3);
}

//--------------------------------------
// Print integer on LCD
//--------------------------------------
void lcd_int(int n)
{
    unsigned char arr[10];
    int i = 0, j;

    if (n == 0) { lcd_data('0'); return; }

    if (n < 0) { lcd_data('-'); n = -n; }

    while (n > 0) {
        arr[i++] = (n % 10);
        n /= 10;
    }

    for (j = i - 1; j >= 0; j--)
        lcd_data(arr[j] + '0');
}

//--------------------------------------
void lcd_str(char *s)
{
    while (*s) 
	lcd_data(*s++);
}


void lcd_float(float n)
{
	
 int i=0,f,j=0;
	int cnt;	
	int arr[12];
	float temp;
	unsigned char flag=1;
	
	if(n>0&&n<10)
	{
  temp=1000.0;
		cnt=1;
}else if(n>=10.0&&n<100.0)
{
 temp=100.0;
	cnt=2;
}
else if(n>=100.0&&n<1000.0)
{
 temp=10.0;
	cnt=3;
}
f=n*temp;
while(f>0)
{
 arr[i++]=f%10;
	f/=10;
}

for(i=i-1;i>=0;i--)
{
 if(j==cnt&&flag)
 {
 lcd_data('.');
	 i++; flag=0;
}
else
{
 lcd_data(arr[i]+'0');
}
j++;
}
}
void lcd_inti(int n)
{ char a[10];
sprintf(a,"%d",n);
lcd_str(a);
}

void floate(float f)
{
	int x=f;
	lcd_data(((x/10)%10)+48);
	lcd_data((x%10)+48);
	x=f*100;
	lcd_data('.');
	lcd_data(((x/10)%10)+48);
	lcd_data((x%10)+48);
	lcd_data('0');
	lcd_data('0');
	
}
void delay(unsigned int s)
{
    T0PR = 60 - 1;        // Prescaler for 1µs delay (15MHz PCLK)
    T0TC = 0;
    T0TCR = 0x01;
    while(T0TC < s);
    T0TCR = 0x00;
}void delay_ms(unsigned int s)
{
    T0PR = 60000 - 1;        // Prescaler for 1µs delay (15MHz PCLK)
    T0TC = 0;
    T0TCR = 0x01;
    while(T0TC < s);
    T0TCR = 0x00;
}



