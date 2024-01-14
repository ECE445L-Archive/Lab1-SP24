// ECE445L Lab 1
// grader output sent to terminal window
// UART baud 115200 bps
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/pll.h"
#include "../inc/ST7735.h"
#include "Lab1Grader.h"

char EID[16]="ABC123";      // replace with your EID
void Pin_Init(void);        // initialize GPIO pin
void Pin_Out(uint32_t out); // output to GPIO pin
void ADC_Init(void);        // Initialize a pin for ADC input 
uint32_t ADC_In(void);      // this is given
uint32_t Convert(uint32_t data); // Convert 12-bit ADC sample to measured resistance in ohms 
void SysTick(void);         // Initialize SysTick for real time sampling 
void EnableInterrupts(void);  // Enable interrupts
void DisableInterrupts(void);  // Enable interrupts
uint32_t R1 = 2200;         // replace this with your R1 in ohms
uint32_t R2;                // measured unknown resistance in ohms
uint32_t Data;
uint32_t Flag;

// step 1, get your specifications
int main(void){ //main1 use to get info
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(INFO);
  while(1){
  }
}
int main2(void){ //main2 use to debug Pin_Init and Pin_Out
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Pin_Init(); // your function
  while(1){
   Pin_Out(0); // your function
   Pin_Out(1); // your function
 }
}
int main3(void){ //main3 use to grade Pin_Init and Pin_Out 
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(GPIO); // this will call your Pin_Init once and your Pin_Out multiple times
  while(1){
  }
}
int main4(void){ //main4 use to debug ADC_Init 
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  ADC_Init(); // your function
  while(1){
   Data = ADC_In(); 
 }
}
int main5(void){ //main5 use to grade ADC_Init 
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(ADC); // this will call your ADC_Init once 
  while(1){
  }
}
// set the global R1 with your specification
int main6(void){ //main6 use to debug Convert 
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  UART0_Init(); // serial data to PC
  for(uint32_t TrueR2 = 500; TrueR2<=4000; TrueR2+=500){
    Data = (4095*TrueR2)/(R1+TrueR2); // simulate circuit and ADC
    R2 = Convert(Data);   // your function
    UART0_OutString("True R2 ="); UART0_OutUDec(TrueR2);
    UART0_OutString(", Data ="); UART0_OutUDec(Data);
    UART0_OutString(", YourR2 ="); UART0_OutUDec(R2); OutCRLF0();
  }
  while(1){
  }
}
int main7(void){ //main7 use to grade Convert 
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(CONVERT); // this will call your Convert 8 times 
  while(1){
  }
}
int main8(void){ //main8 use to grade SysTick  
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(SYSTICK); // this will call your SysTick once 
  while(1){
  }
}
int main9(void){ //main9 use to grade entire lab
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  Lab1Grader(ALL); // this will call and grade all your functions
  while(1){
  }
}
int main10(void){ //main10 use to run the data acquisition system
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(0);                 // set screen to black  
  ST7735_OutString("Real-time DAS\n"); 
  ST7735_OutString(EID); 
  ST7735_OutString("\nR2= "); ST7735_OutUDec5(R2); ST7735_OutString(" ohms");
  Pin_Init(); // your function
  ADC_Init(); // your function
  SysTick();  // your function
  EnableInterrupts();
  while(1){
    if(Flag){  // wait for semaphore
      Flag = 0;
      R2 = Convert(Data);
      ST7735_SetCursor(4,2); ST7735_OutUDec5(R2); 
    }
  }
}
// Interrupt service routine
// Executed at fs
void SysTick_Handler(void){
  Pin_Out(1);      // debugging heartbeat
  Data = ADC_In(); // sample
  Flag = 1;        // set semaphore
  Pin_Out(0);      // debugging heartbeat
}

// version 1: C floating point
// run with compiler options selected for floating-point hardware
volatile float T;    // temperature in C
volatile uint32_t N; // 12-bit ADC value
void Test1(void) {
  for(N=0; N<4096; N++){
    T = 10.0 + 0.009768 * N; 	
  }
}

// version 2: C fixed-point
volatile uint32_t Ti;    // temperature in 0.01 C
volatile uint32_t Ni;    // 12-bit ADC value
void Test2(void){
  for(Ni=0; Ni<4096; Ni++){
    Ti = 1000+ ((125*Ni+64)>>7); 	
  }
}
uint32_t start,stop,elapsed;
void Test3(void);
void Test4(void);
int main11(void){ // main11
  DisableInterrupts();
  PLL_Init(Bus80MHz);
  NVIC_ST_RELOAD_R = 0x00FFFFFF ;// reload value
  NVIC_ST_CURRENT_R = 0;  // any write to current clears it
  NVIC_ST_CTRL_R = 0x05;
  UART0_Init(); // serial data to PC
  UART0_OutString("\n\rTest1 floating point C: ");
  start = NVIC_ST_CURRENT_R;
  Test1();
  stop = NVIC_ST_CURRENT_R; elapsed = (start-stop)&0xFFFFFF;
  UART0_OutUDec(elapsed/80); UART0_OutString(" us ");
  UART0_OutString("\n\rTest2 integer C: ");
  start = NVIC_ST_CURRENT_R;
  Test2();
  stop = NVIC_ST_CURRENT_R; elapsed = (start-stop)&0xFFFFFF;
  UART0_OutUDec(elapsed/80); UART0_OutString(" us ");  
  UART0_OutString("\n\rTest3 floating point assembly: ");
  start = NVIC_ST_CURRENT_R;
  Test3();
  stop = NVIC_ST_CURRENT_R; elapsed = (start-stop)&0xFFFFFF;
  UART0_OutUDec(elapsed/80); UART0_OutString(" us ");
  UART0_OutString("\n\rTest4 integer assembly: ");
  start = NVIC_ST_CURRENT_R;
  Test4();
  stop = NVIC_ST_CURRENT_R; elapsed = (start-stop)&0xFFFFFF;
  UART0_OutUDec(elapsed/80); UART0_OutString(" us "); 
  while(1){
  }
}
// enable GPIO pin for output
// Input: none
// Output: none
// comment: this function must be friendly
void Pin_Init(void){
  // ECE445L students write this for Lab 1

 
}
// output to GPIO pin
// Input: out is 0 or 1
// Output: none
// comment: this function must be friendly
void Pin_Out(uint32_t out){
  // ECE445L students write this for Lab 1

}


// initialize sequencer 3 on ADC0
// software start
// channel matches your circuit specification
void ADC_Init(void){
  // ECE445L students write this
  
  
}
//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// this function is given
uint32_t ADC_In(void){  
	uint32_t result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}

// Convert 12-bit ADC sample to measured resistance in ohms 
// Input: data is 12-bit ADC sample
// Output: resistance R2 in ohms
uint32_t Convert(uint32_t data){
  // ECE445L students write this for Lab 1

  
  return 42; // replace this with solution
}

// Initialize SysTick for real time sampling
// Inputs: none
// Output: none
// do not worry about interrupt priority
// do not EnableInterrupts
void SysTick(void){
  // ECE445L students write this for Lab 1
  // set NVIC_ST_RELOAD_R
  // set NVIC_ST_CTRL_R

}  