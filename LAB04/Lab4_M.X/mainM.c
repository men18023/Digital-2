/* Lab04 - I2C
 * File:   mainM.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: 
 * Enlace Github: https://github.com/men18023/Digital-2/tree/main/LAB04
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
#include "I2C.h"
//#include "LCD.h"
#define _XTAL_FREQ 4000000

// VARIABLES
//uint8_t ADC0, ADC1;
//uint8_t contador;
//char BUFFER[30];
// valor


// PROTOTIPOS FUNCIONES 
void setup(void);


//INTERRUPCIONES
void __interrupt() isr(void){
    
    }

// loop principal
void main(void){
    
    setup(); // config generales de I/O
    //configLCD();
    //clear_LCD();
    while (1) // loop
    {
         // slave 1
        I2C_Master_Start();
        I2C_Master_Write(0x51); 
        PORTB = I2C_Master_Read(0); //mando a los leds
        I2C_Master_Stop();
        __delay_ms(200);
        //Osensor de temperatura
        I2C_Master_Start();
        I2C_Master_Write(0x80); //seleccionar el sensor
        I2C_Master_Write(0xF3); //leer la temp
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x81); //read
        PORTD = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);
                 
}
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0
    //TRISA = 0;
    TRISB = 0;
    PORTB = 0;
    TRISD = 0;
    //TRISC = 0;
    //TRISD = 0;
    //TRISE = 0;
    //LIMPIAR PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    //RELOJ INTERNO
    OSCCONbits.IRCF0 = 0;//reloj  4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    //I2C
    I2C_Master_Init(100000);
}


