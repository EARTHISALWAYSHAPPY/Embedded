#include <avr/io.h>
int main()
{
    DDRD |= (1 << PD4);  // LED RED 1
    DDRD |= (1 << PD5);  // LED YELLOW 1
    DDRD |= (1 << PD6);  // LED GREEN 1
    DDRD &= ~(1 << PD2); // SW1 0
    DDRD &= ~(1 << PD3); // SW2 0 
    while (true)
    {
        if (!(PIND & (1 << PD2)))
        {
            PORTD |= (1 << PD4);
            PORTD &= ~(1 << PD5);
            PORTD &= ~(1 << PD6);
        }
        else if (!(PIND & (1 << PD3)))
        {
            PORTD &= ~(1 << PD4);
            PORTD &= ~(1 << PD5);
            PORTD |= (1 << PD6);
        }
        else
        {
            PORTD &= ~(1 << PD4);
            PORTD |= (1 << PD5);
            PORTD &= ~(1 << PD6);
        }
    }
}