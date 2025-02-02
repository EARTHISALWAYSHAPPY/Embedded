#include <avr/io.h>
uint16_t Count16;
int main()
{
    DDRD |= (1 << PD4);
    TCCR1A = 0b00000000;
    TCCR1B = 0x05;
    TCNT1 = 0;
    PORTD &= ~(1 << PD4);
    while (true)
    {
        Count16 = TCNT1;
        if ((Count16 > 62500)) // check (4 second) ?
        {
            PORTD ^= (1 << PD4); //(XOR)
            TCNT1 = 0;
        }
    }
}

