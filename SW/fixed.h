/**
 * @file fixed.h
 * @author your name (you@domain.com), Jonathan Valvano, Matthew Yu
 *    <TA NAME and LAB SECTION # HERE>
 * @brief 
 *    Possible header file for Lab 1.
 *    Feel free to edit this to match your specifications. You may move these
 *    functions into ST7735.h, ST7735.c.
 * @version 1.1.0
 * @date 2022-08-24 <REPLACE WITH DATE OF LAST REVISION>
 * 
 * @copyright Copyright (c) 2022
 */

/** File includes. */
#include <stdint.h>


/** Function declarations. */
/**
 * @brief ST7735_sDecOut3 converts a signed 32-bit decimal fixed point number
 *        into LCD format. 
 * 
 * @param n Signed 32-bit integer part of fixed-point number.
 * @note  resolution 0.001
 *        range -99.999 to +99.999
 * @example Expected Output
 *    Parameter | LCD display
 *            0 |       0.000
 *            4 |       0.004
 *           -5 |       0.005
 *           78 |       0.078
 *        -1254 |      -1.254
 *         9999 |       9.999
 *       -10000 |     -10.000
 *        12345 |      12.345
 *       -56789 |     -56.789
 *       -99999 |     -99.999
 *        99999 |      99.999
 *       100000 |      **.***
 *      -100000 |     -**.***
 */
void ST7735_sDecOut3(int32_t n);

/**
 * @brief ST7735_uBinOut5 converts an unsigned 32-bit binary fixed-point number
 *        into LCD format.
 * 
 * @param n Unsigned 32-bit integer part of binary fixed-point number.
 * @note  resolution 1/32
 *        range 0.00 to 999.99
 *        if the integer part is larger than 31999, it signifies an error.
 * @example Expected Output
 *    Parameter | LCD display
 *            0 |        0.00
 *            1 |        0.03
 *            5 |        0.16
 *          100 |        3.13
 *          127 |        3.97
 *          252 |        7.88
 *          535 |       16.72
 *         2560 |       80.00
 *         6092 |      190.38
 *        13000 |      406.25
 *        16383 |      511.97
 *        17283 |      540.09
 *        31999 |      999.97
 *        32000 |      ***.**
 */
void ST7735_uBinOut5(uint32_t n); 
