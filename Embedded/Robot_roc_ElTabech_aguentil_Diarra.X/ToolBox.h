/* 
 * File:   ToolBox.h
 * Author: TP_EO_6
 *
 * Created on 23 octobre 2023, 11:47
 */

#ifndef TOOLBOX_H
#define	TOOLBOX_H

#define PI 3.141592653589793

float Abs(float value);
float Max(float value, float value2);
float Min(float value, float value2);
float LimitToInterval(float value, float lowLimit, float highLimit);
float RadianToDegree(float value);
float DegreeToRadian(float value);

#endif	/* TOOLBOX_H */

