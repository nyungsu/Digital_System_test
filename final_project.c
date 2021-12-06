/*
 * 프로그램이름 : final_project
 * 프로그램 내용: 초음파 센서를 이용한 주차장
 * Created: 2021-11-13 오전 10:41:38
 * 개발자 : 2조 /정윤수,이희웅,이현민
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"

//LED define
#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED1 4
#define LED2 5
#define LED3 6

//FND define
#define  FND_DATA_DDR   DDRD
#define  FND_DATA_PORT  PORTD
#define  FND_COM_DDR    DDRF
#define  FND_COM_PORT   PORTF
#define  FND_4th		2

//UltraSonic sensor define
#define SONIC_DDR DDRE
#define SONIC_PORT PORTE
#define TRIG1 6  // srf04-1 트리거                                                        
#define ECHO1 7  // srf04-1 에코
#define TRIG2 4 // srf04-2 트리거
#define ECHO2 5  // srf04-2 에코
#define TRIG3 2 // srf04-3 트리거
#define ECHO3 3  // srf04-3 에코
#define SOUND_VELOCITY 340UL	//소리속도 (m/sec)

//FND parameter
unsigned char fnd_num[4];
const unsigned char digit[]={
	
	//hgfedcba
	0b00111111, // num 0
	0b00000110, // num 1
	0b01011011, // num 2
	0b01001111, // num 3
	0b01100110, // num 4
	0b01101101, // num 5
	0b01111100, // num 6
	0b00100111, // num 7
	0b01111111, // num 8
	0b01101111, // num 9
	0b00000000, // BLANK
	
};


//LCD parameter
static char LCD_Title1[] = "JYS LHW LHM";	
static char LCD_Title2[] = "parking space";	

//Ultrasonic parameter
unsigned int distance1,distance2,distance3;

void sonic1()
{
	TCCR1B=0x03;				//Timer,Counter1 클록 4us(64분주)
	PORTE &=~(1<<TRIG1);		//trig LOW 상태
	_delay_ms(10);
	PORTE |=(1<<TRIG1);			//Trig HIGH 상태
	_delay_ms(10);
	PORTE &= ~(1<<TRIG1);		//trig LOW 상태 => 거리 측정 명령 끝
	
	while(!(PINE&(1<<ECHO1)));	// Echo 가 HIGH 될 때까지 대기
	TCNT1=0x0000;				//값 초기화
	
	while(PINE&(1<<ECHO1));		//Echo 가 LOW 될 때 까지 대기
	TCCR1B=0x00;				// Timer,Counter 클록 정지
	
	distance1 = (unsigned int)(SOUND_VELOCITY*(TCNT1*4/2)/1000);
	//거리 = 속도 x 시간
	
	if(distance1<60) //6cm이내
	{
		LED_PORT |= 0x10;
	}
	else if(distance1>60)
	{
		LED_PORT &= ~(1<<LED1);
	}
	
}
void sonic2()
{
	TCCR1B=0x03;				//Timer,Counter1 클록 4us(64분주)
	PORTE &=~(1<<TRIG2);		//trig LOW 상태
	_delay_ms(10);
	PORTE |=(1<<TRIG2);			//Trig HIGH 상태
	_delay_ms(10);
	PORTE &= ~(1<<TRIG2);		//trig LOW 상태 => 거리 측정 명령 끝
	
	while(!(PINE&(1<<ECHO2)));	// Echo 가 HIGH 될 때까지 대기
	TCNT1=0x0000;				//값 초기화
	
	while(PINE&(1<<ECHO2));		//Echo 가 LOW 될 때 까지 대기
	TCCR1B=0x00;				// Timer,Counter 클록 정지
	
	distance2 = (unsigned int)(SOUND_VELOCITY*(TCNT1*4/2)/1000);
	//거리 = 속도 x 시간
	
	if(distance2<60) //6cm이내
	{
		LED_PORT |= 0x20;
	}
	else if(distance2>60)
	{
		LED_PORT &= ~(1<<LED2);
	}
	
	
}
void sonic3()
{
	TCCR1B=0x03;				//Timer,Counter1 클록 4us(64분주)
	PORTE &=~(1<<TRIG3);		//trig LOW 상태
	_delay_ms(10);
	PORTE |=(1<<TRIG3);			//Trig HIGH 상태
	_delay_ms(10);
	PORTE &= ~(1<<TRIG3);		//trig LOW 상태 => 거리 측정 명령 끝
	
	while(!(PINE&(1<<ECHO3)));	// Echo 가 HIGH 될 때까지 대기
	TCNT1=0x0000;				//값 초기화
	
	while(PINE&(1<<ECHO3));		//Echo 가 LOW 될 때 까지 대기
	TCCR1B=0x00;				// Timer,Counter 클록 정지
	
	distance3 = (unsigned int)(SOUND_VELOCITY*(TCNT1*4/2)/1000);
	//거리 = 속도 x 시간
	
	if(distance3<60) //6cm이내
	{
		LED_PORT |= 0x40;
	}
	else if(distance3>60)
	{
		LED_PORT &= ~(1<<LED3);
	}
	
}

void fnd_number(unsigned int num)
{
	
	fnd_num[3] = num % 10;
	num = num/10;
	fnd_num[2] = num % 10;
	num = num/10;
	fnd_num[1] = num % 10;
	num = num/10;
	fnd_num[0] = num %10;
	
}
void fnd_display()
{
		FND_COM_PORT = 1<<(FND_4th);			//max 3대라서 fnd 4개중에 끝에꺼
		FND_DATA_PORT = ~digit[fnd_num[3]];		//max 3대라서 1의자리만 출력하게 함
			
}

void cheaking_port()
{
	if(LED_PORT == 0x00)		//주차가능대수 3대
	{
		fnd_number(3);
		fnd_display();
	}
	else if(LED_PORT == 0x10 || LED_PORT == 0x20 || LED_PORT == 0x40)		//주차가능대수 1대
	{
		fnd_number(2);
		fnd_display();
	}
	
	else if(LED_PORT == 0x30 ||LED_PORT == 0x50 ||LED_PORT == 0x60)		//주차가능대수 2대
	{
		fnd_number(1);
		fnd_display();
	}
	
	else if(LED_PORT == 0x70)	//주차가능대수 3대
	{
		fnd_number(0);
		fnd_display();
	}
}



int main(void)
{
    LED_DDR = 0xf0; //7~4 LED 출력
		
	SONIC_DDR |= ((1<<TRIG1)|(1<<TRIG2)|(1<<TRIG3));
	SONIC_DDR &= ~((1<<ECHO1)|(1<<ECHO2)|(1<<ECHO3));

	FND_DATA_DDR |= 0xff;
	FND_COM_DDR |= ((1<<PF5)|(1<<PF4)|(1<<PF3)|(1<<PF2));
	
	FND_DATA_PORT = 0x00;
	FND_COM_PORT &= ~((1<<PF5)|(1<<PF4)|(1<<PF3)|(1<<PF2));
	
	MCU_Init();			//MCU 초기화
	LCDInit();			// LCD 초기화
	
	LCDMove(0,3);				// 첫 번째 줄
	LCDPuts(LCD_Title1);		// jys lhw lhm
	LCDMove(1,3);				// 두 번째 줄
	LCDPuts(LCD_Title2);		// parking space 출력

	while(1)
	{	
		sonic1();
		sonic2();
		sonic3();	
	
		cheaking_port();
	}		
	
}

