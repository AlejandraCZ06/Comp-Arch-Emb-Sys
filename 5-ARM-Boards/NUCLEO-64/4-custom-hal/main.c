#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define PERIPH_BASE  0x40000000U
#define AHB1_BASE    (PERIPH_BASE + 0x20000U)

#define GPIOA_BASE   (AHB1_BASE + 0x0000U)
#define GPIOB_BASE   (AHB1_BASE + 0x0400U)
#define GPIOC_BASE   (AHB1_BASE + 0x0800U)
#define RCC_BASE     (AHB1_BASE + 0x3800U)

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)

#define RCC_AHB1ENR ((volatile uint32_t *)(RCC_BASE + 0x30U))


void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |=  (1U << (pin * 2));
}


void configurar_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    // Entrada
    puerto->MODER &= ~(3U << (pin * 2));

    // Pull-up
    puerto->PUPDR &= ~(3U << (pin * 2));
    puerto->PUPDR |=  (1U << (pin * 2));
}


void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}


uint8_t leer_switch(uint8_t pin)
{
    // Switch activado = 0
    // Switch apagado = 1

    if ((GPIOC->IDR & (1U << pin)) == 0)
        return 1;

    return 0;
}


int main(void)
{
    // Activar GPIOA, GPIOB y GPIOC
    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);
    *RCC_AHB1ENR |= (1U << 2);


    // =========================
    // SWITCHES PC0-PC7
    // =========================

    configurar_entrada_pullup(GPIOC, 0);
    configurar_entrada_pullup(GPIOC, 1);
    configurar_entrada_pullup(GPIOC, 2);
    configurar_entrada_pullup(GPIOC, 3);
    configurar_entrada_pullup(GPIOC, 4);
    configurar_entrada_pullup(GPIOC, 5);
    configurar_entrada_pullup(GPIOC, 6);
    configurar_entrada_pullup(GPIOC, 7);


    // =========================
    // SEGMENTOS
    // =========================

    // A = PA8
    configurar_salida(GPIOA, 8);

    // B = PA9
    configurar_salida(GPIOA, 9);

    // C = PA10
    configurar_salida(GPIOA, 10);

    // D = PB3
    configurar_salida(GPIOB, 3);

    // E = PB4
    configurar_salida(GPIOB, 4);

    // F = PB5
    configurar_salida(GPIOB, 5);

    // G = PB6
    configurar_salida(GPIOB, 6);

    // DP = PB8
    configurar_salida(GPIOB, 8);


    while (1)
    {
        // PC0 -> A
        escribir(GPIOA, 8, leer_switch(0));

        // PC1 -> B
        escribir(GPIOA, 9, leer_switch(1));

        // PC2 -> C
        escribir(GPIOA, 10, leer_switch(2));

        // PC3 -> D
        escribir(GPIOB, 3, leer_switch(3));

        // PC4 -> E
        escribir(GPIOB, 4, leer_switch(4));

        // PC5 -> F
        escribir(GPIOB, 5, leer_switch(5));

        // PC6 -> G
        escribir(GPIOB, 6, leer_switch(6));

        // PC7 -> DP
        escribir(GPIOB, 8, leer_switch(7));
    }
}