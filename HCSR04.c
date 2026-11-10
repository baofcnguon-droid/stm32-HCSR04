#include "stm32f1xx.h"
#include "HCSR04.h"

void TIM_Int(void){
	RCC->APB2ENR |=1<<11;
	TIM1->PSC	=		72-1;
	TIM1->ARR =		7199;
	TIM1->EGR	|=		1U<<0;
	TIM1->SR 	&=	~(1U<<0);
	TIM1->CR1 &=	~(1U<<0);
}

void Delay_us(uint32_t us){
	TIM1->CNT	=	0;
	TIM1->CR1	|=1U<<0;
	while(TIM1->CNT <us);
	TIM1->CR1 &=	~(1U<<0);
}

void Delay_ms(uint32_t ms) {
    while (ms--) {
        Delay_us(1000);
    }
}

uint16_t HCSR04(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin_Trig,uint16_t GPIO_Pin_Echo){
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin_Trig,GPIO_PIN_SET);
	Delay_us(10);																					
	HAL_GPIO_WritePin(GPIOx,GPIO_Pin_Trig,GPIO_PIN_RESET);
	
	uint16_t t=0;
	uint16_t timeout =0;
	while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_Echo)==0){
		timeout++;
		Delay_us(1);
		if(timeout>25000){
			return 0;
		}
	}
	timeout=0;
	while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_Echo)==1){
		t++;
		timeout++;
		Delay_us(1);
		if(timeout>25000){
			return 0;
		}
	}
	uint16_t d=t/58;
	return d;
}
