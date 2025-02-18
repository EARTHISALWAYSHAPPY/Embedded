#include <Arduino.h>
//-------------------//
#include <avr/io.h>
#include <util/delay.h>

#define CS PB2
#define MOSI PB3
#define SCK PB5

void spi_init()
{
    DDRB = (1 << SCK) | (1 << MOSI) | (1 << CS); // Set MOSI, SCK, Reset as Output
    SPSR = 0b00000001;                           //
    SPCR = 0b01010000;                           // Enable SPI, Set as Master, Clock 10 MHz.
}
void spi_putc(char data)
{
    char busy;
    // load data to buffer
    SPDR = data;
    // Wail until transmission complete
    do
    {
        busy = SPSR & 0b10000000;
    } while (busy == 0);
}

void max7219_wr(char addr, char data)
{
    PORTB &= 0b11111011; // SS = 0 do
    spi_putc(addr);
    spi_putc(data);
    PORTB |= 0b00000100; // SS = 1 dont
}
void max7219_config()
{
    max7219_wr(0x0C, 0);  // Shutdown
    max7219_wr(0x09, 0);  // decode mode
    max7219_wr(0x0A, 15); // light intensity
    max7219_wr(0x0B, 7);  // scan limit
    max7219_wr(0x0C, 1);  // turn ON
}
const uint8_t K[8] = {
    0b11000111,
    0b11001111,
    0b11011100,
    0b11111000,
    0b11111000,
    0b11011100,
    0b11001111,
    0b11000111
};
const uint8_t U[8] = {
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11111111,
    0b01111110,
    0b00111100
};
const uint8_t N[8] = {
    0b10000011,
    0b11100011,
    0b11110011,
    0b11111111,
    0b11111111,
    0b11001111,
    0b11000111,
    0b11000001
};
const uint8_t A[8] = {
    0b00111100,
    0b01111110,
    0b11000011,
    0b11000011,
    0b11111111,
    0b11111111,
    0b11000011,
    0b11000011
};
const uint8_t O[8] = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11100111,
    0b11100111,
    0b01111110,
    0b00111100,
    0b00011000
};
int main()
{
    spi_init();
    max7219_config();
    while (1)
    {
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,K[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,U[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,N[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,A[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,N[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,O[row]);
        }
        _delay_ms(2000);
        for (int row = 0; row < 8; row++)
        {
            max7219_wr(row+1,N[row]);
        }
        _delay_ms(2000);
    }
}
