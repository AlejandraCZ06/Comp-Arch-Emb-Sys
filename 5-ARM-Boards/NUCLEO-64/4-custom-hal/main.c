#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    /* PA3 como salida */
    GPIOA->MODER &= ~(3 << (3 * 2));
    GPIOA->MODER |=  (1 << (3 * 2));

    /* PA3 en alto */
    GPIOA->ODR |= (1 << 3);

    while (1)
    {
    }

    return 0;
}