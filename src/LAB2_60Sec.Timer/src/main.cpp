#include <avr/io.h>

#define F_CPU 4000000UL // Frequency: 4 MHz
#define sw1 PD2
#define sw2 PD3
#define led1 PD4
#define led2 PD5
#define led3 PD6
#define time_for_check 78 
#define ten_second 39062  

uint16_t count_time_work = 0;
uint8_t count_time_start_reset = 0;
uint8_t count_num = 0;
bool work = false;

void Setpin()
{
  // ตั้งค่า sw1, sw2, led1(red), led2(yellow), led3(green)
  DDRD = (DDRD & ~((1 << sw1) | (1 << sw2))) | ((1 << led1) | (1 << led2) | (1 << led3));

  TCCR0A = 0x00; 
  TCCR0B = 0x05; 
  TCNT0 = 0;    

  TCCR1A = 0x00; 
  TCCR1B = 0x05; 
  TCNT1 = 0;     
}

void time_zero()
{ 
  count_time_work = 0;
  TCNT1 = 0;   
}

void off_led()
{
  PORTD &= ~((1 << led1) | (1 << led2) | (1 << led3));
}

void start()
{
  if (!(PIND & (1 << sw1)) && (count_time_start_reset >= time_for_check)) 
  {
    work = true;
  }
}

void binary()
{
  if (work == true)
  {
    count_time_work = TCNT1;
    if ((count_num <= 7) && (count_time_work >= ten_second)) 
    {
      off_led();
      switch (count_num)
      {
      case 0:
        off_led();
        count_num++;
       
        break;
      case 1:
        PORTD |= 0b01000000;
        count_num++;
       
        break;
      case 2:
        PORTD |= 0b00100000;
        count_num++;
       
        break;
      case 3:
        PORTD |= 0b01100000;
        count_num++;
        
        break;
      case 4:
        PORTD |= 0b00010000;
        count_num++;
        
        break;
      case 5:
        PORTD |= 0b01010000;
        count_num++;
        
        break;
      case 6:
        PORTD |= 0b01110000;
      default:
        break;
      }
      time_zero();
    }
  }
}

void reset()
{
  if (!(PIND & (1 << sw2)) && (count_time_start_reset >= time_for_check)) 
  {
    time_zero();
    count_num = 0;
    work = false;
    off_led();
  }
}

int main()
{
  Setpin();
  while (1)
  {
    count_time_start_reset = TCNT0; 
    start();                        
    binary();                      
    reset();                      
  }
}
