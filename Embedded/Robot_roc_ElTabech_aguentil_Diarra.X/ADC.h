/* 
 * File:   ADC.h
 * Author: TP_EO_6
 *
 * Created on 29 novembre 2023, 09:44
 */

#ifndef ADC_H
#define	ADC_H

void InitADC1(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned char ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);

#endif	/* ADC_H */

