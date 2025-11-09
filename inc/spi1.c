#include "stm32f10x.h"
#include "spi1.h"

void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN;
    
    // PA4 (CS) - обычный выход, PA1 (DC) - обычный выход
    GPIOA->CRL &= ~(0xFF << 16);
    GPIOA->CRL |= (0x03 << 16) | (0x03 << 20) | (0x03 << 28); // Output 50MHz
    GPIOA->CRL |= (0x02 << 18) | (0x02 << 22) | (0x02 << 30); // Alternate function
    
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x03 << 4); // Output 50MHz
    
    SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_CPHA |  // Режим 3
               SPI_CR1_MSTR |                 // Мастер
               SPI_CR1_BR |                   // fPCLK/256
               SPI_CR1_SSM | SPI_CR1_SSI |    // Программный CS
               SPI_CR1_SPE;                   // Включить SPI
    
    GPIOA->ODR |= (1 << 4);  // PA4 = 1
}

void SPI1_Write(uint8_t data) {
    while(!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
}

uint8_t SPI1_Read(void) {
    SPI1->DR = 0;
    while(!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}