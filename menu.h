#ifndef MENU_H_
#define MENU_H_
#include "ST7735.h"
#include "stdbool.h"

typedef enum{
    Splash,
        Menu,
            Settings,
                SetDate,
                SetTime,
                SetUnits,
                //SetLights,
            Testing,
                Testing_1,
            Warning   
} menu_choice;

/* Structure for menu page items: location, font, and text*/
typedef struct{
    int x, y;
    int font;
    char* text;
    bool select; 
} menu_item; 

/* Structure for menu page: title, title location, header, body, footer */
typedef struct{
    menu_item title; 
    menu_item header[3];
    menu_item body  [5];
    menu_item footer[2]; 
} menu_page; 

menu_page splashScreen; 
menu_page menuScreen; 
menu_page settingScreen; 
menu_page setDateScreen;
menu_page setTimeScreen;
menu_page setUnitScreen;
menu_page testingScreen; 
menu_page warningScreen; 

void LCD_highlight(menu_choice screen, menu_page page, uint8_t item);
void LCD_header(menu_page page);
void LCD_body  (menu_page page, uint8_t item);
void LCD_footer(menu_page page);

void decode_Date(void);
void decode_Temp(void);
void decode_Time(void);

void menu_Setup(menu_choice screen); 
 
void splashScreen_Setup(void);
void menuScreen_Setup(void);
void settingScreen_Setup(void);
void setDateScreen_Setup(void);
void setTimeScreen_Setup(void);
void setUnitScreen_Setup(void);
void testingScreen_Setup(void);
void warningScreen_Setup(void);


// font: 15 x 20
/* 
Menu Structure:
    // Splashscreen:
    //     - Date          Temp
    //     1. Time
    //     2. Fuel Level Indicator
    //     3. Error Warning
    //     -               Menu

    //     Menu: 
    //         - Date Time Temp
    //         1. Settings
    //         2. Testing
    //         3. Warnings
    //         -            Exit 
            
    //         Settings: 
    //         - Date Time Temp
    //         1. Set Date
    //         2. Set Time
    //         3. Set Units
    //         -            Exit 

                Set Date: 
                - Date Time Temp
                1. DAY 
                2. MM/DD/YY
                - Set       Exit 

                Set Time: 
                - Date Time Temp
                1. HH:MM:SS AM/PM
                2. Set
                - Set       Exit 

                Set Units: 
                - Date Time Temp
                1. Temperature: C/F
                2. Speed: Mph/Kph
                3. FIXME: something here
                - Set       Exit 

            Testing: 
            - Date Time Temp
            1. Test feature 1
            ...
            n. Test feature n
            - Exit 

            Warnings: ** 
            - Date Time Temp
            1. Temperature Warnings: n 
            2. Proximity Warnings: n  
            - Exit 
*/
#endif /* MENU_H_ */
