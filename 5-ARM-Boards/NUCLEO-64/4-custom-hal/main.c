#include "gpio_config.h"
#include "stdint.h"


/* ==================================================
 * ESTADO DEL TABLERO
 *
 * 0 = casilla libre
 * 1 = jugador humano (azul)
 * 2 = computador (rojo)
 * ================================================== */

uint8_t tablero[9] = {0};


/* ==================================================
 * FUNCIONES PARA DWT
 *
 * Se utiliza el contador de ciclos del Cortex-M4
 * para obtener una semilla variable.
 * ================================================== */

#define DEMCR        (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL     (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT   (*(volatile uint32_t *)0xE0001004)

void iniciar_contador_aleatorio(void)
{
    /* Activar DWT */
    DEMCR |= (1 << 24);

    /* Reiniciar contador */
    DWT_CYCCNT = 0;

    /* Activar contador de ciclos */
    DWT_CTRL |= 1;
}


uint32_t obtener_aleatorio(void)
{
    return DWT_CYCCNT;
}


/* ==================================================
 * COMPROBAR GANADOR
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
 * LIMPIAR TABLERO
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
 * ESPERAR A SOLTAR BOTÓN
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
 * JUGADA DEL COMPUTADOR
 * ================================================== */

void jugada_computadora(void)
{
    uint8_t casilla;

    casilla = buscar_casilla_libre();

    if (casilla < 9)
    {
        tablero[casilla] = 2;

        /* LED rojo */
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
    uint8_t turno;

    GPIO_Config();

    /* Iniciar contador utilizado para aleatoriedad */
    iniciar_contador_aleatorio();

    while (1)
    {
        /* ==========================================
         * NUEVA PARTIDA
         * ========================================== */

        limpiar_tablero();

        /*
         * Obtener valor pseudoaleatorio.
         *
         * Si es par:
         *     empieza humano
         *
         * Si es impar:
         *     empieza computador
         */

        turno = obtener_aleatorio() & 1;

        juego_terminado = 0;


        /* ==========================================
         * SI EMPIEZA EL COMPUTADOR
         * ========================================== */

        if (turno == 1)
        {
            jugada_computadora();

            if (hay_ganador(2))
            {
                juego_terminado = 1;
            }
        }


        /* ==========================================
         * JUEGO
         * ========================================== */

        while (!juego_terminado)
        {
            /* ======================================
             * TURNO DEL HUMANO
             * ====================================== */

            for (i = 0; i < 9; i++)
            {
                if (boton_presionado(i + 1))
                {
                    /*
                     * Si la casilla ya está ocupada,
                     * ignorar la jugada.
                     */

                    if (tablero[i] != 0)
                    {
                        esperar_soltar_boton(i + 1);
                        continue;
                    }


                    /* ==================================
                     * JUGADA HUMANA
                     * ================================== */

                    tablero[i] = 1;

                    /* Encender azul */
                    led_azul(i + 1, 1);

                    esperar_soltar_boton(i + 1);


                    /* ==================================
                     * ¿GANÓ EL HUMANO?
                     * ================================== */

                    if (hay_ganador(1))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * ¿EMPATE?
                     * ================================== */

                    if (tablero_lleno())
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * TURNO COMPUTADOR
                     * ================================== */

                    jugada_computadora();


                    /* ==================================
                     * ¿GANÓ EL COMPUTADOR?
                     * ================================== */

                    if (hay_ganador(2))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* ==================================
                     * ¿EMPATE?
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
         * ========================================== */

        for (volatile uint32_t delay = 0;
             delay < 1000000;
             delay++)
        {
            /* Retardo */
        }
    }

    return 0;
}