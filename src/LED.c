
#include "LED.h"
#include "DIO.h"


void LED_vInit(unsigned char portname,unsigned char pinnumber){
	
	DIO_vsetPINDir(portname,pinnumber,1);
}

void LED_vTurnOn(unsigned char portname,unsigned char pinnumber){
	
	DIO_vwrite(portname,pinnumber,1);
}

void LED_vTurnOff(unsigned char portname,unsigned char pinnumber){
	
	DIO_vwrite(portname,pinnumber,0);
}


void LED_vToggle(unsigned char portname,unsigned char pinnumber){
	DIO_vtoggle(portname,pinnumber);
}