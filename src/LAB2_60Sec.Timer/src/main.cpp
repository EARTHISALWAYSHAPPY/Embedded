#include <avr/io.h>
#define F_CPU 16000000UL // Frequency: 4 MHz
#include <Arduino.h>
#define sw1 PD2
#define sw2 PD3
#define led1 PD4
#define led2 PD5
#define led3 PD6

#define one_second_for_count 62500
uint8_t count_num = 0;
bool work = false;

void Setpin()
{
  // set sw1, sw2, led1(red), led2(yellow), led3(green)
  DDRD |= ((1 << led1) | (1 << led2) | (1 << led3));
  DDRD &= ~((1 << sw1) | (1 << sw2));
  // use timer1
  TCCR1A = 0x00;
  TCCR1B = 0x04; // 256 prescale
  TCNT1 = 0;
}

void off_led()
{
  PORTD &= ~((1 << led1) | (1 << led2) | (1 << led3));
}

void start()
{
  if (!(PIND & (1 << sw1)))
  {
    work = true;
  }
}

void counting()
{
  if ((work == true) && (TCNT1 > one_second_for_count))
  {
    count_num += 1;
    TCNT1 = 0;
  }
}

void binary()
{
  switch (count_num)
  {
  case 0:
    off_led();
    break;
  case 10:
    off_led();
    PORTD |= 0b01000000;
    break;
  case 20:
    off_led();
    PORTD |= 0b00100000;
    break;
  case 30:
    off_led();
    PORTD |= 0b01100000;
    break;
  case 40:
    off_led();
    PORTD |= 0b00010000;
    break;
  case 50:
    off_led();
    PORTD |= 0b01010000;
    break;
  case 60:
    off_led();
    PORTD |= 0b01110000;
    break;
  default:
    if (count_num > 60)
    {
      off_led();
      PORTD |= 0b01110000;
    }
    break;
  }
}

void reset()
{
  if (!(PIND & (1 << sw2)))
  {
    work = false;
    count_num = 0;
    off_led();
  }
}

int main()
{
  Serial.begin(9600);
  Setpin();
  while (true)
  {
    Serial.println(count_num);
    start();
    reset();
    counting();
    binary();
  }
}

// การทำงานที่ต้องการ
// ต้องการสร้าง นาฬกาจับเวลาที่สามารถนับเวลาได้ 60 วินาที โดยมีการแสดงค่าเวลาที่นับได้เป็นเลขฐานสอง ผ่าน
// ทาง LED D1 D2 D3
// การควบคุมโดย เมื่อกด Switch SW1 จะเริ่มจับเวลา LED ติดแสดงเวลาในหน่วยของ 10 วินาที เมื่อครบ 60 วินาที
// ให้ LED ทุกหลอดติด และเมื่อกด SW2 จะเป็นการให้เริ่มต้นการทำงานใหม่
// 1. เริ่มต้น LED ทั้งหมด ดับ (000)
// 2. กด Switch SW1 เพื่อเริ่มจับเวลา จากนี้ทุกๆ 10 วินาที จะทำให้ค่าที่แสดงผลเพิ่มขึ้น 1 000 -> 001 ...- >
// 101 -> 111 เมื่อครบเวลา
// 3. เมื่อต้องการ Reset ค่าการนับให้กลับเป็น 0 ทำได้โดยการกด Switch SW2 1 ครั้ง การกด SW2 นี้จะกดเมื่อใด
// ก็ได้