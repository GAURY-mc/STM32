#include <stdint.h>

// Registers
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)
#define GPIOC_MODER   (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40020810)

int main(void)
{
    // 1️⃣ Enable clocks for GPIOA (LED) and GPIOC (Button)
    RCC_AHB1ENR |= (1 << 0); // GPIOA
    RCC_AHB1ENR |= (1 << 2); // GPIOC

    // 2️⃣ Configure PA5 as output (LED)
    GPIOA_MODER &= ~(3 << (5 * 2));  // Clear mode
    GPIOA_MODER |=  (1 << (5 * 2));  // Output

    // 3️⃣ Configure PC13 as input (User Button)
    GPIOC_MODER &= ~(3 << (13 * 2)); // Input mode

    while(1)
    {
        if((GPIOC_IDR & (1 << 13)) == 0) // Button pressed (active low)
        {
            GPIOA_ODR |= (1 << 5); // LED ON
        }
        else
        {
            GPIOA_ODR &= ~(1 << 5); // LED OFF
        }
    }
}
