/*
 * File:   Controles.c
 * Author: diego
 *
 * Created on 17 de octubre de 2021, 05:28 PM
 */

//-------------------------- Bits de configuraciÓn -----------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT            // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON                       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON                         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V                   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 8000000
#include <stdint.h>
#include <xc.h>
#include <stdio.h> 

//------------------------------ Variables ------------------------------------- 
// Variable para las rutinas de bit-banging
char    val0;
char    val1;
char    val2;
char    val3;

//----------------------------- Prototipos ------------------------------------- 
//Se definen todas las funciones antes de utilizarlas
void setup(void);                       // Funcion de configuración principal.                     
void bitb1(void);                       // Funciones para el funcionamiento -
void bitb2(void);                       // del bit-banging para la señal del
void bitb3(void);                       // tercer servo.
void putch(char data);                  // Función especial para printf
//void comandos(void);

//--------------------------- Interrupciones -----------------------------------
void __interrupt() isr(void){ 
    
    // Interrupción para el funcionamiento de los servos
    if(PIR1bits.ADIF == 1)
       {
        if(ADCON0bits.CHS == 0){
            val0 = ADRESH;
            if (val0 >=200){
                PORTCbits.RC0 = 1;
            }
            else if (val0<=50){
                PORTCbits.RC1 = 1;            
            }
            else
                PORTCbits.RC0 = 0;
                PORTCbits.RC1 = 0;
        }    
        else if(ADCON0bits.CHS == 1){ 
            val1 = ADRESH;
            if (val1 >=200){
                PORTCbits.RC2 = 1;
            }
            else if (val1<=50){
                PORTCbits.RC3 = 1;            
            }
            else
                PORTCbits.RC2 = 0;
                PORTCbits.RC3 = 0;
        }
        else if(ADCON0bits.CHS == 2){ 
            val2 = ADRESH;
            if (val2 >=200){
                PORTCbits.RC4 = 1;
            }
            else if (val2<=50){
                PORTCbits.RC5 = 1;            
            }
            else
                PORTCbits.RC4 = 0;
                PORTCbits.RC5 = 0;
        }  
        else if(ADCON0bits.CHS == 3){ 
            val3 = ADRESH;
            if (val3 >=200){
                PORTCbits.RC6 = 1;
            }
            else if (val3<=50){
                PORTCbits.RC7 = 1;            
            }
            else
                PORTCbits.RC6 = 0;
                PORTCbits.RC7 = 0;
        }
           PIR1bits.ADIF = 0; 
}
}
// ---------------------------------- MAIN -------------------------------------
void main(void) 
{
    setup();                                    // Llamo a mi configuracion principal
    ADCON0bits.GO = 1;                          // Bit para que comience la conversion
    // Loop principal
    while(1)  
    {
        if (ADCON0bits.GO == 0)
        {
            if(ADCON0bits.CHS == 0){            // Rutina para el camio de canales analógicos
                ADCON0bits.CHS =1;
            }
            else if (ADCON0bits.CHS == 1)      // Cambio del 9 al 11 y luego al 13
            {
                ADCON0bits.CHS = 2;
            }
            else if (ADCON0bits.CHS == 2)      // Cambio del 9 al 11 y luego al 13
            {
                ADCON0bits.CHS = 3;
            }
            else
                ADCON0bits.CHS = 0;
            __delay_us(50);
            ADCON0bits.GO = 1;
        }   
    }
}

//----------------------------- SUB-RUTINAS ------------------------------------
void setup(void){
    // Se configura el oscilador
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;                   // Se configura a 8MHz
    OSCCONbits.SCS = 1;
    
    // Configuraciones de puertos digitales
    ANSEL = 0b00001111;
    ANSELH = 0;                    // Pines analógicos de PORTB
    
    // Configurar bits de salida o entradaas
    // Inputs para los joysticks
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;

    
    // Outputs para los LEDS en el puerto C
    TRISC = 0xFF;
    
    // Se limpian los puertos
    PORTA = 0x00;
    PORTC = 0x00;
    
    // Configuracion del ADC    
    ADCON1bits.ADFM = 0;                    // Justificado a la izquierda
    ADCON1bits.VCFG0 = 0;                   // Vref en VSS y VDD 
    ADCON1bits.VCFG1 = 0;   
    
    ADCON0bits.ADCS = 0b10;                 // Se configura el oscilador interno FOSC/32
    ADCON0bits.ADON = 1;                    // Activar el ADC
    ADCON0bits.CHS = 0;                     // Canal 0
    __delay_us(50);

    // Configuacion de las interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;                    // Periferical interrupt
    PIE1bits.ADIE = 1;                      // Activar la interrupcion del ADC
    PIR1bits.ADIF = 0;                      // Bandera del ADC
    
    // Configuraciones TX y RX
//    TXSTAbits.SYNC = 0;                     // Apaga SYNC
//    TXSTAbits.BRGH = 1;                     // Prende BRGH
//    BAUDCTLbits.BRG16 = 1;                  // Prende BRG16
//    
//    SPBRG = 208;
//    SPBRGH = 0;
//    
//    RCSTAbits.SPEN = 1;                     // Prende SPEN
//    RCSTAbits.RX9 = 0;                      // Apaga RX9
//    RCSTAbits.CREN = 1;                     // Prende CREN
//    
//    TXSTAbits.TXEN = 1;                     // Prende TXTEN
//    
//    PIR1bits.RCIF = 0;                      // Bandera rx
//    PIR1bits.TXIF = 0;                      // bandera tx

}
