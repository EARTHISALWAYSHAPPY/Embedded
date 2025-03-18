// use baudrate 57600
// D2 D4 active low
// D3 actice high
#include <Arduino.h>
#include <avr/io.h>
#define UART_RX_vect _VECTOR(18)

#define pin_volt 4

int adc;
float volt, amp, R;
char choice, text[10];

void setting()
{
  DDRD |= (1 << PD4) | (1 << PD3) | (1 << PD2);
}
void clear_led()
{
  PORTD |= (1 << PD2) | (1 << PD4);
  PORTD &= ~(1 << PD3);
}

// ADC//
void ADC_init()
{
  ADMUX = 0b01000000;
  ADCSRA = 0b10000111;
}
int ADC_read(char port)
{
  char busy;
  ADMUX = ADMUX & 0b11110000;
  ADMUX = ADMUX | port;
  ADCSRA = ADCSRA | 0b01000000;
  do
  {
    busy = ADCSRA & 0b01000000;
  } while (busy != 0);
  return (ADC);
}

// UART//
void Serial_begin(int baudrate)
{
  UBRR0 = baudrate;    // UBRRn(34) = ( 16x10^6 / (8 x baud rate(57600))) - 1
  UCSR0A = 0b00000010; // double speed
  UCSR0B = 0b10011000; //
  UCSR0C = 0b00000110; //
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
void condition()
{
  int choice_int = choice - '0'; // ASCII
  switch (choice_int)
  {
  case 0:
    clear_led();
    PORTD &= ~(1 << PD2);
    dtostrf(volt, 8, 2, text);
    Serial_puts("voltage is ");
    Serial_puts(text);
    Serial_puts(" V\r\n");
    break;
  case 1:
    clear_led();
    PORTD |= (1 << PD3);
    dtostrf(amp, 8, 6, text);
    Serial_puts("Currrent is ");
    Serial_puts(text);
    Serial_puts(" A\r\n");
    break;
  case 2:
    clear_led();
    PORTD &= ~(1 << PD4);
    dtostrf(R, 8, 2, text);
    Serial_puts("Resistor is ");
    Serial_puts(text);
    Serial_puts(" Ohm\r\n");
    break;
  default:
    Serial_puts("error");
    break;
  }
  Serial_puts("Voltage(0) Current(1) Resistor(2) : ");
}
ISR(UART_RX_vect)
{
  adc = ADC_read(pin_volt);
  volt = ((float)adc * 5.00 / 1024.00);
  R = abs(((10000.00 * volt) / (5.00 - volt)));
  amp = abs(volt / R);
  choice = Serial_getc();
  Serial_putc(choice);
  Serial_puts("\r\n");
  condition();
}
int main()
{
  setting();
  clear_led();
  Serial_begin(34);
  ADC_init();
  Serial_puts("Voltage(0) Current(1) Resistor(3) : ");
  while (1)
  {
  }
}