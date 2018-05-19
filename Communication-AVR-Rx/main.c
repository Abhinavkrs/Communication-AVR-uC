#include<avr/io.h>
#include<avr/interrupt.h>
#define F_CPU      16000000UL
#define USART_Baud 9600

#define BaudH ((unsigned int)(((F_CPU/16/USART_Baud)-1) >> 8))

#define BaudL ((unsigned int)(((F_CPU/16/USART_Baud)-1) & 0xFF))
#include<util/delay.h>

int t1 = 0;

#define		LCD_DATA_PORT		PORTB
#define 	LCD_CONT_PORT		PORTC
#define 	LCD_RS 		PC1
#define 	LCD_EN 		PC2

void lcd_data_write(char data);
void lcd_command_write( char command);
void lcd_init();
void lcd_string_write( char *string);

void usart_init()
{
	
	UCSRB = (1<<RXEN);
	UCSRB |= (1 << RXCIE);
	UCSRC = (1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	UBRRL =BaudL;
}

void lcd_data_write_int(int data)
{
	LCD_CONT_PORT=_BV(LCD_EN)|_BV(LCD_RS);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	LCD_CONT_PORT=_BV(LCD_RS);
	_delay_ms(1);
}


ISR(USART_RXC_vect)
{
	char ReceivedByte;
	ReceivedByte = UDR; // Fetch the received byte value into the variable "ByteReceived"
	if(ReceivedByte == 'A')
	{
		t1++;
	}
	else
	{
		PORTC = (1<<4);
	}
	
}


int main(void)
{
	DDRB=0xFF;
	DDRC=0xFF;
	
	sei();
	lcd_init();
	usart_init();
	lcd_data_write('A');
	lcd_command_write(0xc0);
	char buffer[7];
	while(1)
	{
		itoa(t1, buffer, 10);		
		lcd_string_write(buffer);
		_delay_ms(1000);
		lcd_command_write(0xc0);
		
	}
}

void lcd_data_write(char data)
{
	LCD_CONT_PORT=_BV(LCD_EN)|_BV(LCD_RS);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	LCD_CONT_PORT=_BV(LCD_RS);
	_delay_ms(1);
}

void lcd_command_write(char command)
{
	LCD_CONT_PORT=_BV(LCD_EN);
	LCD_DATA_PORT=command;
	_delay_ms(1);
	LCD_CONT_PORT=0x00;
	_delay_ms(1);
}

void lcd_init()
{
	lcd_command_write(0x38);
	lcd_command_write(0x01);
	//lcd_command_write(0x06);
	lcd_command_write(0x0e);
}

void lcd_string_write(char *string)
{
	while (*string)
	lcd_data_write(*string++);
}



