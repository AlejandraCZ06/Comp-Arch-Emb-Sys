#include "gpio_config.h"


int main(void)
{
    uint8_t i;


    /* Configurar GPIO */

    GPIO_Config();


    while (1)
    {
        for (i = 0; i < 9; i++)
        {
            if (boton_presionado(i))
            {
                led_rojo(i, 1);
            }
            else
            {
                led_rojo(i, 0);
            }
        }
    }


    return 0;
}