#include "stm32f10x.h"   
#include "math.h"
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"
#include "Motor.h"
#include "MPU6050.h"


int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
volatile float Angle,RotateY;

int main(void){
	
	Motor_Init();
	OLED_Init();
	MPU6050_Init();
	Timer_Init();
	
	while (1){
		
		uint16_t f=Motor_PID(Angle,RotateY);
		
//		OLED_ShowSignedNum(1,1, AccX,5);
//		OLED_ShowSignedNum(2,1, AccY,5);
//		OLED_ShowSignedNum(3,1, AccZ,5);
//		OLED_ShowSignedNum(1,8, GyroX,5);
//		OLED_ShowSignedNum(2,8, GyroY,5);
//		OLED_ShowSignedNum(3,8, GyroZ,5);
//		OLED_ShowSignedNum(4,8, f,5);
	}
}



void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET){
		MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
		float AccelerX=(float)AccX/2048.0f;
		float AccelerZ=(float)AccZ/2048.0f;
		float Acc_angle = atan2f(AccelerX, AccelerZ) * 57.29578f;
		
		RotateY=(float)GyroY/16.4f;
		Angle=0.98f*(Angle+RotateY*0.001f)+0.02f*Acc_angle;

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
