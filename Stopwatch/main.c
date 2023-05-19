#include "STD_TYPES.h"
#include "PORT_interface.h"
#include "CLCD_interface.h"
#include "KEYPAD_interface.h"
#include <util/delay.h>

void main(){
	PORT_voidInit();
	CLCD_voidInit();
	s8 Local_u8PressedVal=0,Local_u8Col=0,Local_u8Hour1=0,Local_u8Hour2=0,Local_u8Min1=0;
	s8 Local_u8Sec1=0,Local_u8Sec2=0,Local_u8Min2=0,Local_u8LastNum=0;
	u8 Local_u8HamokshaUp[]={0b00001110,0b00010001,0b00001110,0b00010101
			,0b00001110,0b00000100,0b00001110,0b00010001};
	u8 Local_u8HamokshaLow[]={0b00001110,0b00010001,0b00001110,0b00000100
				,0b00001110,0b00010101,0b00001110,0b00010001};
	CLCD_voidSendString("00:00:00");
	CLCD_voidGoToXY(0,0);
	while(1){
		do{
			Local_u8PressedVal=KPD_u8GetPressedKey();
		}while(Local_u8PressedVal==0xff);
		switch(Local_u8PressedVal){
		case KPD_START:
			while(Local_u8Sec2>0 || Local_u8Sec1>0 || Local_u8Min1>0 || Local_u8Min2>0
					|| Local_u8Hour2>0 || Local_u8Hour1>0){

				Local_u8Sec2--;
				if(Local_u8Sec2<0 && (Local_u8Sec1>0 || Local_u8Min2>0 || Local_u8Min1>0 || Local_u8Hour2>0 ||Local_u8Hour1>0)){
					Local_u8Sec2=9;
					Local_u8Sec1--;
				}
				if(Local_u8Sec1<0 && (Local_u8Min2>0|| Local_u8Min1>0 || Local_u8Hour2>0 ||Local_u8Hour1>0)){
					Local_u8Sec1=5;
					Local_u8Min2--;
				}
				if(Local_u8Min2<0 && (Local_u8Min1>0|| Local_u8Hour2>0 ||Local_u8Hour1>0)){
					Local_u8Min2=9;
					Local_u8Min1--;
				}
				if(Local_u8Min1<0 && (Local_u8Hour2>0||Local_u8Hour1>0)){
					Local_u8Min1=5;
					Local_u8Hour2--;
				}
				if(Local_u8Hour2<0 && Local_u8Hour1>0){
					Local_u8Hour2=3;
					Local_u8Hour1--;
				}
				else if(Local_u8Hour2<0){
					Local_u8Hour2=0;
				}
				if(Local_u8Hour1<0){
					Local_u8Hour1=0;
				}
				CLCD_voidGoToXY(0,0);
				CLCD_voidWriteNumber(Local_u8Hour1);
				CLCD_voidWriteNumber(Local_u8Hour2);

				CLCD_voidGoToXY(0,3);
				CLCD_voidWriteNumber(Local_u8Min1);
				CLCD_voidWriteNumber(Local_u8Min2);

				CLCD_voidGoToXY(0,6);
				CLCD_voidWriteNumber(Local_u8Sec1);
				CLCD_voidWriteNumber(Local_u8Sec2);
				_delay_ms(1000);
			}
			CLCD_voidInit();
			CLCD_voidSendString("It's Time!!");
			CLCD_voidWriteSpecialCharacter(Local_u8HamokshaUp,0,0,13);
			_delay_ms(1000);
			CLCD_voidInit();
			CLCD_voidSendString("It's Time!!");
			CLCD_voidWriteSpecialCharacter(Local_u8HamokshaLow,1,0,13);
			_delay_ms(1000);
			CLCD_voidInit();
			CLCD_voidSendString("It's Time!!");
			CLCD_voidWriteSpecialCharacter(Local_u8HamokshaUp,0,0,13);
			_delay_ms(1000);
			break;

		case KPD_ENTER:
			switch(Local_u8Col){
			case 0:
				Local_u8Hour1=Local_u8LastNum;
				break;
			case 1:
				Local_u8Hour2=Local_u8LastNum;
				break;

			case 3:
				Local_u8Min1=Local_u8LastNum;
				break;
			case 4:
				Local_u8Min2=Local_u8LastNum;
				break;

			case 6:
				Local_u8Sec1=Local_u8LastNum;
				break;
			case 7:
				Local_u8Sec2=Local_u8LastNum;
				break;
			}
			if(Local_u8Col==0 || Local_u8Col==3 ||Local_u8Col==6){
				Local_u8Col++;
			}
			else if(Local_u8Col==1 || Local_u8Col==4){
				Local_u8Col+=2;
			}
			if(Local_u8Col>=7){
				Local_u8Col=7;
			}
			CLCD_voidGoToXY(0,Local_u8Col);
			break;

			case KPD_RIGHT:
				if(Local_u8Col==0 || Local_u8Col==3 ||Local_u8Col==6){
					Local_u8Col++;
				}
				else if(Local_u8Col==1 || Local_u8Col==4){
					Local_u8Col+=2;
				}
				if(Local_u8Col>=7){
					Local_u8Col=7;
				}
				CLCD_voidGoToXY(0,Local_u8Col);
				break;

			case KPD_LEFT:
				if(Local_u8Col==7 || Local_u8Col==4||Local_u8Col==1){
					Local_u8Col--;
				}
				else if(Local_u8Col==6 || Local_u8Col==3){
					Local_u8Col=Local_u8Col-2;
				}
				if(Local_u8Col<=0){
					Local_u8Col=0;
				}
				CLCD_voidGoToXY(0,Local_u8Col);
				break;

			case 0 ... 9:
			CLCD_voidGoToXY(0,Local_u8Col);
			CLCD_voidWriteNumber(Local_u8PressedVal);
			Local_u8LastNum=Local_u8PressedVal;
			break;
			case KPD_RESET:
				CLCD_voidInit();
				CLCD_voidSendString("00:00:00");
				CLCD_voidGoToXY(0,0);
				Local_u8Col=0;
				Local_u8Sec1=0;
				Local_u8Sec2=0;
				Local_u8Min2=0;
				Local_u8Min1=0;
				Local_u8Hour2=0;
				Local_u8Hour1=0;
		}
	}
}
