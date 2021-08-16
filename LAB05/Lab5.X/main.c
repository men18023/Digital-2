/* Lab05 - AdafruitIO
 * File:   main.c
 * Author: Jonathan Menendez, 18023
 * Enlace Video: 
 * Enlace Github: https://github.com/men18023/Digital-2/tree/main/LAB05
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
#include <string.h>
#include <stdlib.h>
#define _XTAL_FREQ 4000000

// VARIABLES
int cont;
char seleccionar;
char valor, d, c, u , residuo;
char dd, cc, uu;
char vv, con1;
char completo;

// PROTOTIPOS DE FUNCIONES
void setup(void);
void mensaje(void);
char division(char valor);

//INTERRUPCIONES
void __interrupt() isr(void){
    //PB
    if (RBIF == 1)
    {
        if (PORTBbits.RB0 == 0) //Se prueba el PB de subir
        {
            PORTD = PORTD + 1;  //se aumenta el valor en contador leds
        }
        if  (PORTBbits.RB1 == 0) //se prueba el PB de bajar
        {
            PORTD = PORTD - 1;  //se disminuye el valor en contador leds
        }
        INTCONbits.RBIF = 0;
        }   
    }

// loop principal
void main(void) {
    setup();
    while(1){
        PORTD = cont;
        PORTA = seleccionar;
        mensaje();
    }
    return;
}

// configuraciones generales
void setup(void){
    // PUERTOS ANALOGICOS
    ANSEL = 0;
    ANSELH = 0;
    // I/0
    TRISA = 0;
    //TRISB = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    //TRISC = 0;
    TRISD = 0;
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
    //CONFIG PULL-UPS
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;     //pull-ups para pines en puerto B
    IOCBbits.IOCB1 = 1;
    //INTERRUPCIONES
    INTCONbits.GIE = 1;  // globales
    INTCONbits.PEIE = 1; // perifericas
    INTCONbits.RBIE = 1;    //interrupcion del puerto B
    INTCONbits.RBIF = 0;
    PIE1bits.RCIE = 0;      // Interrupcion rx
    PIE1bits.TXIE = 0;      // Interrupcion TX
    //TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    
    SPBRG = 25;
    SPBRGH = 1;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    TXSTAbits.TX9 = 0; // 8 bits
    
    TXSTAbits.TXEN = 1;
    
    PIR1bits.RCIF = 0;  // Bandera rx
    PIR1bits.TXIF = 0;  // bandera tx
}

void putch(char dato){  //para uso en transmision
    while(TXIF == 0);
    TXREG = dato;
    return;
}
    
void mensaje(void){
    __delay_ms(250); //Tiempos para el despliegue de los caracteres
    division(cont);
    printf("Valor de contador:\r");
    __delay_ms(250);
    TXREG = d;
    __delay_ms(250);
    TXREG = c;
    __delay_ms(250);
    TXREG = u;
    __delay_ms(250);
    printf("\r");
    
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
        uu = RCREG -48;

       if(cc == 2 && dd == 5){
           while(RCREG > '5'){
               goto checkU;
           }
       }
      con1 = concatenado(cc, dd);
      completo = concatenado(con1, uu);
      __delay_ms(250);
      printf("\rEl valor seleccionado es: %d\r", completo);
      cont = completo;
      seleccionar = completo;
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

char division(char valor){ //proceso de division para displays
    d = valor/100; //centena = contador dividio 50
    residuo = valor%100; //variable utilizado como residuo
    c = residuo/10; //decena = residuo divido 10
    u = residuo%10; //se coloca el residuo en variable unidad para mostrar
    d += 48;
    c += 48;
    u += 48;
}