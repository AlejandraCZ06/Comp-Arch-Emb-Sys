#include "gpio_config.h"
#include "stdint.h"


/* =========================================================
   TABLERO

   0 = casilla libre
   1 = jugador humano
   2 = computadora

   Posiciones:

        1 | 2 | 3
       ---+---+---
        4 | 5 | 6
       ---+---+---
        7 | 8 | 9
   ========================================================= */
uint8_t tablero[9] = {0};


/* =========================================================
   GANADOR

   0 = nadie
   1 = humano
   2 = computadora
   ========================================================= */
uint8_t ganador_final = 0;


/* =========================================================
   LINEA GANADORA

   Guarda las 3 casillas que hicieron ganar.

   Ejemplo:

   {0,1,2} = primera fila
   {0,4,8} = diagonal
   ========================================================= */
uint8_t linea_ganadora[3] = {0, 0, 0};


/* =========================================================
   REGISTROS DWT PARA NUMERO PSEUDOALEATORIO
   ========================================================= */

#define DEMCR      (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL   (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)


/* =========================================================
   INICIAR CONTADOR DWT
   ========================================================= */
void iniciar_aleatorio(void)
{
    /* Habilitar trace */
    DEMCR |= (1 << 24);

    /* Reiniciar contador */
    DWT_CYCCNT = 0;

    /* Habilitar contador */
    DWT_CTRL |= 1;
}


/* =========================================================
   NUMERO PSEUDOALEATORIO

   Devuelve un numero entre:

   0 y max-1
   ========================================================= */
uint8_t numero_aleatorio(uint8_t max)
{
    if (max == 0)
        return 0;

    return DWT_CYCCNT % max;
}


/* =========================================================
   COMPROBAR GANADOR

   jugador = 1 -> humano
   jugador = 2 -> computadora

   Si gana:
   guarda las 3 posiciones ganadoras.
   ========================================================= */
uint8_t ganador(uint8_t jugador)
{
    uint8_t c[8][3] =
    {
        /* Filas */
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},

        /* Columnas */
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},

        /* Diagonales */
        {0, 4, 8},
        {2, 4, 6}
    };

    uint8_t i;


    for (i = 0; i < 8; i++)
    {
        if (tablero[c[i][0]] == jugador &&
            tablero[c[i][1]] == jugador &&
            tablero[c[i][2]] == jugador)
        {
            /*
             * Guardar exactamente las 3
             * casillas de la linea ganadora.
             */
            linea_ganadora[0] = c[i][0];
            linea_ganadora[1] = c[i][1];
            linea_ganadora[2] = c[i][2];

            return 1;
        }
    }

    return 0;
}


/* =========================================================
   COMPROBAR SI EL TABLERO ESTA LLENO
   ========================================================= */
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
   LIMPIAR TABLERO PARA NUEVA PARTIDA
   ========================================================= */
void limpiar_tablero(void)
{
    uint8_t i;

    ganador_final = 0;

    linea_ganadora[0] = 0;
    linea_ganadora[1] = 0;
    linea_ganadora[2] = 0;


    for (i = 0; i < 9; i++)
    {
        tablero[i] = 0;

        /* Apagar rojo */
        led_rojo(i + 1, 0);

        /* Apagar azul */
        led_azul(i + 1, 0);
    }
}


/* =========================================================
   ESPERAR A QUE SE SUELTE UN BOTON
   ========================================================= */
void esperar_soltura(uint8_t boton)
{
    while (boton_presionado(boton))
    {
    }
}


/* =========================================================
   ESPERAR A QUE SE PRESIONE B1
   ========================================================= */
void esperar_B1(void)
{
    while (!boton_start())
    {
    }
}


/* =========================================================
   ESPERAR A QUE SE SUELTE B1
   ========================================================= */
void soltar_B1(void)
{
    while (boton_start())
    {
    }
}


/* =========================================================
   BUSCAR UNA CASILLA LIBRE ALEATORIAMENTE

   Devuelve:
   0..8 = casilla libre
   9    = no hay casillas
   ========================================================= */
uint8_t casilla_libre(void)
{
    uint8_t libres[9];
    uint8_t cantidad = 0;
    uint8_t i;


    /*
     * Guardar todas las casillas libres.
     */
    for (i = 0; i < 9; i++)
    {
        if (tablero[i] == 0)
        {
            libres[cantidad] = i;
            cantidad++;
        }
    }


    /*
     * No hay casillas libres.
     */
    if (cantidad == 0)
        return 9;


    /*
     * Elegir aleatoriamente una
     * de las casillas libres.
     */
    return libres[numero_aleatorio(cantidad)];
}


/* =========================================================
   JUGADA DE LA COMPUTADORA
   ========================================================= */
void jugada_computadora(void)
{
    uint8_t casilla;


    casilla = casilla_libre();


    if (casilla < 9)
    {
        /*
         * Marcar casilla como computadora.
         */
        tablero[casilla] = 2;


        /*
         * Encender LED rojo.
         */
        led_rojo(casilla + 1, 1);
    }
}


/* =========================================================
   JUEGO
   ========================================================= */
void jugar(void)
{
    uint8_t i;


    while (1)
    {
        /*
         * Revisar los 9 botones.
         */
        for (i = 0; i < 9; i++)
        {
            if (boton_presionado(i + 1))
            {
                /*
                 * Si la casilla ya esta ocupada,
                 * ignorar el boton.
                 */
                if (tablero[i] != 0)
                {
                    esperar_soltura(i + 1);
                    continue;
                }


                /* =========================================
                   JUGADA DEL HUMANO
                   ========================================= */

                tablero[i] = 1;

                /*
                 * Encender LED azul.
                 */
                led_azul(i + 1, 1);


                /*
                 * Esperar a que el usuario suelte
                 * el boton.
                 */
                esperar_soltura(i + 1);


                /*
                 * Comprobar si gano el humano.
                 */
                if (ganador(1))
                {
                    ganador_final = 1;
                    return;
                }


                /*
                 * Comprobar empate.
                 */
                if (tablero_lleno())
                    return;


                /* =========================================
                   JUGADA DE LA COMPUTADORA
                   ========================================= */

                jugada_computadora();


                /*
                 * Comprobar si gano la computadora.
                 */
                if (ganador(2))
                {
                    ganador_final = 2;
                    return;
                }


                /*
                 * Comprobar empate.
                 */
                if (tablero_lleno())
                    return;
            }
        }
    }
}


/* =========================================================
   TITILAR SOLAMENTE LOS 3 LEDS GANADORES

   Si gana humano:
       titilan los 3 LEDS AZULES ganadores.

   Si gana computadora:
       titilan los 3 LEDS ROJOS ganadores.

   Los otros 6 LEDs quedan apagados.
   ========================================================= */
void titilar_ganador(void)
{
    uint8_t i;
    uint8_t j;

    volatile uint32_t d;


    /*
     * Si no hay ganador, no hacer nada.
     *
     * Esto ocurre en caso de empate.
     */
    if (ganador_final == 0)
        return;


    /* =====================================================
       APAGAR TODOS LOS LEDS
       ===================================================== */

    for (i = 0; i < 9; i++)
    {
        led_rojo(i + 1, 0);
        led_azul(i + 1, 0);
    }


    /* =====================================================
       TITILAR 3 VECES
       ===================================================== */

    for (j = 0; j < 3; j++)
    {
        /* ================================================
           APAGAR LOS 3 GANADORES
           ================================================ */

        for (i = 0; i < 3; i++)
        {
            if (ganador_final == 1)
            {
                /*
                 * Gano humano -> azul
                 */
                led_azul(linea_ganadora[i] + 1, 0);
            }
            else
            {
                /*
                 * Gano computadora -> rojo
                 */
                led_rojo(linea_ganadora[i] + 1, 0);
            }
        }


        /*
         * Retardo
         */
        for (d = 0; d < 500000; d++)
        {
        }


        /* ================================================
           ENCENDER LOS 3 GANADORES
           ================================================ */

        for (i = 0; i < 3; i++)
        {
            if (ganador_final == 1)
            {
                /*
                 * Gano humano -> azul
                 */
                led_azul(linea_ganadora[i] + 1, 1);
            }
            else
            {
                /*
                 * Gano computadora -> rojo
                 */
                led_rojo(linea_ganadora[i] + 1, 1);
            }
        }


        /*
         * Retardo
         */
        for (d = 0; d < 500000; d++)
        {
        }
    }


    /*
     * Al terminar los 3 destellos,
     * dejar los 3 ganadores encendidos.
     *
     * Los otros 6 permanecen apagados.
     */
}


/* =========================================================
   MAIN
   ========================================================= */
int main(void)
{
    uint8_t turno;


    /* Configurar GPIO */
    GPIO_Config();


    /* Iniciar generador pseudoaleatorio */
    iniciar_aleatorio();


    while (1)
    {
        /* =================================================
           ESPERAR B1

           B1 inicia una nueva partida.
           ================================================= */
        esperar_B1();


        /* =================================================
           DECIDIR ALEATORIAMENTE QUIEN EMPIEZA

           turno = 0 -> humano
           turno = 1 -> computadora
           ================================================= */
        turno = numero_aleatorio(2);


        /* =================================================
           ESPERAR QUE B1 SEA SOLTADO
           ================================================= */
        soltar_B1();


        /* =================================================
           LIMPIAR TABLERO
           ================================================= */
        limpiar_tablero();


        /* =================================================
           SI EMPIEZA LA COMPUTADORA
           ================================================= */
        if (turno == 1)
        {
            jugada_computadora();


            /*
             * Comprobar si la computadora gano.
             *
             * Normalmente no puede ganar en la primera
             * jugada, pero se mantiene la comprobacion.
             */
            if (ganador(2))
            {
                ganador_final = 2;
            }
        }


        /* =================================================
           JUGAR

           Si la computadora no gano inmediatamente,
           empieza el juego normal.
           ================================================= */
        if (ganador_final == 0)
        {
            jugar();
        }


        /* =================================================
           MOSTRAR RESULTADO

           Si hubo ganador:
               solo titilan los 3 ganadores.

           Si hubo empate:
               no titila nada.
               El tablero queda lleno.
           ================================================= */
        titilar_ganador();


        /*
         * Volver al principio.
         *
         * El siguiente B1 comienza otra partida.
         */
    }


    return 0;
}