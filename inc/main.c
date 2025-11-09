#include "stm32f10x.h"
#include "spi1.h"
#include "ssd1306.h"

int main(void) {
    // Инициализация SPI
    SPI1_Init();
    
    // Инициализация дисплея
    SSD1306_Init();
    
    // Отрисовка шахматной доски
    SSD1306_DrawChessboard();

    while (1) {
    }
}