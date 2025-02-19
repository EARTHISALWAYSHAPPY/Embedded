#define F_CPU 16000000UL
#include  <avr/io.h>
#include  <util/delay.h>

void i2c_start()
{
    char busy;
    TWCR = 0b10100100; // TWINT,TWSTA,TWEN;
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
}

void i2c_stop(void)
{
    char busy;
    TWCR = 0b10010100; // TWINT,TWSTO,TWEN;
    _delay_ms(1) ; //wait for a short time
}

void i2c_putc(char data)
{
    char busy;
    TWDR = data;
    TWCR = 0b10000100; // TWINT,TWEN;
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
}

char i2c_getc(char ack)
{
    char busy;
    if (ack==1) TWCR = 0b11000100; // TWINT,TWEN,TWEA=1
    else        TWCR = 0b10000100; // TWINT,TWEN,TWEA=0
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
    return TWDR;
}

void mcp23017_wr(char id, char addr, char data)
{  
    i2c_start();
    if (id==0) i2c_putc(0b01000000);
    else       i2c_putc(0b01000010);
    i2c_putc(addr);
    i2c_putc(data);
    i2c_stop(); 
}

char mcp23017_rd(char id, char addr)
{
    char var;
    
    i2c_start();
    if (id==0) i2c_putc(0b01000000);
    else       i2c_putc(0b01000010);
    i2c_putc(addr);
    i2c_start();
    if (id==0) i2c_putc(0b01000001);
    else       i2c_putc(0b01000011);
    var = i2c_getc(0); // ack=0
    i2c_stop(); 
    return var;
}

void i2c_init()
{
    DDRB = 0b00000010; // D9 PB1 Reset as Output 
    TWSR = 0b00000000; // set prescaler bits to zero
    TWBR = 72;         // SCL frequency is 100K for 16Mhz
    TWCR = 0b00000100; // enab1e TWI module
    // Reset
    PORTB = PORTB & 0b11111101;
    _delay_ms(100);
    PORTB = PORTB | 0b00000010;
}

int main(void)
{
  char rdata;
  
  i2c_init();
  
  mcp23017_wr(0,0x00,0xFF); // Input
  mcp23017_wr(1,0x00,0x00); // Output

  while(1) {
      rdata = mcp23017_rd(0,0x12);
      mcp23017_wr(1,0x12,rdata);
      _delay_ms(200);
  }
}