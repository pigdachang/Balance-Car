#ifndef __MOTOR_H
#define __MOTOR_H


void Motor_Init(void);
void Motor_SetPWM(int16_t PWM);
uint16_t Motor_PID(float Angle,float RotateX);

#endif
