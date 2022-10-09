/**
 * @file Lab1.c
 * @author your name (you@domain.com), Jonathan Valvano, Matthew Yu
 *    <TA NAME and LAB SECTION # HERE>
 * @brief
 *    Possible main program to test Lab 1.
 *    Feel free to edit this to match your specifications.
 *
 *    For this lab, the student must implement four functions:
 *       - ST7735_sDecOut3   (Defined in lib/fixed/fixed.h L19)
 *       - ST7735_uBinOut5   (Defined in lib/fixed/fixed.h L38)
 *       - ST7735_XYplotInit (Defined in inc/ST7735.h L557)
 *       - ST7735_XYplot     (Defined in inc/ST7735.h L570)
 * @version 1.2.0
 * @date 2022-10-08 <REPLACE WITH DATE OF LAST REVISION>
 *
 * @copyright Copyright (c) 2022
 * @note Pinout:
 *    Backlight (pin 10) connected to +3.3 V
 *    MISO (pin 9) unconnected
 *    SCK (pin 8) connected to PA2 (SSI0Clk)
 *    MOSI (pin 7) connected to PA5 (SSI0Tx)
 *    TFT_CS (pin 6) connected to PA3 (SSI0Fss)
 *    CARD_CS (pin 5) unconnected
 *    Data/Command (pin 4) connected to PA6 (GPIO)
 *    RESET (pin 3) connected to PA7 (GPIO)
 *    VCC (pin 2) connected to +3.3 V
 *    Gnd (pin 1) connected to ground
 */

/** File includes. */
#include <stdio.h>
#include <stdint.h>
#include "./inc/ST7735.h"
#include "./inc/PLL.h"
#include "./inc/tm4c123gh6pm.h"
#include "./lib/fixed/fixed.h"


/** MMAP Pin definitions. */
#define PF2   (*((volatile uint32_t *)0x40025010))
#define PF3   (*((volatile uint32_t *)0x40025020))
#define PF4   (*((volatile uint32_t *)0x40025040))


/** Function declarations. */
/**
 * @brief DelayWait10ms delays the current process by n*10ms. Approximate.
 * 
 * @param n Number of times to delay 10ms.
 * @note Based on a 80MHz clock.
 */
void DelayWait10ms(uint32_t n);

/**
 * @brief PortF_Init initializes PF4 (Left Button <=> SW1) as an input and PF2
 *        (Blue onboard LED) as a digital output.
 * 
 */
void PortF_Init(void);

/**
 * @brief Blocks the current process until PF4 (Left Button <=> SW1) is pressed.
 */
void Pause(void);


/** Global variables. */
// const will place these structures in ROM
struct outTestCase1{    // used to test routines
  int32_t InNumber;     // test input number
  char OutBuffer[14];   // Output String
};
typedef const struct outTestCase1 outTestCaseType1;
outTestCaseType1 outTests1[13] = {
    {     0,  " =   0.000?\r" }, //     0/1000 = 0.000
    {     4,  " =   0.004?\r" }, //     4/1000 = 0.004
    {    -5,  " =  -0.005?\r" }, //    -5/1000 = -0.005
    {   178,  " =   0.178?\r" }, //    178/1000 = 0.178
    {  -1254, " =  -1.254?\r" }, //  -1254/1000 = -1.254
    {  9999,  " =   9.999?\r" }, //   9999/1000 = 9.909
    { -10000, " = -10.000?\r" }, // -10000/1000 = -10.000
    {  12345, " =  12.345?\r" }, //  12345/1000 = 12.345
    { -56789, " = -56.789?\r" }, // -56789/1000 = -56.789
    { -99999, " = -99.999?\r" }, // -9999/1000 = -99.999
    {  99999, " =  99.999?\r" }, //  9999/1000 = 99.999
    { 100000, " =  **.***?\r" }, // positive error
    {-100000, " = -**.***?\r" }  // negative error
};

// const will place these structures in ROM
struct outTestCase2{   // used to test routines
  uint32_t InNumber;   // test input number
  char OutBuffer[12];  // Output String
};
typedef const struct outTestCase2 outTestCaseType2;
outTestCaseType2 outTests2[14] = {
    {     0,  " =   0.00?\r" }, //      0/32 = 0.00
    {     1,  " =   0.03?\r" }, //      1/32 = 0.03
    {     5,  " =   0.16?\r" }, //      5/32 = 0.16
    {   100,  " =   3.13?\r" }, //    100/32 = 3.13
    {   127,  " =   3.97?\r" }, //    127/32 = 3.97
    {   252,  " =   7.88?\r" }, //    252/32 = 7.88
    {   535,  " =  16.72?\r" }, //    535/32 = 10.72
    {  2560,  " =  80.00?\r" }, //   2560/32 = 80.00
    {  6092,  " = 190.38?\r" }, //   6092/32 = 190.38
    { 13000,  " = 406.25?\r" }, //  13000/32 = 406.25
    { 16383,  " = 511.97?\r" }, //  16383/32 = 511.97
    { 17283,  " = 540.09?\r" }, //  17283/32 = 540.09
    { 31999,  " = 999.97?\r" }, //  31999/32 = 999.97
    { 32000,  " = ***.**?\r" }  // error
};

/** 180 points on a circle. */
const int32_t CircleXbuf[180] = { 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70, 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999 };
const int32_t CircleYbuf[180] = { 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999, 2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798, 1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176, 1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70, 0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939, -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576, -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941, -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970, -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658, -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060, -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140, -70 };
/** 50 points on a star. */
const int32_t StarXbuf[50] = { 0, -6, -12, -18, -24, -30, -35, -41, -47, -53, 59, 53, 47, 41, 35, 30, 24, 18, 12, 6, 95, 76, 57, 38, 19, 0, -19, -38, -57, -76, -59, -44, -28, -13, 3, 18, 33, 49, 64, 80, -95, -80, -64, -49, -33, -18, -3, 13, 28, 44 };
const int32_t StarYbuf[50] = { 190, 172, 154, 136, 118, 100, 81, 63, 45, 27, 9, 27, 45, 63, 81, 100, 118, 136, 154, 172, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 9, 20, 31, 43, 54, 65, 76, 87, 99, 110, 121, 110, 99, 87, 76, 65, 54, 43, 31, 20 };


/** Main functions. */
int main(void) {
    uint32_t i;
    PLL_Init(Bus80MHz);
    PortF_Init();
    ST7735_InitR(INITR_REDTAB);
    while(1) {
        /* Plot sDecOut3 results. */
        ST7735_FillScreen(ST7735_BLACK);
        ST7735_SetCursor(0,0);
        printf("Lab 1\rST7735_sDecOut3\r");
        for(i = 0; i < 13; i++) {
            ST7735_sDecOut3(outTests1[i].InNumber);  // your solution
            ST7735_OutString((char*)outTests1[i].OutBuffer); // expected solution
        }
        Pause();

        /* Plot uBinOut5 results. */
        ST7735_FillScreen(0);  // set screen to black
        ST7735_SetCursor(0,0);
        printf("ST7735_uBinOut5\r");
        for(i = 0; i < 14; i++) {
            ST7735_uBinOut5(outTests2[i].InNumber);  // your solution
            ST7735_OutString((char*)outTests2[i].OutBuffer); // expected solution
        }
        Pause();

        /* Plot circle. */
        ST7735_XYplotInit("Circle", -2500, 2500, -2500, 2500);
        ST7735_XYplot(180,(int32_t *)CircleXbuf, (int32_t *)CircleYbuf, ST7735_BLUE);
        Pause();

        /* Plot Star. */
        ST7735_XYplotInit("Star- upper right", -450, 150, -400, 200);
        ST7735_XYplot(50,(int32_t *)StarXbuf, (int32_t *)StarYbuf, ST7735_BLUE);
        Pause();
    }
}

int main2(void) {
    PortF_Init();
    while(1){
        DelayWait10ms(1);
        PF2 ^= 0x04;
    }
}

int main3(void) {
    PortF_Init();
    while(1){
        DelayWait10ms(100);
        PF2 ^= 0x04;
    }
}


/** Function Implementations. */
void DelayWait10ms(uint32_t n) {
    uint32_t volatile time;
    while(n){
        time = 727240*2/91;  // 10msec
        while(time){
            time--;
        }
        n--;
    }
}

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          // 1) activate clock for Port F
    while((SYSCTL_PRGPIO_R&0x20)==0) {};// allow time for clock to start
                                        // 2) no need to unlock PF2, PF4
    GPIO_PORTF_PCTL_R &= ~0x000F0F00;   // 3) regular GPIO
    GPIO_PORTF_AMSEL_R &= ~0x14;        // 4) disable analog function on PF2, PF4
    GPIO_PORTF_PUR_R |= 0x10;           // 5) pullup for PF4
    GPIO_PORTF_DIR_R |= 0x04;           // 5) set direction to output
    GPIO_PORTF_AFSEL_R &= ~0x14;        // 6) regular port function
    GPIO_PORTF_DEN_R |= 0x14;           // 7) enable digital port
}

void Pause(void) {
    while(PF4==0x00) {
        DelayWait10ms(10);
    }
    while(PF4==0x10) {
        DelayWait10ms(10);
    }
}
