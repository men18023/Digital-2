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
#include "LCD.h"
#include "ADC.h"
#include "USART.h"

//******************************************************************************
//  VARIABLES
//******************************************************************************
uint8_t ADC0, ADC1;
uint8_t contador = 0;
char readUSART = 0;
char entregado = 0;
float V1,V2;
char BUFFER[20];

//******************************************************************************
//  PROTOTIPO DE FUNCIONES
//******************************************************************************

void setup(void);
float conversion(uint8_t x);
void Enviar_1(void);
void Enviar_2(void);

//******************************************************************************
// INTERRPCIONES
//******************************************************************************
void __interrupt() ISR(){
    if (RCIF == 1){
        RCIF = 0;
        readUSART = Read_USART();
    if (readUSART == '+'){
        contador++;
    }
    else if (readUSART == '-'){
        contador--;
    }    
  }
}
//******************************************************************************
//  CICLO PRINCIPAL
//******************************************************************************
void main(void) {       
    setup();
    _baudios();
    config_tx();
    config_rc();
    configLCD();
    clear_LCD();
    while(1){
        clear_LCD();
        cursor(1,1);
        write_string("V1   V2   CONT"); //LCD
        
        ADC0 = configADC(0);
        ADC1 = configADC(1);
        
        V1 = ADC0*0.0196;
        V2 = ADC1*0.0196;
        //sprintf(BUFFER, "%2.1f   %2.1f   %d" , V1,V2,contador);
        sprintf(BUFFER, "%2.1f  %2.1f  %d" , V1,V2, contador);
       
        
        cursor(2,1);
        write_string(BUFFER);   //LCD

        Write_USART_String(BUFFER);
        Write_USART(13);
        Write_USART(10);
        Enviar_1();
        Enviar_2();
        
        if (RCIF == 1){
            entregado = RCREG;
            if(entregado == '+'){contador = contador +1;}
            if(entregado == '-'){contador = contador -1;}
        }
        __delay_ms(500);
    }

}

//******************************************************************************
//  CONFIG PRINCIPAL I/O
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    PORTD = 0;
    PORTE = 0;
    PORTC = 0;
    TRISA = 0;
    
    TRISD = 0;
    TRISE = 0;

    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
    INTCONbits.GIE = 1;
    }

void Enviar_1(void){//Envio de datos
    TXREG = V1;
    while (TXSTAbits.TRMT == 1){//Retorna y envia el voltaje a ADC1
        return;
    }
}
void Enviar_2(void){//Envio de datos
    TXREG = V2;
    while (TXSTAbits.TRMT == 1){//Retorna y envia el voltaje a ADC2
        return;
    }
}