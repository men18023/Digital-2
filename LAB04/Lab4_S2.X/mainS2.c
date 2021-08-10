/* Lab4 - 12C
 * File:   mainS1.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: https://youtu.be/yjBZK1R30_I
 * Enlace Github: https://github.com/men18023/Digital-2/tree/main/LAB04
 * Created on 07/08/2021
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
#include "I2C.h"
#define _XTAL_FREQ 4000000

// VARIABLES
//uint8_t contador;
uint8_t c;


// PROTOTIPOS FUNCIONES 
void setup(void);

//INTERRUPCIONES
void __interrupt() isr(void){
    //PB
    if (RBIF == 1)
    {
        if (PORTBbits.RB0 == 0) //Se prueba el PB de subir
        {
            PORTA = (PORTA + 1) & 0x0F;  //se aumenta el valor en contador leds
        }
        if  (PORTBbits.RB1 == 0) //se prueba el PB de bajar
        {
            PORTA = (PORTA - 1) & 0x0F;  //se disminuye el valor en contador leds
        }
        INTCONbits.RBIF = 0;
        }   
    //SPI
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            c = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;  // Clear the overflow flag
            SSPCONbits.WCOL = 0;    // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            c = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;    // Limpia bandera de interrupci?n recepci?n/transmisi?n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci?n se complete
            PORTA = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci?n
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            c = SSPBUF;
            BF = 0;
            SSPBUF = PORTA;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0; 
    }
} 
       
// loop principal
void main(void){
    
    setup(); // config generales de I/O
    while (1) // loop
    {
        
     //contador= PORTA; //se asigna el valor del contador de leds a variable
    
  }
    return;
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    //I/O
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISD = 0;
    TRISA = 0;
    //LIMPIAR PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //RELOJ INTERNO
    OSCCONbits.IRCF0 = 0; //4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    //INTERRUPCIONES
    INTCONbits.GIE = 1;  // globales
    INTCONbits.PEIE = 1; // perifericas
    INTCONbits.RBIE = 1;    //interrupcion del puerto B
    INTCONbits.RBIF = 0;
    //I2C
    I2C_Slave_Init(0x60);
    //CONFIG PULL-UPS
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;     //pull-ups para pines en puerto B
    IOCBbits.IOCB1 = 1; 
}
