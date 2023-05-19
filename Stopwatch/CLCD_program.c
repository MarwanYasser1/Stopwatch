/**************************************************************/
/***************     Author: Marwan Yasser   ******************/
/***************     Layer: HAL              ******************/
/***************     SWC: CLCD   			 ******************/
/***************     Version: 1.00   		 ******************/
/**************************************************************/
/**************************************************************/
#include "STD_TYPES.h"
#include <util/delay.h>
#include "DIO_interface.h"
#include "CLCD_interface.h"
#include "CLCD_config.h"
#include "CLCD_private.h"


void CLCD_voidSendCommand(u8 Copy_u8Command){
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_LOW);

	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);

	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Command);

	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_LOW);
}

void CLCD_voidSendData(u8 Copy_u8Data){
//	//See counter
//	u8 Local_u8PortValue=0;
//	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_LOW);
//	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_HIGH);
//	DIO_u8SetPortDirection(CLCD_DATA_PORT,DIO_u8PORT_INPUT);
//	DIO_u8GetPortValue(CLCD_DATA_PORT,&Local_u8PortValue);
//	if(Local_u8PortValue<128){
//		if(Local_u8PortValue>0x0f){
//			CLCD_voidGoToXY(1,0);
//		}
//	}
	// Send data
//	static u8 Local_u8ColCounter=0;
//	if(Local_u8ColCounter>=16){
//		Local_u8ColCounter=0;
//		CLCD_voidGoToXY(1,0);
//	}
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_HIGH);

	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Data);

	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_LOW);
	//Local_u8ColCounter++;
}

void CLCD_voidInit(void){
	_delay_ms(40);
	CLCD_voidSendCommand(0b00111000);
	CLCD_voidSendCommand(0b00001100);
	CLCD_voidSendCommand(1);
}

void CLCD_voidSendString(const char *Copy_pcString){
	u8 Local_u8Counter=0;
	while (Copy_pcString[Local_u8Counter]!='\0'){
		CLCD_voidSendData(Copy_pcString[Local_u8Counter]);
		Local_u8Counter++;
	}
//	u8 *ptr=Copy_pcString;
//	while(*ptr!='\0'){
//		CLCD_voidSendData(*ptr);
//		ptr++;
//	}
}

void CLCD_voidGoToXY(u8 Copy_u8XPos,u8 Copy_u8YPos){
	u8 Local_u8Address=0;
	if(Copy_u8XPos==0){
		Local_u8Address=Copy_u8YPos;
	}
	else if(Copy_u8XPos==1){
		Local_u8Address=Copy_u8YPos+0x40;
	}
	CLCD_voidSendCommand(Local_u8Address+128);
}

void CLCD_voidWriteSpecialCharacter(u8 * Copy_pu8Pattern, u8 Copy_u8PatternNum,u8 Copy_u8XPos,u8 Copy_u8YPos){
	u8 Local_u8CGRAMAddress=Copy_u8PatternNum*8;
	CLCD_voidSendCommand(Local_u8CGRAMAddress+64);
	u8 Local_u8Iterator=0;
	for(Local_u8Iterator=0;Local_u8Iterator<8;Local_u8Iterator++){
		CLCD_voidSendData(Copy_pu8Pattern[Local_u8Iterator]);
	}
	CLCD_voidGoToXY(Copy_u8XPos,Copy_u8YPos);
	CLCD_voidSendData(Copy_u8PatternNum);
}

void CLCD_voidWriteNumber(u32 Copy_u32Number){
	if(Copy_u32Number==0){
		CLCD_voidSendData('0');
		return;
	}
	u32 Local_u8Reverse=0;
	u8 Local_u8NumOfDigits=0;
	u8 Local_u8Remainder=0,Local_u8Send=0;
	while (Copy_u32Number != 0) {
		Local_u8Remainder = Copy_u32Number % 10;
		Local_u8Reverse = Local_u8Reverse * 10 + Local_u8Remainder;
		Copy_u32Number /= 10;
		Local_u8NumOfDigits++;
	  }
	while(Local_u8NumOfDigits!=0){
		Local_u8Send=(Local_u8Reverse%10)+48;
		CLCD_voidSendData(Local_u8Send);
		Local_u8Reverse/=10;
		Local_u8NumOfDigits--;
	}
}
void CLCD_voidWriteFloatNum(f32 Copy_f32Num){
	u8 Local_u8NumInt=(int)Copy_f32Num;
	CLCD_voidWriteNumber(Local_u8NumInt);
	CLCD_voidSendData('.');
	f32 Local_f32FloatNum=Copy_f32Num-Local_u8NumInt;
	Local_f32FloatNum*=100000;
	CLCD_voidWriteNumber(Local_f32FloatNum);
}
