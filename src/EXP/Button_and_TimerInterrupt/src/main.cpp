// INT0 pull-up(falling edge) , INT1 pull-down(rissing edge)
// D4 D5 D6 active low
#include <Arduino.h>
#include <avr/io.h>
#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)
#define TIMER1_OVF_vect _VECTOR(13)

#define pin_red PD4
#define pin_yellow PD5
#define pin_green PD6

bool dir = 0;
uint8_t speed = 0, color = 0;
uint16_t speed_use = 200;

void setting()
{
    DDRD |= ((1 << pin_green) | (1 << pin_yellow) | (1 << pin_red));
    // interrupt
    // INT0 pull-up(falling edge) , INT1 pull-down(rissing edge)
    EICRA = 0b00001110;
    EIMSK |= (1 << INT1) | (1 << INT0);

    TCCR1A = 0b00000000;
    TCCR1B = 0b00000100; // prescale 256
    TIMSK1 = 1;
    TCNT1 = 53036; // default 200 ms
}

void clear()
{
    // off set 1 because active low
    PORTD |= (1 << pin_green) | (1 << pin_yellow) | (1 << pin_red);
}

ISR(INT0_vect) // for dir
{
    dir ^= 1;
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
}

ISR(TIMER1_OVF_vect)
{
    float time_sec = (F_CPU / 256.0) * (speed_use / 1000.0);
    TCNT1 = 65535 - time_sec;
    clear();
    switch (dir)
    {
    case 0: // Red --> Yellow --> Green
        switch (color)
        {
        case 0:
            PORTD &= ~(1 << pin_red);
            color += 1;
            break;
        case 1:
            PORTD &= ~(1 << pin_yellow);
            color += 1;
            break;
        case 2:
            PORTD &= ~(1 << pin_green);
            color = 0;
            break;
        }
        break;
    case 1: // Green --> Yellow --> Red
        switch (color)
        {
        case 0:
            PORTD &= ~(1 << pin_green);
            color += 1;
            break;
        case 1:
            PORTD &= ~(1 << pin_yellow);
            color += 1;
            break;
        case 2:
            PORTD &= ~(1 << pin_red);
            color = 0;
            break;
        }
        break;
    }
}

int main()
{
    setting();
    sei();
    while (true)
    {
    }
}