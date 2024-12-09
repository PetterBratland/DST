#ifndef  LIGHTSENSOR_H
  #define  LIGHTSENSOR_H

extern int ClockLight;
extern unsigned char lightSens;
extern unsigned char lightFlag;
extern unsigned char darkFlag;
extern unsigned char ledFlag;
extern int ClockDark;
extern int ClockLed;
extern int degrees;

void init_light_sensor();
void ADC_Handler();
void ADC_start_measurement();
double LSensor1();
double LSensor2();
void to_display_LS1();
void to_display_LS2();
void printlight(void);
void calculateSum(void);
void Set_Led(unsigned int nLed);
void ControlLightLed(void);

#endif