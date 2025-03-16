#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#define UART_RX_vect _VECTOR(18)
#define F_CPU 16000000UL
#define pin_volt 1
int adc;
float volt, amp, R;
char choice, text[10];
void ADC_init()
{
  ADMUX = 0b01000001;  // 0 1 : AVcc , ADC1
  ADCSRA = 0b10000111; // ADC : Enable , Prescale : 128
}
int ADC_read(char ch)
{
  char busy;
  ADMUX = ADMUX & 0b11110000;
  ADMUX = ADMUX | ch;
  ADCSRA = ADCSRA | 0b01000000;
  do
  {
    busy = ADCSRA & 0b01000000;
  } while (busy != 0);
  return (ADC);
}
void Serial_begin(int baudrate)
{
  UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(38400))) - 1
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
    busy = UCSR0A & 0b00100000;
  } while (busy == 0);
  UDR0 = data;
}
char Serial_getc()
{
  char busy;
  do
  {
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
    dtostrf(volt, 8, 2, text);
    Serial_puts("voltage is ");
    Serial_puts(text);
    Serial_puts(" V\r\n");
    break;
  case 1:
    dtostrf(amp, 8, 6, text);
    Serial_puts("Currrent is ");
    Serial_puts(text);
    Serial_puts(" A\r\n");
    break;
  case 2:
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
  Serial_begin(25); // 38400
  ADC_init();
  Serial_puts("Voltage(0) Current(1) Resistor(3) : ");
  while (true)
  {
  }
}