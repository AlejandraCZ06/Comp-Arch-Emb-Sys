#include "gpio_config.h"
#include "stdint.h"

uint8_t tablero[9] = {0};

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
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    uint8_t i;

    for (i = 0; i < 8; i++)
        if (tablero[c[i][0]] == jugador &&
            tablero[c[i][1]] == jugador &&
            tablero[c[i][2]] == jugador)
            return 1;

    return 0;
}

uint8_t tablero_lleno(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
        if (tablero[i] == 0)
            return 0;

    return 1;
}

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

void esperar_soltura(uint8_t boton)
{
    while (boton_presionado(boton));
}

void esperar_B1(void)
{
    while (!boton_start());
}

void soltar_B1(void)
{
    while (boton_start());
}

uint8_t casilla_libre(void)
{
    uint8_t i;

    for (i = 0; i < 9; i++)
        if (tablero[i] == 0)
            return i;

    return 9;
}

void jugada_computadora(void)
{
    uint8_t casilla = casilla_libre();

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

                tablero[i] = 1;

                /* ENCENDER ANTES DE COMPROBAR */
                led_azul(i + 1, 1);

                esperar_soltura(i + 1);

                if (ganador(1) || tablero_lleno())
                    return;

                jugada_computadora();

                if (ganador(2) || tablero_lleno())
                    return;
            }
        }
    }
}

int main(void)
{
    uint8_t turno;

    GPIO_Config();
    iniciar_aleatorio();

    esperar_B1();
    soltar_B1();

    while (1)
    {
        limpiar_tablero();

        turno = DWT_CYCCNT & 1;

        if (turno == 1)
            jugada_computadora();

        jugar();

        esperar_B1();
        soltar_B1();
    }

    return 0;
}