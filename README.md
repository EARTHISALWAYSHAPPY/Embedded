# Embedded System KMITL 01046714

## Documentation📄

- [Language Reference](https://docs.arduino.cc/language-reference/?_gl=1*n81774*_up*MQ..*_ga*MjAwMjgzMjg5OC4xNzM5MTkwMjk2*_ga_NEXN8H46L5*MTczOTE5MDI5NC4xLjEuMTczOTE5MDI5OS4wLjAuOTU5MDUyMjI.#functions)

## Recommend command👍

### ❗Part Interrupt :

```code
#define INT0_vect _VECTOR(1)
#define INT1_vect _VECTOR(2)
#define TIMER1_OVF_vect _VECTOR(13)

ISR(INT0_vect) 
{
  ////////Code////////
}
ISR(INT1_vect) 
{
  ////////Code////////
}
ISR(TIMER1_OVF_vect)
{
  ////////Code////////
}
```

### ❗Part ADC :

config ADC :

```code
  void ADC_init()
{
  ADMUX = 0b01000000;  // 0 1 : AVcc , ADC1
  ADCSRA = 0b10000111; // ADC : Enable , Prescale : 128
}
```

read ADC PORT (0-7) :

```code
  int ADC_read(char port) //config port
{
  char busy;
  ADMUX = ADMUX & 0b11110000;
  ADMUX = ADMUX | port;
  ADCSRA = ADCSRA | 0b01000000;
  do
  {
    busy = ADCSRA & 0b01000000;
  } while (busy != 0);
  return (ADC);
}
```
 
 ### ❗Part UART Serial :

Serial_begin : 

```code
  void Serial_begin(int baudrate) // config baudrate
{
  UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(38400))) - 1
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000;
  UCSR0C = 0b00000110;
  sei();
}
```

Serial_putc : 

```code
  void Serial_putc(char data)
{
  char busy;
  do
  {
    // Check data register empty?
    busy = UCSR0A & 0b00100000;
  } while (busy == 0);
  UDR0 = data;
}
```

Serial_getc :

```code
  char Serial_getc()
{
  char busy;
  do
  {
    // Check receive data complete?
    busy = UCSR0A & 0b10000000;
  } while (busy == 0);
  return (UDR0);
}
```

Serial_puts :

```code
  void Serial_puts(char *data)
{
  while (*data)
  {
    Serial_putc(*data++);
  }
}
```
### ❗Part SPI :

spi_init :
```code
void spi_init()
{
    DDRB = 0b00101110; // Set MOSI, SCK, Reset as Output
    SPCR = 0b01010001; // Enable SPI, Set as Master, Clock = 1 MHz
}
```

spi_putc :
```code
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
```

spi_getc :
```code
char spi_getc()
{
    SPDR = 0xFF; // Send dummy data
    while (!(SPSR & 0b10000000)); // Wait for receive complete
    return SPDR;
}
```

mcp23s17_wr (write for mcp23s17) :
```code
void mcp23s17_wr(char addr, char data)
{
    PORTB &= 0b11111011; // SS = 0 (Active Low)
    spi_putc(0b01000000);
    spi_putc(addr);
    spi_putc(data);
    PORTB |= 0b00000100; // SS = 1 (Inactive)
}
```

mcp23s17_rd (read for mcp23s17) : 
```code
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
```

max7219_config :
```code
void max7219_config()
{
    max7219_wr(0x0C, 0);    // Shutdown
    max7219_wr(0x09, 0x00); // decode mode
    max7219_wr(0x0A, 0x00); // light intensity
    max7219_wr(0x0B, 0x07); // scan limit
    max7219_wr(0x0C, 1);    // turn ON
}
```

max7219_wr (write for max7219) :
```code
void max7219_wr(char addr, char data)
{
    PORTB &= 0b11111011; // SS = 0 do
    spi_putc(addr);
    spi_putc(data);
    PORTB |= 0b00000100; // SS = 1 dont
}
```

### ❗Part I2C :

i2c_init :
```code
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
```

i2c_start :
```code
void i2c_start()
{
    char busy;
    TWCR = 0b10100100; // TWINT,TWSTA,TWEN;
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
}
```

i2c_stop :
```code
void i2c_stop()
{
    char busy;
    TWCR = 0b10010100; // TWINT,TWSTO,TWEN;
    _delay_ms(1) ; //wait for a short time
}
```

i2c_putc :
```code
void i2c_putc(char data)
{
    char busy;
    TWDR = data;
    TWCR = 0b10000100; // TWINT,TWEN;
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
}
```

i2c_getc :
```code
char i2c_getc(char ack)
{
    char busy;
    if (ack==1) TWCR = 0b11000100; // TWINT,TWEN,TWEA=1
    else        TWCR = 0b10000100; // TWINT,TWEN,TWEA=0
    do { busy = TWCR & 0b10000000; }
    while (busy==0);
    return TWDR;
}
```

mcp23017_rd (read for mcp23017)
```code
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
```

mcp23017_wr (write for mcp23017) :
```code
void mcp23017_wr(char id, char addr, char data)
{  
    i2c_start();
    if (id==0) i2c_putc(0b01000000);
    else       i2c_putc(0b01000010);
    i2c_putc(addr);
    i2c_putc(data);
    i2c_stop(); 
}
```


