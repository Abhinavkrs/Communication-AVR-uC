//tx serial
#include<avr/io.h>
#define F_CPU 16000000
#define USART_Baud 9600

#define BaudH ((unsigned int)(((F_CPU/16/USART_Baud)-1) >> 8))

#define BaudL ((unsigned int)(((F_CPU/16/USART_Baud)-1) & 0xFF))

#include <util/delay.h>

void usart_init()
{
	UCSRB = (1<<TXEN);
	UCSRC = (1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
	UBRRL = BaudL;
}

void usart_send(unsigned char ch)
{
	while(! (UCSRA & (1<<UDRE)));
	UDR = ch;
}
int main()
{
	unsigned char x;
	DDRA = 0b11111100;
	PORTA = 0b00000011;
	
	usart_init();
	
	while(1)
	{
		x = PINA;
		x = x & 0b00000011;
		switch(x)
		{
		case(1):
		{
			usart_send('A');
			_delay_ms(200);
			break;
		}
		case(2):
		{
			usart_send('B');
			_delay_ms(200);
			break;
		}
		}
		/*else
		{
			usart_send('0');
			_delay_ms(1000);
		}*/
	}
	return 0;
}