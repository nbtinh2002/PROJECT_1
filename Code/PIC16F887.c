#include <16F887.h>
#device ADC=10
#fuses INTRC_IO
#use delay(CLOCK=8MHz)
#use RS232(BAUD=9600,BITS=8,STOP=1,PARITY=N,xmit=PIN_C6,rcv=PIN_C7)
#define btn0       PIN_B0 // Positive low
#define btn1       PIN_B1// Positive low
#define btn2       PIN_B2 // Positive low
#define btn3       PIN_B3 // Positive low
#define btn4       PIN_B4 //opening skylight
#define btn5       PIN_B5 //closing skylight
#define relay1     PIN_D0 //device 1
#define relay2     PIN_D1 //device 2
#define relay3     PIN_D2 //device 3
#define relay4     PIN_D3 //device 4
#define mi1        PIN_E0 //l298n
#define mi2        PIN_E1 //l298n
#define rainmodule PIN_C0 //Positive low
#define ht1        PIN_D6 //LOW if the skylight is fully open
#define ht2        PIN_D7 //LOW if the skylight is fully close
int relay1_state;
void control_button(){
   if (input(btn0)==0) 
   {
      delay_ms(50);
      output_toggle(relay1);
      relay1_state = input(relay1);
      while(input(btn0)==0);
      delay_ms(50);
   }
   if (input(btn1)==0) 
   {
      delay_ms(50);
      output_toggle(relay2);
      while(input(btn1)==0);
      delay_ms(50);
   }
   if (input(btn2)==0) 
   {
      delay_ms(50);
      output_toggle(relay3);
      while(input(btn2)==0);
      delay_ms(50);
   }
   if (input(btn3)==0) 
   {
      delay_ms(50);
      output_toggle(relay4);
      while(input(btn3)==0);
      delay_ms(50);
   }
}
void control_skylight(){
   if((input(rainmodule)==0)&&(input(ht2)!=0)){
      output_high(mi1);
      output_low(mi2);
   }
   else if((input(rainmodule)!=0)&&(input(ht2)!=0)&&(input(btn5)==0)){
      output_high(mi1);
      output_low(mi2);
   }
   else if((input(rainmodule)!=0)&&(input(ht1)!=0)&&(input(btn4)==0)){
      output_high(mi1);
      output_low(mi2);
   }
   else{
      output_low(mi1);
      output_low(mi2);
   }
}
void main()
{
   set_tris_b(0b11111111);
   set_tris_c(0b1);
   set_tris_d(0b10011111);
   set_tris_e(0b001);
   
   output_low(relay1);
   output_low(relay2);
   output_low(relay3);
   output_low(relay4);
   output_low(mi1);
   output_low(mi2);
   while(TRUE)
   {
      control_button();
      control_skylight();
      
   }
}




