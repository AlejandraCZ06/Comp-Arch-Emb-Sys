#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    while (1)
    {
        if (read_pin_state(GPIOC, 10) == 0)
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