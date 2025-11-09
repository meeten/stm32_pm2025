#include "stm32f10x.h"
#include "spi1.h"

#define SSD1306_DC_PIN   1   // PA1 (согласно презентации)
#define SSD1306_CS_PIN   4   // PA4 
#define SSD1306_RES_PIN  2   // PA2 (предположительно)

static void delay(uint32_t count) {
    for(volatile uint32_t i = 0; i < count; i++);
}

static void SSD1306_Command(uint8_t cmd) {
    // 1. Установить CS=0
    GPIOA->ODR &= ~(1 << SSD1306_CS_PIN);  // CS=0
    // 2. Установить DC=0 (команда)
    GPIOA->ODR &= ~(1 << SSD1306_DC_PIN);  // DC=0
    // 3. Задержка
    delay(1000);
    // 4. Отправить команду
    SPI1_Write(cmd);
    // 5. Установить CS=1
    GPIOA->ODR |= (1 << SSD1306_CS_PIN);   // CS=1
}

static void SSD1306_Data(uint8_t data) {
    // 1. Установить CS=0
    GPIOA->ODR &= ~(1 << SSD1306_CS_PIN);  // CS=0
    // 2. Установить DC=1 (данные)
    GPIOA->ODR |= (1 << SSD1306_DC_PIN);   // DC=1
    // 3. Задержка
    delay(1000);
    // 4. Отправить данные
    SPI1_Write(data);
    // 5. Установить CS=1
    GPIOA->ODR |= (1 << SSD1306_CS_PIN);   // CS=1
}

void SSD1306_Init(void) {
    // Настройка RES пина как выход
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |= (0x03 << 8); // Output 50MHz
    
    // Аппаратный сброс
    GPIOA->ODR &= ~(1 << SSD1306_RES_PIN);
    delay(10000);
    GPIOA->ODR |= (1 << SSD1306_RES_PIN);
    delay(10000);
    
    // Инициализация дисплея
    SSD1306_Command(0xAE); // Display OFF
    SSD1306_Command(0x20); SSD1306_Command(0x00); // Horizontal addressing mode
    SSD1306_Command(0x21); SSD1306_Command(0x00); SSD1306_Command(0x7F); // Column address
    SSD1306_Command(0x22); SSD1306_Command(0x00); SSD1306_Command(0x07); // Page address
    SSD1306_Command(0x8D); SSD1306_Command(0x14); // Charge pump
    SSD1306_Command(0xAF); // Display ON
}

void SSD1306_DrawChessboard(void) {
    const uint8_t cellWidth = 16;
    const uint8_t cellHeight = 8;
    
    // Устанавливаем адресацию на весь дисплей
    SSD1306_Command(0x21); SSD1306_Command(0x00); SSD1306_Command(0x7F);
    SSD1306_Command(0x22); SSD1306_Command(0x00); SSD1306_Command(0x07);
    
    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t col = 0; col < 128; col++) {
            uint8_t cellX = col / cellWidth;
            uint8_t cellY = page / (cellHeight / 8);
            
            // Шахматный паттерн
            if ((cellX + cellY) % 2 == 0) {
                SSD1306_Data(0xFF); // Белая клетка
            } else {
                SSD1306_Data(0x00); // Черная клетка
            }
        }
    }
}