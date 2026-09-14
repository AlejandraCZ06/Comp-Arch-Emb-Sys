#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    /*
     * PRUEBA DIRECTA DE PA3
     *
     * LED 9 -> PA3
     */

    GPIOA->ODR |= (1 << 3);

    while (1)
    {
    }

    return 0;
}