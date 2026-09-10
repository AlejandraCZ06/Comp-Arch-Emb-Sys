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
    puerto->MODER |= (1U << (pin * 2));
}


void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}


void apagar_digitos(void)
{
    // Ánodo común
    // 1 = apagado

    escribir(GPIOB, 9, 1);
    escribir(GPIOB, 10, 1);
    escribir(GPIOA, 6, 1);
}


void mostrar_8(void)
{
    // Ánodo común
    // 0 = segmento encendido

    // A
    escribir(GPIOA, 8, 0);

    // B
    escribir(GPIOA, 9, 0);

    // C
    escribir(GPIOA, 10, 0);

    // D
    escribir(GPIOB, 3, 0);

    // E
    escribir(GPIOB, 4, 0);

    // F
    escribir(GPIOB, 5, 0);

    // G
    escribir(GPIOB, 6, 0);

    // DP apagado
    escribir(GPIOB, 8, 1);
}


void retardo(volatile uint32_t ciclos)
{
    while (ciclos--);
}


int main(void)
{
    // Activar GPIOA y GPIOB
    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);


    // =========================
    // SEGMENTOS
    // =========================

    configurar_salida(GPIOA, 8);   // A
    configurar_salida(GPIOA, 9);   // B
    configurar_salida(GPIOA, 10);  // C

    configurar_salida(GPIOB, 3);   // D
    configurar_salida(GPIOB, 4);   // E
    configurar_salida(GPIOB, 5);   // F
    configurar_salida(GPIOB, 6);   // G
    configurar_salida(GPIOB, 8);   // DP


    // =========================
    // DIGITOS
    // =========================

    configurar_salida(GPIOB, 9);   // Display 1
    configurar_salida(GPIOB, 10);  // Display 2
    configurar_salida(GPIOA, 6);   // Display 3


    while (1)
    {
        // DISPLAY 1
        apagar_digitos();
        mostrar_8();
        escribir(GPIOB, 9, 0);
        retardo(1000);


        // DISPLAY 2
        apagar_digitos();
        mostrar_8();
        escribir(GPIOB, 10, 0);
        retardo(1000);


        // DISPLAY 3
        apagar_digitos();
        mostrar_8();
        escribir(GPIOA, 6, 0);
        retardo(1000);
    }
}