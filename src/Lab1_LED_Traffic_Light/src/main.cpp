#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
void traffic();
void LED_GREEN();
void LED_YELLOW();
void LED_RED();
int main()
{
  DDRD = (DDRD & ~(1 << PD2)) | ((1 << PD4) | (1 << PD5) | (1 << PD6));
  while (true)
  {
    traffic();
  }
}
void traffic(void)
{
  LED_GREEN(); // เขียวติด
  if (!(PIND & (1 << PD2)))
  {
    LED_YELLOW(); // เหลืองติด
    _delay_ms(3000);
    LED_RED(); // แดงติด
    _delay_ms(10000);
  }
}

void LED_GREEN(void)
{
  PORTD = (PORTD & ~((1 << PD4) | (1 << PD5)) | (1 << PD6));
}
void LED_YELLOW(void)
{
  PORTD = (PORTD & ~((1 << PD4) | (1 << PD6)) | (1 << PD5));
}
void LED_RED(void)
{
  PORTD = (PORTD & ~((1 << PD5) | (1 << PD6)) | (1 << PD4));
}
// D3(SW) INPUT (0)
// D4(RED) D5(YEllOW) D6(GREEN) OUTPUT (1)
// 0b00111000
// 1.เริ่มต้น LED D1 (ไฟเขียว) ติด นอกนั้นดับ ทั้งหมด
// 2.เมื่อต้องการข้ามถนน ให้กด Switch SW1 จะทำให้ LED D1 (ไฟเขียว)ดับ และ LED D2 (ไฟเหลือง) ติด
// 3.LED D2 (ไฟเหลือง) จะติดสว่างเป็นเวลา 3 วินาที แล้วดับ
// 4.หลังจากที่ LED D2 (ไฟเหลือง) ดับ ให้ LED D3 (ไฟแดง) ติดสว่าง ให้ข้ามถนนได้
// 5.เมื่อเวลาผ่านไปอีก 10 วินาที LED D3 (ไฟแดง) จะดับ และ LED D1 (ไฟเขียว) ติด นั่นคือ กลับไปข้อ 1 อีกครั้ง