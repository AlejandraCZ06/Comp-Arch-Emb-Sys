#include "gpio_config.h"
#include "stdint.h"

uint8_t tablero[9] = {0};
uint8_t ganador_final = 0;

#define DEMCR      (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CTRL   (*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)

void iniciar_aleatorio(void)
{
    DEMCR |= (1 << 24);
    DWT_CYCCNT = 0;
    DWT_CTRL |= 1;
}

uint8_t ganador(uint8_t jugador)
{
    uint8_t c[8][3] = {
        {0,1,2},
        {3,4,5},
        {6,7,8},
        {0,3,6},
        {1,4,7},
        {2,5,8},
        {0,4,8},
        {2,4,6}
    };

    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (tablero[c[i][0]] == jugador &&
            tablero[c[i][1]] == jugador &&
            tablero[c[i][2]] == jugador)
        {
            return 1;
        }
    }

    return 0;
}

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

void limpiar_tablero(void)
{
    uint8_t i;

    ganador_final = 0;

    for (i = 0; i < 9; i++)
    {
        tablero[i] = 0;

        led_rojo(i + 1, 0);
        led_azul(i + 1, 0);
    }
}

void esperar_soltura(uint8_t boton)
{
    while (boton_presionado(boton))
    {
    }
}

void esperar_B1(void)
{
    while (!boton_start())
    {
    }
}

void soltar_B1(void)
{
    while (boton_start())
    {
    }
}

uint8_t casilla_libre(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
    {
        if (tablero[i] == 0)
            return i;
    }

    return 9;
}

void jugada_computadora(void)
{
    uint8_t casilla;

    casilla = casilla_libre();

    if (casilla < 9)
    {
        tablero[casilla] = 2;
        led_rojo(casilla + 1, 1);
    }
}

void jugar(void)
{
    uint8_t i;

    while (1)
    {
        /* JUGADA HUMANA */
        for (i = 0; i < 9; i++)
        {
            if (boton_presionado(i + 1))
            {
                /* Casilla ocupada: ignorar */
                if (tablero[i] != 0)
                {
                    esperar_soltura(i + 1);
                    continue;
                }

                /* Registrar jugada */
                tablero[i] = 1;

                /* Encender LED azul */
                led_azul(i + 1, 1);

                esperar_soltura(i + 1);

                /* Si gana el humano, terminar */
                if (ganador(1))
                {
                    ganador_final = 1;
                    return;
                }

                /* Si se llenaron las 9, empate */
                if (tablero_lleno())
                    return;

                /* JUGADA COMPUTADORA */
                jugada_computadora();

                /* Si gana la computadora, terminar */
                if (ganador(2))
                {
                    ganador_final = 2;
                    return;
                }

                /* Si se llenaron las 9, empate */
                if (tablero_lleno())
                    return;
            }
        }
    }
}

void titilar_ganador(void)
{
    uint8_t i;
    uint8_t j;
    volatile uint32_t d;

    if (ganador_final == 0)
        return;

    /* Apagar LEDs del perdedor */
    for (i = 0; i < 9; i++)
    {
        if (tablero[i] != ganador_final)
        {
            led_rojo(i + 1, 0);
            led_azul(i + 1, 0);
        }
    }

    /* Titilar ganador 3 veces */
    for (j = 0; j < 3; j++)
    {
        /* Apagar ganador */
        for (i = 0; i < 9; i++)
        {
            if (tablero[i] == ganador_final)
            {
                if (ganador_final == 1)
                    led_azul(i + 1, 0);
                else
                    led_rojo(i + 1, 0);
            }
        }

        for (d = 0; d < 500000; d++)
        {
        }

        /* Encender ganador */
        for (i = 0; i < 9; i++)
        {
            if (tablero[i] == ganador_final)
            {
                if (ganador_final == 1)
                    led_azul(i + 1, 1);
                else
                    led_rojo(i + 1, 1);
            }
        }

        for (d = 0; d < 500000; d++)
        {
        }
    }
}

int main(void)
{
    uint8_t turno;

    GPIO_Config();
    iniciar_aleatorio();

    while (1)
    {
        /* B1 = INICIO + ALEATORIO */
        esperar_B1();
        soltar_B1();

        /* Nueva partida */
        limpiar_tablero();

        /* Elegir aleatoriamente quien empieza */
        turno = DWT_CYCCNT & 1;

        /* Si empieza computadora */
        if (turno == 1)
        {
            jugada_computadora();

            if (ganador(2))
                ganador_final = 2;
        }

        /* Jugar partida */
        if (ganador_final == 0)
        {
            jugar();
        }

        /* Si hay ganador, titilar 3 veces */
        titilar_ganador();

        /* Si fue empate, simplemente queda
           el tablero completo y espera B1 */
    }

    return 0;
}