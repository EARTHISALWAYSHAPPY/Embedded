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
    SPCR = 0b01010000;                           // Enable SPI, Set as Master, Clock.
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
    max7219_wr(0x0C, 0);    // Shutdown
    max7219_wr(0x09, 0x00); // decode mode
    max7219_wr(0x0A, 0x00); // light intensity
    max7219_wr(0x0B, 0x07); // scan limit
    max7219_wr(0x0C, 1);    // turn ON
}
void display(byte dot[8])
{
    for (int row = 0; row < 8; row++)
    {
        max7219_wr(row + 1, dot[row]);
    }
}
const byte data[8] = {
    B00000000,
    B01100110,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100,
    B00011000
  };
int main()
{
    spi_init();
    max7219_config();
    while (1)
    {
        byte show[8] = {};
        for (int shift = 0; shift < 8; shift++)
        {
            for (int row = 0; row < 8; row++)
            {
                show[row] = 0b00000000 | (data[row] >> (7 - shift));
            }
            display(show);
            _delay_ms(80);
        }
        for (int clear = 0; clear < 8; clear++)
        {
            for (int row = 0; row < 8; row++)
            {
                show[row] = 0b00000000 | (data[row] << (clear + 1));
            }
            display(show);
            _delay_ms(80);
        }
    }
}


