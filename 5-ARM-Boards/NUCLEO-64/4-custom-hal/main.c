#include <stdint.h>


// Estructura que representa los registros de un puerto GPIO.
typedef struct {
    volatile uint32_t MODER;    // Modo del pin: entrada, salida, etc.
    volatile uint32_t OTYPER;   // Tipo de salida
    volatile uint32_t OSPEEDR;  // Velocidad del pin
    volatile uint32_t PUPDR;    // Pull-up / Pull-down
    volatile uint32_t IDR;      // Registro de ENTRADA (para leer pines)
    volatile uint32_t ODR;      // Registro de SALIDA (para escribir pines)
    volatile uint32_t BSRR;     // Set/Reset atómico (no se que es)
    volatile uint32_t LCKR;     // Bloqueo de configuración
    volatile uint32_t AFR[2];   // Funciones alternas
} GPIO_TypeDef;
 
#define PERIPH_BASE   (0x40000000U)
#define AHB1_BASE     (PERIPH_BASE + 0x20000U)

#define GPIOA_BASE    (AHB1_BASE + 0x0000U)
#define GPIOB_BASE    (AHB1_BASE + 0x0400U)
#define GPIOC_BASE    (AHB1_BASE + 0x0800U)
#define RCC_BASE      (AHB1_BASE + 0x3800U)

/* Converti cada dirección base en un puntero a GPIO_TypeDef.
 * Así, GPIOA->MODER, GPIOA->ODR, etc. apuntan directo al registro real. */
#define GPIOA  ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB  ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC  ((GPIO_TypeDef *) GPIOC_BASE)

/* Registro que enciende el reloj de cada periférico en el bus AHB1 */
#define RCC_AHB1ENR ((volatile uint32_t *)(RCC_BASE + 0x30U))
#define RCC_AHB1ENR_GPIOAEN (1U << 0)
#define RCC_AHB1ENR_GPIOBEN (1U << 1)
#define RCC_AHB1ENR_GPIOCEN (1U << 2)


 //  CONFIGURACION DEL DISPLAY

#define CATODO_COMUN 1

typedef struct {
    GPIO_TypeDef *puerto;
    uint8_t pin;
} pin_t;

// Pines de los segmentos, en el orden: a, b, c, d, e, f, g, dp
 
static const pin_t segmentos[8] = {
    {GPIOA, 8},   // a
    {GPIOA, 9},   // b
    {GPIOA, 10},  // c
    {GPIOB, 3},   // d
    {GPIOB, 4},   // e
    {GPIOB, 5},   // f
    {GPIOB, 6},   // g
    {GPIOB, 8},   // dp (punto decimal
};

// Pines para seleccionar cada digito.
 // digitos[0] = unidades, digitos[1] = decenas, digitos[2] = centenas.
 // digitos[3] no se usa. 
static const pin_t digitos[4] = {
    {GPIOB, 9},   // Digito 1 -> unidades
    {GPIOB, 10},  // Digito 2 -> decenas
    {GPIOA, 6},   // Digito 3 -> centenas
    {GPIOA, 7},   // Digito 4 -> sin usar
};

// Patron de segmentos para cada numero del 0 al 9.
 
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



 //  FUNCIONES "HAL" (acceso directo a registros)


/* Enciende el reloj de los 3 puertos que vamos a usar */
void habilitar_relojes(void)
{
    *RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
    
}

/* Configura un pin como salida digital.
 * MODER usa 2 bits por pin: 00=entrada, 01=salida, 10=funcion alterna, 11=analogo */
void configurar_pin_salida(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(0x3U << (pin * 2));  // Borra los 2 bits del pin
    puerto->MODER |=  (0x1U << (pin * 2));  // Pone 01 = salida
}

/* Configura un pin como entrada, activando su pull-up interno.
 * PUPDR usa 2 bits por pin: 00=sin resistencia, 01=pull-up, 10=pull-down */
void configurar_pin_entrada_pullup(GPIO_TypeDef *puerto, uint8_t pin)
{
    puerto->MODER &= ~(0x3U << (pin * 2));   // 00 = entrada
    puerto->PUPDR &= ~(0x3U << (pin * 2));   // Borram configuracion previa
    puerto->PUPDR |=  (0x1U << (pin * 2));   // 01 = pull-up interno
}

/* Escribe 0 o 1 en un pin configurado como salida */
void escribir_pin(GPIO_TypeDef *puerto, uint8_t pin, uint8_t valor)
{
    if (valor)
        puerto->ODR |= (1U << pin);
    else
        puerto->ODR &= ~(1U << pin);
}

/* Retardo simple por software: solo cuenta hasta que "ciclos" llegue a 0.
 * volatile evita que el compilador borre este bucle pensando que "no sirve". */
void retardo(volatile uint32_t ciclos)
{
    while (ciclos--);
}


//  FUNCIONES DEL DISPLAY
 

/* Configura todos los pines de segmentos y de seleccion de digito como salida */
void configurar_display(void)
{
    for (uint8_t i = 0; i < 8; i++)
        configurar_pin_salida(segmentos[i].puerto, segmentos[i].pin);

    for (uint8_t i = 0; i < 4; i++)
        configurar_pin_salida(digitos[i].puerto, digitos[i].pin);
}

/* Apaga los 4 digitos (deja de seleccionar todos display) */
void apagar_todos_los_digitos(void)
{
    uint8_t nivel_apagado = CATODO_COMUN ? 1 : 0;
    for (uint8_t i = 0; i < 4; i++)
        escribir_pin(digitos[i].puerto, digitos[i].pin, nivel_apagado);
}

/* Prende los segmentos necesarios para mostrar "numero" (0-9) */
void mostrar_numero_en_segmentos(uint8_t numero)
{
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t bit = tabla_digitos[numero][i];
        uint8_t nivel = CATODO_COMUN ? bit : (uint8_t)!bit;
        escribir_pin(segmentos[i].puerto, segmentos[i].pin, nivel);
    }
    /* El punto decimal se deja siempre apagado */
    uint8_t dp_apagado = CATODO_COMUN ? 0 : 1;
    escribir_pin(segmentos[7].puerto, segmentos[7].pin, dp_apagado);
}

/* Activa (selecciona) un digito especifico: 0=unidades, 1=decenas, 2=centenas */
void seleccionar_digito(uint8_t indice)
{
    uint8_t nivel_encendido = CATODO_COMUN ? 0 : 1;
    escribir_pin(digitos[indice].puerto, digitos[indice].pin, nivel_encendido);
}


 //FUNCIONES DEL DIP SWITCH
 

/* Configura PC0..PC7 como entradas con pull-up interno */
void configurar_dip_switch(void)
{
    for (uint8_t pin = 0; pin < 8; pin++)
        configurar_pin_entrada_pullup(GPIOC, pin);
}

/* Lee los 8 switches como un solo numero de 0 a 255 */
uint8_t leer_dip_switch(void)
{
    uint8_t valor = (uint8_t)(GPIOC->IDR & 0xFFU);

    /* Con pull-up interno: switch abierto = 1, switch cerrado (a GND) = 0.
     * Invertimos el valor para que sea mas facil de leer:
     * switch "activado" = 1, switch "apagado" = 0. */
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
        uint8_t entrada = leer_dip_switch();   // numero de 0 a 255

        /* Conversion binario -> BCD usando division y modulo,
         * que es la forma mas facil de explicar. */
        uint8_t centenas = entrada / 100;
        uint8_t decenas  = (entrada / 10) % 10;
        uint8_t unidades = entrada % 10;

        /* Multiplexado: mostramos un digito, esperamos un poco,
         * lo apagamos, y pasamos al siguiente. Se repite tan rapido
         * que el ojo humano ve los 3 digitos encendidos a la vez. */

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