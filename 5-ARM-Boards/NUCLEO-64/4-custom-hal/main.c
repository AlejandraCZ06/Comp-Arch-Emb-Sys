#include "gpio_config.h"
#include "stdint.h"

/* =========================================================
 * ESTADO DEL TABLERO
 *
 * 0 = casilla libre
 * 1 = jugador humano (azul)
 * 2 = microcontrolador (rojo)
 * ========================================================= */

uint8_t tablero[9] = {0};


/* =========================================================
 * DWT - CONTADOR DE CICLOS
 *
 * Se utiliza el contador de ciclos del Cortex-M4
 * para obtener un valor pseudoaleatorio.
 * ========================================================= */

#define DEMCR      (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL   (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)


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


/* =========================================================
 * COMPROBAR GANADOR
 *
 * jugador = 1 -> humano
 * jugador = 2 -> microcontrolador
 * ========================================================= */

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


/* =========================================================
 * COMPROBAR EMPATE
 * ========================================================= */

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


/* =========================================================
 * LIMPIAR TABLERO
 *
 * Apaga todos los LEDs y deja todas las casillas libres.
 * ========================================================= */

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


/* =========================================================
 * ESPERAR A QUE SE SUELTE UN BOTÓN DE CASILLA
 * ========================================================= */

void esperar_soltar_boton(uint8_t boton)
{
    while (boton_presionado(boton))
    {
        /* Esperar */
    }
}


/* =========================================================
 * ESPERAR A QUE SE SUELTE B1
 *
 * B1 está conectado a PC13.
 * ========================================================= */

void esperar_soltar_B1(void)
{
    while (boton_start())
    {
        /* Esperar */
    }
}


/* =========================================================
 * ESPERAR A QUE SE PRESIONE B1
 * ========================================================= */

void esperar_B1(void)
{
    while (!boton_start())
    {
        /* Esperar */
    }
}


/* =========================================================
 * BUSCAR PRIMERA CASILLA LIBRE
 * ========================================================= */

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


/* =========================================================
 * JUGADA DEL MICROCONTROLADOR
 *
 * Por ahora el microcontrolador selecciona
 * la primera casilla disponible.
 * ========================================================= */

void jugada_computadora(void)
{
    uint8_t casilla;

    casilla = buscar_casilla_libre();

    if (casilla < 9)
    {
        tablero[casilla] = 2;

        /* Encender LED rojo */
        led_rojo(casilla + 1, 1);
    }
}


/* =========================================================
 * MAIN
 *
 * B1 = PC13
 *
 * Cada vez que se presiona B1:
 *
 * 1. Se limpia el tablero.
 * 2. Se obtiene un valor pseudoaleatorio.
 * 3. Se decide quién comienza.
 * 4. Se inicia la partida.
 *
 * turno = 0 -> humano comienza
 * turno = 1 -> microcontrolador comienza
 * ========================================================= */

int main(void)
{
    uint8_t i;
    uint8_t juego_terminado;
    uint8_t turno;

    /* Configurar GPIO */
    GPIO_Config();

    /* Iniciar contador DWT */
    iniciar_contador_aleatorio();

    /* =====================================================
     * ESPERAR PRIMERA PRESIÓN DE B1
     * ===================================================== */

    esperar_B1();
    esperar_soltar_B1();


    while (1)
    {
        /* =================================================
         * NUEVA PARTIDA
         *
         * B1 inicia la partida y decide aleatoriamente
         * quién comienza.
         * ================================================= */

        limpiar_tablero();

        /*
         * Obtener valor pseudoaleatorio.

         * Si es 0 -> comienza humano
         * Si es 1 -> comienza microcontrolador
         */

        turno = obtener_aleatorio() & 1;

        juego_terminado = 0;


        /* =================================================
         * SI EMPIEZA EL MICROCONTROLADOR
         * ================================================= */

        if (turno == 1)
        {
            jugada_computadora();

            if (hay_ganador(2))
            {
                juego_terminado = 1;
            }
        }


        /* =================================================
         * JUEGO
         * ================================================= */

        while (!juego_terminado)
        {
            /* =============================================
             * TURNO DEL HUMANO
             * ============================================= */

            for (i = 0; i < 9; i++)
            {
                if (boton_presionado(i + 1))
                {
                    /* =====================================
                     * CASILLA OCUPADA
                     *
                     * Si ya tiene un LED encendido,
                     * se ignora la jugada.
                     * ===================================== */

                    if (tablero[i] != 0)
                    {
                        esperar_soltar_boton(i + 1);
                        continue;
                    }


                    /* =====================================
                     * JUGADA DEL HUMANO
                     * ===================================== */

                    tablero[i] = 1;

                    /* Encender LED azul */
                    led_azul(i + 1, 1);

                    /* Esperar a soltar botón */
                    esperar_soltar_boton(i + 1);


                    /* =====================================
                     * ¿GANÓ EL HUMANO?
                     * ===================================== */

                    if (hay_ganador(1))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* =====================================
                     * ¿EMPATE?
                     * ===================================== */

                    if (tablero_lleno())
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* =====================================
                     * TURNO DEL MICROCONTROLADOR
                     * ===================================== */

                    jugada_computadora();


                    /* =====================================
                     * ¿GANÓ EL MICROCONTROLADOR?
                     * ===================================== */

                    if (hay_ganador(2))
                    {
                        juego_terminado = 1;
                        break;
                    }


                    /* =====================================
                     * ¿EMPATE?
                     * ===================================== */

                    if (tablero_lleno())
                    {
                        juego_terminado = 1;
                        break;
                    }
                }
            }
        }


        /* =================================================
         * PARTIDA TERMINADA
         *
         * Los LEDs permanecen encendidos mostrando
         * el resultado.
         *
         * Para comenzar otra partida:
         * presionar B1.
         * ================================================= */

        esperar_B1();

        /* Esperar a que B1 sea soltado */
        esperar_soltar_B1();
    }

    return 0;
}