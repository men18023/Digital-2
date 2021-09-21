/* prueba sensor ultrasonico
 * File:   ultrasonico.c
 * Created on 30/08/2021
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
#include <pic16f887.h>

#define _XTAL_FREQ 20000000
#define Trigger RB1
#define Echo RB2
// VARIABLES
char temp, z, y;
char tk;
char dis;
// PROTOTIPOS FUNCIONES 
void setup(void);


//INTERRUPCIONES
void __interrupt() isr(void){
    
    }

// loop principal
void main(void){
    
    setup(); // config generales de I/O

    while (1) // loop
    {
        TMR1H = 0;
        TMR1L = 0;
        
        Trigger = 1;
        __delay_us(10);
        Trigger = 0;
        
        while(Echo==0){
            TMR1ON = 1;
        }
        while(Echo==1){
            TMR1ON = 0;
        }
        tk = (TMR1L | (TMR1H<<8));
        dis = (0.0272*tk)/2;
        
        tk = tk * 0.8;
        
         
     }
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0

    TRISB0 = 1;
    TRISB1 = 0;
    TRISB2 = 1;
    TRISB3 = 0;
    TRISD = 0;

    //LIMPIAR PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //RELOJ INTERNO
    //TIMER 1
    T1CON = 0x20;
    
    
}


