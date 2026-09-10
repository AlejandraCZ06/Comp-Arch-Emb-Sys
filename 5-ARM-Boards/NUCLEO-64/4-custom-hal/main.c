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


void mostrar_numero(uint8_t numero)
{
    // Ánodo común
    // 0 = encendido
    // 1 = apagado

    // A
    if (numero == 0 || numero == 2 || numero == 3 ||
        numero == 5 || numero == 6 || numero == 7 ||
        numero == 8 || numero == 9)
        escribir(GPIOA, 8, 0);
    else
        escribir(GPIOA, 8, 1);

    // B
    if (numero == 0 || numero == 1 || numero == 2 ||
        numero == 3 || numero == 4 || numero == 7 ||
        numero == 8 || numero == 9)
        escribir(GPIOA, 9, 0);
    else
        escribir(GPIOA, 9, 1);

    // C
    if (numero == 0 || numero == 1 || numero == 3 ||
        numero == 4 || numero == 5 || numero == 6 ||
        numero == 7 || numero == 8 || numero == 9)
        escribir(GPIOA, 10, 0);
    else
        escribir(GPIOA, 10, 1);

    // D
    if (numero == 0 || numero == 2 || numero == 3 ||
        numero == 5 || numero == 6 || numero == 8 ||
        numero == 9)
        escribir(GPIOB, 3, 0);
    else
        escribir(GPIOB, 3, 1);

    // E
    if (numero == 0 || numero == 2 || numero == 6 || numero == 8)
        escribir(GPIOB, 4, 0);
    else
        escribir(GPIOB, 4, 1);

    // F
    if (numero == 0 || numero == 4 || numero == 5 ||
        numero == 6 || numero == 8 || numero == 9)
        escribir(GPIOB, 5, 0);
    else
        escribir(GPIOB, 5, 1);

    // G
    if (numero == 2 || numero == 3 || numero == 4 ||
        numero == 5 || numero == 6 || numero == 8 ||
        numero == 9)
        escribir(GPIOB, 6, 0);
    else
        escribir(GPIOB, 6, 1);

    // DP apagado
    escribir(GPIOB, 8, 1);
}


void retardo_corto(volatile uint32_t ciclos)
{
    while (ciclos--);
}


void mostrar_3_digitos(uint16_t numero)
{
    uint8_t centenas;
    uint8_t decenas;
    uint8_t unidades;

    centenas = numero / 100;
    decenas = (numero / 10) % 10;
    unidades = numero % 10;


    // DISPLAY IZQUIERDO = CENTENAS
    apagar_digitos();
    mostrar_numero(centenas);
    escribir(GPIOB, 10, 0);
    retardo_corto(1000);


    // DISPLAY CENTRAL = DECENAS
    apagar_digitos();
    mostrar_numero(decenas);
    escribir(GPIOB, 9, 0);
    retardo_corto(1000);


    // DISPLAY DERECHO = UNIDADES
    apagar_digitos();
    mostrar_numero(unidades);
    escribir(GPIOA, 6, 0);
    retardo_corto(1000);
}


void mostrar_durante_250ms(uint16_t numero)
{
    uint16_t i;

    // Repetimos el multiplexado muchas veces
    // para que el número parezca fijo.

    for (i = 0; i < 250; i++)
    {
        mostrar_3_digitos(numero);
    }
}


int main(void)
{
    uint16_t anterior;
    uint16_t actual;
    uint16_t siguiente;


    // Activar GPIOA y GPIOB
    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);


    // Segmentos
    configurar_salida(GPIOA, 8);   // A
    configurar_salida(GPIOA, 9);   // B
    configurar_salida(GPIOA, 10);  // C

    configurar_salida(GPIOB, 3);   // D
    configurar_salida(GPIOB, 4);   // E
    configurar_salida(GPIOB, 5);   // F
    configurar_salida(GPIOB, 6);   // G
    configurar_salida(GPIOB, 8);   // DP


    // Dígitos
    configurar_salida(GPIOB, 9);   // Display central
    configurar_salida(GPIOB, 10);  // Display izquierdo
    configurar_salida(GPIOA, 6);   // Display derecho


    // Primeros dos números
    anterior = 0;
    actual = 1;


    while (1)
    {
        // Mostrar número actual durante aproximadamente 250 ms
        mostrar_durante_250ms(actual);


        // Calcular siguiente número
        siguiente = anterior + actual;

        anterior = actual;
        actual = siguiente;


        // Cuando supera 987, reiniciar
        if (actual > 987)
        {
            anterior = 0;
            actual = 1;
        }
    }
}