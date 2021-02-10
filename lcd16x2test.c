/*
 * lcdChar_ATtiny2313.c
 *
 * Attiny2313 rodando a 1mhz
 * lcd caracter 16x2 interface 4bits
 * lcd na porta D
 * Created: 30/12/2016 23:02:38
 * Author : Iran Melo
 */ 

//////////////////////////////////////////////////////////////////////////

// definicoes globais
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

//////////////////////////////////////////////////////////////////////////

// definicoes do lcd
//instrucoes do lcd

#define LCD_TRY_CONFIG_4BITS	0x03
#define LCD_CONFIG_4BITS		0x02
#define LCD_TWO_LINES			0x28

#define LCD_DISPLAY_CONFIG		0x0C
#define LCD_CLEARDISPLAY		0x01
#define LCD_ENTRY_CONFIG		0x06

#define LCD_FISRT_LINE			0x80
#define LCD_SECOND_LINE			0xC0

//////////////////////////////////////////////////////////////////////////

// prototipos das funcoes auxiliares

void iniciaLcd();						// configura
void enviaDados(uint8_t, uint8_t);
void escreve4bits(uint8_t);
void habilitaPulso();


//////////////////////////////////////////////////////////////////////////

// mapeamento de hardware na porta D
#define  LCD_DADOS_D4 (1 << 2)  //PD2 conectado ao pino de dados D4 do lcd
#define  LCD_DADOS_D5 (1 << 3)	//PD3 conectado ao pino de dados D5 do lcd
#define  LCD_DADOS_D6 (1 << 4)	//PD4 conectado ao pino de dados D6 do lcd
#define  LCD_DADOS_D7 (1 << 5)  //PD5 conectado ao pino de dados D7 do lcd

#define LCD_RS (1 << 0) //PD1  low=comando  high=caractere
#define LCD_E (1 << 1)  //PD2  low-to-high=ler o baramento
//#define LCD_RW(1 << 2) LOW: write to LCD.  HIGH: read from LCD.

//////////////////////////////////////////////////////////////////////////

// funcao principal
int main(void)
{
   
   	DDRD = 0xFF;  // configura a porta D como saida
  	iniciaLcd();
   	
   	// escreve caracteres na primeira linha
   	// primeira linha = 0x80
   	enviaDados((LCD_FIRST_LINE + 3),0); // primeira linha mais 3 posicoes
   	
	  enviaDados("L",1); //enviando caractere por caractere
    enviaDados("C",1); //enviando caractere por caractere
    enviaDados("D",1); //enviando caractere por caractere
    enviaDados(" ",1); //enviando caractere por caractere
    enviaDados("T",1); //enviando caractere por caractere
    enviaDados("E",1); //enviando caractere por caractere
    enviaDados("S",1); //enviando caractere por caractere
    enviaDados("T",1); //enviando caractere por caractere
    
	 // loop infinito
    while (1) 
    { 
		    for(uint8_t i= 0; i < 10; i++){
    
        enviaDados((LCD_SECOND_LINE + 3),0); //desloca o cursor para a segunda linha do LCD comandos
      	enviaDados((i + 0x30),1); //enviando caractere por caractere
        _delay_ms(750);
    
      } // end for
    
    } // end while
	
}  // end main

//////////////////////////////////////////////////////////////////////////

// desenvolvimento das funcoes auxiliares

void iniciaLcd() {
	
	// configura o pinos para o ldc como saida
	DDRD |= LCD_RS | LCD_E | LCD_DADOS_D4 | LCD_DADOS_D5 | LCD_DADOS_D6 | LCD_DADOS_D7;
	
	// Antes de enviar comandos, esperamos 50 milisegundos
	//para a estabilizacao da tensao de alimentacao
	_delay_us(50000);
	// Para enviar instrucoes, colocamos LCD_RS e LCD_E em nivel baixo(LOW)
	PORTD &= ~LCD_RS;
	PORTD &= ~LCD_E;
	
	// O lcd incia no modo 8 bits.
	// Primeira tentativa para configurar em modo 4 bits
	escreve4bits(LCD_TRY_CONFIG_4BITS);
	_delay_us(4500); // Espera minima 4.1ms(milisegundos)

	// Segunda tentativa
	escreve4bits(LCD_TRY_CONFIG_4BITS);
	_delay_us(4500); // Espera minima 4.1ms(milisegundos)
	
	// Terceira tentativa
	escreve4bits(LCD_TRY_CONFIG_4BITS);
	_delay_us(150);  // Espera minima 150 microsegundos

	// Configurado do modo 4 bits de dados
	escreve4bits(LCD_CONFIG_4BITS);

	// habilita a segunda linha do lcd
	enviaDados(LCD_TWO_LINES,0);
	_delay_us(4500);  // Espera mais que 4.1ms

	// Segunda tentativa
	enviaDados(LCD_TWO_LINES,0);
	_delay_us(150);

	// Terceira tentativa
	enviaDados(LCD_TWO_LINES,0);
	_delay_us(150);

	// Liga o display sem cursor or sem cursor piscando
	enviaDados(LCD_DISPLAY_CONFIG,0);

	enviaDados(LCD_CLEARDISPLAY,0);  // Limpa o display, configura o cursor na posicao zero
	_delay_us(2000);  // Este comando gasta muito tempo!

	// configura o modo de entrada de texto, da esquerda para a direita
	enviaDados(LCD_ENTRY_CONFIG,0);
  
} // end inicia lcd


void enviaDados(uint8_t valor, uint8_t modo) {
	
	if (modo==0)
	{
		// Se LCD_RS e nivel baixo(LOW), envia instrucoes para o lcd
		PORTD &= ~LCD_RS;
		escreve4bits(valor>>4);  // envia os 4 bits mais signicativos primeiro
		escreve4bits(valor);     // depois os 4 bits menos significativos
    
	} // end if
  
	else{
		// Se LCD_RS e nivel alto(HIGH), envia caracteres para o lcd
		PORTD |= LCD_RS;
		escreve4bits(valor>>4);
		escreve4bits(valor);
    
	} // end else
	
} // end envia dados


void escreve4bits(uint8_t valor) {
	// carrega os bits de dados, nos seus respectivos pinos de acordo com seus pesos binarios
	PORTD = (valor & 0x01) ? (PORTD | LCD_DADOS_D4) : (PORTD & ~LCD_DADOS_D4); // PD2
	PORTD = (valor & 0x02) ? (PORTD | LCD_DADOS_D5) : (PORTD & ~LCD_DADOS_D5); // PD3
	PORTD = (valor & 0x04) ? (PORTD | LCD_DADOS_D6) : (PORTD & ~LCD_DADOS_D6); // PD4
	PORTD = (valor & 0x08) ? (PORTD | LCD_DADOS_D7) : (PORTD & ~LCD_DADOS_D7); // PD5
	habilitaPulso();
  
} // end escreve 4 bits


void habilitaPulso(void) {

	// lcd e ativado pelo pulso em nivel alto(HIGH)
	PORTD &= ~LCD_E;  // low
	_delay_us(1);
	PORTD |= LCD_E;   // high
	_delay_us(1);    // duracao do deve ser maior que 450 nanosegundos
	PORTD &= ~LCD_E;  // low
	_delay_us(100);   // instrucoes precisam de tempo maior que 37 microsegundos para processar
  
} // end habilita pulso


