#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t ticks) {
    for (uint32_t i = 0; i < ticks; i++) {
        __NOP();
    }
}

int main(void) {
    // Включаем тактирование AFIO, GPIOC и GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // PC13 как выход push-pull
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13_0; // 10 MHz

    // PA0 и PA1 как входы с подтяжкой
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1; // вход с pull-up/pull-down
    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= GPIO_CRL_CNF1_1;

    // подтяжка вверх
    GPIOA->ODR |= (1U<<0) | (1U<<1);

    float frequency = 1.0f;
    const float maxFreq = 64.0f;
    const float minFreq = 1.0f / 64.0f;

    while (1) {
        GPIOC->ODR ^= (1U<<13U);

        uint32_t ticks = (uint32_t)(500000 / frequency);
        delay(ticks);

        // Проверка кнопки A (PA0) — увеличить частоту
        if (!(GPIOA->IDR & (1U<<0))) {
            if (frequency < maxFreq) frequency *= 2.0f;
            delay(500000);
        }

        // Проверка кнопки B (PA1) — уменьшить частоту
        if (!(GPIOA->IDR & (1U<<1))) {
            if (frequency > minFreq) frequency /= 2.0f;
            delay(500000);
        }
    }

	return 0;
}
