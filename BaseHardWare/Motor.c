#include "stm32f10x.h"                  // Device header
#include "PWM.h"


#define KP          60.0f    	// 原来是 30，太大了
#define KD          5.0f   		// 原来是 1.0，稍微降一点配合新 KP
#define MAX_PWM     3599    	// 配合 ARR=3600
#define DEAD_ZONE   10       	// 死区，小于此值电机不转

#define LEFT_1			GPIO_Pin_8 
#define LEFT_2			GPIO_Pin_9
#define RIGHT_1    	GPIO_Pin_10
#define RIGHT_2			GPIO_Pin_11

void Motor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LEFT_1 | LEFT_2 |RIGHT_1 |RIGHT_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
	
}

void Motor_SetPWM(uint8_t CS, int16_t PWM){

	if(CS==0){
		if (PWM >= 0){
			GPIO_SetBits(GPIOA, LEFT_1);
			GPIO_ResetBits(GPIOA, LEFT_2);
			PWM_SetCompare1(PWM);
		}
		else{
			GPIO_ResetBits(GPIOA, LEFT_1);
			GPIO_SetBits(GPIOA, LEFT_2);
			PWM_SetCompare1(-PWM);
		}	
	}
	if(CS==1){
		if (PWM >= 0){
			GPIO_SetBits(GPIOA, RIGHT_1);
			GPIO_ResetBits(GPIOA, RIGHT_2);
			PWM_SetCompare2(PWM);
		}
		else{
			GPIO_ResetBits(GPIOA, RIGHT_1);
			GPIO_SetBits(GPIOA, RIGHT_2);
			PWM_SetCompare2(-PWM);
		}	
	}
}

uint16_t Motor_PID(float Angle, float RotateX) {
    float output;
    int16_t pwm;
    
    // 目标角度 = 0°，误差 = 0 - Angle = -Angle
    // 公式：输出 = -Kp × Angle - Kd × RotateX
    output = -KP * Angle - KD * RotateX;
    pwm = (int16_t)output;
    
    if (pwm > MAX_PWM) pwm = MAX_PWM;
		else if (pwm < -MAX_PWM) pwm = -MAX_PWM;
    
    if (pwm > -DEAD_ZONE && pwm < DEAD_ZONE) pwm = 0;
    
    Motor_SetPWM(0, (int16_t)pwm);
    Motor_SetPWM(1, (int16_t)pwm);
    
    return (uint16_t)(pwm > 0 ? pwm : -pwm);
}










