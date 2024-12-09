#include "allLib.c"
float lightsensor1;
float lightsensor2;
float ls1;
float ls2;
float ls1procent;
float ls2procent;
int degrees;
char flagLs1;
char flagLs2;
int status;
unsigned char lightFlag = 1;
unsigned char darkFlag = 0;
unsigned char ledFlag = 0;
unsigned char lightSens = 0;
int ClockLight;
int ClockDark;
int ClockLed;


void init_light_sensor(void){
  *AT91C_PMC_PCER1  = (1<<5);//(PMC) Peripheral Clock Enable Register, PID 37, Enables ADC interrupt
  //Lightsensor pins
  *AT91C_PIOA_PER = (1<<4);
  *AT91C_PIOA_PER = (1<<3);
  *AT91C_PIOA_PPUDR = (1<<4);
  *AT91C_PIOA_PPUDR = (1<<3);
  *AT91C_PIOA_ODR = (1<<4);
  *AT91C_PIOA_ODR = (1<<3);
  *AT91C_ADCC_CHER = (1<<1); //Enables channel 1
  *AT91C_ADCC_CHER = (1<<2); //Enables channel 2
  *AT91C_ADCC_IER = (1<<1); //Enables interrupt channel 1
  *AT91C_ADCC_IER = (1<<2); //Enables interrupt channel 2
  
  
  *AT91C_ADCC_MR = (*AT91C_ADCC_MR & 0xFFFF00FF)|0x200; //Sets the value 2 in prescaler
  
  *AT91C_ADCC_SR;
  NVIC_ClearPendingIRQ(ADC_IRQn);
  NVIC_SetPriority(ADC_IRQn,1);//sets interrupt priority to level 1
  NVIC_EnableIRQ(ADC_IRQn); //Enable interrupt
}

void ADC_Handler(void){
  status = *AT91C_ADCC_SR;
  if((status&(1<<1))==(1<<1)){
    flagLs1 = 1;
    *AT91C_ADCC_IDR  = (1<<1);
  }
  if((status&(1<<2))==(1<<2)){
    flagLs2 = 1;
    *AT91C_ADCC_IDR  = (1<<2);
  }
}

void ADC_start_measurement(void){
  *AT91C_ADCC_CR = (1<<1); //Controll register, begins analog-to-digital conversion
  *AT91C_ADCC_IER = (1<<1); //Enables interrupt channel 1
  *AT91C_ADCC_IER = (1<<2); //Enables interrupt channel 2
  while(!((*AT91C_ADCC_SR)&(1<<24))); //Wait until DRDY interrupt is set at ADC_SR
  
}

double LSensor1(){
  ADC_start_measurement();
  if(flagLs1==1){
    lightsensor1 = (*AT91C_ADCC_CDR1)*(3.3/4096);
    flagLs1=0;
  }
  return lightsensor1;
}

double LSensor2(){
  ADC_start_measurement();
  if(flagLs2==1){
    lightsensor2 = (*AT91C_ADCC_CDR2)*(3.3/4096);
    flagLs2=0;
  }
  return lightsensor2;
  
}

void Set_Led(unsigned int nLed){
  
  if (nLed == 1){
    *AT91C_PIOD_SODR = (1<<3);         
  }else {
    *AT91C_PIOD_CODR = (1<<3);
  }
  
} 

void to_display_LS1(void){
  displayPos(0,1);
  
  Write_Data_2_Display(0x33); //S
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x45); //e
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4E); //n
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x53); //s
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4F); //o
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x52); //r
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x11); //1
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  ls1= LSensor1();
  char lstr[10];
  sprintf(lstr,"%f",ls1);
  for(int i = 0; i < 5; i++){
    Write_Data_2_Display(lstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
}

void to_display_LS2(void){
  displayPos(0,2);
  
  Write_Data_2_Display(0x33); //S
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x45); //e
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4E); //n
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x53); //s
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4F); //o
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x52); //r
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x12); //2
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  
  ls2= LSensor2();
  char lstr2[10];
  sprintf(lstr2,"%f",ls2);
  for(int i = 0; i < 5 ; i++){
    Write_Data_2_Display(lstr2[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
}

void printlight(void){
  if((hourDark+hourLight+hourLed)>=24){ //Nollställer
    hourDark=0;
    hourLight=0;
    hourLed=0;
  }
  ControlLightLed();
  
  char text2[30] = {0};
  displayPos(0,15);
  sprintf(text2,"*Main menu");
  text_2_screen(text2);
  if(fastMode == 0){
    displayPos(19,15);
    sprintf(text2,"  #FastMode");
    text_2_screen(text2);
  }
  else if(fastMode == 1){
    displayPos(19,15);
    sprintf(text2,"#NormalMode");
    text_2_screen(text2);
  }
  
  
  
  displayPos(0,7);
  char degString[12]={0};
  sprintf(degString,"Degrees:%03d",degrees+90);
  for(int i = 0; i < 11 ; i++){
    Write_Data_2_Display(degString[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  displayPos(0,8);
  sprintf(text2,"Dark Hour:%03d",hourDarkTot);
  for(int i = 0; i < 13 ; i++){
    Write_Data_2_Display(text2[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  displayPos(0,9);
  sprintf(text2,"Light Hour:%03d",hourLightTot);
  for(int i = 0; i < 14 ; i++){
    Write_Data_2_Display(text2[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  displayPos(0,10);
  sprintf(text2,"Led Hour:%03d",hourLedTot);
  for(int i = 0; i < 12 ; i++){
    Write_Data_2_Display(text2[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }


  
  if(Read_Key()==10){
    Clear_Display();
    mainMenu=1;
    lightSens=0;
    while (Read_Key()!=15);
  }
}

void calculateSum(void){ // sensor 1 vänster, sensor 2 höger dvs närmre led
  ls1procent = LSensor1()/(LSensor1()+LSensor2());
  ls2procent = LSensor2()/(LSensor1()+LSensor2());
  degrees = (int)(90 * (ls1procent-ls2procent));
  if(degrees>90){
    degrees = 90;
  }
  if(degrees<-90){
    degrees = -90;
  }
  
}

char isDark=0;
char isLight=0;

void ControlLightLed(void){
  
  
  
  if ( (LSensor1() >= 0.6)&&(LSensor2() >= 0.6) || (hourLight >= 16) ){
    isDark = 1;
    isLight = 0;
  }
  else{
    isDark = 0;
    isLight = 1;
  }
  
  
  if (isDark && (hourDark >= 8)){
    Set_Led(1);
    ledFlag = 1;
    darkFlag = 0;
    lightFlag = 0;
  }
  else if(isLight && (hourLight < 16)){
    Set_Led(0);
    ledFlag = 0;
    darkFlag = 0;
    lightFlag = 1;
  }
  else if ((isDark) || !(isLight && (hourLight < 16))){
    Set_Led(0);
    ledFlag = 0;
    darkFlag = 1;
    lightFlag = 0;
  }

}