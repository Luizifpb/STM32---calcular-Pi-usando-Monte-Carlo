#include "stm32f4xx.h"            //bibliotecas padrão STM32F4
#include <stdio.h>
#include "Utility.h"        //biblioteca de funções utilitárias

double numAleatorioEntreZeroEUm(){
    if(RNG->SR & RNG_SR_DRDY){
        uint32_t number = RNG->DR;
        double numNormalizado = (double) number / 0xFFFFFFFF;
        return numNormalizado;
    }
}


//n é o numero de amostragens
double pi(int n){
    int acertos = 0;

    for(int i = 0; i < n; i++){
        double x = numAleatorioEntreZeroEUm();
        double y = numAleatorioEntreZeroEUm();

        if(x * x + y * y < 1.0){
            acertos++;
        }
    }
    return (double) (4.0 * acertos / n);
}


//função principal
int main(void)
{
    Configure_Clock();                //configura o sistema de clock
    USART1_Init();
    Delay_Start();

    printf("\n--------  Calculo de Pi utilizando metodo de Monte Carlo  --------\n\n");
    printf("\n--------  Aperte o botão k0 para gerar uma amostragem--------\n\n");
    Delay_ms(2000);

    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;    //habilita o clock do RNG
    RNG->CR |= RNG_CR_RNGEN;            //liga o RNG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;//LIGA CLOCK na porta E
	GPIOE->PUPDR |= (0b01 << 8);//ativa o pull up em PE4 = k0

    

    while(1)
    {
        if(!(GPIOE->IDR & (1 << 4))){
            printf("Calculando...\n");
            int n = 1000000; //quantidade de numeros a ser gerado
            double resultado = pi(n);

            printf("O valor de pi com %d pontos sorteados é: %.10f\n", n, resultado);

            Delay_ms(75);
			while(!(GPIOE->IDR & (1 << 4)));
        }
    }
}