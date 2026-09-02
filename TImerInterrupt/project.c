/****************************************************************************************************************************/
/* project : ATmega128                                                                                              */ 	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
/* Name : Lee Min Ha         																											*/
/* Modify :           																										*/
/* Date : 2026.08.28               																									*/
/* Description : 																											*/	
/****************************************************************************************************************************/

#include <mega128.h>
#include <delay.h> 
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32; 

typedef struct FLAG_FIELD {
 U8 EINT4_FLAG_BIT : 1;
 U8 EINT5_FLAG_BIT : 1;
 U8 EINT6_FLAG_BIT : 1;
 U8 EINT7_FLAG_BIT : 1; 
 U8 TIMER1_CTC_FLAG_BIT : 1;
 U8 TIMER2_OVF_FLAG_BIT : 1;
 U8 TIMER0_OVF_FLAG_BIT : 1;
 U8 REMAIN_BIT : 1;
}_BYTE_INT_CHECK;

_BYTE_INT_CHECK INT_CHECK_ALL;

const char seg_pat[10]= {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
U32 display=0;
U32 TIMER2_4500ms_COUNT = 0;
U32 TIMER1_1000ms_COUNT = 0;
U32 TEMP_ADC_VALUE = 0;
U8 ADC_RESULT_FLAG = 0;

U8 N1000=0, N100=0, N10=0, N1=0;
U8 NV100=0, NV10=0, NV1=0;



void Seg4_out(U32);

void AD_disp(U32);

void EXT4_TIMER2(void);

void AD_Value(U32);

void UART_volt(void);

void main(void) {
 
 DDRB = 0xF0;
 DDRD = 0xF0;
 DDRG = 0x0F;
 
 PORTB = 0x00;
 PORTD = 0x00;
 
 EIMSK = 0b11110000;   
 EICRB = 0b10111011; 
 
 SREG |= 0x80;
 Seg4_out(1994);
 
 while(1) {
     
  if(ADC_RESULT_FLAG ==0) Seg4_out(1110);
  else AD_disp(TEMP_ADC_VALUE);
 
 
  if(INT_CHECK_ALL.EINT4_FLAG_BIT==1){
     EXT4_TIMER2();
	 INT_CHECK_ALL.EINT4_FLAG_BIT = 0;
	 
  }
  
  if(INT_CHECK_ALL.EINT5_FLAG_BIT==1){
	 EXT5_TIMER2
     INT_CHECK_ALL.EINT5_FLAG_BIT = 0;
  
  }
  if(INT_CHECK_ALL.TIMER2_OVF_FLAG_BIT==1){
      TIMER2_4500ms_COUNT++;
	  INT_CHECK_ALL.TIMER2_OVF_FLAG_BIT=0;
	  if(TIMER2_4500ms_COUNT >= 450){
		TIMER2_4500ms_COUNT =0;
		ADC_RESULT_FLAG++;
		ADMUX = 0x07;
		ADCSRA = 0xE2;
		delay_us(100);
		TEMP_ADC_VALUE = (int)ADCL + ((int)ADCH << 8);
		
		if(ADC_RESULT_FLAG ==10){
		   ADC_RESULT_FLAG=0;
		   TCCR2 = 0b00000000;                               
		   TIMSK &= 0b10111111;         
			   
     	}
	  }
  }

  if(INT_CHECK_ALL.TIMER1_CTC_FLAG_BIT==1){
     TIMER1_1000ms_COUNT++;
	 INT_CHECK_ALL.TIMER1_CTC_FLAG_BIT=0;
	 if(TIMER_1000ms_COUNT >= 100){
	   TIMER2_4500ms_COUNT =0;
	   ADMUX = 0x07;
	   ADCSRA = 0xE2;
	   delay_us(100);
	   TEMP_ADC_VALUE =(int)ADCL + ((int)ADCH << 8);
	   AD_Value(TEMP_ADC_VALUE);
	   UART()
	 }
	 
    
  
  }	  
 
 
 }
  

}


void Seg4_out(U32 num) {
  
  U32 N1000, N100, N10, N1, buf;
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

void AD_disp(U32 val){

    float fval;
    int ival, buf;

    fval = (float)val * 5.0/1024.0;
    ival = (int)(fval* 1000.0);
    
    N1000 = ival/1000;
    buf = ival%1000;
    
    N100 = buf/100;
    buf = buf % 100;
    
    N10 = buf/10;
    N1 = buf % 10;
    
    PORTG = 0b00001000;         
    // 전압 값으로 변환
    // 반올림 후 정수화, (소수 둘째자리까지) 
    // 정수부 추출
    // 소수 첫째 자리추출
    // 소수 둘째 자리추출
    // PG3=1, 소수 셋째 자리
    PORTD = ((seg_pat[N1] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1] & 0x70 ) | (PORTB & 0x0F);
    delay_ms(1);
    PORTG = 0b00000100;         
    // PG2=1, 소수 둘째 자리
    PORTD = ((seg_pat[N10] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N10] & 0x70 ) | (PORTB & 0x0F); 
    delay_ms(1);                          
    PORTG = 0b00000010;         
    // PG1=1, 소수 첫째 자리
    PORTD = ((seg_pat[N100] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N100] & 0x70 ) | (PORTB & 0x0F);             
    delay_ms(1); 
    PORTG = 0b0000001;  
    // PG0=1, 정수부 
    PORTD = ((seg_pat[N1000] & 0x0F) << 4) | (PORTD & 0x0F);
    PORTB = (seg_pat[N1000] & 0x70 ) | (PORTB & 0x0F);      
    PORTB = PORTB | 0x80;
    delay_ms(1);  
    

}


interrupt [EXT_INT4] void external_int4(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT4_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [EXT_INT5] void external_int5(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT5_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [EXT_INT6] void external_int6(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT6_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [TIM2_OVF] void TIMER2_OVF_int(void){
  SREG &=0x7F;
  INT_CHECK_ALL.TIMER2_OVF_FLAG_BIT=1;
  TCNT2 = 100;
  SREG |= 0x80;
  
}

interrupt [TIM1_COMPA] void TIMER1_CTC_int(void){
  SREG &=0x7F;
  INT_CHECK_ALL.TIMER1_CTC_FLAG_BIT =1;
  SREG |=0x80;

}

void AD_Value(U32 val){

    float fval;
    int ival, buf;

    fval = (float)val * 5.0/1024.0;
    ival = (int)(fval* 1000.0);
    
    NV100 = ival/100;
    buf = ival%100;
    
    NV10 = buf/10;
    NV1 = buf % 10;   
}


interrupt [EXT_INT4] void external_int4(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT4_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [EXT_INT5] void external_int5(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT5_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [EXT_INT6] void external_int6(void){
  SREG &=0X7F;
  INT_CHECK_ALL.EINT6_FLAG_BIT=1;
  SREG |=0x80;
}

interrupt [TIM2_OVF] void TIMER2_OVF_int(void){
  SREG &=0x7F;
  INT_CHECK_ALL.TIMER2_OVF_FLAG_BIT=1;
  TCNT2 = 100;
  SREG |= 0x80;
  
}

interrupt [TIM1_COMPA] void TIMER1_CTC_int(void){
  SREG &=0x7F;
  INT_CHECK_ALL.TIMER1_CTC_FLAG_BIT =1;
  SREG |=0x80;

}

void EXT4_TIMER2(void){
   TIMSK |= 0b01000000;
   TCCR2 |=0b00000101;
   TCNT2 = 100;
}    

void EXT5_TIMER1(void){
    TCCR1A = 0b00000000;                        
    TCCR1B = 0b00001101;                       
    TCCR1C = 0x00;
	
	TCNT1H =0x00;
	TCNT1L =0x00;
	OCR1AH =0x00;
	OCR1AL =0x9B;
	
	DDRE |= 0x08;

}

void UART_volt(void){
    UCSR0A = 0x0;               
    UCSR0B = 0b00001000;         
    UCSR0C = 0b00000110;         
    UBRR0H = 0;                 
    UBRR0L = 103;  
    while((UCSR0A & 0x20) == 0x0);        
     UDR0 =NV100+0x30;                 
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='.';                       
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =NV10+0x30;                  
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =NV1+0x30;                   
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 ='V';                        
    while((UCSR0A & 0x20) == 0x0);          
     UDR0 =' ';                       


}
