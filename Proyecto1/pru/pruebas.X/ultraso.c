/* Lab2 - LCD Y USART
 * File:   main.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: NO HAY
 * Enlace Github: https://github.com/men18023/Digital2
 * Created on 24/07/2021
 */

//******************************************************************************
//  BITS DE CONFIGURACION
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V     // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//  VARIABLES
//******************************************************************************
#define _XTAL_FREQ 4000000 
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
//#include "LCD.h"
//#define trigger RB0
//#define echo RB4
#include <pic16f887.h>

//******************************************************************************
//  VARIABLES
//******************************************************************************
char BUFFER;
float V1;

//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************

void setup(void);


//******************************************************************************
// INTERRPCIONES
//******************************************************************************
void __interrupt() ISR(){

}
//******************************************************************************
//  CICLO PRINCIPAL
//******************************************************************************
void main(void) {  
    int a;
    setup();
    configLCD();
    clear_LCD();
    while(1){
        TMR1H = 0;
        TMR1L = 0;
        trigger = 1;
        __delay_us(10);
        trigger = 0;
        while(!echo);
        TMR1ON = 1;
        while(echo);
        TMR1ON = 0;
        a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
        a = a/58.82;              //Converts Time to Distance
        a = a + 1;                //Distance Calibration
        if(a>=2 && a<=400){
        clear_LCD();
        cursor(1,1);
        write_string("DISTANCIA = "); //LCD
       
        cursor(1,14);
        write_char(a%10 +48);   //LCD
        
        a = a/10;
        cursor(1,13);
        write_char(a%10 +48);
        
        a = a/10;
        cursor(1,12);
        write_char(a%10 +48);

        }
        __delay_ms(100);
    }
}


//******************************************************************************
//  CONFIG PRINCIPAL I/O
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;

    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB4 = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;    
    PORTE = 0;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;   //8MHz
    OSCCONbits.SCS = 1;
    //TIMER 1
    T1CON = 0x10;
    
    }

