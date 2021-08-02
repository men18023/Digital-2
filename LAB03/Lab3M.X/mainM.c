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
char V1, V2;
char centena, decena, unidad, residuo;
char cc, dd, uu;
char temp;
int completo;



// PROTOTIPOS FUNCIONES 
void setup(void);
void ReadSlave(void);
char division (char valor);
void msj1(void);
void msj2(void);
void ingreso(void);

//INTERRUPCIONES
/*void __interrupt() isr(void){
    
    }
*/
// loop principal
void main(void){
    
    setup(); // config generales de I/O
    
    while (1) // loop
    {
       PORTCbits.RC2 = 0;//Slave delect
       __delay_ms(1);
       
       spiWrite(0x0A);
       V1 = spiRead();
       __delay_ms(1);
       spiWrite(0x0A);
       V2 = spiRead();
       __delay_ms(1);
       
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;//Slave deselect 
       msj1();
       msj2();
       ingreso();
       
       PORTB = completo;
        /*sprintf(BFFR1, "%d", cc);
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
        PORTD = variable; //    */             
}
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0
    TRISC2 = 0;
    TRISB = 0;
    //LIMPIAR PUERTOS
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTCbits.RC2 = 1;
    //SPI
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE,
            SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //RELOJ INTERNO
    OSCCONbits.IRCF0 = 0;//reloj  4mhz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.SCS = 1;  
    //USART
    configUSART();
}

void putch(char dato){  //para uso en transmision
    while(TXIF == 0);
    TXREG = dato;
    return;
}

char division(char valor){ //proceso de division para displays
    centena = valor/50; //centena = contador dividio 50
    residuo = valor%100; //variable utilizado como residuo
    decena = residuo/10; //decena = residuo divido 10
    unidad = residuo%10; //se coloca el residuo en variable unidad para mostrar
    centena += 48;
    decena += 48;
    unidad += 48;
}


void msj1 (void){
    division(V1);
    __delay_ms(100); //Tiempos para el despliegue de los caracteres
    printf("\rValor en el Pot 1:\r");
    __delay_ms(100);
    printf("  ");
    __delay_ms(100);
    TXREG = centena;
    __delay_ms(100);
    TXREG = 46;
    __delay_ms(100);
    TXREG = decena;
    __delay_ms(100);
    TXREG = unidad;
    __delay_ms(100);
    printf(" V");
    __delay_ms(100);
    printf("\r");
}
    
void msj2 (void){
    division(V2);
    __delay_ms(100); 
    printf("\rValor en el Pot 2:\r");
    __delay_ms(100);
    printf("  ");
    __delay_ms(100);
    TXREG = centena;
    __delay_ms(100);
    TXREG = 46;
    __delay_ms(100);
    TXREG = decena;
    __delay_ms(100);
    TXREG = unidad;
    __delay_ms(100);
    printf(" V");
    __delay_ms(100);
    printf("\r");
}

void ingreso(void){
    printf("\rIngrese el valor de centena: (0-2)\r");
      checkC:  
       while(RCIF == 0);
        cc = RCREG -48;  

       while(RCREG > '2'){ 
           goto checkC;
       }
    
    printf("Ingrese el valor de decena: (0-5)\r");
      checkD:
        while(RCIF == 0); 
         dd = RCREG -48; 

        if(cc == 2){
           while(RCREG > '5'){
               goto checkD;
           }
       }

    printf("Ingrese el valor de unidad: (0-5)\r");
      checkU:
       while(RCIF == 0); 
        uu = RCREG - 48;

       if(cc == 2 && dd == 5){
           while(RCREG > '5'){
               goto checkU;
           }
       }
      temp = concatenado(cc, dd);
      completo = concatenado(temp, uu);
      __delay_ms(250);
    printf("\rEl valor seleccionado es: %d\r", completo);
}

int concatenado(int a, int b)
{   
    char BFFR1[20];
    char BFFR2[20];
    // Convertir int a strng
    sprintf(BFFR1, "%d", a);
    sprintf(BFFR2, "%d", b);
    // concatenar los strng
    strcat(BFFR1, BFFR2);
    // regrear a int
    int c = atoi(BFFR1);
    return c;
}
