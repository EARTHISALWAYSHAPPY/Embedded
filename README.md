# 🔧 Embedded System Reference — KMITL 01046714

> A comprehensive quick-reference guide for AVR/Arduino embedded systems programming — covering Interrupts, ADC, UART, SPI, and I²C protocols.

![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-blue?style=flat-square&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-AVR%20%7C%20Arduino-teal?style=flat-square&logo=arduino)
![Course](https://img.shields.io/badge/Course-KMITL%2001046714-orange?style=flat-square)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)

---

## 📚 Table of Contents

- [📄 Documentation](#-documentation)
- [⚡ Interrupt](#-interrupt)
- [📡 ADC (Analog-to-Digital Converter)](#-adc-analog-to-digital-converter)
- [🔌 UART Serial Communication](#-uart-serial-communication)
- [🔄 SPI Communication](#-spi-communication)
- [🔗 I²C Communication](#-i2c-communication)

---

## 📄 Documentation

| Resource | Link |
|----------|------|
| Arduino Language Reference | [docs.arduino.cc/language-reference](https://docs.arduino.cc/language-reference/?_gl=1*n81774*_up*MQ..*_ga*MjAwMjgzMjg5OC4xNzM5MTkwMjk2*_ga_NEXN8H46L5*MTczOTE5MDI5NC4xLjEuMTczOTE5MDI5OS4wLjAuOTU5MDUyMjI.#functions) |

---

## ⚡ Interrupt

External and timer interrupt vectors for AVR microcontrollers.

```c
#define INT0_vect       _VECTOR(1)   // External Interrupt 0
#define INT1_vect       _VECTOR(2)   // External Interrupt 1
#define TIMER1_OVF_vect _VECTOR(13)  // Timer1 Overflow

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

---

## 📡 ADC (Analog-to-Digital Converter)

### Initialize ADC

```c
void ADC_init()
{
  ADMUX  = 0b01000000;  // Reference: AVcc | Channel: ADC0
  ADCSRA = 0b10000111;  // ADC Enable | Prescaler: 128
}
```

### Read ADC Channel (0–7)

```c
int ADC_read(char port)
{
  char busy;
  ADMUX = ADMUX & 0b11110000;   // Clear channel selection
  ADMUX = ADMUX | port;         // Set desired channel
  ADCSRA = ADCSRA | 0b01000000; // Start conversion

  do {
    busy = ADCSRA & 0b01000000;
  } while (busy != 0);          // Wait for conversion complete

  return (ADC);
}
```

---

## 🔌 UART Serial Communication

### Initialize Serial

```c
void Serial_begin(int baudrate)
{
  // UBRRn = (F_CPU / (16 × baudrate)) - 1
  // e.g. 38400 baud → UBRR0 = 25
  UBRR0  = baudrate;
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000; // RX Enable | TX Enable | RX Interrupt Enable
  UCSR0C = 0b00000110; // 8-bit data, 1 stop bit
  sei();               // Enable global interrupts
}
```

### Transmit a Character

```c
void Serial_putc(char data)
{
  char busy;
  do {
    busy = UCSR0A & 0b00100000; // Wait: Data Register Empty?
  } while (busy == 0);
  UDR0 = data;
}
```

### Receive a Character

```c
char Serial_getc()
{
  char busy;
  do {
    busy = UCSR0A & 0b10000000; // Wait: Receive Complete?
  } while (busy == 0);
  return (UDR0);
}
```

### Transmit a String

```c
void Serial_puts(char *data)
{
  while (*data)
  {
    Serial_putc(*data++);
  }
}
```

---

## 🔄 SPI Communication

### Initialize SPI

```c
void spi_init()
{
  DDRB = 0b00101110; // MOSI, SCK, SS → Output
  SPCR = 0b01010001; // SPI Enable | Master Mode | Clock = 1 MHz
}
```

### Transmit via SPI

```c
void spi_putc(char data)
{
  char busy;
  SPDR = data;                      // Load data into SPI buffer
  do {
    busy = SPSR & 0b10000000;       // Wait: Transmission Complete?
  } while (busy == 0);
}
```

### Receive via SPI

```c
char spi_getc()
{
  SPDR = 0xFF;                      // Send dummy byte to clock in data
  while (!(SPSR & 0b10000000));     // Wait for receive complete
  return SPDR;
}
```

---

### 🔩 MCP23S17 (SPI GPIO Expander)

**Write to register:**

```c
void mcp23s17_wr(char addr, char data)
{
  PORTB &= 0b11111011;   // CS = LOW (Active)
  spi_putc(0b01000000);  // Control byte (Write)
  spi_putc(addr);        // Register address
  spi_putc(data);        // Data
  PORTB |= 0b00000100;   // CS = HIGH (Inactive)
}
```

**Read from register:**

```c
char mcp23s17_rd(char addr)
{
  char var;
  PORTB &= 0b11111011;   // CS = LOW
  spi_putc(0b01000001);  // Control byte (Read)
  spi_putc(addr);
  var = spi_getc();
  PORTB |= 0b00000100;   // CS = HIGH
  return var;
}
```

---

### 💡 MAX7219 (SPI LED Driver)

**Configure MAX7219:**

```c
void max7219_config()
{
  max7219_wr(0x0C, 0);    // Shutdown mode ON
  max7219_wr(0x09, 0x00); // No decode mode
  max7219_wr(0x0A, 0x00); // Minimum intensity
  max7219_wr(0x0B, 0x07); // Scan all 8 digits
  max7219_wr(0x0C, 1);    // Normal operation
}
```

**Write to MAX7219:**

```c
void max7219_wr(char addr, char data)
{
  PORTB &= 0b11111011;   // CS = LOW
  spi_putc(addr);
  spi_putc(data);
  PORTB |= 0b00000100;   // CS = HIGH
}
```

---

## 🔗 I²C Communication

### Initialize I²C

```c
void i2c_init()
{
  DDRB = 0b00000010;   // PB1 (Reset pin) → Output
  TWSR = 0b00000000;   // Prescaler = 1
  TWBR = 72;           // SCL = 100 kHz @ 16 MHz
  TWCR = 0b00000100;   // Enable TWI

  // Hardware reset sequence
  PORTB = PORTB & 0b11111101;
  _delay_ms(100);
  PORTB = PORTB | 0b00000010;
}
```

### Start Condition

```c
void i2c_start()
{
  char busy;
  TWCR = 0b10100100;                  // TWINT | TWSTA | TWEN
  do { busy = TWCR & 0b10000000; }
  while (busy == 0);
}
```

### Stop Condition

```c
void i2c_stop()
{
  TWCR = 0b10010100;   // TWINT | TWSTO | TWEN
  _delay_ms(1);
}
```

### Transmit a Byte

```c
void i2c_putc(char data)
{
  char busy;
  TWDR = data;
  TWCR = 0b10000100;                  // TWINT | TWEN
  do { busy = TWCR & 0b10000000; }
  while (busy == 0);
}
```

### Receive a Byte

```c
char i2c_getc(char ack)
{
  char busy;
  if (ack == 1) TWCR = 0b11000100;   // ACK:  TWINT | TWEN | TWEA
  else          TWCR = 0b10000100;   // NACK: TWINT | TWEN
  do { busy = TWCR & 0b10000000; }
  while (busy == 0);
  return TWDR;
}
```

---

### 🔩 MCP23017 (I²C GPIO Expander)

**Read from register:**

```c
char mcp23017_rd(char id, char addr)
{
  char var;
  i2c_start();
  i2c_putc(id == 0 ? 0b01000000 : 0b01000010); // Write address
  i2c_putc(addr);
  i2c_start();                                   // Repeated START
  i2c_putc(id == 0 ? 0b01000001 : 0b01000011); // Read address
  var = i2c_getc(0);                             // Read with NACK
  i2c_stop();
  return var;
}
```

**Write to register:**

```c
void mcp23017_wr(char id, char addr, char data)
{
  i2c_start();
  i2c_putc(id == 0 ? 0b01000000 : 0b01000010); // Write address
  i2c_putc(addr);
  i2c_putc(data);
  i2c_stop();
}
```

---

## 🗂️ Repository Structure

```
Embedded/
├── src/          # Source code files
├── PDF&DOC/      # Supporting documents and datasheets
└── README.md     # This file
```

---

## 🏫 About

This repository is part of the **Embedded Systems** course at **King Mongkut's Institute of Technology Ladkrabang (KMITL)**, course code `01046714`.

---

*Made with ❤️ by EARTHISALWAYSHAPPY*