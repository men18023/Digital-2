/*
 * File:   ADC.c
 * Author: jonam
 *
 * Created on 1 de agosto de 2021, 11:18 AM
 */

#include <xc.h> 
#include <stdint.h>  
#include "ADC.h"

#define _XTAL_FREQ 4000000

void configADC(char fr) {
    switch(fr){
        case 0: 
            ADCON0bits.CHS = 0;     //canal 0
            __delay_us(100);
    
            ADCON0bits.ADCS0 = 0;   //para que el clock select sea FOSC/
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    //ADC enable bit
            ADCON1bits.ADFM = 0;    //left justified
            ADCON1bits.VCFG1 = 0;   //5 voltios
            ADCON1bits.VCFG0 = 0;   //tierra
            break;
        case 1: 
            ADCON0bits.CHS = 0;     //canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   //FOSC/8
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    //ADC enable
            ADCON1bits.ADFM = 0;    //justificado izq
            ADCON1bits.VCFG1 = 0;   //5V
            ADCON1bits.VCFG0 = 0;   
            break;
        case 2: 
            ADCON0bits.CHS = 0;     //canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 0;   //FOSC/32
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    //ADC enable 
            ADCON1bits.ADFM = 0;    //justificado izq
            ADCON1bits.VCFG1 = 0;   //5V
            ADCON1bits.VCFG0 = 0;   
            break;
        case 3: //FRC
            ADCON0bits.CHS = 0;     //canal 0
            __delay_us(100);

            ADCON0bits.ADCS0 = 1;   // FRC
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    //ADC enable 
            ADCON1bits.ADFM = 0;    //justificado izq
            ADCON1bits.VCFG1 = 0;   //5V
            ADCON1bits.VCFG0 = 0;   
            break;
    }
}
