/* Lab3 - SPI
 * File:   mainS.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: https://youtu.be/oTNTV9lghbw
 * Enlace Github: https://github.com/men18023/Digital-2/tree/main/LAB03
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
#include "SPI.h"
#include "ADC.h"
#define _XTAL_FREQ 4000000

// VARIABLES
int ADC1, ADC2;
int T1, T2;
int contador;

// PROTOTIPOS FUNCIONES 
void setup(void);
void putch(char data);

//INTERRUPCIONES
void __interrupt() isr(void){
    //SPI
        if(SSPIF == 1){
            T1 = spiRead();
            spiWrite(ADC2);
            T2 = spiRead();
            spiWrite(ADC1);
            SSPIF = 0;
    }
    //ADC
    if(PIR1bits.ADIF == 1) // bandera del ADC
       {
           if(ADCON0bits.CHS == 0) { // revisa canal 0
               ADC1 = ADRESH;
               ADCON0bits.CHS = 1;
           }
           else if(ADCON0bits.CHS == 1){
            ADCON0bits.CHS = 0;
               ADC2 = ADRESH;      
            }
           PIR1bits.ADIF = 0;// Baja la bandera
       }
       
}

// loop principal
void main(void){
    
    setup(); // config generales de I/O
    while (1) // loop
    {
     if (ADCON0bits.GO == 0){ //encender el ADC
         __delay_us(100);      
         ADCON0bits.GO = 1;
    }
  }
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0b00000011;
    ANSELH = 0;
    //I/O
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA5 = 1;// Slave select
    TRISB = 0;
    TRISD = 0;
    //LIMPIAR PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    //RELOJ INTERNO
    OSCCONbits.IRCF0 = 0; //4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    //INTERRUPCIONES
    INTCONbits.GIE = 1;  // globales
    INTCONbits.PEIE = 1; // perifericas
    PIR1bits.SSPIF = 0; // banderas
    PIE1bits.SSPIE = 1; // Habilitamos int MSSP
    //SPI
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW,
            SPI_IDLE_2_ACTIVE);
    //ADC
    configADC(2);   
}