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
    PORTB &= 0b11111011; // SS = 0 do // 0xFB
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
const byte text_kunanon[7][8] = {
    {0xC7, 0xCF, 0xDC, 0xF8, 0xF8, 0xDC, 0xCF, 0xC7},
    {0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0x7E, 0x3C},
    {0x83, 0xE3, 0xF3, 0xFF, 0xFF, 0xCF, 0xC7, 0xC1},
    {0x3C, 0x7E, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3},
    {0x83, 0xE3, 0xF3, 0xFF, 0xFF, 0xCF, 0xC7, 0xC1},
    {0x18, 0x3C, 0x7E, 0xE7, 0xE7, 0x7E, 0x3C, 0x18},
    {0x83, 0xE3, 0xF3, 0xFF, 0xFF, 0xCF, 0xC7, 0xC1}};

int main()
{
    spi_init();
    max7219_config();
    while (1)
    {
        max7219_wr(0x0C, 0);
        _delay_ms(1000);
        max7219_wr(0x0C, 1);
        int temp_count = sizeof(text_kunanon) / sizeof(text_kunanon[0]);
        for (int text = 0; text < temp_count; text++)
        {
            for (int row = 0; row < 8; row++)
            {
                max7219_wr(row + 1, text_kunanon[text][row]);
            }
            _delay_ms(2000);
        }
        // max7219_wr(0x0C, 0);
        // _delay_ms(100);
        // max7219_wr(0x0C, 1);
    }
}