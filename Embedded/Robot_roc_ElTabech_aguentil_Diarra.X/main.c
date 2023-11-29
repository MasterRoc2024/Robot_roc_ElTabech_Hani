#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h" 
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"

int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();

/****************************************************************************************************/
// Configuration des entrées sorties
/****************************************************************************************************/
InitIO();
InitPWM();
InitTimer23();
InitTimer1();
InitADC1();



LED_BLANCHE = 0;
LED_BLEUE = 0;
LED_ORANGE = 0;

unsigned int ADCValue0, ADCValue1, ADCValue2;
/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
   
    
    if (ADCIsConversionFinished() == 1)
    {
        ADCClearConversionFinishedFlag();
        unsigned int * result = ADCGetResult();
        float volts = ((float) result [0])* 3.3 / 4096 * 3.2;
        robotState.distanceTelemetreDroit = 34 / volts - 5;// erreur de 1.7cm
        volts = ((float) result [1])* 3.3 / 4096 * 3.2;
        robotState.distanceTelemetreCentre = 34 / volts - 5;// erreur de 2cm
        volts = ((float) result [2])* 3.3 / 4096 * 3.2;
        robotState.distanceTelemetreGauche = 34 / volts - 5;// erreur de 1cm
        
        if(robotState.distanceTelemetreDroit > 30)LED_ORANGE = 1;
        else LED_ORANGE = 0;
        if(robotState.distanceTelemetreCentre > 30)LED_BLEUE = 1;
        else LED_BLEUE = 0;
        if(robotState.distanceTelemetreGauche > 30)LED_BLANCHE = 1;
        else LED_BLANCHE = 0;
        
        int i = 0;
    }

    
    /*
    if(ADCIsConversionFinished()){
        ADCClearConversionFinishedFlag();
        unsigned int * result = ADCGetResult();
        ADCValue0 = result[0];
        ADCValue1 = result[1];
        ADCValue2 = result[2];
        if(ADCValue0 < 353)LED_ORANGE = 1;
        else LED_ORANGE = 0;
        if(ADCValue1 < 353)LED_BLEUE = 1;
        else LED_BLEUE = 0;
        if(ADCValue2 < 353)LED_BLANCHE = 1;
        else LED_BLANCHE = 0;
    }*/
    /*if (LED_BLANCHE == 1) LED_BLANCHE = 0;
    else LED_BLANCHE = 1;*/
}
}