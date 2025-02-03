#include <avr/io.h>
#include <util/delay.h>
void serial_begin()
{

  UBRR0 = 103; // UBRRn = ( 16x10^6 / (16 x baud rate)) - 1
  UCSR0A = 0b00000000; // |
  UCSR0B = 0b00011000; // |-> Control and Satus Register.
  UCSR0C = 0b00000110; // |
}

void serial_putc(char data)
{
  char busy;
  do
  { 
    // Check data register empty?
    busy = UCSR0A & 0b00100000;
  } while (busy == 0);
  UDR0 = data;
}
char serial_getc()
{
  char busy;
  do
  {
    // Check receive data complete?
    busy = UCSR0A & 0b10000000;
  } while (busy == 0);
  return (UDR0);
}

void serial_puts(char *data)
{
  while (*data)
  {
    serial_putc(*data++);
  }
}
int main(void)
{
  char ans;
  serial_begin();
  DDRD |= ((1 << PD4) | (1 << PD5) | (1 << PD6));
  while (1)
  {
    serial_puts("TURN ON LED? (y/n) : ");
    ans = serial_getc();
    serial_putc(ans);
    serial_puts("\r");
    if ((ans == 'y') || (ans == 'Y'))
      PORTD |= ((1 << PD4) | (1 << PD5) | (1 << PD6));
    else if ((ans == 'n') || (ans == 'N'))
      PORTD &= ~((1 << PD4) | (1 << PD5) | (1 << PD6));
    else
      serial_puts("Wrong Input. Please Try Again \r\n");
  }
}