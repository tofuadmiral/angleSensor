
// filename ******** Main.C ************** 



//***********************************************************************
//    2DP4 Final Project
//    Ahmed Fuad Ali, alia78, 400075937
//***********************************************************************

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative information */
#include "SCI.H"


char string[20];
unsigned int val; 
unsigned int mode; 
long int angle;
unsigned int communicate;
unsigned int tens;
unsigned int ones;

long int castedangle = 0;
void msDelay(unsigned int);



// function prototypes

void setClock();
void delay1ms();
void OutCRLF();



// Functions 



void setClock(){
  CPMUPROT =0x26;     // disable clock write protections
  CPMUCLKS =0x80; // select bus clock source
  CPMUOSC = 0x01;  // selecting clock reference 1MHz oscillator
  
  // set our ratios of syndiv and postdiv to get clock speed proper 
  CPMUSYNR = 0x0F;  // set SYNDIV to fifteen
  CPMUPOSTDIV = 0x03;  // set POSTDIV to three 
}



void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0x20;          // toggle LED D2
}


void delay1ms(unsigned int k){      // delay function
  unsigned int i;
 
  TSCR1 = 0x90; 
  TSCR2 = 0x00; 
  TIOS = 0x01; 
  TC0 = TCNT + 4000; 
  
  for(i=0; i<k; i++){
   while(!(TFLG1_C0F));
   TC0+=4000;
  }
  
  TIOS &= 0x01; // copied from lecture
}



                            
void main(void) {           // this is our main function ----------------------
	
	// initialize things
	setClock();
	SCI_Init(19200);
	
	
	// ADC Setup
		
	ATDCTL1 = 0x25;		// set for 12-bit resolution
	ATDCTL3 = 0x88;		// right justified, one sample per sequence
	ATDCTL4 = 0x00;		// prescaler = 2; ATD clock = 6.25MHz / (2 * (2 + 1)) == 1.04MHz
	ATDCTL5 = 0x25;		// continuous conversion on channel 0


  // Data Direction Setup
  
  DDRJ = 0b1;     // output for oustanding LED      
  DDR1AD = 0b111111011111;  // set for output, other than AN5 bit 5 bc that's input
  DDRT = 0b11111;  // all as input for now  
  DDRM = 0b00;     // input for buttons 
  
  // Port Initalization
  
  PT1AD = 0x00;   // initialize our values
  PTT = 0b00000;     // not getting any input yet 
  PTM = 0b00;  // input for BUTTONS 
  PTJ = 0b0;
  
  
  // INITIALIZE our values  
  angle = 0;
  communicate = 0;  // means don't communicate
  val = 0;         
  mode=9;        // means no mode selected
  ones = 0;
  tens = 0;
  
  

  for(;;) {
  
    if((PTM & 0b01)==0b01){    // detect start serial being pressed
    
      if(communicate == 0){
        delay1ms(500);
        communicate = 1;
      } else if (communicate ==1){
        delay1ms(500);
        communicate = 0;
      }

    }
    if((PTM & 0b10) == 0b10){      // toggle modes
      if(mode==9 || mode == 1){
        mode = 0;
      } else if(mode==0){
        mode = 1;
      }
      delay1ms(500);

    }
    
    if(mode == 0 && communicate == 1){                        // mode 0 = BCD
      // start serial
      
      val = ATDDR0;
      
      if (val > 531 && val < 602){
        // now get angle and display actual value
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 43800;
        angle = angle/100;
      } 
      if (val < 531){
        angle = 0;
      }
      if(val < 626 && val > 601){
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 42400;
        angle = angle/100;
      }
      

      // now start processing BCD                 BCD ---------------
      
      castedangle = angle;
      SCI_OutUDec(castedangle); OutCRLF();      
      
      
      
      // BCD Stuff
      ones = angle % 10;
      tens = (angle - ones)/10;
      
      // now based on ones and tens, communicate to LEDs
      
      if(ones==0){
        PT1AD = 0b000000000000;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==1){
        PT1AD = 0b000000000001;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==2){
        PT1AD = 0b000000000010;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==3){
        PT1AD = 0b000000000011;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==4){
        PT1AD = 0b000000000100;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==5){     
        PT1AD = 0b000000000101;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==6){
        PT1AD = 0b000000000110;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==7){
        PT1AD = 0b000000000111;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==8){
        PT1AD = 0b000000001000;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if(ones==9){
        PT1AD = 0b000000001001;
        PTT = 0b00000;
        PTJ = 0b0;
      }     
      
      
      // now check tens digit
      if(tens==0){
        PTT = 0b00000;   // no PT1AD bc don't want to overwrite the ones
        PTJ = 0b0;
      } else if(tens==1){
        PTT = 0b00000;
        PTJ = 0b1;           // PTJ = ones place
      } else if(tens==2){
        PTT = 0b00010;       // PTT starting from 2 not 0
        PTJ = 0b0;
      } else if(tens==3){
        PTT = 0b00010;
        PTJ = 0b1;
      } else if(tens==4){
        PT1AD = 0b000000000000;
        PTT = 0b00100;  
        PTJ = 0b0;
      } else if(tens==5){
        PTT = 0b00100;
        PTJ = 0b1;
      } else if(tens==6){
        PTT = 0b00110;
        PTJ = 0b0;
      } else if(tens==7){
        PTT = 0b00110;
        PTJ = 0b1;
      } else if(tens==8){
        PTT = 0b01000;
        PTJ = 0b0;
      } else if(tens==9){
        PTT = 0b01000;
        PTJ = 0b1;
      }         
      
      
                 
            
    } else if(mode == 1 && communicate ==1){                 // mode 1 = Bar Decimal 
      /* do our linear approximation
           0 = 521
          10 = 544 
          20 = 561
          30 = 576
          40 = 588
          50 = 602
          60 = 611
          70 = 619
          80 = 622
          90 = 624
          
          our equation is 0.824*x - 438 = angle given reading
      */ 

      val = ATDDR0;
      
      if (val > 531 && val < 602){
        // now get angle and display actual value
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 43800;
        angle = angle/100;
      } 
      if (val < 531){
        angle = 0;
      }
      if(val < 626 && val > 601){
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 42400;
        angle = angle/100;
      }
      
      
      // now start processing Bar             Bar -----------------
      
      if (angle <5){
        PT1AD = 0b000000000000;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if (angle > 4 && angle <15){
        PT1AD = 0b000000000001;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if (angle > 14 && angle < 25){
        PT1AD = 0b000000000011;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if (angle > 24 && angle < 35){
        PT1AD = 0b000000000111;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if (angle > 34 && angle < 45){
        PT1AD = 0b000000001111;
        PTT = 0b00000;
        PTJ = 0b0;
      } else if (angle > 44 && angle < 55){   // one doesn't work on PTT
        PT1AD = 0b000000001111; 
        PTJ = 0b1; 
      } else if (angle > 54 && angle < 65){
        PT1AD = 0b000000001111; 
        PTT = 0b00011; 
        PTJ = 0b1; 
      } else if (angle > 64 && angle < 75){
        PT1AD = 0b000000001111; 
        PTT = 0b00111;
        PTJ = 0b1; 
      } else if (angle > 74 && angle < 85){
        PT1AD = 0b000000001111; 
        PTT = 0b01111;
        PTJ = 0b1; 
      } else if (angle > 84 && angle < 100){
        PT1AD = 0b000000001111; 
        PTT = 0b11111;
        PTJ = 0b1;
      } 
      
      
      castedangle=angle;
      SCI_OutUDec(castedangle); OutCRLF();      

    } else if(mode == 9 && communicate ==1){   // just transmit, don't show on LED
      val = ATDDR0;
      
      if (val > 531 && val < 602){
        // now get angle and display actual value
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 43800;
        angle = angle/100;
      } 
      if (val < 531){
        angle = 0;
      }
      if(val < 626 && val > 601){
        angle = val*82;
        angle = angle + (val*4)/10;
        angle = angle - 42400;
        angle = angle/100;
      }
      castedangle = angle;
      SCI_OutUDec(castedangle); OutCRLF();      
    }
    delay1ms(250);
  }
}




