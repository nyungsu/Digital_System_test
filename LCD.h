/*
 * LCD.h
 * 설명 : LCD 헤더 파일
 * Created: 2021-11-13 오전 10:49:47
 *  Author: 1723522 정윤수
 */ 


#ifndef LCD_H_
#define LCD_H_


//LCD control command
#define ALLCLR				0x01	//화면을 지움
#define HOME				0x02	//커서를 홈으로 이동
#define LN21				0xc0	//커서를 2번째출 첫째 칸으로 이동
#define ENTMOD				0x06	//Entry mode
#define FUNSET				0x38	//Function set, 8-bit mode
#define DISP_ON				0x0c	//turn on Display.
#define DISP_OFF			0x08	//turn off Display.
#define CURSOR_ON			0x0e	//turn on Cursor.
#define CURSOR_OFF			0x0c	//turn off Cursor.
#define CURSOR_LSHIFT		0x10	//커서를 좌측으로 이동
#define CURSOR_RSHIFT		0x14	//커서를 우측으로 이동
#define DISP_LSHIFT			0x18	//Display를 좌측으로 이동
#define DISP_RSHIFT			0x1c	//Display를 우측으로 이동

//LCD control Function
void MCU_Init(void);

void LCDInit(void); //LCD초기화 
void LCDCommand(char command); // LCD 명령 
void LCDMove(char line, char pos); //이동
void LCDPutchar(char ch); //문자 출력
void LCDPuts(char *str); //문자열 출력
void LCDNewchar(char ch, char font[]); //폰트등록 

#endif /* LCD_H_ */
