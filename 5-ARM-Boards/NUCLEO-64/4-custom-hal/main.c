#include "gpio_config.h"

int main(void)
{
    GPIO_Config();

    while (1)
    {
        /* BOTON 1 -> LED ROJO 1 + LED AZUL 1 */
        if (boton_presionado(1))
        {
            led_rojo(1, 1);
            led_azul(1, 1);
        }

        /* BOTON 2 -> LED ROJO 2 + LED AZUL 2 */
        if (boton_presionado(2))
        {
            led_rojo(2, 1);
            led_azul(2, 1);
        }

        /* BOTON 3 -> LED ROJO 3 + LED AZUL 3 */
        if (boton_presionado(3))
        {
            led_rojo(3, 1);
            led_azul(3, 1);
        }

        /* BOTON 4 -> LED ROJO 4 + LED AZUL 4 */
        if (boton_presionado(4))
        {
            led_rojo(4, 1);
            led_azul(4, 1);
        }

        /* BOTON 5 -> LED ROJO 5 + LED AZUL 5 */
        if (boton_presionado(5))
        {
            led_rojo(5, 1);
            led_azul(5, 1);
        }

        /* BOTON 6 -> LED ROJO 6 + LED AZUL 6 */
        if (boton_presionado(6))
        {
            led_rojo(6, 1);
            led_azul(6, 1);
        }

        /* BOTON 7 -> LED ROJO 7 + LED AZUL 7 */
        if (boton_presionado(7))
        {
            led_rojo(7, 1);
            led_azul(7, 1);
        }

        /* BOTON 8 -> LED ROJO 8 + LED AZUL 8 */
        if (boton_presionado(8))
        {
            led_rojo(8, 1);
            led_azul(8, 1);
        }

        /* BOTON 9 -> LED ROJO 9 + LED AZUL 9 */
        if (boton_presionado(9))
        {
            led_rojo(9, 1);
            led_azul(9, 1);
        }
    }

    return 0;
}