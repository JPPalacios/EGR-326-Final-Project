#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "msp.h"
#include "RTC.h"
#include "menu.h"
#include "stepper.h"

#define HEADER_TEXT_FONT 1 
#define TITLE_TEXT_FONT 2
#define BODY_TEXT_FONT  2
#define FOOTER_TEXT_FONT 1

char lcdTempString1[21];    // Line 1
char lcdTempString2[21];    // Line 2
char lcdTempString3[21];    // Line 3

char lcdTempString4[21];    // mph

//uint8_t i = 0;

void LCD_highlight(menu_choice screen, menu_page page, uint8_t item){
//    uint8_t prev = item - 1;
//    uint8_t next = item + 1;

    
    if((item < 4) && (screen != Splash)){
        
        if(item == 1){
            ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
            ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
            ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
        }else if(item)

        
        ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
        ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
        ST7735_DrawString_mod(page.body[item].x, page.body[item].y, page.body[item].text, ST7735_WHITE, page.body[item].font, page.body[item].select);
    }else if((item > 3) || (screen == Splash)){
        if(screen != Splash)
            item = 5 - item; // 5 - 4, 5 - 2
        ST7735_DrawString_mod(page.footer[item].x, page.footer[item].y, page.footer[item].text, ST7735_WHITE, page.footer[item].font, page.footer[item].select);
    }
    
}

void LCD_header(menu_page page){

	ST7735_DrawString_mod(page.header[0].x, page.header[0].y, page.header[0].text, ST7735_WHITE, page.header[0].font, false);
	ST7735_DrawString_mod(page.header[1].x, page.header[1].y, page.header[1].text, ST7735_WHITE, page.header[1].font, false);
	ST7735_DrawString_mod(page.header[2].x, page.header[2].y, page.header[2].text, ST7735_WHITE, page.header[2].font, false);
}

void LCD_body(menu_page page, uint8_t item){

    ST7735_DrawString_mod(page.title.x,   page.title.y,   page.title.text,   ST7735_WHITE, page.title.font  , page.title.select);

    if(item == 0){
        ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font, false);
        ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font, false);
        ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font, false);        
    }
    if(item == 1){
        ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font,  page.body[item].select);
	    ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font, false);
	    ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font, false);
    }else if(item == 2){
        ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font, false);
	    ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font, page.body[item].select);
	    ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font, false);
    }else if(item == 3){
        ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font, false);
	    ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font, false);
	    ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font, page.body[item].select);
    }


    //ST7735_DrawString_mod(page.body[0].x, page.body[0].y, page.body[0].text, ST7735_WHITE, page.body[0].font, false);
	//ST7735_DrawString_mod(page.body[1].x, page.body[1].y, page.body[1].text, ST7735_WHITE, page.body[1].font, false);
	//ST7735_DrawString_mod(page.body[2].x, page.body[2].y, page.body[2].text, ST7735_WHITE, page.body[2].font, false);
    //ST7735_DrawString_mod(page.body[3].x, page.body[3].y, page.body[3].text, ST7735_WHITE, page.body[3].font);
	//ST7735_DrawString_mod(page.body[4].x, page.body[4].y, page.body[4].text, ST7735_WHITE, page.body[4].font);
}

void LCD_footer(menu_page page){

	ST7735_DrawString_mod(page.footer[0].x, page.footer[0].y, page.footer[0].text, ST7735_WHITE, page.footer[0].font, false);
	ST7735_DrawString_mod(page.footer[1].x, page.footer[1].y, page.footer[1].text, ST7735_WHITE, page.footer[1].font, false);
}


void menu_Setup(menu_choice screen){
   switch(screen){
       case Splash: 
            splashScreen_Setup(); 
            break; 
       case Menu: 
            menuScreen_Setup(); 
            break; 
       case Settings: 
            settingScreen_Setup(); 
            break; 
        case SetDate:
            setDateScreen_Setup();
            break;
        case SetTime: 
            setTimeScreen_Setup(); 
            break; 
        case SetUnits:
            setUnitScreen_Setup(); 
            break;
        case Testing: 
            testingScreen_Setup();
            break;
        case Warning: 
            warningScreen_Setup();
            break; 
        default: screen = Splash; 
   }
}

void splashScreen_Setup(void){

    splashScreen.title.text = "SplashScreen";  // PLACE HOLDER
    splashScreen.title.x = 0;      splashScreen.title.y     = 1; splashScreen.title.font     = TITLE_TEXT_FONT;  splashScreen.title.select     = false; // FIXME: remove this 
    splashScreen.header[0].x =  0; splashScreen.header[0].y = 0; splashScreen.header[0].font = HEADER_TEXT_FONT; splashScreen.header[0].select = false; // Date
    splashScreen.header[1].x = 17; splashScreen.header[1].y = 0; splashScreen.header[1].font = HEADER_TEXT_FONT; splashScreen.header[1].select = false; // Temperature
    decode_Date(); splashScreen.header[0].text = lcdTempString1;
    decode_Temp(); splashScreen.header[1].text = lcdTempString2;

    splashScreen.body[0].x =  1; splashScreen.body[0].y =  3; splashScreen.body[0].font = BODY_TEXT_FONT; splashScreen.body[0].select = false; // Time without seconds
    splashScreen.body[1].x =  0; splashScreen.body[1].y =  6; splashScreen.body[1].font = 3; splashScreen.body[0].select = false; // Light Ambience
    splashScreen.body[2].x =  0; splashScreen.body[2].y =  8; splashScreen.body[2].font = BODY_TEXT_FONT; splashScreen.body[0].select = false; // Fuel Level Indicator
    decode_Time(); splashScreen.body[0].text = lcdTempString3;
    splashScreen.body[1].text = lcdTempString4;
    splashScreen.body[2].text = "";

    splashScreen.footer[0].x =  0; splashScreen.footer[0].y = 15; splashScreen.footer[0].font = FOOTER_TEXT_FONT; splashScreen.footer[0].select = false;// N/A or extras! 
    splashScreen.footer[1].x = 17; splashScreen.footer[1].y = 15; splashScreen.footer[1].font = FOOTER_TEXT_FONT; splashScreen.footer[1].select = true; // Exit
    splashScreen.footer[0].text = "Extras"; 
    splashScreen.footer[1].text = "Menu";

}

void menuScreen_Setup(void){

    menuScreen.title.text  = "Menu"; 
    menuScreen.title.x     =  4; menuScreen.title.y     = 2; menuScreen.title.font     =  TITLE_TEXT_FONT; menuScreen.title.select     = false;
    menuScreen.header[0].x =  0; menuScreen.header[0].y = 0; menuScreen.header[0].font = HEADER_TEXT_FONT; menuScreen.header[0].select = false; // Date
    menuScreen.header[1].x = 17; menuScreen.header[1].y = 0; menuScreen.header[1].font = HEADER_TEXT_FONT; menuScreen.header[1].select = false; // Temperature
    menuScreen.header[2].x =  6; menuScreen.header[2].y = 1; menuScreen.header[2].font = HEADER_TEXT_FONT; menuScreen.header[2].select = false; // Time
    decode_Date(); menuScreen.header[0].text = lcdTempString1;
    decode_Temp(); menuScreen.header[1].text = lcdTempString2;
    decode_Time(); menuScreen.header[2].text = lcdTempString3;

     
    menuScreen.body[0].x =  0; menuScreen.body[0].y =  4; menuScreen.body[0].font = BODY_TEXT_FONT; menuScreen.body[0].select = true; // Date
    menuScreen.body[1].x =  0; menuScreen.body[1].y =  6; menuScreen.body[1].font = BODY_TEXT_FONT; menuScreen.body[1].select = true; // Temperature
    menuScreen.body[2].x =  0; menuScreen.body[2].y =  8; menuScreen.body[2].font = BODY_TEXT_FONT; menuScreen.body[2].select = true; // Time
    menuScreen.body[0].text = "- Settings"; 
    menuScreen.body[1].text = "- Testing";
    menuScreen.body[2].text = "- Warnings"; 


    menuScreen.footer[0].x =  0; menuScreen.footer[0].y = 15; menuScreen.footer[0].font = FOOTER_TEXT_FONT; menuScreen.footer[0].select = true;// N/A
    menuScreen.footer[1].x = 17; menuScreen.footer[1].y = 15; menuScreen.footer[1].font = FOOTER_TEXT_FONT; menuScreen.footer[1].select = true; // Exit
    menuScreen.footer[0].text = " "; 
    menuScreen.footer[1].text = "Home";

}

void settingScreen_Setup(void){
    
    settingScreen.title.text = "Settings"; 
    settingScreen.title.x = 2; settingScreen.title.y = 2; settingScreen.title.font = TITLE_TEXT_FONT; settingScreen.title.select = false; 
	
    settingScreen.header[0].x =  0; settingScreen.header[0].y = 0; settingScreen.header[0].font = HEADER_TEXT_FONT; settingScreen.header[0].select = false; // Date
    settingScreen.header[1].x = 17; settingScreen.header[1].y = 0; settingScreen.header[1].font = HEADER_TEXT_FONT; settingScreen.header[1].select = false; // Temperature
    settingScreen.header[2].x =  6; settingScreen.header[2].y = 1; settingScreen.header[2].font = HEADER_TEXT_FONT; settingScreen.header[2].select = false; // Time    
    decode_Date(); settingScreen.header[0].text = lcdTempString1;
    decode_Temp(); settingScreen.header[1].text = lcdTempString2;
    decode_Time(); settingScreen.header[2].text = lcdTempString3;
    

    settingScreen.body[0].x =  0; settingScreen.body[0].y =  4; settingScreen.body[0].font = BODY_TEXT_FONT; settingScreen.body[0].select = true; // Date
    settingScreen.body[1].x =  0; settingScreen.body[1].y =  6; settingScreen.body[1].font = BODY_TEXT_FONT; settingScreen.body[1].select = true; // Temperature
    settingScreen.body[2].x =  0; settingScreen.body[2].y =  8; settingScreen.body[2].font = BODY_TEXT_FONT; settingScreen.body[2].select = true; // Time
    settingScreen.body[0].text = "- Set Date"; 
    settingScreen.body[1].text = "- Set Time";
    settingScreen.body[2].text = "- Set Units"; 


    settingScreen.footer[0].x =  0; settingScreen.footer[0].y = 15; settingScreen.footer[0].font = FOOTER_TEXT_FONT; settingScreen.footer[0].select = true; // N/A
    settingScreen.footer[1].x = 17; settingScreen.footer[1].y = 15; settingScreen.footer[1].font = FOOTER_TEXT_FONT; settingScreen.footer[0].select = true; // Exit
    settingScreen.footer[0].text = "Home"; 
    settingScreen.footer[1].text = "Exit";
}

void setDateScreen_Setup(void){

}



void setTimeScreen_Setup(void){

}

void setUnitScreen_Setup(void){



}

void testingScreen_Setup(void){

    menuScreen.title.text = "Testing"; 
    menuScreen.title.x = 2; menuScreen.title.y = 2; menuScreen.title.font = TITLE_TEXT_FONT; 
    menuScreen.header[0].x =  0; menuScreen.header[0].y = 0; menuScreen.header[0].font = HEADER_TEXT_FONT;  // Date
    menuScreen.header[1].x = 17; menuScreen.header[1].y = 0; menuScreen.header[1].font = HEADER_TEXT_FONT;  // Temperature
    menuScreen.header[2].x =  6; menuScreen.header[2].y = 1; menuScreen.header[2].font = HEADER_TEXT_FONT;  // Time
    decode_Date(); menuScreen.header[0].text = lcdTempString1;
    decode_Temp(); menuScreen.header[1].text = lcdTempString2;
    decode_Time(); menuScreen.header[2].text = lcdTempString3;


    // FIXME: 


    menuScreen.footer[0].x =  0; menuScreen.footer[0].y = 15; menuScreen.footer[0].font = FOOTER_TEXT_FONT;  // N/A
    menuScreen.footer[1].x = 17; menuScreen.footer[1].y = 15; menuScreen.footer[1].font = FOOTER_TEXT_FONT;  // Exit
    menuScreen.footer[0].text = "Save"; 
    menuScreen.footer[1].text = "Exit";

}

void warningScreen_Setup(void){

    menuScreen.title.text = "Warnings"; 
    menuScreen.title.x = 2; menuScreen.title.y = 2; menuScreen.title.font = TITLE_TEXT_FONT; 
    menuScreen.header[0].x =  0; menuScreen.header[0].y = 0; menuScreen.header[0].font = HEADER_TEXT_FONT;  // Date
    menuScreen.header[1].x = 17; menuScreen.header[1].y = 0; menuScreen.header[1].font = HEADER_TEXT_FONT;  // Temperature
    menuScreen.header[2].x =  6; menuScreen.header[2].y = 1; menuScreen.header[2].font = HEADER_TEXT_FONT;  // Time
    decode_Date(); menuScreen.header[0].text = lcdTempString1;
    decode_Temp(); menuScreen.header[1].text = lcdTempString2;
    decode_Time(); menuScreen.header[2].text = lcdTempString3;


    // FIXME: 


    menuScreen.footer[0].x =  0; menuScreen.footer[0].y = 15; menuScreen.footer[0].font = FOOTER_TEXT_FONT;  // N/A
    menuScreen.footer[1].x = 17; menuScreen.footer[1].y = 15; menuScreen.footer[1].font = FOOTER_TEXT_FONT;  // Exit
    menuScreen.footer[0].text = "Save"; 
    menuScreen.footer[1].text = "Exit";

}


/* Decoding from the RTC to print to the LCD */ 
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
        sprintf(lcdTempString3, "%0d:%02d:%02d PM ", readRTC.hour, readRTC.min, readRTC.sec);
    else
        sprintf(lcdTempString3, "%0d:%02d:%02d AM ", readRTC.hour, readRTC.min, readRTC.sec);
}

void decode_Speed(void){
    sprintf(lcdTempString4, "%03d MPH", (int)averageMPH);
}

/* */
