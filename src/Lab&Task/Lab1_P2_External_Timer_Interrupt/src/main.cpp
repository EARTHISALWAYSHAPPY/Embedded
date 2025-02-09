#include <Arduino.h>
///////////////
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)
#define TIMER1_OVF_vect _VECTOR(13)

#define pin_red PD4
#define pin_yellow PD5
#define pin_green PD6

// variable:
bool dir = 0;
uint8_t speed = 0, color = 0;
// speed : 0 --> 1 --> 2 --> 3 --> 4 --> 0 (+ 1:200 ms)
// color : red(0) , yellow(1) , green(2)
uint16_t speed_use = 200, timer;
char speed_text[20];

void setting_port()
{
  // red(D4) , yellow(D5) , green(D6)
  DDRD |= ((1 << pin_green) | (1 << pin_yellow) | (1 << pin_red));
}
void setting_interrupt()
{
  // use sw1(D2) : INT0, sw2(D3) : INT1, Timer1

  EICRA = EICRA | 0b00001010;         // failling edge
  EIMSK |= (1 << INT1) | (1 << INT0); // enable INT1 , INT0

  TCCR1A = 0b00000000;
  TCCR1B = 0b00000101; // prescale 1024
  TIMSK1 = 0b00000001;
}

void serial_begin(int baudrate)
{
  UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(19200))) - 1
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000;
  UCSR0C = 0b00000110;
}
void clear()
{
  PORTD &= ~((1 << pin_green) | (1 << pin_yellow) | (1 << pin_red));
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
  switch (dir)
  {
  case 0:
    Serial_puts("Red --> Yellow --> Green : ");
    break;
  case 1:
    Serial_puts("Green --> Yellow --> Red : ");
    break;
  default:
    break;
  }
  sprintf(speed_text, "%d", speed_use);
  strcat(speed_text, "ms");
  strcat(speed_text, "\r\n");
  Serial_puts(speed_text);
}
ISR(INT0_vect) // for dir
{
  dir ^= 1;
  serial_display();
}
ISR(INT1_vect) // for speed : 0 --> 1 --> 2 --> 3 --> 4 --> 0
{
  if (speed < 4)
  {
    speed++;
  }
  else
    speed = 0;
  speed_use = (speed + 1) * 200;
  serial_display();
}
ISR(TIMER1_OVF_vect)
{
  float time_sec = speed_use / 1000.0;
  timer = 65535 - (F_CPU / 1024) * time_sec;
  TCNT1 = timer;

  clear();
  clear();
  switch (dir)
  {
  case 0: // Red --> Yellow --> Green
    switch (color)
    {
    case 0:
      PORTD |= (1 << pin_red);
      break;
    case 1:
      PORTD |= (1 << pin_yellow);
      break;
    case 2:
      PORTD |= (1 << pin_green);
      break;
    }
    break;
  case 1: // Green --> Yellow --> Red
    switch (color)
    {
    case 0:
      PORTD |= (1 << pin_green);
      break;
    case 1:
      PORTD |= (1 << pin_yellow);
      break;
    case 2:
      PORTD |= (1 << pin_red);
      break;
    }
    break;
  }
  color = (color + 1) % 3;
}

int main()
{
  serial_begin(51);
  setting_interrupt();
  setting_port();
  sei();
  while (1)
  {
  }
}