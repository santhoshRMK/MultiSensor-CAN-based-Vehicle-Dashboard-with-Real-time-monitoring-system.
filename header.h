/*header.h*/
#ifndef __HEADER_H_
#define __HEADER_H_
#define PORT0 0
#define PORT1 1
#define SET   1
#define CLEAR 0
typedef unsigned int u32;
typedef 	signed int s32;
typedef unsigned char u8;
typedef 	signed char s8;
typedef unsigned short int u16;
typedef   signed short int s16;
typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	u32 byteA;
	u32 byteB;
}CAN2_MSG;
void delay_sec(u32 sec);
//void delay_ms(u32 ms);
void delay_ms(unsigned int ms);

void gpio_direction_input(u32 port,u32 pin);
void gpio_direction_output(u32 port,u32 pin);
u32 gpio_get_value(u32 port,u32 pin);
void gpio_set_value(u32,u32,u32);

void uart0_init(u32 baud);
void uart0_tx(u8 ch);
u8 uart0_rx(void);
void uart0_string(s8 *s);
void uart0_integer(s32 n);

u32 adc0_read(u32 channel);
void adc0_init(void);
void spi0_init(void);
void spi0_write(u8 data);

u8 spi0_read(u8 data);
void enable_write_operation(void);
void byte_write_25aa010(u8 mr,u8 data);
void disable_write_operation(void);

u8 byte_read_25aa010(u8 mr);
void config_eint(void);
void config_vic(void);
void config_nvirq(void);
void I2C_INIT(void);
void I2C_START(void);
void I2C_STOP(void);
void I2C_WRITE(u8);

u8 I2C_READ(void);
void byte_write_eeprom_at24c512(u8 sa,u32 mr_addr,u8 data_byte);

u8 byte_read_eeprom_at24c512(u8 sa, u32 mr_addr);
void page_write_eeprom_at24c512(u8 sa,u32 mr_addr,u8 max_bytes,u8 *buf);
void sequantial_read_eeprom_at24c512(u8 sa,u8 mr_addr,u8 max_bytes,u8 *buf);
void can2_init(void);
void can2_tx(CAN2_MSG m1);
void can2_rx(CAN2_MSG *m2);

#endif


