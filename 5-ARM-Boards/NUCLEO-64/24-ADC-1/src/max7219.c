#include "max7219.h"
#include "spi.h"
#include "gpio_config.h"

uint8_t init_data[5][2] = {
    {0x9,0x00},
    {0xA,0x0f},
    {0xB,0x07},
    {0xC,0x01},
    {0xF,0x00}
};

void max7219_config(void){
    WRITE_REG_FIELD(RCC->APB1ENR,RCC_APB1ENR_SPI2EN,1);
    volatile unsigned int dummy;
    dummy =  RCC->APB1ENR;
    dummy =  RCC->APB1ENR;
    SPI_GPIO_Config(SPI2);
    SPI_Master_Init(SPI2);
    SPI_Enable(SPI2,1);
    write_pin_state(GPIOB,GPIO_ODR_OD9,1);
    
    for(int8_t i=0; i<5; i++){
        write_pin_state(GPIOB,GPIO_ODR_OD9,0);
        SPI_Master_Tranfer(SPI2, init_data[i],2);
        write_pin_state(GPIOB,GPIO_ODR_OD9,1);
    }

}


void max7219_print(uint8_t * Matrix){
    uint8_t row[2];
    for(uint8_t i=0; i<8; i++){
        row[0] = i+1;
        row[1] = Matrix[i];
        write_pin_state(GPIOB,GPIO_ODR_OD9,0);
        SPI_Master_Tranfer(SPI2, row,2);
        write_pin_state(GPIOB,GPIO_ODR_OD9,1);
    }
}