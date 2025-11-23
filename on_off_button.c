#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)
#define GPIOC_MODER   (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40020810)

// Simple delay for software debouncing
void delay(volatile int count)
{
    for(volatile int i=0; i<count; i++);
}

int main(void)
{
    // Enable clocks for GPIOA (LED) and GPIOC (Button)
    RCC_AHB1ENR |= (1 << 0); // GPIOA
    RCC_AHB1ENR |= (1 << 2); // GPIOC

    // Configure PA5 as output (LED)
    GPIOA_MODER &= ~(3 << (5*2));
    GPIOA_MODER |=  (1 << (5*2));

    // Configure PC13 as input (User Button)
    GPIOC_MODER &= ~(3 << (13*2)); // Input mode

    uint8_t led_state = 0;

    while(1)
    {
        // Check if button is pressed (active low)
        if((GPIOC_IDR & (1 << 13)) == 0)
        {
            delay(1000000); // Debounce delay

            // Toggle LED
            led_state ^= 1;

            if(led_state)
                GPIOA_ODR |= (1 << 5);  // LED ON
            else
                GPIOA_ODR &= ~(1 << 5); // LED OFF

            // Wait until button is released
            while((GPIOC_IDR & (1 << 13)) == 0);
        }
    }

    return 0; // Optional for bare-metal
}
