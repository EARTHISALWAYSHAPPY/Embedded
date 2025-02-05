#include <Arduino.h>
//////////////////////////////////////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#define UART_RX_vect _VECTOR (18)
#define pin_temp 3 // ADC3
int adc_temp;
float cel, fah;
char choice, text[10];
void ADC_init() {
    ADMUX = 0b01000000; //0 1 : AVcc
    ADCSRA = 0b10000111; // ADC : Enable , Prescale : 128
}
int ADC_read(char ch) {
    char busy;
    ADMUX = ADMUX & 0b11110000;
    ADMUX = ADMUX | ch;
    ADCSRA = ADCSRA | 0b01000000;
    do {
        busy = ADCSRA & 0b01000000;
    } while (busy != 0);
    return(ADC);
}
void Serial_begin(int baudrate) {
    UBRR0 = baudrate; // UBRRn = ( 16x10^6 / (16 x baud rate(38400))) - 1
    UCSR0A = 0b00000000;
    UCSR0B = 0b10011000;
    UCSR0C = 0b00000110;
    sei();
}
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
void Serial_puts(char* data)
{
    while (*data)
    {
        Serial_putc(*data++);
    }
}
float cel2fah() {
    fah = (((9 * cel) / 5) + 32);
    return(fah);
}
void condition() {
    int choice_int = choice - '0'; //ASCII
    switch (choice_int)
    {
    case 1:
        dtostrf(cel, 8, 2, text);
        Serial_puts("Temperature is ");
        Serial_puts(text);
        Serial_puts(" C\r\n");
        break;
    case 2:
        fah = cel2fah();
        dtostrf(fah, 8, 2, text);
        Serial_puts("Temperature is ");
        Serial_puts(text);
        Serial_puts(" F\r\n");
        break;
    default:
        Serial_puts("Please type only 1 or 2\r\n");
        break;
    }
    Serial_puts("Read Temperature in Celsius (1) or Fahrenheit (2): ");
}
ISR(UART_RX_vect) {
    adc_temp = ADC_read(pin_temp);
    cel = ((float)adc_temp * 5.0 / 1024.0) * 100.0;
    choice = Serial_getc();
    Serial_putc(choice);
    Serial_puts("\r\n");
    condition();
}
int main() {
    Serial_begin(25); // 38400
    ADC_init();
    Serial_puts("Read Temperature in Celsius (1) or Fahrenheit (2): ");
    while (true) {
    }
}




