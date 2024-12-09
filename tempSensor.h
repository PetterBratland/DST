#ifndef  TEMPSENSOR_H
  #define  TEMPSENSOR_H

extern unsigned int ClockU;
extern char flagTemp;
extern float settempMax;
extern float settempMin;


void init_tempsensor();
void start_temp_measurement();
void TC0_Handler();
float tempValue();
void to_display();
void to_displayMin(void);
void to_displayMax(void);
void config_Temp_Margin(void);
void tempAlarm(void);
void tempAlarmReset(void);
void alarmBlink(void);

#endif