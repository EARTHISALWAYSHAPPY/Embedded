#include <avr/io.h>
uint8_t Count8;
uint16_t Count16;
int main()
{

    DDRD |= (1 << PD4) | (1 << PD6); //(D4 D6 : set output) (D4 : LED red) (D6 : LED green)
    // DDRD &= ~(1 << PD2) & ~(1 << PD3); //(D2 D3 : set input) (SW1 : 3) (SW2 : 2)
    TCCR0A = 0b00000000;
    TCCR0B = 0b00000101;
    TCCR1A = 0b00000000;
    TCCR1B = 0b00000101;
    TCNT0 = 0;
    TCNT1 = 0;
    PORTD &= ~((1 << PD4) | (1 << PD6)); //(Start = Off)
    while (true)
    {
        Count8 = TCNT0;
        Count16 = TCNT1;
        if (!(PORTD & (1 << PD4)) && (Count8 > 156))
        {
            PORTD |= (1 << PD4);
            TCNT0 = 0;
        }
        else if ((PORTD & (1 << PD4)) && (Count8 > 156))
        {
            PORTD &= ~(1 << PD4);
            TCNT0 = 0;
            Count8 = 0;
        }
        if (!(PORTD & (1 << PD6)) && (Count16 > 15625))
        {
            PORTD |= (1 << PD6);
            TCNT1 = 0;
        }
        else if ((PORTD & (1 << PD6)) && (Count16 > 15625))
        {
            PORTD &= ~(1 << PD6);
            TCNT1 = 0;
            Count16 = 0;
        }
    }
}