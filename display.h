#ifndef  DISPLAY_H
  #define  DISPLAY_H
  
  void delay(int Value);
  void Init_Display();
  void displayPos(unsigned short x, unsigned short y);
  unsigned char Read_Status_Display();
  void Write_Command_2_Display(unsigned char Command);
  void Write_Data_2_Display(unsigned char Data);
  void Clear_Display();
  void text_2_screen(char *text);

#endif