#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h" 
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"

unsigned char stateRobot;
float speed = 10;

void OperatingSystemLoop(void)
{
    switch (stateRobot)
    {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
            stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(speed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(speed, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(speed, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(speed, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(speed/2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-speed/2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-speed/2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(speed/2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default :
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot=0;

void SetNextRobotStateInAutomaticMode()
{
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < 30 &&
        robotState.distanceTelemetreCentre > 20 &&
        robotState.distanceTelemetreGauche > 30) //Obstacle à droite
                positionObstacle = OBSTACLE_A_DROITE;
    else if(robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
                positionObstacle = OBSTACLE_A_GAUCHE;
    else if(robotState.distanceTelemetreCentre < 20) //Obstacle en face
                positionObstacle = OBSTACLE_EN_FACE;
    else if(robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
                positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}

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
InitTimer4();
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
