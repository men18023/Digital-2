/* Lab3 - SPI
 * File:   mainM.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: 
 * Enlace Github: 
 * Created on 31/07/2021
 */

//CONFIG 1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (LP oscillator: Low-power crystal on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF          // Code Protection bit (Program memory code protection is enabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code protection is enabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = OFF       // Flash Program Memory Self Write Enable bits (0000h to 0FFFh write protected, 1000h to 1FFFh may be modified by EECON control)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XTAL_FREQ 4000000
#include "SPI.h"

// VARIABLES
uint8_t V1, V2;
char BUFFER[30];

// PROTOTIPOS FUNCIONES 
void setup(void);
void putch(char dato);
void division(char dividendo);
void msj1(void);
void msj2(void);


//INTERRUPCIONES
/*void __interrupt() isr(void){
    // pushbuttons
    
    }
*/

// loop principal
void main(void){
    
    setup(); // config generales de I/O
    while (1) // loop
    {
        PORTCbits.RC2 = 0;
        __delay_ms(1);
        
        spiWrite(0x0A);
        V1 = spiRead();
        __delay_ms(1);
        
        spiWrite(0x0A);
        V2 = spiRead();
        __delay_ms(1);
        
        PORTCbits.RC2 = 1;
        
    }
    return;              
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0
    TRISC1 = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    //LIMPIAR PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //RELOJ INTERNO
    OSCCONbits.IRCF0 = 0; // 4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    //SPI
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    return;
}
