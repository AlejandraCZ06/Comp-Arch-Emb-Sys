#include <stdint.h>

// Estructura que representa los registros de un puerto GPIO.
typedef struct {
    volatile uint32_t MODER;    // Modo del pin: entrada, salida, etc.
    volatile uint32_t OTYPER;   // Tipo de salida
    volatile uint32_t OSPEEDR;  // Velocidad del pin
    volatile uint32_t PUPDR;    // Pull-up / Pull-down
    volatile uint32_t IDR;      // Registro de ENTRADA (para leer pines)
    volatile uint32_t ODR;      // Registro de SALIDA (para escribir pines)
    volatile uint32_t BSRR;     // Set/Reset atómico
    volatile uint32_t LCKR;     // Bloqueo de configuración
    volatile uint32_t AFR[2];   // Funciones alternas
} GPIO_TypeDef;
 
#define PERIPH_BASE   (0x40000000U)
#define AHB1_BASE     (PERIPH_BASE + 0x20000U)

#define GPIOA_BASE    (AHB1_BASE + 0x0000U)
#define GPIOB_BASE    (AHB1_BASE + 0x0400U)
#define GPIOC_BASE    (AHB1_BASE + 0x0800U)
#define RCC_BASE      (AHB1_BASE + 0x3800U)

#define GPIOA  ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB  ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC  ((GPIO_TypeDef *) GPIOC_BASE)

#define RCC_AHB1ENR ((volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_AHB1ENR_GPIOAEN (1U << 0)
#define RCC_AHB1ENR_GPIOBEN (1U << 1)
#define RCC_AHB1ENR_GPIOCEN (1U << 2)

#define CATODO_COMUN 1

typedef struct {
    GPIO_TypeDef *puerto;
    uint8_t pin;
} pin_t;

static const pin_t segmentos[8] = {
    {GPIOA, 8},   // a
    {GPIOA, 9},   // b
    {GPIOA, 10},  // c
    {GPIOB, 3},   // d
    {GPIOB, 4},   // e
    {GPIOB, 5},   // f
    {GPIOB, 6},   // g
    {GPIOB, 8},   // dp
};

static const pin_t digitos[4] = {
    {GPIOB, 9},   // Digito 1 -> unidades
    {GPIOB, 10},  // Digito 2 -> decenas
    {GPIOA, 6},   // Digito 3 -> centenas
    {GPIOA, 7},   // Digito 4 -> sin usar
};

static const uint8_t tabla_digitos[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}, // 9
};

void habilitar_relojes(void)
{
    *RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
}

void configurar_pin_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(0x3U << (pin * 2));  
    puerto->MODER |=  (0x1U << (pin * 2));  
}

void configurar_pin_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(0x3U << (pin * 2));   
    puerto->PUPDR &= ~(0x3U << (pin * 2));   
    puerto->PUPDR |=  (0x1U << (pin * 2));   
}

void escribir_pin(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
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

void configurar_display(void)
{
    for (uint8_t i = 0; i < 8; i++)
        configurar_pin_salida(segmentos[i].puerto, segmentos[i].pin);

    for (uint8_t i = 0; i < 4; i++)
        configurar_pin_salida(digitos[i].puerto, digitos[i].pin);
}

void apagar_todos_los_digitos(void)
{
    uint8_t nivel_apagado = CATODO_COMUN ? 1 : 0;
    for (uint8_t i = 0; i < 4; i++)
        escribir_pin(digitos[i].puerto, digitos[i].pin, nivel_apagado);
}

void mostrar_numero_en_segmentos(uint8_t numero)
{
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t bit = tabla_digitos[numero][i];
        uint8_t nivel = CATODO_COMUN ? bit : (uint8_t)!bit;
        escribir_pin(segmentos[i].puerto, segmentos[i].pin, nivel);
    }
    uint8_t dp_apagado = CATODO_COMUN ? 0 : 1;
    escribir_pin(segmentos[7].puerto, segmentos[7].pin, dp_apagado);
}

void seleccionar_digito(uint8_t indice)
{
    uint8_t nivel_encendido = CATODO_COMUN ? 0 : 1;
    escribir_pin(digitos[indice].puerto, digitos[indice].pin, nivel_encendido);
}

void configurar_dip_switch(void)
{
    for (uint8_t pin = 0; pin < 8; pin++)
        configurar_pin_entrada_pullup(GPIOC, pin);
}

uint8_t leer_dip_switch(void)
{
    uint8_t valor = (uint8_t)(GPIOC->IDR & 0xFFU);
    valor = (uint8_t)~valor;
    return valor;
}

int main(void)
{
    habilitar_relojes();
    configurar_display();
    configurar_dip_switch();
    apagar_todos_los_digitos();

    while (1) {
        uint8_t entrada = leer_dip_switch();   

        uint8_t centenas = entrada / 100;
        uint8_t decenas  = (entrada / 10) % 10;
        uint8_t unidades = entrada % 10;

        apagar_todos_los_digitos();
        mostrar_numero_en_segmentos(unidades);
        seleccionar_digito(0);
        retardo(1500);

        apagar_todos_los_digitos();
        mostrar_numero_en_segmentos(decenas);
        seleccionar_digito(1);
        retardo(1500);

        apagar_todos_los_digitos();
        mostrar_numero_en_segmentos(centenas);
        seleccionar_digito(2);
        retardo(1500);
    }
}