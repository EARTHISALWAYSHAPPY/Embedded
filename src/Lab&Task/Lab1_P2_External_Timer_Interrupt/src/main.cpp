#include <Arduino.h>
///////////////
#include <avr/io.h>
#include <util/delay.h>

#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)
#define TIMER1_OVF_vect _VECTOR(13)

// variable:
bool dir = 0;
uint8_t speed = 0, color = 0;
// speed : 0 --> 1 --> 2 --> 3 --> 4 --> 0 (+ 1:200 ms)
// color : red(0) , yellow(1) , green(2)

void setting_port()
{
  // red(D4) , yellow(D5) , green(D6)
}
void setting_interrupt()
{
  // use sw1(D2) : INT0, sw2(D3) : INT1, Timer1

  EICRA = EICRA | 0b00001010;         // failling edge
  EIMSK |= (1 << INT1) | (1 << INT0); // enable INT1 , INT0

  TCCR1A = 0b00000000;
  TCCR1B = 0b00000101;
  TIMSK1 = 0b00000001;
}

void serial_begin(int baudrate)
{
  UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(19200))) - 1
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000;
  UCSR0C = 0b00000110;
}
void Serial_putc(char data)
{
  char busy;
  do
  {
    // Check data register empty?
    busy = UCSR0A & 0b00100000;
  } while (busy == 0);
  UDR0 = data;
}

char Serial_getc()
{
  char busy;
  do
  {
    // Check receive data complete?
    busy = UCSR0A & 0b10000000;
  } while (busy == 0);
  return (UDR0);
}

void Serial_puts(char *data)
{
  while (*data)
  {
    Serial_putc(*data++);
  }
}

void serial_display()
{
  ///////////////////
}
ISR(INT0_vect) // for dir
{
  ///////////////////
}
ISR(INT1_vect) // for speed : 0 --> 1 --> 2 --> 3 --> 4 --> 0
{
  ///////////////////
}

int main()
{
  while (1)
  {
  }
}