#include <lpc21xx.h>

// LCD pins
#define LCD_DATA    (0xFF)      // P0.0 - P0.7
#define RS          (1<<8)      // P0.8
#define EN          (1<<9)      // P0.9

// HC-SR04 pins
#define TRIG        (1<<10)     // P0.10
#define ECHO        (1<<11)     // P0.11

void delay_ms(unsigned int ms){
    unsigned int i,j;
    for(i=0;i<ms;i++)
        for(j=0;j<6000;j++);
}

void lcd_cmd(unsigned char cmd){
    IO0CLR = LCD_DATA;    // Clear lower 8 bits
    IO0SET = cmd & 0xFF;  // Put command on P0.0-P0.7
    IO0CLR = RS;          // RS = 0 for command
    IO0SET = EN;          // EN = 1
    delay_ms(2);
    IO0CLR = EN;          // EN = 0
}

void lcd_data(unsigned char data){
    IO0CLR = LCD_DATA;     // Clear lower 8 bits
    IO0SET = data & 0xFF;  // Put data on P0.0-P0.7
    IO0SET = RS;           // RS = 1 for data
    IO0SET = EN;           // EN = 1
    delay_ms(2);
    IO0CLR = EN;           // EN = 0
}

void lcd_init(){
    IO0DIR |= LCD_DATA | RS | EN;  // Set P0.0-P0.9 as output

    delay_ms(20);
    lcd_cmd(0x38);    // 8-bit, 2-line, 5x7 font
    lcd_cmd(0x0C);    // Display ON, Cursor OFF
    lcd_cmd(0x06);    // Entry mode
    lcd_cmd(0x01);    // Clear display
    delay_ms(5);
}

void lcd_print(char *str){
    while(*str){
        lcd_data(*str++);
    }
}

// Print integer without sprintf
void lcd_print_int(unsigned int num){
    unsigned char digits[5];
    int i=0;
    if(num == 0){
        lcd_data('0');
        return;
    }
    while(num > 0){
        digits[i++] = (num % 10) + '0';
        num /= 10;
    }
    for(i=i-1;i>=0;i--){
        lcd_data(digits[i]);
    }
}

// Measure pulse width (approximate)
unsigned int measure_pulse(){
    unsigned int count=0;
    while(!(IO0PIN & ECHO));    // Wait HIGH
    while(IO0PIN & ECHO){       // Count duration
        count++;
        delay_ms(0);
    }
    return count;
}

int main(void){
    unsigned int duration;
    unsigned int distance;

    IO0DIR |= TRIG;  // Trig as output
    IO0CLR |= TRIG;

    lcd_init();

    while(1){
        // Trigger pulse
        IO0SET = TRIG;
        delay_ms(1);     // ~10us pulse
        IO0CLR = TRIG;

        // Measure echo
        duration = measure_pulse();

        // Convert to cm (approx)
        distance = (duration * 34) / 2000;

        // Clear LCD and print from start
        lcd_cmd(0x01);         // Clear display
        lcd_print("Distance: ");
        lcd_print_int(distance);
        lcd_print(" cm");

        delay_ms(500);
    }
}
