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
    RCC_AHB1ENR |= (1 << 0); // GPIOA clock
    #define GPIOA_AFRL (*(volatile uint32_t*)0x40020020)
    GPIOA_AFRL &= ~(0xF << (2*4));
    GPIOA_AFRL |=  (7 << (2*4)); // PA2 TX
    GPIOA_MODER &= ~(3 << (2*2));
    GPIOA_MODER |=  (2 << (2*2)); // alternate function
    RCC_APB1ENR |= (1 << 17);      // USART2 clock
    USART2_BRR = 0x8B;             // 115200 baud
    USART2_CR1 |= (1 << 3) | (1 << 13); // TE + UE
}

void uart2_send_char(char c)
{
    while(!(USART2_SR & (1 << 7)));
    USART2_DR = c;
}

void uart2_send_string(const char* str)
{
    while(*str)
        uart2_send_char(*str++);
}

void uart2_send_number(int num)
{
    char buf[10];
    int i = 0;
    if(num == 0)
    {
        uart2_send_char('0');
        return;
    }
    while(num > 0)
    {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    for(int j = i-1; j >= 0; j--)
        uart2_send_char(buf[j]);
}

void delay(volatile int count)
{
    for(volatile int i=0; i<count; i++);
}

int main(void)
{
    // LED PA5 setup
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER &= ~(3 << (5*2));
    GPIOA_MODER |=  (1 << (5*2));

    // Button PC13 setup
    RCC_AHB1ENR |= (1 << 2);
    GPIOC_MODER &= ~(3 << (13*2));

    // UART setup
    uart2_init();

    int counter = 0;

    while(1)
    {
        if((GPIOC_IDR & (1 << 13)) == 0) // button pressed
        {
            counter++;                 // increment counter
            counter %= 2;              // keep only 0 or 1 (1-bit LED)
            GPIOA_ODR = (GPIOA_ODR & ~(1 << 5)) | (counter << 5); // LED
            uart2_send_string("Counter: ");
            uart2_send_number(counter);
            uart2_send_string("\r\n");
            while((GPIOC_IDR & (1 << 13)) == 0); // wait release
            delay(50000); // debounce
        }
    }
}
