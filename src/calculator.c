/*
 * calculator.c
 *
 * Created: 10/02/2024 12:57:52
 *  Author: Eslam
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "keypad.h"
#include "Button.h"
#include "std_macros.h"
#include <util/delay.h>


double num1=0,num2=0,result;
char op,floated;
signed char length=0;
char res_length=0,notloop=1;
char no_inturpt=1;
void in_num1(){
	char x=0;
	length=0;
	floated=0;
	double sign=1;
	char s=0;
	if(notloop==1){
	while(1){
		  x=keypad_u8check_press();
		  if (x!=0xff)
		  {
			 if (x>='0' && x<='9' && length<5)
			 {
				 	if (!(length==0 && x=='0')){
					 	LCD_vSend_char(x);
					 	length++;
						if (floated>0)
						{
							double a=(x-'0');
							for(char i=0;i<floated;i++)
								a/=10;
								
							num1+=a;
							floated++;
						}else{
					 		num1*=10;
					 		num1+=(x-'0');
						}						 
				 	}
				 	
								 			 
			 }else if(x=='-' && length==0){
				LCD_vSend_char(x);
				sign=-1;
				s=1;
			 }
			 else if (length!=0 &&(x=='+' || x=='-' || x=='*' || x=='/')){
				op=x;
				LCD_vSend_char(x);
			 _delay_ms(500);
				
				break;
			 }else if (floated==0 && x=='.')
			 {
				 LCD_vSend_char(x);
				 floated=1;
				 length-=2;
			 }
			 _delay_ms(500);				 				 
		  }
	}
	
	num1*=sign;
	}else{
		
		while(1){
			x=keypad_u8check_press();
			if (x=='+' || x=='-' || x=='*' || x=='/')
			{
				op=x;
				LCD_vSend_char(x);
				_delay_ms(300);
				break;
			}
		}
	}	
}

void in_num2(){
	char x=0;
	length=0;
	num2=0;
	floated=0;
	double sign=1;
	char s=0;
	while(1){
		x=keypad_u8check_press();
		if (x!=0xff)
		{
			if (x>='0' && x<='9' && length<5)
			{
				if (!(length==0 && x=='0')){
				LCD_vSend_char(x);
				length++;
				if (floated>0)
				{
					double a=(x-'0');
					for(char i=0;i<floated;i++)
					a/=10;
					
					num2+=a;
					floated++;
				}else{
					num2*=10;
					num2+=(x-'0');
				}
				}				 
				
			}else if(x=='-' && length==0){
				LCD_vSend_char(x);
				sign=-1;
				s=1;
			}else if (floated==0 && x=='.')
			{
				LCD_vSend_char(x);
				floated=1;
				length-=2;
			}
			else if(x=='='){
				
				break;
			}
			_delay_ms(400);
		}
	}
	
	
	num2*=sign;
}

void do_op(){
	switch (op) {
		case '-':
		result=num1-num2;
		break;
		case '+':
		result=num1+num2;
		break;
		case '*':
		result=num1*num2;
		break;
		case '/':
		result=num1/num2;
		break;
		default:
		break;
	}
};

void display_num(char ro,char c){
	char n=0;
	double result1=result;
	if (result1<0)
	{
		n=1;
		result1*=-1;
		res_length++;
	}
	res_length=0;
	char cc=c;
	long long res=result1;
	long long rem=(result1-res)*100.0;
	char point=0;
	char y=rem%(long long)10;
	while(rem)
	{
		point++;
		res_length++;
		LCD_movecursor(ro,c--);
		LCD_vSend_char('0'+y);
		rem/=10;
		y=rem%10;
	}
	if (point)
	{
		LCD_movecursor(ro,c--);
		LCD_vSend_char('.');
		res_length++;
	}
	
	y=res%(long long)10;
	while(res)
	{
		res_length++;
		LCD_movecursor(ro,c--);
		LCD_vSend_char('0'+y);
		res/=(long long)10;
		y=res%(long long)10;
	}
	if (n==1)
	{
		LCD_movecursor(ro,c--);
		LCD_vSend_char('-');
	}
	LCD_movecursor(ro,cc+1);
}
int main(void)
{

	
	keypad_vInit();
	LCD_vInit();
	LCD_movecursor(2,16);
	LCD_vSend_char('0');
	LCD_movecursor(1,1);
	
	
    while(1)
    {

		// in num returns false if clear screen
       //in num1
	   in_num1();
	   //in num2
	   in_num2();
	   do_op();
	   display_num(2,16);
	   LCD_vSend_cmd(CURSOR_OF_DISPLAY_ON);
	   _delay_ms(1000);
	  while(keypad_u8check_press()!='=');
	   LCD_vSend_cmd(CLR_SCREEN); 
	   LCD_vSend_cmd(CURSOR_BLINK_DISPLAY_ON);
	   display_num(1,res_length+1);
	   notloop=0;
	   num1=result;
	  
    }
}