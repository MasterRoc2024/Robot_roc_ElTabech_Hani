/* 
 * File:   PWM.h
 * Author: TP_EO_6
 *
 * Created on 23 octobre 2023, 11:55
 */

#ifndef PWM_H
#define	PWM_H

#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void InitPWM(void);
void PWMSetSpeed(float, int);

#endif	/* PWM_H */

