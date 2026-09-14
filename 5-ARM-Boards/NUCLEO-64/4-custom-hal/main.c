#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    /*
     * PRUEBA LED 9
     *
     * LED 9 -> PC9 -> CN10-1
     */

    led_rojo(9, 1);

    while (1)
    {
    }

    return 0;
}