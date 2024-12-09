#include "allLib.c"

void init_PWM(void){
  //Servo pins
  *AT91C_PIOB_PER = (1<<17);
  *AT91C_PIOB_PPUDR = (1<<17);
  *AT91C_PMC_PCER1 = (1<<4); //(PMC) Peripheral Clock Enable Register, PWM CONTROLLER
  
  *AT91C_PIOB_PDR = (1<<17); //Let peripheral control the pin
  *AT91C_PIOB_ABMR = (1<<17); //Activate peripheral B to control the pin
  *AT91C_PWMC_ENA = (1<<1); //Enable PWM-channel in channel 1
  *AT91C_PWMC_CH1_CMR  |= 0x05;   //Set Pre-scaler to Master_CLK/32 in PWM_CMRx
  *AT91C_PWMC_CH1_CPRDR = 52500; //Set channel period to 20ms, (32*52500)/84MHz = 20ms
  *AT91C_PWMC_CH1_CDTYR = 2625; //Set channel duty cycle to 1ms, (32*2625)/84MHz = 1ms
}

/* 1550+(degree*32) --> y = kx + m    4430 ifall 0 grader ska vara mellan ljussensor                  */
void move_servo(void){
  
  if(lightFlag){
    calculateSum();
    *AT91C_PWMC_CH1_CDTYR = 4430+(degrees*32);
    delay(15);
  }
  else if(ledFlag){
    degrees = 80;
    *AT91C_PWMC_CH1_CDTYR = 4050;
  }
}
