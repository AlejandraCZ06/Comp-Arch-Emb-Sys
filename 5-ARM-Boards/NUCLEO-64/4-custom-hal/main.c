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

uint8_t numero_aleatorio(uint8_t max)
{
    if (max == 0)
        return 0;

    return DWT_CYCCNT % max;
}

uint8_t ganador(uint8_t jugador)
{
    uint8_t c[8][3] =
    {
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
    uint8_t libres[9];
    uint8_t cantidad = 0;
    uint8_t i;

    for (i = 0; i < 9; i++)
    {
        if (tablero[i] == 0)
        {
            libres[cantidad] = i;
            cantidad++;
        }
    }

    if (cantidad == 0)
        return 9;

    return libres[numero_aleatorio(cantidad)];
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
        for (i = 0; i < 9; i++)
        {
            if (boton_presionado(i + 1))
            {
                if (tablero[i] != 0)
                {
                    esperar_soltura(i + 1);
                    continue;
                }

                /* Jugada humana */
                tablero[i] = 1;
                led_azul(i + 1, 1);

                esperar_soltura(i + 1);

                /* Gana humano */
                if (ganador(1))
                {
                    ganador_final = 1;
                    return;
                }

                /* Empate */
                if (tablero_lleno())
                    return;

                /* Jugada computadora */
                jugada_computadora();

                /* Gana computadora */
                if (ganador(2))
                {
                    ganador_final = 2;
                    return;
                }

                /* Empate */
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
        /* B1 inicia la partida */
        esperar_B1();

        /* Obtener aleatorio mientras B1 esta presionado */
        turno = numero_aleatorio(2);

        /* Esperar que B1 sea soltado */
        soltar_B1();

        /* Nueva partida */
        limpiar_tablero();

        /* Computadora empieza */
        if (turno == 1)
        {
            jugada_computadora();

            if (ganador(2))
                ganador_final = 2;
        }

        /* Jugar */
        if (ganador_final == 0)
            jugar();

        /* Mostrar resultado */
        titilar_ganador();

        /* Esperar B1 para la siguiente partida */
    }

    return 0;
}