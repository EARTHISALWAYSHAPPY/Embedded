#include <Arduino.h>
// KUNANON WANYEN 66010103 //
#define F_CPU 16000000UL
#define TIMER1_OVF_vect _VECTOR(13)

#include <avr/io.h>
#include <util/delay.h>
float temp;
void setting_interrupt()
{
  // use sw1(D2) : INT0, sw2(D3) : INT1, Timer1

  EICRA = EICRA | 0b00001010;         // failling edge
  EIMSK |= (1 << INT1) | (1 << INT0); // enable INT1 , INT0

  TCCR1A = 0b00000000;
  TCCR1B = 0b00000101; // prescale 1024
  TIMSK1 = 0b00000001;
  TCNT1 = 49910; //  1000 ms
}
// ---------------------------------- UART ----------------------------------------------
// fscl 200 kHz
// send id , W(0) , R(1)
void serial_begin(int baudrate) // config baudrate
{
  UBRR0 = baudrate; // UBRRn(51) = ( 16x10^6 / (16 x baud rate(19200))) - 1
  UCSR0A = 0b00000000;
  UCSR0B = 0b10011000;
  UCSR0C = 0b00000110;
  sei();
}

void serial_putc(char data)
{
  char busy;
  do
  {
    busy = UCSR0A & 0b00100000;
  } while (busy == 0);
  UDR0 = data;
}

char serial_getc()
{
  char busy;
  do
  {
    busy = UCSR0A & 0b10000000;
  } while (busy == 0);
  return (UDR0);
}

void serial_puts(char *data)
{
  while (*data)
  {
    serial_putc(*data++);
  }
}

// ----------------------------- I2C -------------------------------------

void i2c_init()
{
  TWSR |= (1 << TWPS1);
  TWBR = 2;
  //TWCR |= (1 << TWEN);
}

void i2c_start()
{
  char busy;
  TWCR = 0b10100100; // TWINT,TWSTA,TWEN;
  do
  {
    busy = TWCR & 0b10000000;
  } while (busy == 0);
}

void i2c_stop(void)
{
  TWCR = 0b10010100; // TWINT,TWSTO,TWEN;
  _delay_ms(1);      // wait for a short time
}

void i2c_putc(char data)
{
  char busy;
  TWDR = data;
  TWCR = 0b10000100; // TWINT,TWEN;
  do
  {
    busy = TWCR & 0b10000000;
  } while (busy == 0);
}

char i2c_getc(char ack)
{
  char busy;
  if (ack == 1)
    TWCR = 0b11000100; // TWINT,TWEN,TWEA=1
  else
    TWCR = 0b10000100; // TWINT,TWEN,TWEA=0
  do
  {
    busy = TWCR & 0b10000000;
  } while (busy == 0);
  return TWDR;
}

void mcp9803_wr(char addr, char data)
{
  i2c_start();
  i2c_putc(0b10010000); // ID 
  i2c_putc(addr);
  i2c_putc(data);
  i2c_stop();
}

float mcp9803_rd(char addr)
{
  byte D1,D2;
  i2c_start();
  i2c_putc(0x90); // ID 
  i2c_putc(addr);       

  i2c_start();
  i2c_putc(0x91); // ID 
  // for read d1 d2
  D1 = i2c_getc(1); // get d1 send ack
  D2 = i2c_getc(0); // get d2 send no ack --> stop
  i2c_stop();

  float cal_temp = D1 + (D2 / 256.0);
  return cal_temp;
}

ISR(TIMER1_OVF_vect){
  TCNT1 = 49910;
  char text[6];
  temp = mcp9803_rd(0);
  dtostrf(temp, 5, 3, text);
  serial_puts("Temperature is ");
  serial_puts(text);
  serial_puts(" \r\n");
}

int main()
{
  serial_begin(51);
  i2c_init();
  mcp9803_wr(1, 0x60); // mcp9803 12 bits

  setting_interrupt();
  sei();

  while (true)
  {
    
  }
  return 0;
}