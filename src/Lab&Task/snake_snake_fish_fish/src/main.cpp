#include <Arduino.h>
//--------------------//
//--------hard--------//
//--------------------//
#include <avr/io.h>
#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)
#define TIMER1_OVF_vect _VECTOR(13)

#define CS PB2
#define MOSI PB3
#define SCK PB5

int x[4] = {4, 5, 6, 7}, y[4] = {7, 7, 7, 7}; // default row7 : 11110000
int horizontal = 1, vertical = 0;             // default shift right -->

void setting_interrupt()
{
    // use sw1(D2) : INT0, sw2(D3) : INT1, Timer1
    EICRA = EICRA | 0b00001010;         // failling edge
    EIMSK |= (1 << INT1) | (1 << INT0); // enable INT1 , INT0
    TCCR1A = 0b00000000;
    TCCR1B = 0b00000101; // prescale 1024
    TIMSK1 = 0b00000001;
    TCNT1 = 57723; // 500 ms
}

void spi_init()
{
    DDRB |= (1 << SCK) | (1 << MOSI) | (1 << CS);  // Set MOSI, SCK, CS as Output
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Master Mode, Clock /16
}

void spi_putc(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ; // Wait for transmission complete
}

void max7219_wr(char addr, char data)
{
    PORTB &= ~(1 << CS); // SS = 0
    spi_putc(addr);
    spi_putc(data);
    PORTB |= (1 << CS); // SS = 1
}

void max7219_config()
{
    max7219_wr(0x0C, 0x00); // Shutdown
    max7219_wr(0x09, 0x00); // Decode mode
    max7219_wr(0x0A, 0x15); // Intensity
    max7219_wr(0x0B, 0x07); // Scan limit
    max7219_wr(0x0C, 0x01); // Turn on
}
void display()
{
    byte matrix[8] = {0};
    for (int i = 0; i < 4; i++)
    {
        matrix[y[i]] |= (1 << x[i]);
    }
    for (int row = 0; row < 8; row++)
    {
        max7219_wr(row + 1, matrix[row]);
    }
}
void move()
{
    display();
    // update x,y 4 dot
    for (int position = 3; position > 0; position--)
    {
        x[position] = x[position - 1];
        y[position] = y[position - 1];
    }
    // shift led for update
    x[0] -= horizontal;
    y[0] -= vertical;
    // horizontal
    if (x[0] < 0)
    {
        x[0] = 7; // reset หลุดจอ
    }
    // vertical
    if (y[0] < 0)
    {
        y[0] = 7; // reset หลุดจอ
    }
}

ISR(INT0_vect)
{
    // shift right (default)
    horizontal = 1, vertical = 0;
}

ISR(INT1_vect)
{
    // shift up
    horizontal = 0, vertical = 1;
}

ISR(TIMER1_OVF_vect)
{
    move();
    TCNT1 = 57723; // 500 ms
}
void clear()
{
    for (int row = 0; row <= 8; row++)
    {
        max7219_wr(row + 1, 0x00);
    }
}
int main()
{
    setting_interrupt();
    spi_init();
    max7219_config();
    clear();
    sei();
    while (1)
    {
    }
}
/*
row
-------------->
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
7 6 5 4 3 2 1 0
colume
0 0 0 0 0 0 0 ^
1 1 1 1 1 1 1 |
2 2 2 2 2 2 2 |
3 3 3 3 3 3 3 |
4 4 4 4 4 4 4 |
5 5 5 5 5 5 5 |
6 6 6 6 6 6 6 |
7 7 7 7 7 7 7 |
*/