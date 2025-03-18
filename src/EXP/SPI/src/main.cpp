#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)

#define CS PB2
#define MOSI PB3
#define SCK PB5

int x = 7, y = 7;
byte clear = 0x00;

void setting_interrupt()
{
    EICRA |= (1 << ISC11) | (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0) | (1 << INT1);
}

void spi_init()
{
    DDRB |= (1 << SCK) | (1 << MOSI) | (1 << CS); // Set MOSI, SCK, CS as Output
    SPSR = 0b00000001;
    SPCR = 0b01010000; // Enable SPI, Set as Master, Clock.
}

void spi_putc(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF)))
        ; // Wait for transmission complete
}

void max7219_wr(char addr, char data)
{
    PORTB &= ~(1 << CS); // SS = 0 // 0b11111011;
    spi_putc(addr);
    spi_putc(data);
    PORTB |= (1 << CS); // SS = 1 //0b00000100;
}

void max7219_config()
{
    max7219_wr(0x0C, 0x00); // Shutdown mode
    max7219_wr(0x09, 0x00); // Decode mode (no decode)
    max7219_wr(0x0A, 0x0F); // Intensity max
    max7219_wr(0x0B, 0x07); // Scan limit
    max7219_wr(0x0C, 0x01); // Turn on
}
void up_dot()
{
    for (int row = 0; row < 8; row++)
    {
        max7219_wr(row + 1, clear);
    }
    max7219_wr(y + 1, (1 << x));
}
ISR(INT0_vect)
{
    x -= 1;
    if (x < 0)
    {
        x = 7; // reset
    }
    up_dot();
}
ISR(INT1_vect)
{
    y -= 1;
    if (y < 0)
    {
        y = 7; // reset
    }
    up_dot();
}
int main()
{
    setting_interrupt();
    spi_init();
    max7219_config();
    sei();
    up_dot();
    while (1)
    {
    }
}
