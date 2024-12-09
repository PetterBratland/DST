#ifndef CLOCK_H
  #define CLOCK_H

  extern char fastMode;
  extern unsigned short ms;
  extern unsigned short sec;
  extern unsigned short min;
  extern unsigned short hour;
  extern unsigned short day;
  extern unsigned short month;
  extern unsigned short year;
  extern unsigned short secLight;
  extern unsigned short hourLight;
  extern unsigned short secDark;
  extern unsigned short hourDark;
  extern unsigned short secLed;
  extern unsigned short hourLed;
  extern unsigned short hourDarkTot;
  extern unsigned short hourLightTot;
  extern unsigned short hourLedTot;
  extern char tempflag;
  extern unsigned short AlarmClock;


  void fastModeClick(void);
  void SysTick_Handler(void);
  void hour_date(void);
  void to_display_Clock(void);
  void to_display_Date(void);
  void configClock(void);
  
  
#endif