#include <Arduino.h>
//-------------------//
#include <avr/io.h>
#include <util/delay.h>
void spi_init()
{
    DDRB = 0b00101110; // Set MOSI, SCK, Reset as Output
    SPCR = 0b01010001; // Enable SPI, Set as Master, Clock = 1 MHz

    // Reset MCP23S17
    PORTB &= 0b11111101;
    __builtin_avr_delay_cycles(100);
    PORTB |= 0b00000010;

    PORTB &= 0b11111011;
    __builtin_avr_delay_cycles(10);
    PORTB |= 0b00000100;
}

void spi_putc(char data)
{
    SPDR = data;
    while (!(SPSR & 0b10000000)); // Wait for transmission to complete
}

void mcp23s17_wr(char addr, char data)
{
    PORTB &= 0b11111011; // SS = 0 (Active Low)
    spi_putc(0b01000000);
    spi_putc(addr);
    spi_putc(data);
    PORTB |= 0b00000100; // SS = 1 (Inactive)
}

char spi_getc()
{
    SPDR = 0xFF; // Send dummy data
    while (!(SPSR & 0b10000000)); // Wait for receive complete
    return SPDR;
}

char mcp23s17_rd(char addr)
{
    char var;
    PORTB &= 0b11111011; // SS = 0
    spi_putc(0b01000001);
    spi_putc(addr);
    var = spi_getc();
    PORTB |= 0b00000100; // SS = 1
    return var;
}

int main()
{
    spi_init();
    mcp23s17_wr(0x00, 0xFF); // Configure GPA0-GPA7 as Input
    mcp23s17_wr(0x01, 0x00); // Configure GPB0-GPB7 as Output

    while (1)
    {
        char rdata = mcp23s17_rd(0x12);
        mcp23s17_wr(0x13, rdata);
        __builtin_avr_delay_cycles(200);
    }
}
