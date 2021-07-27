/* Lab1 - Interrupciones y uso de librerías
 * File:   main1.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: https://youtu.be/4Wh5qyne0No
 * Enlace Github: https://github.com/men18023/Digital2/tree/main/LAB01
 * Created on 18/07/2021
 */

//CONFIG 1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits (LP oscillator: Low-power crystal on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
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
#include "ADC.h" 
#include "7seg.h"
#define _XTAL_FREQ 4000000

// VARIABLES
int multi; //bandera para multiplexado
unsigned char contador; //valor para el ADRESH
char nib1;  //  nibble 1 del adresh
char nib2;  //  nibble 2 del adresh

// PROTOTIPOS FUNCIONES 
void setup(void); 
unsigned  char  swap_nib ( unsigned  char n);

//INTERRUPCIONES
void __interrupt() isr(void){
    // pushbuttons
    if (RBIF == 1){
        if (PORTBbits.RB0 == 0)
        {
            PORTC++; // Se suma si se presiona el pushbutton 1
        }                    
        if (PORTBbits.RB1 == 0)
        {
            PORTC-- ;// Se resta si se presiona el pushbutton 2 
        }                     
        INTCONbits.RBIF = 0;// Se limpia la bandera  
    }                      
    // TIMER0
    if (T0IF == 1) {   
        if (multi == 0) 
        {
            PORTEbits.RE1 = 0; //se apaga primer display
            PORTEbits.RE0 = 1; //se enciende segundo display
            PORTD = 0; //Se limpia
            PORTD = display(nib2); //valor de nibble que queremos en el display
            multi = 1; //se cambia al otro display
        }                           
        
        else if (multi == 1) // si la bandera tiene este valor se dan
        {                               // la siguientes instrucciones
            PORTEbits.RE0 = 0; //se enciende primer display
            PORTEbits.RE1 = 1; //se apaga segundo display
            PORTD = 0; // se limpia
            PORTD = display(nib1); //valor de nibble que queremos en el display  
            multi = 0; // se cambia al otro display
        }                             
        INTCONbits.T0IF = 0;// Se limpia la bandera
        TMR0 = 10; //valor inicial del tmr0 en 5ms
    }
    //ADC
    if (PIR1bits.ADIF == 1){
        if (ADCON0bits.CHS == 0) //se revisa canal 0
        {
            contador = valor_ADC();// Se guarda el valor de ADRESH .
        }                    
        __delay_us(50); //delay de cambio de canal
        PIR1bits.ADIF = 0; // se apaga la bandera adc
    }
}
// loop principal
void main(void){
    
    setup(); // config generales de I/O
    while (1) // loop
    {
        ADCON0bits.GO = 1;
        nib1 = contador & 0x0F;
        nib2 = swap_nib(contador)& 0x0F;
        if (PORTC < valor_ADC())
        {
            PORTBbits.RB7 = 1;
        }
        else
            PORTBbits.RB7 = 0;
    }              
}
// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0b00000001; // RA0 como entrada analogica
    ANSELH = 0X00; 
    // PUERTOS I/O
    TRISAbits.TRISA0 = 1; // entrada para el pot
    TRISC = 0X00;
    TRISD = 0X00;        
    TRISE = 0X00; // salidas de 7seg y leds
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB7 = 0; // pushbuttons y led de alarma
    // se limpian los puertos
    PORTA = 0X00;
    PORTC = 0X00;
    PORTD = 0X00;
    PORTB = 0X00;
    PORTE = 0X00;
    // RELOJ INTERNO 
    OSCCONbits.IRCF2 = 0; //frec  de 250khz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;   
    //IIMER0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    //PULL UPS
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    //ADC
    conADC();
    //INTERRUPCIONES
    PIE1bits.ADIE = 1;  //ADC   
    PIR1bits.ADIF = 0;     
    INTCONbits.GIE = 1;   //GLOBALES
    INTCONbits.PEIE = 1;  //PERIFERICAS 

    IOCBbits.IOCB0 = 1;  // interupciones de ambos push
    IOCBbits.IOCB1 = 1;   
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;    // limpiar bandera
}
unsigned  char  swap_nib ( unsigned  char n)
{
    unsigned  char num; //rotacion para el valor del nibble y contador
    num = (((n &  0x0F ) <<  4)  | ((n &  0xF0 ) >>  4));
    return num;
}