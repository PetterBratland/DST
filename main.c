#include "allLib.c"


int main()
{
  SystemInit();
  Initiate();
  Systick_Init();
  init_tempsensor();
  Init_Display();
  init_light_sensor();
  init_PWM();
  Clear_Display();
  while(1){ 
    tempAlarm();
    move_servo();
    ControlLightLed();
    if (flagTemp == 1){
      start_temp_measurement();
      flagTemp = 0;
    }
    if(mainMenu==1){
      mainMenuOnScreen();
      fastModeClick();
      if(Read_Key()==1){
        Clear_Display();
        confClock = 1;
        mainMenu = 0;
        while(Read_Key()!=15);
      }
      else if(Read_Key()==2){
        Clear_Display();
        lightSens = 1;
        mainMenu = 0;
        while(Read_Key()!=15);
      }
      else if(Read_Key()==3){
        Clear_Display();
        confTemp=1;
        mainMenu=0;
        while (Read_Key()!=15);
      }
      
    }
    if(confClock==1){
      configClock();
    }
    
    if(lightSens == 1){
      fastModeClick();
      to_display_LS1();
      to_display_LS2();
      ClockU=0;
      while(ClockU <= 2000);
      printlight();
    }
    if(confTemp==1){
    config_Temp_Margin();
    }
   
    
    /*
    fillList();
    
    to_display_Clock();
    to_display_Date();
    */
  }
  return 0;
}
