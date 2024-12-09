#include "allLib.c"

char fastMode = 0;
unsigned short ms = 0;
unsigned short sec = 0;
unsigned short min = 0;
unsigned short hour = 0;
unsigned short day = 0;
unsigned short month = 0;
unsigned short year = 0;

unsigned short secLight = 0;
unsigned short hourLight = 0;
unsigned short secDark = 0;
unsigned short hourDark = 0;
unsigned short secLed = 0;
unsigned short hourLed = 0;
unsigned short hourDarkTot = 0;
unsigned short hourLightTot = 0;
unsigned short hourLedTot = 0;


unsigned short AlarmClock= 0;
char tempflag = 0;
unsigned short countKeyPress = 0;
unsigned short marker = 0;
int lastButtonPressed;

void SysTick_Handler(void){
  if(confClock==0){
    ms++;
    hour_date();
  }
  ClockU++;
  if(lightFlag == 1){
    ClockLight++;
  }
  if(darkFlag == 1){
    ClockDark++;
  }
  if(ledFlag == 1){
    ClockLed++;
  }
  AlarmClock++;
}



/* Funtion to set fastmode and normalmode */
void fastModeClick(void){
  if((fastMode==0)&&(Read_Key()==12)){ // Vroom
    fastMode = 1;
    while(Read_Key()!=15);
  }
  else if((fastMode==1)&&(Read_Key()==12)){ // no vroom :(
    fastMode = 0;
    while(Read_Key()!=15);
  }  
}


/*  Function that enumerates our timebases   */
void hour_date(void){
  if(fastMode==0){      // clock inte go vroom
    if (ms%10000==0){
      sec++;
      ms = 0;
      if(sec>=60){
        min++;
        tempflag=1;
        sec=0;
        if(min>=60){
          hour++;
          min=0;
          if(hour>=24){
            day++;
            hour=0;
            if(day>=30){
              month++;
              day=0;
              if(month>12){
                year++;
                month=0;     
              }
            }
          }
        }
      } 
    }
    if(lightFlag){
      if((ClockLight%10000==0)&&(ClockLight!=0)){
        secLight++;
        ClockLight=0;
        if(secLight>=3600){
          hourLightTot++;
          hourLight++;
          secLight=0;
        }
      }
    }
    else if(darkFlag){
      if((ClockDark%10000==0)&&(ClockDark!=0)){
        secDark++;
        ClockDark=0;
        if(secDark>=3600){
          hourDarkTot++;
          hourDark++;
          secDark=0;
        }
      }   
    }
    else if (ledFlag){
      if((ClockLed%10000==0)&&(ClockLed!=0)){
        secLed++;
        ClockLed=0;
        if(secLed>=3600){
          hourLedTot++;
          hourLed++;
          secLed=0;
        }
      }
    }
  }
  else if (fastMode==1){ // clock go vroom
    if (ms%5==0){
      sec++;
      ms = 0;
      if(sec>=60){
        min++;
        tempflag=1;
        sec=0;
        if(min>=60){
          hour++;
          min=0;
          if(hour>=24){
            day++;
            hour=0;
            if(day>=30){
              month++;
              day=0;
              if(month>12){
                year++;
                month=0;     
              }
            }
          }
        }
      } 
    }
    if(lightFlag){
      if((ClockLight%5==0)&&(ClockLight!=0)){
        secLight++;
        ClockLight=0;
        if(secLight>=3600){
          hourLightTot++;
          hourLight++;
          secLight=0;
        }
      }
    }
    else if(darkFlag){
      if((ClockDark%5==0)&&(ClockDark!=0)){
        secDark++;
        ClockDark=0;
        if(secDark>=3600){
          hourDarkTot++;
          hourDark++;
          secDark=0;
        }
      }   
    }
    else if (ledFlag){
      if((ClockLed%5==0)&&(ClockLed!=0)){
        secLed++;
        ClockLed=0;
        if(secLed>=3600){
          hourLedTot++;
          hourLed++;
          secLed=0;
        }
      }
    }
  }
}






void to_display_Clock(void){
  
  char text2[40]={0};
  
  sprintf(text2,"Clock:");
  text_2_screen(text2);
  
  /*Write_Data_2_Display(0x23); // C
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4C); //l
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4F); //o 
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x43); // c
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4B); // k
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  */
  
  char hourstr[3];
  sprintf(hourstr,"%02d",hour);
  for(int i = 0; i < 2 ; i++){
    Write_Data_2_Display(hourstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  Write_Data_2_Display(0xF); //:
  Write_Command_2_Display(0xc0); //Move to next slot  
  
  
  char minstr[3];
  sprintf(minstr,"%02d",min);
  for(int i = 0; i < 2 ; i++){
    Write_Data_2_Display(minstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  Write_Data_2_Display(0xF); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char secstr[3];
  sprintf(secstr,"%02d",sec);
  for(int i = 0; i < 2 ; i++){
    Write_Data_2_Display(secstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
}

void to_display_Date(void){
  
  Write_Data_2_Display(0x24); // D
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x41); // a
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x54); // t 
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x45); // e
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  
  
  char daystr[3]={0};
  
  sprintf(daystr,"%02d",day);
  for(int i = 0; i < 2 ; i++){
    Write_Data_2_Display(daystr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  Write_Data_2_Display(0xF); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  
  
  char monthstr[3]={'M','M','M'};
  sprintf(monthstr,"%02d",month);
  for(int i = 0; i < 2 ; i++){
    Write_Data_2_Display(monthstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
    
  }
  
  Write_Data_2_Display(0xF); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  
  char yearstr[5];
  sprintf(yearstr,"%04d",year);
  for(int i = 0; i < 4 ; i++){
    Write_Data_2_Display(yearstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  
  
  
  
}

void configClock(void){
  char text2[30] = {0};
  displayPos(0,0);
  sprintf(text2,"Config the clock/date.");
  text_2_screen(text2);
  displayPos(7,5);
  to_display_Date();
  
  displayPos(7,8);
  to_display_Clock();
  
  displayPos(0,15);
  sprintf(text2,"*Main menu");
  text_2_screen(text2);
  
  displayPos(24,15);
  sprintf(text2,"#Reset");
  text_2_screen(text2);
  
  if(countKeyPress < 8){
    displayPos(20,9);
    sprintf(text2," ");
    text_2_screen(text2);
    if(countKeyPress == 2){
      marker = 3;
    }
    if(countKeyPress == 4){
      marker = 6;
    }
    displayPos(9+marker,6);
    sprintf(text2,"   ^");
    text_2_screen(text2);
  }
  else if(countKeyPress == 8){
    marker = 0;
    displayPos(21,6);
    sprintf(text2," ");
    text_2_screen(text2);
    displayPos(10+marker,9);
    sprintf(text2,"   ^");
    text_2_screen(text2);
  }
  else{
    if(countKeyPress == 10){
      marker = 3;
    }
    if(countKeyPress == 12){
      marker = 6;
    }
    displayPos(10+marker,9);
    sprintf(text2,"   ^");
    text_2_screen(text2);
  }
  
  //Day config
  if((Read_Key()<=3)&&(countKeyPress==0)){
    day =  Read_Key()* 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==0)){
    day = 0;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((day<=20)&&((Read_Key()<=9)&&(countKeyPress==1))){
    day = day + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((day<=30)&&((Read_Key()==11)&&(countKeyPress==1))){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  else if((day == 30)&&(countKeyPress==1)&&(Read_Key()<=1)){
    day = day + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  //Month config
  else if((Read_Key()<=1)&&(countKeyPress==2)){
    month = Read_Key() * 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==2)){
    month = 0;
    marker++;
    countKeyPress++;
    while(Read_Key()!=15);
  }
  else if((month == 10)&&(countKeyPress==3)&&(Read_Key()<=2)){
    month = month + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((month == 10)&&(countKeyPress==3)&&(Read_Key()==11)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((month == 0)&&(countKeyPress==3)&&(Read_Key()<=9)){
    month = month + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  //Year config
  else if((Read_Key()<=9)&&(countKeyPress==4)){
    year = Read_Key() * 1000;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==4)){
    year = 0;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPress==5)){
    year = year + Read_Key() * 100;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==5)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPress==6)){
    year = year + Read_Key() * 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==6)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPress==7)){
    year = year + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==7)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  //Hour config
  else if((Read_Key()<=2)&&(countKeyPress==8)){
    hour = Read_Key() * 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==8)){
    hour = 0;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((hour==20)&&(countKeyPress==9)&&(Read_Key()<=3)){
    hour = hour + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((hour<=10)&&(countKeyPress==9)&&(Read_Key()<=9)){
    hour = hour + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((hour<=20)&&(countKeyPress==9)&&(Read_Key()==11)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  //Minute config
  else if((Read_Key()<=5)&&(countKeyPress==10)){
    min = Read_Key() * 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==10)){
    min = 0;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPress==11)){
    min = min + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==11)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  //Second config
  else if((Read_Key()<=5)&&(countKeyPress==12)){
    sec = Read_Key() * 10;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==12)){
    sec = 0;
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPress==13)){
    sec = sec + Read_Key();
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPress==13)){
    countKeyPress++;
    marker++;
    while(Read_Key()!=15);
  }
  
  
  //Reset the counter
  if(countKeyPress==14){
    countKeyPress = 0;
    marker=0;
  }
  
  //Reset the config
  if(Read_Key()==12){
    Clear_Display();
    day=0;
    month=0;
    year=0;
    hour=0;
    min=0;
    sec=0;
    countKeyPress=0;
    marker=0;
  }
  
  //Back to main menu
  if(Read_Key()==10){
    Clear_Display();
    mainMenu=1;
    confClock=0;
  }
}





