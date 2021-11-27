#include "msp.h"
#include "RTC.h"
#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define BODY_TEXT_FONT 1

char lcdTempString1[21];    // Line 1
char lcdTempString2[21];    // Line 2
char lcdTempString3[21];    // Line 3

void LCD_header(menu_page page){

	ST7735_DrawString_mod(page.header[0].x, page.header[0].y, page.header[0].text, ST7735_WHITE, page.header[0].font);
	ST7735_DrawString_mod(page.header[1].x, page.header[1].y, page.header[1].text, ST7735_WHITE, page.header[1].font);
	ST7735_DrawString_mod(page.header[2].x, page.header[2].y, page.header[2].text, ST7735_WHITE, page.header[2].font);

}

void LCD_body(menu_page page){

	ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font);
	ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font);
	ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font);
    ST7735_DrawString_mod(page.body[3].x, page.body[3].y, page.body[3].text, ST7735_WHITE, page.body[3].font);
	ST7735_DrawString_mod(page.body[4].x, page.body[4].y, page.body[4].text, ST7735_WHITE, page.body[4].font);

}

void LCD_footer(menu_page page){

	ST7735_DrawString_mod(page.footer[0].x, page.footer[0].y, page.footer[0].text, ST7735_WHITE, page.footer[0].font);
	ST7735_DrawString_mod(page.footer[1].x, page.footer[1].y, page.footer[1].text, ST7735_WHITE, page.footer[1].font);

}

void menu_Setup(){

    // splashScreen.title = "Splash  "; 
	// splashScreen.header.x = 0; splashScreen.header.y = 0;
	// splashScreen.header.font = 1;
	// strcpy(splashScreen.header.text, decode_RTC());

	settingScreen.title = "Settings"; 
	
    settingScreen.header[0].x =  0; settingScreen.header[0].y = 0; settingScreen.header[0].font = 1;  // Date
    settingScreen.header[1].x = 17; settingScreen.header[1].y = 0; settingScreen.header[1].font = 1;  // Temperature
    settingScreen.header[2].x =  6; settingScreen.header[2].y = 1; settingScreen.header[2].font = 1;  // Time
    
    //strcpy(settingScreen.header[0].text, decode_Weekday(readRTC.day)); 
    
    decode_Date();
    settingScreen.header[0].text = lcdTempString1;
    decode_Temp(); 
    settingScreen.header[1].text = lcdTempString2;
    decode_Time();
    settingScreen.header[2].text = lcdTempString3;

    settingScreen.body[0].x =  0; settingScreen.body[0].y =  3; settingScreen.body[0].font = BODY_TEXT_FONT;  // Date
    settingScreen.body[1].x =  0; settingScreen.body[1].y =  6; settingScreen.body[1].font = BODY_TEXT_FONT;  // Temperature
    settingScreen.body[2].x =  0; settingScreen.body[2].y =  9; settingScreen.body[2].font = BODY_TEXT_FONT;  // Time
    settingScreen.body[3].x =  0; settingScreen.body[3].y = 12; settingScreen.body[3].font = BODY_TEXT_FONT;  // Temperature
    settingScreen.body[4].x =  0; settingScreen.body[4].y = 15; settingScreen.body[4].font = BODY_TEXT_FONT;  // Time
    settingScreen.body[0].text = "-Set  Date"; 
    settingScreen.body[1].text = "-Set  Time";
    settingScreen.body[2].text = "-Set  Units"; 
    settingScreen.body[3].text = "-Set  Lights";
    settingScreen.body[4].text = "";

    settingScreen.footer[0].x =  0; settingScreen.footer[0].y = 15; settingScreen.footer[0].font = 1;  // N/A
    settingScreen.footer[1].x = 17; settingScreen.footer[1].y = 15; settingScreen.footer[1].font = 1;  // Exit
    settingScreen.footer[0].text = ""; 
    settingScreen.footer[1].text = "Exit";
    
}

void decode_Date(void){

    sprintf(lcdTempString1, "%s %s %d, %d ", decode_Weekday(readRTC.day), decode_Month(readRTC.month), readRTC.date, readRTC.year);

}

void decode_Temp(void){

   if(readRTC.temperature_mode)
       sprintf(lcdTempString2, "%d F", (int)((1.8 * readRTC.temperature) + 32));
   else
       sprintf(lcdTempString2, "%d C", readRTC.temperature);    
}

void decode_Time(void){
    
    if(readRTC.ampm)
        sprintf(lcdTempString3, "%d:%02d:%02d PM", readRTC.hour, readRTC.min, readRTC.sec);
    else
        sprintf(lcdTempString3, "%d:%02d:%02d AM", readRTC.hour, readRTC.min, readRTC.sec);

}



