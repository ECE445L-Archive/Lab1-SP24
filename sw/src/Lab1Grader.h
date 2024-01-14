#include <stdint.h>

enum GraderMode{
  INFO,
  GPIO,
	ADC,
  CONVERT,
	SYSTICK,
  ALL
};

// ************Lab1Grader*****************
// Inputs: mode
// Outputs: none
void Lab1Grader(enum GraderMode mode);

//------------UART0_Init------------
// Wait for new serial port input
// Initialize the UART for 115,200 baud rate (assuming busfrequency system clock),
// 8 bit word length, no parity bits, one stop bit, FIFOs enabled
// Input: none
//        bus clock frequency in Hz
//        baud rate in bps
// Output: none
void UART0_Init(void);

//------------UART0_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART0_OutChar(char data);

//------------UART0_OutString------------
// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
// Output: none
void UART0_OutString(char *pt);

//-----------------------UART0_OutUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_OutUDec(uint32_t n);

//---------------------OutCRLF0---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void OutCRLF0(void);