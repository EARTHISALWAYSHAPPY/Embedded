#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
int adc0;
void set()
{
  DDRD |= 0xff;
  DDRB |= 0x03;
}
void ADC_init()
{
  ADMUX = 0b01000000;  // 0 1 : AVcc
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
int main()
{
  set();
  ADC_init();
  while (true)
  {
    adc0 = ADC_read(0);
    PORTD = adc0 & 0xff;
    PORTB = adc0 >> 8 & 0x03;
    _delay_ms(1000);
  }
}