#include "allLib.c"

#define row1 (1<<5)
#define row2 (1<<2)
#define row3 (1<<3)
#define row4 (1<<4)
#define col1 (1<<8)
#define col2 (1<<9)
#define col3 (1<<7)

#define rows row1 | row2 | row3 | row4
#define cols col1 | col2 | col3

#define screen (1<<12) | (1<<13) | (1<<14) | (1<<15) | (1<<16) | (1<<17) | (1<<18) | (1<<19) //Pin 44-51
// Sentinelvärde för ingen knapptryckning

uint32_t *PIOD_AIMER = (uint32_t *)0x400E14B0;
uint32_t *PIOD_DIFSR = (uint32_t *)0x400E1484;
uint32_t *PIOD_SCDR = (uint32_t *)0x400E148C;

void Initiate(){
  *AT91C_PMC_PCER = (1<<14); //Initialize PMC - PIOD
  *AT91C_PMC_PCER = (1<<13); //Initialize PMC - PIOC
  *AT91C_PMC_PCER = (1<<12); //Initialize PMC - PIOB
  *AT91C_PMC_PCER = (1<<11); //Initialize PMC - PIOA
  
  
  *AT91C_PIOC_PER = (rows | cols); //Pin enable register rows and columns
  
  *AT91C_PIOC_PPUDR = (rows | cols); //Pull up disable register rows and columns
  
  *AT91C_PIOC_ODR = (rows | cols);
  
  *AT91C_PIOD_PER = (1<<2); //PIN 27
  *AT91C_PIOD_OER = (1<<2); //PIN 27
  *AT91C_PIOD_PPUDR = (1<<2); //PIN 27
  *AT91C_PIOD_CODR = (1<<2); //PIN 27
  
  *AT91C_PIOC_PER = screen; //Pin enable register screen
  *AT91C_PIOC_PPUDR = screen; //Pull up disable register screen
  
  *AT91C_PIOD_PER = 1; //Pin 25
  *AT91C_PIOD_OER = 1;
  *AT91C_PIOD_PPUDR = 1; //Pin 25
  
  *AT91C_PIOC_OER = (1<<18);
  *AT91C_PIOC_OER = (1<<19);
  
  *AT91C_PIOC_SODR = (1<<18);
  *AT91C_PIOC_CODR = (1<<19);
  
  *AT91C_PIOD_OER = 1;
  
  *AT91C_PIOD_PER       = (1<<3); // LED (1<<3)
  *AT91C_PIOD_OER       = (1<<3);
  *AT91C_PIOD_PPUDR     = (1<<3);
  
  *AT91C_PIOD_PER       = (1<<1); // Knapp (1<<1)
  *AT91C_PIOD_PPUDR     = (1<<1);
  *AT91C_PIOD_ODR       = (1<<1);
  *AT91C_PIOD_ISR       = (1<<1);
    
  *PIOD_AIMER           = (1<<1); 
  *AT91C_PIOD_IFER      = (1<<1); 
  *PIOD_DIFSR           = (1<<1); 
  *PIOD_SCDR            = (1<<8);
  *AT91C_PIOD_IER       = (1<<1);
  
}

void Systick_Init(){
  SysTick_Config((SystemCoreClock / 10000)-1);
}

int Read_Key(void){
  int value = 15;
  
  *AT91C_PIOD_OER = (1<<2);
  *AT91C_PIOD_CODR = (1<<2); //Clear OE KEY BUS (Active Low) 
  
  *AT91C_PIOC_SODR = cols; //All columns high
  for (int i = 0; i < 3; i++) {
    *AT91C_PIOC_OER = cols;
    switch(i) {
    case 0:
      *AT91C_PIOC_CODR = col1;
      break;
    case 1:
      *AT91C_PIOC_CODR = col2;
      break;
    case 2:
      *AT91C_PIOC_CODR = col3;
      break;
    }
    
    for(int j = 0; j < 4; j++){
      switch (j) {
      case 0:
        if ((*AT91C_PIOC_PDSR & row1) == 0) {
          value = j * 3 + i + 1;
        }
        break;
      case 1:
        if ((*AT91C_PIOC_PDSR & row2) == 0) {
          value = j * 3 + i + 1;
        }
        break;
      case 2:
        if ((*AT91C_PIOC_PDSR & row3) == 0) {
          value = j * 3 + i + 1;
        }
        break;
      case 3:
        if ((*AT91C_PIOC_PDSR & row4) == 0) {
          value = j * 3 + i + 1;
        }
        break;
      }
    }
    
    switch(i) {
    case 0:
      *AT91C_PIOC_SODR = col1;
      break;
    case 1:
      *AT91C_PIOC_SODR = col2;
      break;
    case 2:
      *AT91C_PIOC_SODR = col3;
      break;
    }
  }
  
  *AT91C_PIOC_ODR = cols;
  
  return value;
}



int readButton(void) {
  int NO_BUTTON_PRESSED = 100; 
int lastButtonState = NO_BUTTON_PRESSED;
    int currentButtonState = Read_Key();/* kod för att läsa av knappvärdet från hårdvaran */;
    if (currentButtonState != lastButtonState) {
        delay(50); // Vänta en kort stund för debouncing
        currentButtonState = Read_Key();/* läs av knappvärdet igen */;
        lastButtonState = currentButtonState;
        if (currentButtonState != NO_BUTTON_PRESSED) { // Om knappen trycks ned, returnera värdet
            return currentButtonState;
        }
    }
    return NO_BUTTON_PRESSED; // Om ingen knapp är tryckt, returnera sentinelvärdet
}
