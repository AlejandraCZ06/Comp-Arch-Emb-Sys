#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    /*
     * PRUEBA DE LOS LEDS 3 Y 9
     *
     * LED 3 -> PB10
     * LED 9 -> PA3
     */

    led_rojo(3, 1);
    led_rojo(9, 1);

    while (1)
    {
    }

    return 0;
}