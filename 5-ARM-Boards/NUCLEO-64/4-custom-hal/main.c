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

// --------------------------------------------------
// FUNCIONES GENERALES
// --------------------------------------------------

void configurar_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(3U << (pin * 2));
    puerto->MODER |=  (1U << (pin * 2));
}

void configurar_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    // Entrada
    puerto->MODER &= ~(3U << (pin * 2));

    // Primero borrar configuración del pull
    puerto->PUPDR &= ~(3U << (pin * 2));

    // 01 = Pull-up
    puerto->PUPDR |= (1U << (pin * 2));
}

void escribir(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}

void retardo(volatile uint32_t ciclos)
{
    while (ciclos--);
}

// --------------------------------------------------
// DISPLAY DE CÁTODO COMÚN
// --------------------------------------------------

// Segmentos: a,b,c,d,e,f,g
static const uint8_t segmentos[7][2] = {
    {0, 8},   // PA8  = a
    {0, 9},   // PA9  = b
    {0, 10},  // PA10 = c
    {1, 3},   // PB3  = d
    {1, 4},   // PB4  = e
    {1, 5},   // PB5  = f
    {1, 6}    // PB6  = g
};

// Tabla para 0-9
static const uint8_t tabla[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};

// --------------------------------------------------
// CONFIGURAR DISPLAY
// --------------------------------------------------

void configurar_display(void)
{
    // Segmentos
    configurar_salida(GPIOA, 8);
    configurar_salida(GPIOA, 9);
    configurar_salida(GPIOA, 10);

    configurar_salida(GPIOB, 3);
    configurar_salida(GPIOB, 4);
    configurar_salida(GPIOB, 5);
    configurar_salida(GPIOB, 6);

    // Dígitos
    configurar_salida(GPIOB, 9);
    configurar_salida(GPIOB, 10);
    configurar_salida(GPIOA, 6);
}

// --------------------------------------------------
// APAGAR TODOS LOS DÍGITOS
// CÁTODO COMÚN:
// 1 = apagado
// 0 = encendido
// --------------------------------------------------

void apagar_digitos(void)
{
    escribir(GPIOB, 9, 1);
    escribir(GPIOB, 10, 1);
    escribir(GPIOA, 6, 1);
}

// --------------------------------------------------
// MOSTRAR UN NÚMERO EN LOS SEGMENTOS
// CÁTODO COMÚN:
// 1 = segmento encendido
// 0 = segmento apagado
// --------------------------------------------------

void mostrar_numero(uint8_t numero)
{
    escribir(GPIOA, 8, tabla[numero][0]);  // a
    escribir(GPIOA, 9, tabla[numero][1]);  // b
    escribir(GPIOA, 10, tabla[numero][2]); // c

    escribir(GPIOB, 3, tabla[numero][3]);  // d
    escribir(GPIOB, 4, tabla[numero][4]);  // e
    escribir(GPIOB, 5, tabla[numero][5]);  // f
    escribir(GPIOB, 6, tabla[numero][6]);  // g
}

// --------------------------------------------------
// DIP SWITCH
// PC7 = bit 7
// PC0 = bit 0
// --------------------------------------------------

void configurar_dip_switch(void)
{
    for (uint8_t pin = 0; pin < 8; pin++)
    {
        configurar_entrada_pullup(GPIOC, pin);
    }
}

uint8_t leer_dip_switch(void)
{
    uint8_t valor;

    valor = (uint8_t)(GPIOC->IDR & 0xFFU);

    // Con pull-up:
    // switch abierto  = 1
    // switch cerrado  = 0
    //
    // Invertimos para que:
    // switch abierto  = 0
    // switch cerrado  = 1

    valor = (uint8_t)~valor;

    return valor;
}

// --------------------------------------------------
// MAIN
// --------------------------------------------------

int main(void)
{
    // Activar relojes de GPIOA, GPIOB y GPIOC
    *RCC_AHB1ENR |= (1U << 0);
    *RCC_AHB1ENR |= (1U << 1);
    *RCC_AHB1ENR |= (1U << 2);

    configurar_display();
    configurar_dip_switch();

    while (1)
    {
        uint8_t entrada;
        uint8_t centenas;
        uint8_t decenas;
        uint8_t unidades;

        // Leer DIP
        entrada = leer_dip_switch();

        // Convertir binario a decimal
        centenas = entrada / 100;
        decenas = (entrada / 10) % 10;
        unidades = entrada % 10;

        // -------------------------
        // MOSTRAR UNIDADES
        // -------------------------

        apagar_digitos();

        mostrar_numero(unidades);

        escribir(GPIOB, 9, 0);

        retardo(1500);

        // -------------------------
        // MOSTRAR DECENAS
        // -------------------------

        apagar_digitos();

        mostrar_numero(decenas);

        escribir(GPIOB, 10, 0);

        retardo(1500);

        // -------------------------
        // MOSTRAR CENTENAS
        // -------------------------

        apagar_digitos();

        mostrar_numero(centenas);

        escribir(GPIOA, 6, 0);

        retardo(1500);
    }
}