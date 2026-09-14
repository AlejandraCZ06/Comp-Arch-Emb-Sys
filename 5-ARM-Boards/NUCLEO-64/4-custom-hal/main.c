#include "gpio_config.h"
#include "stdint.h"

/* ==================================================
 * ESTADO DEL TABLERO
 *
 * 0 = casilla libre
 * 1 = jugador humano (azul)
 * 2 = microcontrolador (rojo)
 * ================================================== */

uint8_t tablero[9] = {0};


/* ==================================================
 * COMPROBAR GANADOR
 *
 * jugador = 1 -> humano
 * jugador = 2 -> microcontrolador
 * ================================================== */

uint8_t hay_ganador(uint8_t jugador)
{
    /* Filas */
    if (tablero[0] == jugador &&
        tablero[1] == jugador &&
        tablero[2] == jugador)
        return 1;

    if (tablero[3] == jugador &&
        tablero[4] == jugador &&
        tablero[5] == jugador)
        return 1;

    if (tablero[6] == jugador &&
        tablero[7] == jugador &&
        tablero[8] == jugador)
        return 1;

    /* Columnas */
    if (tablero[0] == jugador &&
        tablero[3] == jugador &&
        tablero[6] == jugador)
        return 1;

    if (tablero[1] == jugador &&
        tablero[4] == jugador &&
        tablero[7] == jugador)
        return 1;

    if (tablero[2] == jugador &&
        tablero[5] == jugador &&
        tablero[8] == jugador)
        return 1;

    /* Diagonal principal */
    if (tablero[0] == jugador &&
        tablero[4] == jugador &&
        tablero[8] == jugador)
        return 1;

    /* Diagonal secundaria */
    if (tablero[2] == jugador &&
        tablero[4] == jugador &&
        tablero[6] == jugador)
        return 1;

    return 0;
}


/* ==================================================
 * COMPROBAR EMPATE
 * ================================================== */

uint8_t tablero_lleno(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
    {
        if (tablero[i] == 0)
            return 0;
    }

    return 1;
}


/* ==================================================
 * APAGAR TODO EL TABLERO
 * ================================================== */

void limpiar_tablero(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
    {
        tablero[i] = 0;

        led_rojo(i + 1, 0);
        led_azul(i + 1, 0);
    }
}


/* ==================================================
 * ESPERAR A QUE SE SUELTE EL BOTÓN
 * ================================================== */

void esperar_soltar_boton(uint8_t boton)
{
    while (boton_presionado(boton))
    {
        /* Esperar */
    }
}


/* ==================================================
 * BUSCAR PRIMERA CASILLA LIBRE
 * ================================================== */

uint8_t buscar_casilla_libre(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
    {
        if (tablero[i] == 0)
            return i;
    }

    return 9;
}


/* ==================================================
 * JUGADA DEL MICROCONTROLADOR
 *
 * Por ahora el microcontrolador selecciona
 * la primera casilla disponible.
 * ================================================== */

void jugada_computadora(void)
{
    uint8_t casilla;

    casilla = buscar_casilla_libre();

    if (casilla < 9)
    {
        tablero[casilla] = 2;

        /* LED rojo del microcontrolador */
        led_rojo(casilla + 1, 1);
    }
}


/* ==================================================
 * MAIN
 * ================================================== */

int main(void)
{
    uint8_t i;
    uint8_t juego_terminado;

    GPIO_Config();

    limpiar_tablero();

    while (1)
    {
        juego_terminado = 0;

        /* ==========================================
         * TURNO DEL JUGADOR HUMANO
         * ========================================== */

        while (!juego_terminado)
        {
            for (i = 0; i < 9; i++)
            {
                /*
                 * El botón representa la casilla
                 * i + 1.
                 */
                if (boton_presionado(i + 1))
                {
                    /*
                     * Si la casilla está ocupada,
                     * ignoramos la jugada.
                     */
                    if (tablero[i] != 0)
                    {
                        esperar_soltar_boton(i + 1);
                        continue;
                    }

                    /* ==================================
                     * JUGADA DEL HUMANO
                     * ================================== */

                    tablero[i] = 1;

                    /* Encender LED azul */
                    led_azul(i + 1, 1);

                    esperar_soltar_boton(i + 1);


                    /* ==================================
                     * COMPROBAR SI GANÓ EL HUMANO
                     * ================================== */

                    if (hay_ganador(1))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * COMPROBAR EMPATE
                     * ================================== */

                    if (tablero_lleno())
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * TURNO DEL MICROCONTROLADOR
                     * ================================== */

                    jugada_computadora();


                    /* ==================================
                     * COMPROBAR SI GANÓ EL COMPUTADOR
                     * ================================== */

                    if (hay_ganador(2))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * COMPROBAR EMPATE
                     * ================================== */

                    if (tablero_lleno())
                    {
                        juego_terminado = 1;
                        break;
                    }
                }
            }
        }


        /* ==========================================
         * PARTIDA TERMINADA
         *
         * Esperar un momento antes de reiniciar.
         * ========================================== */

        for (volatile uint32_t delay = 0;
             delay < 1000000;
             delay++)
        {
            /* Retardo */
        }

        limpiar_tablero();
    }

    return 0;
}