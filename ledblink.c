#include <stdint.h>

// Registers
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)

// Delay function
void delay(void)
{
    for(volatile int i=0; i<500000; i++);
}

int main(void)
{
    // 1. Enable clock to GPIOA
    RCC_AHB1ENR |= (1 << 0);

    // 2. Set PA5 as output
    GPIOA_MODER &= ~(3 << (5 * 2));  // Clear mode bits
    GPIOA_MODER |=  (1 << (5 * 2));  // Set as general purpose output

    // 3. Blink LED forever
    while(1)
    {
        GPIOA_ODR ^= (1 << 5);  // Toggle PA5 (LD2)
        delay();                 // Simple delay
    }
}
