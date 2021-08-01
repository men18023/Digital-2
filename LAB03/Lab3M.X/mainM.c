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
#include <pic16f887.h>
#define _XTAL_FREQ 4000000
#include "SPI.h"
#include "USART.h"

// VARIABLES
uint8_t V1, V2;
uint8_t centena;
uint8_t decena;
uint8_t unidad;
uint8_t cc, dd, uu;
char BFFR1[10];
char BFFR2[10];
char BFFR3[10];
int variable;

// PROTOTIPOS FUNCIONES 
void setup(void);
void putch(char dato);
void division(char valor);
void msj1(void);
void msj2(void);
void checkC(void);
void checkD(void);
void checkU(void);

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
        PORTCbits.RC2 = 0; //slave select
        __delay_ms(1);
        
        spiWrite(0x0A);
        V1 = spiRead();
        __delay_ms(1);
        
        spiWrite(0x0A);
        V2 = spiRead();
        __delay_ms(1);
        
        PORTCbits.RC2 = 1; //slave deselect
        __delay_ms(50);
        
        division(V1);
        msj1();
        __delay_ms(50);
        division(V2);
        msj2();
        __delay_ms(50);
        
        printf("Ingrese el valor de centena\r");
        while(RCIF == 0);
        cc = RCREG -48;
        while(RCREG > '2'){
            checkC();
        }
        
        printf("Ingrese el valor de decena\r");
        while(RCIF == 0);
        dd = RCREG -48;
        if (cc == 2){
            while(RCREG > '5'){
                checkD();
            }
        }
        
        printf("Ingrese el valor de unidad\r");
        while(RCIF == 0);
        uu = RCREG -48;
        if (cc == 2 && dd == 5){
            while(RCREG > '5'){
                checkU();
            }
        }
        
        sprintf(BFFR1, "%d", cc);
        sprintf(BFFR2, "%d", dd);
        sprintf(BFFR3, "%d", uu);
        strcat(BFFR1, BFFR2);
        strcat(BFFR1, BFFR3);
        variable = atoi(BFFR1);
        division(variable);
        __delay_ms(100);
        TXREG = centena;
        __delay_ms(100);
        TXREG = decena;
        __delay_ms(100);
        TXREG = unidad;
         __delay_ms(100);
        PORTD = variable; //    
    }
    return;              
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0
    TRISB = 0;
    TRISD = 0;
    //PORTCbits.RC2 = 1;
    //PORTCbits.RC1 = 1;
    TRISCbits.TRISC2 = 0;
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

void division(char valor){ //proceso de division para displays
    centena = valor/100; //centena = contador dividio 100
    unidad = valor%100;  //variable de unidad es utilizado como residuo
    decena = unidad/10; //decena = residuo(unidad) divido 10
    unidad = unidad%10; //se coloca el residuo en variable unidad para mostrar
    centena += 48;
    decena += 48;
    unidad += 48;
    return;
}

void putch(char dato){  //para uso en transmision
    while(TXIF == 0);
    TXREG = dato;
    return;
}

void msj1(void){
    printf("\r V1 = ");
    __delay_ms(50);
    TXREG = centena;
    __delay_ms(50);
    printf(".");
    __delay_ms(50);
    TXREG = decena;
    __delay_ms(50);
    TXREG = unidad;
    __delay_ms(50);
    printf("\r\r");
    
    return;
}

void msj2(void){
    printf("\rV2 = ");
    __delay_ms(50);
    TXREG = centena;
    __delay_ms(50);
    printf(".");
    __delay_ms(50);
    TXREG = decena;
    __delay_ms(50);
    TXREG = unidad;
    __delay_ms(50);
    printf("\r\r");
    
    return;
}

void checkC(void){
    if(RCREG > 2){
           printf("Elija un valor entre 0 y 2\r");   
       }
       while(RCIF == 0);
       cc = RCREG -48;
}

void checkD(void){
    if(RCREG > 5){
           printf("Elija un valor menor o igual a 5\r");   
       }
       while(RCIF == 0);
       dd = RCREG -48;
}

void checkU(void){
    if(RCREG > 5){
           printf("Elija un valor menor o igual a 5\r");   
       }
       while(RCIF == 0);
       uu = RCREG -48;
}