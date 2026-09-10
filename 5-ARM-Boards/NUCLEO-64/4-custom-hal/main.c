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
#define RCC_BASE     (AHB1_BASE + 0x3800U)

#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)

#define RCC_AHB1ENR ((volatile uint32_t *)(RCC_BASE + 0x30U))

void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |=  (1U << (pin * 2));
}

void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}

int main(void)
{
    // Activar relojes de GPIOA y GPIOB
    *RCC_AHB1ENR |= (1U << 0) | (1U << 1);

    // Segmentos: a,b,c,d,e,f,g
    configurar_salida(GPIOA, 8);
    configurar_salida(GPIOA, 9);
    configurar_salida(GPIOA, 10);

    configurar_salida(GPIOB, 3);
    configurar_salida(GPIOB, 4);
    configurar_salida(GPIOB, 5);
    configurar_salida(GPIOB, 6);

    // Digito 1
    configurar_salida(GPIOB, 9);

    // Apagar los otros dígitos
    configurar_salida(GPIOB, 10);
    configurar_salida(GPIOA, 6);
    configurar_salida(GPIOA, 7);

    // Suponiendo DISPLAY DE CÁTODO COMÚN:
    // Apagamos todos los dígitos
    escribir(GPIOB, 9, 1);
    escribir(GPIOB, 10, 1);
    escribir(GPIOA, 6, 1);
    escribir(GPIOA, 7, 1);

    // Mostrar 8: a,b,c,d,e,f,g encendidos
    escribir(GPIOA, 8, 1);   // a
    escribir(GPIOA, 9, 1);   // b
    escribir(GPIOA, 10, 1);  // c
    escribir(GPIOB, 3, 1);   // d
    escribir(GPIOB, 4, 1);   // e
    escribir(GPIOB, 5, 1);   // f
    escribir(GPIOB, 6, 1);   // g

    // Encender SOLO el primer display
    escribir(GPIOB, 9, 0);

    while (1)
    {
    }
}