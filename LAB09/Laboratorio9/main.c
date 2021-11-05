//**************************************
/*
 * Laboratorio 9 - TivaWare
 * Jonathan Menéndez 18023
 * main.c
 */
//**************************************
// Librerias
//**************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#define XTAL 16000000
//**************************************
// Variables Globales
//**************************************
int i = 0;
uint32_t ui32Period;
uint8_t test = 0;
//**************************************
// Prototipo de Funciones
//**************************************
void delay(uint32_t msec);
void dms(void);
void Timer0IntHandler(void);
//**************************************
// Código Principal
//**************************************
int main(void)
{
    // Configuración del oscilador externo, usando PLL y teniendo una frecuencia de 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Se habilita el reloj para el puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Se establecen como salidas el puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Se habilita el reloj para el temporizador
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Configuración del Timer 0 como temporizador períodico
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Se calcula el período para el temporizador (1 seg)
    ui32Period = (SysCtlClockGet()) / 2;
    // Establecer el periodo del temporizador
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    // Se habilita la interrupción por el TIMER0A
    IntEnable(INT_TIMER0A);
    // Se establece que exista la interrupción por Timeout
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Se habilitan las interrupciones Globales
    IntMasterEnable();
    // Se habilita el Timer
    TimerEnable(TIMER0_BASE, TIMER_A);
    //************************************
    // Loop Principal
    //************************************
    while (1)
    {
        test = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
                if(test==0){
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08); // tiempo en verde
                    delay(4000);
           //*****************************
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00); //parpadeo
                    delay(700);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                    delay(700);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                    delay(700);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                    delay(700);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                    delay(700);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x08);
                    delay(700);
         //  *****************************
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0A); //amarillo
                    delay(2500);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x02); //rojo
                    delay(3500);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
                    delay(10000);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 4);  //parapdeos azules
                    delay(500);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                    delay(500);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 4);
                    delay(500);
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
                    delay(10000);
                }


            }
    }

void delay(uint32_t msec)
{
    for (i = 0; i < msec; i++)
    {
        dms();
    }

}

void dms(void)
{
    SysTickDisable();
    SysTickPeriodSet(16000);
    SysTickEnable();

    while ((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0); //Pg. 138

}
//**************************************
// Handler de la interrupción del TIMER 0 - Recordar modificar el archivo tm4c123ght6pm_startup_css.c
//**************************************
void Timer0IntHandler(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Read the current state of the GPIO pin and
    // write back the opposite state

    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
}
