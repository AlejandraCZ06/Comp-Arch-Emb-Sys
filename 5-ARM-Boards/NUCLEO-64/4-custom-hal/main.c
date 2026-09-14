#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    while (1)
    {
        /*
         * BOTON 3 -> PB12
         * LED 3   -> PB10
         */

        if (boton_presionado(3))
        {
            led_rojo(3, 1);
        }
        else
        {
            led_rojo(3, 0);
        }
    }

    return 0;
}