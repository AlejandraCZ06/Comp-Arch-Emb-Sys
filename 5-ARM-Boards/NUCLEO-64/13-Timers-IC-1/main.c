```c
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "inc/gpio_config.h"
#include "inc/system_config.h"

volatile SW_Timers timers;

volatile uint32_t period = 0;
volatile uint32_t duty = 0;

volatile uint32_t frecuencia = 0;

volatile uint8_t digit1 = 0;
volatile uint8_t digit2 = 0;
volatile uint8_t digit3 = 0;
volatile uint8_t digit4 = 0;
volatile uint8_t digit5 = 0;


void SysTick_Handler(void)
{
    update_sw_timers(&timers);
}


void TIM5_IRQHandler(void)
{
    if(TIM5->SR & TIM_SR_CC1IF)
    {
        period = TIM5->CCR1;

        if(period != 0)
        {
            duty = TIM5->CCR2;
        }

        WRITE_REG_FIELD(TIM5->SR, TIM_SR_CC1IF, 0);
    }
}


void TIM5_IC_Init(void)
{
    WRITE_REG_FIELD(
        RCC->APB1ENR,
        RCC_APB1ENR_TIM5EN,
        1
    );

    volatile unsigned int dummy;

    dummy = RCC->APB1ENR;
    dummy = RCC->APB1ENR;


    /*
     * TIM5 funciona como contador.
     * PSC = 0 significa que no se aplica
     * división adicional al reloj del timer.
     */

    WRITE_REG(TIM5->PSC, 0);

    /*
     * Valor máximo del contador.
     */

    WRITE_REG(TIM5->ARR, 0xffffffff);


    /*
     * CH1 como Input Capture sobre TI1.
     */

    WRITE_REG_FIELD(
        TIM5->CCMR1,
        TIM_CCMR1_CC1S,
        1
    );


    /*
     * CH1 captura en flanco de subida.
     */

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC1P,
        0
    );

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC1NP,
        0
    );


    /*
     * CH2 captura también la entrada TI1.
     * Se utiliza para obtener el ancho del pulso.
     */

    WRITE_REG_FIELD(
        TIM5->CCMR1,
        TIM_CCMR1_CC2S,
        2
    );


    /*
     * CH2 captura en flanco de bajada.
     */

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC2P,
        1
    );

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC2NP,
        0
    );


    /*
     * TI1FP1 como fuente del trigger.
     */

    WRITE_REG_FIELD(
        TIM5->SMCR,
        TIM_SMCR_TS,
        5
    );


    /*
     * El contador se reinicia cada vez
     * que llega un flanco de subida.
     */

    WRITE_REG_FIELD(
        TIM5->SMCR,
        TIM_SMCR_SMS,
        4
    );


    /*
     * Contador ascendente.
     */

    WRITE_REG_FIELD(
        TIM5->CR1,
        TIM_CR1_DIR,
        0
    );


    WRITE_REG_FIELD(
        TIM5->CR1,
        TIM_CR1_ARPE,
        1
    );


    /*
     * Habilitar captura de CH1.
     */

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC1E,
        1
    );


    /*
     * Habilitar captura de CH2.
     */

    WRITE_REG_FIELD(
        TIM5->CCER,
        TIM_CCER_CC2E,
        1
    );


    /*
     * Habilitar interrupción de CH1.
     */

    WRITE_REG_FIELD(
        TIM5->DIER,
        TIM_DIER_CC1IE,
        1
    );


    /*
     * Encender TIM5.
     */

    WRITE_REG_FIELD(
        TIM5->CR1,
        TIM_CR1_CEN,
        1
    );


    NVIC_EnableIRQ(TIM5_IRQn);
}


void GPIO_board_config(void)
{
    /*
     * Segmentos de los displays
     */

    configurar_salida(GPIOA, 8);
    configurar_salida(GPIOA, 9);
    configurar_salida(GPIOA, 10);

    configurar_salida(GPIOB, 3);
    configurar_salida(GPIOB, 4);
    configurar_salida(GPIOB, 5);
    configurar_salida(GPIOB, 6);


    /*
     * Selectores de los 5 displays
     *
     * PB7  -> Display 1
     * PB8  -> Display 2
     * PB9  -> Display 3
     * PB10 -> Display 4
     * PA6  -> Display 5
     */

    configurar_salida(GPIOB, 7);
    configurar_salida(GPIOB, 8);
    configurar_salida(GPIOB, 9);
    configurar_salida(GPIOB, 10);

    configurar_salida(GPIOA, 6);


    /*
     * PA0 = TIM5_CH1
     *
     * Aquí llega la señal cuadrada
     * proveniente del comparador.
     */

    GPIO_InitTypeDef GPIO_Init;

    GPIO_Init.Pin = 0;
    GPIO_Init.Mode = 2;
    GPIO_Init.Pull = 0;
    GPIO_Init.Speed = 3;
    GPIO_Init.Alternate = 2;

    GPIO_Config(GPIOA, GPIO_Init);
}


void separar_frecuencia(uint32_t valor)
{
    digit1 = valor / 10000;

    digit2 = (valor / 1000) % 10;

    digit3 = (valor / 100) % 10;

    digit4 = (valor / 10) % 10;

    digit5 = valor % 10;
}


void mostrar_frecuencia(void)
{
    /*
     * Display 1
     */

    apagar_digitos();

    mostrar_numero(digit1);

    escribir(GPIOB, 7, 0);

    retardo(1000);


    /*
     * Display 2
     */

    apagar_digitos();

    mostrar_numero(digit2);

    escribir(GPIOB, 8, 0);

    retardo(1000);


    /*
     * Display 3
     */

    apagar_digitos();

    mostrar_numero(digit3);

    escribir(GPIOB, 9, 0);

    retardo(1000);


    /*
     * Display 4
     */

    apagar_digitos();

    mostrar_numero(digit4);

    escribir(GPIOB, 10, 0);

    retardo(1000);


    /*
     * Display 5
     */

    apagar_digitos();

    mostrar_numero(digit5);

    escribir(GPIOA, 6, 0);

    retardo(1000);
}


int main(void)
{
    /*
     * Configuración del reloj del sistema.
     */

    clock_config();


    /*
     * Inicializar SysTick.
     */

    SysTick_Init(1000);

    SysTick_enable_IrQ(1);

    timers.sw_tmr1_period = 10;


    /*
     * Habilitar reloj de GPIOA.
     */

    *RCC_AHB1ENR |= (1U << 0);


    /*
     * Habilitar reloj de GPIOB.
     */

    *RCC_AHB1ENR |= (1U << 1);


    /*
     * Habilitar reloj de GPIOC.
     */

    *RCC_AHB1ENR |= (1U << 2);


    /*
     * Configuración de GPIO.
     */

    GPIO_board_config();


    /*
     * Configuración de TIM5
     * para Input Capture.
     */

    TIM5_IC_Init();


    while(1)
    {
        /*
         * Si ya se detectó un período,
         * podemos calcular la frecuencia.
         */

        if(period != 0)
        {
            frecuencia = 42000000 / period;


            /*
             * Separar la frecuencia
             * en cinco dígitos.
             */

            separar_frecuencia(frecuencia);


            /*
             * Mostrar continuamente
             * la frecuencia.
             */

            mostrar_frecuencia();
        }
    }


    HALT();

    return 0;
}
```