#include <mega128.h>
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

U8 led = 0xFE;
U16 cnt1 = 0;
U8 cnt2 = 0;

void main(void){
	DDRC = 0xFF;
	PORTC = led;
	
	TIMSK = 0x81;
	TCCR0 = 0x07;
	TCNT0 = 0x00;
	
	TCCR2 =0x0D;   TCNT2 =0x00;
	OCR2 = 155;
		
	SREG |= 0x80;
	while(1) {
	
	  if(cnt1 >=280) {
        cnt1 = 0; 
        cnt2++;     
        if(cnt2==1) PORTC = 0xF0;    
        else if(cnt2==2) PORTC = 0x0F; 
        else if(cnt2>=3) cnt2=0;
      
       /*	TCCR0 = 0x07;*/
     }
    
    }

}
interrupt [TIM0_OVF] void timer_int0(void) {
  
  TCNT0 = 0x00;    
  if(cnt2==0) {
  
      led = led << 1;
      led |= 0x01;
      
      if(led == 0xFF) {
       
        led = 0xFE;
      }
      PORTC = led;   
  }
}

interrupt [TIM2_COMP] void timer_int2(void) {
  
/* TCCR0 &=0b11111000;*/ 
  cnt++;

}



