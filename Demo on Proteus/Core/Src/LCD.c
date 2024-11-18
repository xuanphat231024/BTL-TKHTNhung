#include "LCD.h"
#include <stdio.h>  // Để sử dụng sprintf cho việc chuyển đổi số thành chuỗi
// Gửi nửa byte (4-bit) đến LCD
void LCD_SendHalf(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 0x01);
}

// Kích hoạt xung EN
void LCD_Enable(void) {
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_SET);   // Set EN
    HAL_Delay(5);                                        // Delay nhỏ
    HAL_GPIO_WritePin(LCD_PORT, EN_PIN, GPIO_PIN_RESET); // Reset EN
    HAL_Delay(5);
}

// Gửi lệnh đến LCD
void LCD_SendCmd(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);  // Chọn chế độ lệnh
    LCD_SendHalf(cmd >> 4);                               // Gửi 4 bit cao
    LCD_Enable();
    LCD_SendHalf(cmd & 0x0F);                             // Gửi 4 bit thấp
    LCD_Enable();
}

// Gửi dữ liệu đến LCD
void LCD_SendData(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET);    // Chọn chế độ dữ liệu
    LCD_SendHalf(data >> 4);                              // Gửi 4 bit cao
    LCD_Enable();
    LCD_SendHalf(data & 0x0F);                            // Gửi 4 bit thấp
    LCD_Enable();
}

// Khởi tạo LCD
void LCD_Init(void) {
    HAL_Delay(100);  // Chờ LCD khởi động

    // Gửi chuỗi khởi tạo LCD
    LCD_SendCmd(0x02);  // Chế độ 4-bit
    LCD_SendCmd(0x28);  // Giao diện 2 dòng, 5x7 matrix
    LCD_SendCmd(0x0C);  // Bật màn hình, tắt con trỏ
    LCD_SendCmd(0x06);  // Tăng địa chỉ con trỏ sau khi ghi
    LCD_SendCmd(0x01);  // Xóa màn hình
    HAL_Delay(10);
}

// Hiển thị chuỗi ký tự trên LCD
void LCD_Print(char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}

// Xóa màn hình LCD
void LCD_Clear(void) {
    LCD_SendCmd(0x01);  // Lệnh xóa màn hình
    HAL_Delay(10);       // Đợi cho lệnh hoàn tất
}
void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address;

    // Tính toán địa chỉ dựa trên hàng và cột
    if (row == 0) {
        address = col;          // Hàng 1, bắt đầu từ địa chỉ 0x00
    } else {
        address = 0x40 + col;   // Hàng 2, bắt đầu từ địa chỉ 0x40
    }

    // Gửi lệnh di chuyển con trỏ
    LCD_SendCmd(0x80 | address);
}
// Gửi lệnh để bật con trỏ và làm nó nhấp nháy
void LCD_CursorBlink(void) {
    LCD_SendCmd(0x0F); // Lệnh để bật màn hình, con trỏ và nhấp nháy
}

// Gửi lệnh để tắt con trỏ nhấp nháy
void LCD_DisableCursorBlink(void) {
    LCD_SendCmd(0x0C); // Lệnh để tắt con trỏ và nhấp nháy
}
// Hiển thị số nguyên lên màn hình
void LCD_PrintNumber(int32_t number) {
    char buffer[12]; // Tạo một buffer đủ lớn để chứa số nguyên (-2147483648 đến 2147483647)
    sprintf(buffer, "%ld", number); // Chuyển đổi số thành chuỗi
    LCD_Print(buffer); // In chuỗi số ra LCD
}
