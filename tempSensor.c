#include "allLib.c"

float tempV1;
unsigned int ClockU = 0;
char flagTemp = 1;
float f1;
float f2;
float dX;
char globalFlag;
float settempMax = FLT_MAX;
float settempMin = -FLT_MAX;
unsigned char tempAlarmFlag;

void init_tempsensor(void){
  *AT91C_PMC_PCER = (1<<27); //Initialize PMC - TC0
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR & 0xFFF0FFF8)|0x60000; //Select timerclock 1, LDRA: Falling, LDRB: Rising
  *AT91C_TC0_CCR = 0x5;    //Enable counter and make a sw_reset in TC0_CCR0
  //Temp pins
  *AT91C_PIOB_PER = (1<<25);
  *AT91C_PIOB_PPUDR = (1<<25);
  *AT91C_PIOB_OER = (1<<25);
  //Reset
  *AT91C_PIOB_SODR = (1<<25);
  *AT91C_PIOB_CODR = (1<<25);
  ClockU = 0;
  while(ClockU <= 160);
  *AT91C_PIOB_SODR = (1<<25);
  *AT91C_TC0_SR; //Clear old interrupt by read statusreg
  NVIC_ClearPendingIRQ(TC0_IRQn); // Clear IRQn(Interrupt Request) pending status
  NVIC_SetPriority(TC0_IRQn, 1);  // Set the priority grouping // higher or lower prio?   NVIC_EnableIRQ(TC0_IRQn);
  NVIC_EnableIRQ(TC0_IRQn);
}

void start_temp_measurement(void){
  //Create start pulse
  *AT91C_PIOB_OER = (1<<25);
  //Reset
  *AT91C_PIOB_SODR = (1<<25);
  *AT91C_PIOB_CODR = (1<<25);
  ClockU = 0;
  while(ClockU <= 160);
  *AT91C_PIOB_SODR = (1<<25);  
  //Create start pulse 
  *AT91C_PIOB_CODR = (1<<25);
  delay(25);
  *AT91C_PIOB_SODR = (1<<25);
  *AT91C_PIOB_ODR = (1<<25);
  *AT91C_TC0_CCR = (1<<2); //Software reset
  *AT91C_TC0_SR; //Clear old interrupt by read statusreg
  *AT91C_TC0_IER = (1<<6); //Enable interrupt for LDRBS
}

void TC0_Handler(void){
  *AT91C_TC0_SR;
  *AT91C_TC0_IDR = (1<<6);
  
  globalFlag = 1;
}

float tempValue(){
  if(globalFlag == 1){
    globalFlag = 0;
    f1 = ((*AT91C_TC0_RA)/42);
    f2 = ((*AT91C_TC0_RB)/42);
    dX = f2-f1;
    tempV1 = (dX/5)-273.15;
    flagTemp = 1;
  }
  return tempV1;
}

void tempAlarmReset(void){
  if((tempAlarmFlag==1)&&(Read_Key() == 10)){
    Clear_Display();
    settempMax = FLT_MAX;
    settempMin = -FLT_MAX;
    tempAlarmFlag=0;
    confTemp=1;
    while(Read_Key()!=15);
  }
}

void tempAlarm(void){
  if (((tempValue() > settempMax)||(tempValue() < settempMin))&&(confTemp==0)){
    char text[40] = {0};
    Clear_Display();
    displayPos(0,14); // Assuming this function sets the position for text display
    sprintf(text,"* Reset and config new alarm.");
    text_2_screen(text);
    while (((tempValue() > settempMax)||(tempValue() < settempMin))&&(confTemp==0)){
      tempAlarmFlag= 1;
      mainMenu = 0;
      alarmBlink();
    }
  }
}

void alarmBlink(void){
  char text[40] = {0};
  char text2[40] = {0};
  char text3[40] = {0};
  sprintf(text3,"                        "); 
  
  if(tempValue() > settempMax){
    sprintf(text,"WARNING! Temp too high!");   
  }
  else if(tempValue() < settempMin){
    sprintf(text,"WARNING! Temp too low!");
  }
  while (tempAlarmFlag== 1){
    while ((AlarmClock <=5000)){
      displayPos(3,5);
      text_2_screen(text);
      displayPos(0,14); // Assuming this function sets the position for text display
      sprintf(text2,"* Reset and config new alarm.");
      text_2_screen(text2);
    }
    if ((AlarmClock >=5000)){
      displayPos(3,5);
      text_2_screen(text3);
      displayPos(0,14); // Assuming this function sets the position for text display
      sprintf(text2,"* Reset and config new alarm.");
      text_2_screen(text2);
    }
      
    if ((AlarmClock >=10000)){
      
      AlarmClock = 0; // Reset clock
    }
    tempAlarmReset();
  } 
}


void to_display(void){
  displayPos(0,0);
  
  Write_Data_2_Display(0x34); //T
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x45); //e
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4D); //m
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x50); //p
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char str[10];
  sprintf(str,"%f",tempValue());
  for(int i = 0; i < 5; i++){
    Write_Data_2_Display(str[i]-0x20); 
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
}

void to_displayMin(void){
  
  
  Write_Data_2_Display(0x2D); //  M
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x49);  // i
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4E); // n
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char minstr[10]= {0};
  if(settempMin == -FLT_MAX){
    sprintf(minstr,"%02d",00);
    for(int i = 0; i < 2 ; i++){
      Write_Data_2_Display(minstr[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
  }
  else{
    sprintf(minstr,"%02d",(int)settempMin);
    for(int i = 0; i < 2 ; i++){
      Write_Data_2_Display(minstr[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
  }
  
  
}

void to_displayMax(void){
  
  
  Write_Data_2_Display(0x2D); //  M
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x41);  // a
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x58); // x
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char maxstr[10]= {0};
  if(settempMax == FLT_MAX){
    sprintf(maxstr,"%02d",00);
    for(int i = 0; i < 2 ; i++){
      Write_Data_2_Display(maxstr[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
  }
  else{
    sprintf(maxstr,"%02d",(int)settempMax);
    for(int i = 0; i < 2 ; i++){
      Write_Data_2_Display(maxstr[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
  }
  
}

int countKeyPressT = 0;

void config_Temp_Margin(void){
  tempAlarmFlag = 0;
  char text2[30] = {0};
  displayPos(0,0);
  sprintf(text2,"Set the Max/Min temperature.");
  text_2_screen(text2);
  
  displayPos(7,5);
  to_displayMax();
  
  displayPos(7,8);
  to_displayMin();
  
  
  displayPos(0,15);
  sprintf(text2,"*Main menu");
  text_2_screen(text2);
  
  displayPos(24,15);
  sprintf(text2,"#Reset");
  text_2_screen(text2);
  
  // Setup Max Temp
  if((Read_Key()<=9)&&(countKeyPressT==0)){
    settempMax =  Read_Key()* 10;
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPressT==0)){
    settempMax = 0;
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()<=9)&&(countKeyPressT==1)){
    settempMax = settempMax + Read_Key();
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPressT==1)){
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  
  // Setup Min Temp
  else if((Read_Key()<=9)&&(countKeyPressT==2)){
    settempMin = Read_Key() * 10;
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  else if((Read_Key()==11)&&(countKeyPressT==2)){
    settempMin = 0;
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  else if((countKeyPressT==3)&&(Read_Key()<=9)){
    settempMin = settempMin + Read_Key();
    countKeyPressT++;
    while(Read_Key()!=15);
  }  
  else if((Read_Key()==11)&&(countKeyPressT==3)){
    countKeyPressT++;
    while(Read_Key()!=15);
  }
  
  //Reset the counter
  if(countKeyPressT==4){
    countKeyPressT = 0;
  }
  
  //Reset the config
  if(Read_Key()==12){
    settempMax = FLT_MAX;
    settempMin = -FLT_MAX;
    countKeyPressT=0;
  }
  
  //Back to main menu
  if(Read_Key()==10){
    Clear_Display();
    mainMenu=1;
    confTemp=0;
    while (Read_Key()!=15);
  }
  
  
  
}  