# Embedded System KMITL 01046714

## Documentation📄

- [Language Reference](https://docs.arduino.cc/language-reference/?_gl=1*n81774*_up*MQ..*_ga*MjAwMjgzMjg5OC4xNzM5MTkwMjk2*_ga_NEXN8H46L5*MTczOTE5MDI5NC4xLjEuMTczOTE5MDI5OS4wLjAuOTU5MDUyMjI.#functions)

## Recommend command👍

### Part Interrupt :

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

### Part ADC :

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
 
 ### Part UART Serial :

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



