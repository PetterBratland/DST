#include "allLib.c"

unsigned char mainMenu = 1;
unsigned char confClock =0;
unsigned char confTemp  =0;


void mainMenuOnScreen(void){
  
  displayPos(0,0);
  to_display_Date();
  displayPos(16,0);
  to_display_Clock();
  char text2[30] = {0};
  displayPos(0,3);
  sprintf(text2,"1. Config clock/date");
  text_2_screen(text2);
  displayPos(0,4);
  sprintf(text2,"2. Lightsensor");
  text_2_screen(text2);
  displayPos(0,5);
  sprintf(text2,"3. Config min/max temp");
  text_2_screen(text2);
  displayPos(0,6);
  sprintf(text2,"4. Live tempmeasurement");
  text_2_screen(text2);
  
  
}