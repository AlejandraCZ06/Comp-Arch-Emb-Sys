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


// Variables pedidas por el ejercicio
volatile uint8_t hundreds;
volatile uint8_t tens;
volatile uint8_t units;


void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |= (1U << (pin * 2));
}


void configurar_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    // Entrada
    puerto->MODER &= ~(3U << (pin * 2));

    // Pull-up interno
    puerto->PUPDR &= ~(3U << (pin * 2));
    puerto->PUPDR |= (1U << (pin * 2));
}


void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}


uint8_t leer_dip(void)
{
    uint8_t valor;

    valor = GPIOC->IDR & 0xFF;

    // Los switches conectan a GND cuando se activan
    // Por eso invertimos el resultado
    valor = ~valor;

    return valor;
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
    // 0 = segmento encendido
    // 1 = segmento apagado

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


void retardo(volatile uint32_t ciclos)
{
    while (ciclos--);
}


int main(void)
{
    uint8_t entrada;


    // =====================================
    // ACTIVAR GPIOA, GPIOB Y GPIOC
    // =====================================

    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);
    *RCC_AHB1ENR |= (1U << 2);


    // =====================================
    // DIP SWITCH PC0 - PC7
    // =====================================

    configurar_entrada_pullup(GPIOC, 0);
    configurar_entrada_pullup(GPIOC, 1);
    configurar_entrada_pullup(GPIOC, 2);
    configurar_entrada_pullup(GPIOC, 3);
    configurar_entrada_pullup(GPIOC, 4);
    configurar_entrada_pullup(GPIOC, 5);
    configurar_entrada_pullup(GPIOC, 6);
    configurar_entrada_pullup(GPIOC, 7);


    // =====================================
    // SEGMENTOS DEL DISPLAY
    // =====================================

    configurar_salida(GPIOA, 8);    // A
    configurar_salida(GPIOA, 9);    // B
    configurar_salida(GPIOA, 10);   // C

    configurar_salida(GPIOB, 3);    // D
    configurar_salida(GPIOB, 4);    // E
    configurar_salida(GPIOB, 5);    // F
    configurar_salida(GPIOB, 6);    // G
    configurar_salida(GPIOB, 8);    // DP


    // =====================================
    // DIGITOS
    // =====================================

    configurar_salida(GPIOB, 9);
    configurar_salida(GPIOB, 10);
    configurar_salida(GPIOA, 6);


    while (1)
    {
        // =================================
        // 1. LEER DIP SWITCH
        // =================================

        entrada = leer_dip();


        // =================================
        // 2. CONVERTIR A CENTENAS,
        //    DECENAS Y UNIDADES
        // =================================

        hundreds = entrada / 100;

        tens = (entrada / 10) % 10;

        units = entrada % 10;


        // =================================
        // 3. MOSTRAR CENTENAS
        // =================================

        apagar_digitos();

        mostrar_numero(hundreds);

        escribir(GPIOB, 9, 0);

        retardo(1000);


        // =================================
        // 4. MOSTRAR DECENAS
        // =================================

        apagar_digitos();

        mostrar_numero(tens);

        escribir(GPIOB, 10, 0);

        retardo(1000);


        // =================================
        // 5. MOSTRAR UNIDADES
        // =================================

        apagar_digitos();

        mostrar_numero(units);

        escribir(GPIOA, 6, 0);

        retardo(1000);
    }
}