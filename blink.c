/*

Arquivo: blink.c
MCU: ATtiny2313
CLOCK: 1Mhz
Complilador: Atmel Studio 7 versão:7.0.1645

Descrição: pisca um led conectado ao pindo PD0 no modo current source

Autor: Iran melo
Data: janeiro de 2021

*/


// definições globais
#define F_CPU 1000000UL     // define a frequencia do microcontrolador

// inclusão de arquivos de cabeçalhos
#include <avr/io.h>         // arquivo de cabeçalho de I/Os do microcontrolador
#include <util/delay.h>     // arquivo de biblioteca para gerar delays

// mapeamento de hardware
#define LED (1 << 0)         // macro que atribui o valor 0 a led

// prototipos das funções auxiliares
void toggle_led(int tempo);      // função que troca o estado do pino PD0 a 
                                 // cada intervalo de tempo definido no parametro 
void pisca_led(void);            // função que troca o estado do pino PD0


// função principal
void main(void){

  DDRD |= LED;                  // configura o pino PD0 como saida
  
  // loop infinito
  while(1){
  
    toggle_led(1000);
    pisca_led();
    
  } // end while

} // end void main

// desenvolvimneto das funções auxiliares

void toggle_led(int tempo){

  PORTD ^= LED;
  for(int i=0; i< tempo; i++) _delay_ms(1);   
  
} / end void toggle led

void pisca_led(){

  PORTD |= LED;           // seta o pino PD0 (high)
  _delay_ms(500);
  PORTD &= ~LED;          // limpa o pino PD0 (low)
  _delay_ms(500);

} // emd void pisca led
