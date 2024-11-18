/*
 * LCD.h
 *
 *  Created on: Sep 29, 2024
 *      Author: DELL
 */

#ifndef __LCD_H
#define __LCD_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

// Định nghĩa các chân kết nối với LCD
#define RS_PIN GPIO_PIN_0
#define EN_PIN GPIO_PIN_1
#define D4_PIN GPIO_PIN_4
#define D5_PIN GPIO_PIN_5
#define D6_PIN GPIO_PIN_6
#define D7_PIN GPIO_PIN_7
#define LCD_PORT GPIOA

// Khai báo các hàm điều khiển LCD
void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Print(char *str);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_CursorBlink(void);
void LCD_DisableCursorBlink(void);
void LCD_PrintNumber(int32_t number);  // Hiển thị một số nguyên lên LCD



#endif
