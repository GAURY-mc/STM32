#include <stdint.h>

// Registers
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40023844) // not used here
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)
#define GPIOC_MODER   (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40020810)

void delay(volatile int count)
{
    for(volatile int i=0; i<count; i++);
}

int main(void)
{
    // 1️⃣ Enable clocks for GPIOA and GPIOC
    RCC_AHB1ENR |= (1 << 0);  // GPIOA
    RCC_AHB1ENR |= (1 << 2);  // GPIOC

    // 2️⃣ Configure PA5 as output (LED)
    GPIOA_MODER &= ~(3 << (5*2));
    GPIOA_MODER |=  (1 << (5*2));

    // 3️⃣ Configure PC13 as input (User button)
    GPIOC_MODER &= ~(3 << (13*2)); // Input mode (00)

    while(1)
    {
        // Check button state: PC13 == 0 when pressed
        if((GPIOC_IDR & (1 << 13)) == 0)
        {
            // Button pressed → fast blink
            GPIOA_ODR ^= (1 << 5);
            delay(100000);
        }
        else
        {
            // Button released → slow blink
            GPIOA_ODR ^= (1 << 5);
            delay(500000);
        }
    }
}
