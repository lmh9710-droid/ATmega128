#include <mega128.h>
#include <delay.h>
const char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};#include <mega128.h>
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

U8 led = 0xFE;
U8 EINT4_FLAG_BIT = 0;
U8 EINT5_FLAG_BIT = 0;
U8 EINT6_FLAG_BIT = 0;
U8 EINT7_FLAG_BIT = 0;
U32 num = 0;
U32 display = 0;

void Seg4_out(U32);

void main(void) {
   
   DDRC = 0xFF;
   PORTC = led;
   
   DDRB = 0xF0;
   DDRD = 0xF0;
   DDRG = 0x0F;
   
   PORTB = 0x00;
   PORTD = 0x00; 
   
   EIMSK |= 0xF0;
   EICRB |= 0xEE;
   SREG = 0x80;
   
   while(1) {
     
    Seg4_out(display);
     
    if(EINT4_FLAG_BIT ==1) {
      EINT4_FLAG_BIT = 0;
         led = led << 1;
      led |= 0x01;
      if(led == 0xFF) {
         led = 0xFE;
      }
      PORTC = led;
    }/*end of if(EINT4_FLAG_BIT)*/
    
    if(EINT5_FLAG_BIT ==1) {
      EINT5_FLAG_BIT = 0;
      PORTC = 0xFF;
    }/*end of if(EINT5_FLAG_BIT)*/
    
   if(EINT6_FLAG_BIT ==1) {
      EINT6_FLAG_BIT = 0;
      if(num == 0) {
         num = 9999;
      } 
      else {
         num--;
      }
      
      display = num;
       
   }/*end of if(EINT6_FLAG_BIT)*/
   
   if(EINT7_FLAG_BIT ==1){
      EINT7_FLAG_BIT = 0;
      num++;
      display = num;
      
      if(num >9999){
        num = 0;
      }   
   }/*end of if(EINT7_FLAG_BIT)*/
 }/*end of while*/
}/*end of main */

void Seg4_out(U32 num) {
  
  int i, N1000, N100, N10, N1, buf;
  N1000 = num / 1000;
  buf = num % 1000;
  
  N100 = buf / 100; 
  buf = buf % 100;
  
  N10 = buf / 10;
  N1 = buf % 10;
   
      
      PORTG = 0x08;
      PORTD = ((seg_pat[N1] & 0x0F) << 4)|(PORTD & 0x0F);
      PORTB = (seg_pat[N1] & 0x70)|(PORTB & 0x0F);
      delay_ms(5);
      
      PORTG = 0x04;
      PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
      PORTB = (seg_pat[N10] & 0x70)|(PORTB & 0x0F);
      delay_ms(5);
      
      PORTG = 0x02;
      PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
      PORTB = (seg_pat[N100] & 0x70) | (PORTB & 0x0F);
      delay_ms(5);
      
      PORTG = 0x01;
      PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
      PORTB = (seg_pat[N1000] & 0x70) | (PORTB & 0x0F);
      delay_ms(5);
      
  

}

interrupt [EXT_INT4] void external_int4(void) {
  
  SREG &= 0x7F; 
  EINT4_FLAG_BIT = 1;
  SREG |= 0x80;
  
}

interrupt [EXT_INT5] void external_int5(void) {
   
   SREG &= 0x7F;
   EINT5_FLAG_BIT =1; 
   SREG |= 0x80;
  
}

interrupt [EXT_INT6] void external_int6(void) { 

    SREG &= 0x7F;
    EINT6_FLAG_BIT = 1;
    SREG |= 0x80;
 
}

interrupt [EXT_INT7] void external_int7(void) {
       
    SREG &= 0x7F;
    EINT7_FLAG_BIT = 1;
    SREG |= 0x80;
 
}
