#include "allLib.c"

#define databus (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7) | (1<<8) | (1<<9) //Pin 34-41


void delay(int Value){
  for(int i=0;i<Value;i++)
    asm("nop");
} 

void Init_Display(void){
  *AT91C_PIOC_OER = (1 << 18);
  *AT91C_PIOC_OER = (1 << 19);
  
  //*AT91C_PIOC_SODR = (1 << 18); // small text
  //*AT91C_PIOC_CODR = (1 << 19); // big text
  *AT91C_PIOC_CODR = (1 << 18);//big text
  *AT91C_PIOC_CODR = (1 << 19);// small text
  
  *AT91C_PIOD_CODR= 1; // Clear reset display
  delay(10);
  *AT91C_PIOD_SODR= 1; // Set reset display
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);//Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x1e);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
} 

void displayPos(unsigned short x, unsigned short y){
  
  unsigned short pos = x + (y * 30);// CALCULATES POSITION BASED ON INPUTTED X AND Y COORDINATES
  
  Write_Data_2_Display(pos & 0xFF);//SETS LOWER ADDRESS BY MASKING THE LOWER 8 BITS OF THE INPUTTED 16 BITS
  Write_Data_2_Display(pos >> 8);  //SETS UPPER ADDRESS BY BIT SHIFTING THE UPPER 8 BITS OF THE INPUTTED 16 BITS
  Write_Command_2_Display(0x24);   //SET ADDRESS POINTER COMMAND
}

unsigned char Read_Status_Display(void){
  
  unsigned char Temp; 
  *AT91C_PIOC_ODR = databus; //Make databus input
  
  *AT91C_PIOC_OER = (1<<13); 
  *AT91C_PIOC_SODR = (1<<13); //Set dir as input
  
  *AT91C_PIOC_OER = (1<<12);
  *AT91C_PIOC_CODR = (1<<12); //Enable output
  
  *AT91C_PIOC_OER = (1<<14);
  *AT91C_PIOC_SODR = (1<<14); //Set C/D signal high
  
  *AT91C_PIOC_OER = (1<<15);
  *AT91C_PIOC_CODR = (1<<15); //Clear chip select display (CE)
  
  *AT91C_PIOC_OER = (1<<16); 
  *AT91C_PIOC_CODR = (1<<16); //Clear Read Display
  
  delay(5);
  
  Temp = *AT91C_PIOC_PDSR & 0xC;//Read databus and save it in Temp, 0xC is OK status
  
  *AT91C_PIOC_SODR = (1<<15); //Set chip select display 
  *AT91C_PIOC_SODR = (1<<16); //Set read display 
  *AT91C_PIOC_SODR = (1<<12); //Disable output 
  *AT91C_PIOC_CODR = (1<<13); //Set dir as output 
  
  *AT91C_PIOC_OER = databus;
  
  return Temp;
  
}

void Write_Command_2_Display(unsigned char Command){
  while((Read_Status_Display())!=0xc){} //Wait until Read_Status_Display returns an OK
  
  *AT91C_PIOC_OER = databus;
  *AT91C_PIOC_CODR = databus; //Clear databus
  
  *AT91C_PIOC_SODR = (Command<<2); //Set Command to databus 
  
  *AT91C_PIOC_OER = (1<<13);
  *AT91C_PIOC_CODR = (1<<13); //Set dir as output
  
  *AT91C_PIOC_OER = (1<<12);
  *AT91C_PIOC_CODR = (1<<12); //Enable output
  
  *AT91C_PIOC_OER = databus;  //Set databus as output
  
  *AT91C_PIOC_OER = (1<<14);
  *AT91C_PIOC_SODR = (1<<14); //Set C/D
  
  *AT91C_PIOC_OER = (1<<15);
  *AT91C_PIOC_CODR = (1<<15); //Clear chip select display 
  
  *AT91C_PIOC_OER = (1<<17); 
  *AT91C_PIOC_CODR = (1<<17); //Clear write display
  
  delay(5);
  
  *AT91C_PIOC_OER = (1<<15);
  *AT91C_PIOC_SODR = (1<<15); //Set chip select display (CE)
  
  *AT91C_PIOC_OER = (1<<17);
  *AT91C_PIOC_SODR = (1<<17); //Set write display
  
  *AT91C_PIOC_OER = (1<<12);
  *AT91C_PIOC_SODR = (1<<12); //Disable output
  
  *AT91C_PIOC_ODR = databus;  //Make databus as input
  
  
}

void Write_Data_2_Display(unsigned char Data){
  while(Read_Status_Display()!=0xC){} //Wait until Read_Status_Display returns an OK
  
  *AT91C_PIOC_OER = databus;
  *AT91C_PIOC_CODR = databus; //Clear databus
  
  *AT91C_PIOC_SODR = (Data<<2); //Set Data to databus 
  
  *AT91C_PIOC_CODR = (1<<13); //Set dir as output
  
  *AT91C_PIOC_CODR = (1<<12); //Enable output
  
  *AT91C_PIOC_OER = databus;  //Set databus as output
  
  *AT91C_PIOC_CODR = (1<<14); //Clear C/D signal high
  
  *AT91C_PIOC_CODR = (1<<15); //Clear chip select display
  
  *AT91C_PIOC_CODR = (1<<17); //Clear write display
  
  delay(5);
  
  *AT91C_PIOC_SODR = (1<<15); //Set chip select display
  
  *AT91C_PIOC_SODR = (1<<17); //Set write display
  
  *AT91C_PIOC_SODR = (1<<12); //Disable output
  
  *AT91C_PIOC_ODR = databus;  //Make databus as input
  
}

void Clear_Display(void) {
  Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x00); //Select start point
  Write_Command_2_Display(0x24); // start point
  for (int i = 0; i < 16*40; i++){ //The size of the screen that will be cleared
    Write_Data_2_Display(0x00); // Choose char
    Write_Command_2_Display(0xC0);//write command
  }
  Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x24); //start point
}

void text_2_screen(char *text)
{
  Write_Command_2_Display(0x80); // Text mode
  for (int i = 0; text[i] != 0; i++)
  {
    Write_Data_2_Display(text[i] - 0x20); // -0x20 to translate to correct symbol
    Write_Command_2_Display(0xc0);    // jumps to next spot on screen
  }
}

