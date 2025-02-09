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
// speed : 0-->1-->2-->3-->4-->0 (+ 1:200 ms)
// color : red(0) , yellow(1) , green(2)

void setting_interrupt()
{
  ///////////////
}
void serial_begin(int baudrate)
{
  UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(19200))) - 1
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000;
  UCSR0C = 0b00000110;
  sei();
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
  ///////////////
}
ISR(INT0_vect)
{
  ///////////////
}
ISR(INT1_vect)
{
  ///////////////
}
ISR(TIMER1_OVF_vect)
{
  ///////////////
}
int main()
{
  serial_begin(51);
  while (1)
  {
  }
}