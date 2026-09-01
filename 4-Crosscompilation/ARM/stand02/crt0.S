.syntax unified
.cpu cortex-m4
.thumb

.global g_pfnVectors
.global Reset_Handler

/* Default empty handler */
.equ RAM_STACK_TOP, stack_top /* Adjust based on SRAM size (128KB for STM32F405) */

.section .isr_vector,"a",%progbits
g_pfnVectors:
    .word RAM_STACK_TOP        /* Initial Stack Pointer */
    .word Reset_Handler        /* Reset Handler */
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0                    /* Reserved */
    .word 0                    /* Reserved */
    .word 0                    /* Reserved */
    .word 0                    /* Reserved */
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0                    /* Reserved */
    .word PendSV_Handler
    .word SysTick_Handler

.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Copy data segment from flash to RAM */
    LDR R0, =_sdata
    LDR R1, =_edata
    LDR R2, =_sidata
    B LoopCopyDataInit

CopyDataInit:
    LDR R3, [R2], #4
    STR R3, [R0], #4

LoopCopyDataInit:
    CMP R0, R1
    BLT CopyDataInit

    /* Zero fill the bss segment */
    LDR R0, =_sbss
    LDR R1, =_ebss
    MOV R2, #0
    B LoopFillZerobss

FillZerobss:
    STR R2, [R0], #4

LoopFillZerobss:
ToFillZerobss:
    CMP R0, R1
    BLT FillZerobss


    /* ========================================================= */
    /* INICIO DEL LAB EXERCISE 2 (Reemplaza el antiguo BKPT)    */
    /* ========================================================= */
    
    /* 1. Inicializar los 1000 elementos desde el 75 (X=7, Y=5) */
    LDR R0, =mi_arreglo    /* R0 apunta al inicio de los 1000 bytes en el BSS */
    MOV R1, #1000          /* R1 es el contador para los 1000 elementos */
    MOV R2, #75            /* R2 arranca en el número XY (75) */

loop_init_lab:
    STRB R2, [R0], #1      /* Guarda el byte (R2) en la RAM (R0), y avanza R0 1 byte */
    ADD R2, R2, #1         /* Incrementa el valor consecutivo (75, 76, 77...) */
    SUBS R1, R1, #1        /* Resta 1 al contador actualizando banderas */
    BNE loop_init_lab      /* Si R1 no es cero, repite el bucle */

    /* 2. Ejecutar la función de cálculos (Máximo, Mínimo, Promedio, Mediana) */
    BL calcular_todo


_infinite_loop:
    B _infinite_loop


/* ========================================================= */
/* FUNCIONES DE CÁLCULO (Máximo, Mínimo, Promedio y Mediana) */
/* ========================================================= */
calcular_todo:
    PUSH {R4-R8, LR}       /* Guardamos los registros en el Stack */

    /* --- PARTE A: MÁXIMO Y MÍNIMO --- */
    LDR R0, =mi_arreglo    /* R0 apunta al arreglo */
    MOV R1, #1000          /* Contador de elementos */
    LDRB R2, [R0], #1      /* R2 = Mínimo temporal (1er elemento) */
    MOV R3, R2             /* R3 = Máximo temporal (1er elemento) */
    SUBS R1, R1, #1        /* Quedan 999 por evaluar */

loop_min_max:
    LDRB R4, [R0], #1      /* Lee el siguiente byte */
    CMP R4, R2             
    BLT nuevo_min          /* Si es menor, actualiza mínimo */
    CMP R4, R3             
    BGT nuevo_max          /* Si es mayor, actualiza máximo */
    B siguiente_min_max

nuevo_min:
    MOV R2, R4             
    B siguiente_min_max

nuevo_max:
    MOV R3, R4             

siguiente_min_max:
    SUBS R1, R1, #1
    BNE loop_min_max
    /* Al terminar: R2 tiene el Mínimo, R3 tiene el Máximo */


    /* --- PARTE B: PROMEDIO --- */
    LDR R0, =mi_arreglo    
    MOV R1, #1000          
    MOV R5, #0             /* Acumulador de la suma total */

loop_suma:
    LDRB R4, [R0], #1      
    ADD R5, R5, R4         
    SUBS R1, R1, #1
    BNE loop_suma
    
    MOV R1, #1000
    UDIV R6, R5, R1        /* R6 = Promedio exacto usando el Cortex-M4 */


    /* --- PARTE C: MEDIANA --- */
    LDR R0, =mi_arreglo    
    LDRB R7, [R0, #499]    /* Elemento en el índice central 499 */
    LDRB R8, [R0, #500]    /* Elemento en el índice central 500 */
    ADD R7, R7, R8         
    MOV R1, #2
    UDIV R7, R7, R1        /* R7 = Mediana */

    POP {R4-R8, PC}        /* Restauramos y retornamos */


/* Provide weak aliases for unhandled exceptions */
.weak NMI_Handler
.weak HardFault_Handler
.weak MemManage_Handler
.weak BusFault_Handler
.weak UsageFault_Handler
.weak SVC_Handler
.weak DebugMon_Handler
.weak PendSV_Handler
.weak SysTick_Handler

NMI_Handler:
HardFault_Handler:
MemManage_Handler:
BusFault_Handler:
UsageFault_Handler:
SVC_Handler:
DebugMon_Handler:
PendSV_Handler:
SysTick_Handler:
    B .

/* ========================================================= */
/* ZONA DE DATOS Y MEMORIA BSS                               */
/* ========================================================= */

    .data
v1: .word   0x11,0x22,0x33,0x44                            
v2: .word   0x11111111,0x22222222,0x33333333,0x44444444   

    .lcomm b1,4                                         
    .lcomm b2,16                                        

    /* Arreglo de 1000 bytes para el laboratorio (Lab Exercise 2) */
    .lcomm mi_arreglo, 1000