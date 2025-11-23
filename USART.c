#include <stdint.h>

#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR   (*(volatile uint32_t*)0x40023840)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)
#define GPIOC_MODER   (*(volatile uint32_t*)0x40020800)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40020810)
#define USART2_SR     (*(volatile uint32_t*)0x40004400)
#define USART2_DR     (*(volatile uint32_t*)0x40004404)
#define USART2_BRR    (*(volatile uint32_t*)0x40004408)
#define USART2_CR1    (*(volatile uint32_t*)0x4000440C)

void uart2_init(void)
{
    // Enable GPIOA clock (PA2=TX, PA3=RX)
    RCC_AHB1ENR |= (1 << 0);

    // Set PA2 to Alternate Function (AF7=USART2)
    #define GPIOA_AFRL   (*(volatile uint32_t*)0x40020020)
    GPIOA_AFRL &= ~(0xF << (2*4));
    GPIOA_AFRL |=  (7 << (2*4)); // AF7

    // Set PA2 as Alternate Function
    GPIOA_MODER &= ~(3 << (2*2));
    GPIOA_MODER |=  (2 << (2*2));

    // Enable USART2 clock (APB1)
    RCC_APB1ENR |= (1 << 17);

    // Configure baud rate: 115200, assuming 16 MHz PCLK1
    USART2_BRR = 0x8B; // approximate

    // Enable USART2 transmitter
    USART2_CR1 |= (1 << 3); // TE
    USART2_CR1 |= (1 << 13); // UE
}

void uart2_send_char(char c)
{
    while(!(USART2_SR & (1 << 7))); // wait until TXE
    USART2_DR = c;
}

void uart2_send_string(const char* str)
{
    while(*str)
        uart2_send_char(*str++);
}

void delay(volatile int count)
{
    for(volatile int i=0; i<count; i++);
}

int main(void)
{
    // Enable GPIOA clock (LED)
    RCC_AHB1ENR |= (1 << 0);
    // Set PA5 as output (LD2)
    GPIOA_MODER &= ~(3 << (5*2));
    GPIOA_MODER |=  (1 << (5*2));

    // Enable GPIOC clock (button)
    RCC_AHB1ENR |= (1 << 2);
    // Set PC13 as input
    GPIOC_MODER &= ~(3 << (13*2));

    // Initialize UART
    uart2_init();

    while(1)
    {
        if((GPIOC_IDR & (1 << 13)) == 0) // button pressed
        {
            // Toggle LED
            GPIOA_ODR ^= (1 << 5);

            // Send UART message
            uart2_send_string("Button pressed!\r\n");

            // Wait until button released
            while((GPIOC_IDR & (1 << 13)) == 0);
            delay(50000); // simple debounce
        }
    }
}
