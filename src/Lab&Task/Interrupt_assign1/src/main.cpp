#include <Arduino.h>
////////////////////////////////////////////////
#include <avr/io.h>
#include <util/delay.h>
#define INT0_vect _VECTOR(1)
#define TIMER1_OVF_vect _VECTOR(13)
#define sw PD2
#define led_red PB0
#define led_yellow PB1
#define led_green PB2

// time interrupt
#define one_sec 49911
#define four_sec 3036
void Setting()
{
  DDRD &= ~(1 << sw); // sw(PD2) : input
  // led_red(PB0) led_yellow(PB1) led_green(PB2) : output
  DDRB |= ((1 << led_red) | (1 << led_yellow) | (1 << led_green));

  EICRA = EICRA | 0b00000010; // INT0 failling edge
  EIMSK = 1;

  TCCR1A = 0x00;
  TCCR1B = 0x05;

  PORTB |= (1 << led_red); // default led_red on
}
void clear()
{
  PORTB &= ~((1 << led_red) | (1 << led_yellow) | (1 << led_green));
}
ISR(INT0_vect)
{
  clear();
  PORTB |= (1 << led_green);
  TIMSK1 = 1; // enable timer interrupt
  TCNT1 = four_sec;
}
ISR(TIMER1_OVF_vect)
{
  if (PORTB & (1 << led_green))
  {
    clear();
    PORTB |= (1 << led_yellow);
    TCNT1 = one_sec;
  }
  else if (PORTB & (1 << led_yellow))
  {
    clear();
    PORTB |= (1 << led_red);
    TIMSK1 = 1; // disable timer interrupt
  }
}

int main()
{
  Setting();
  sei();
  while (true)
  {
  }
}