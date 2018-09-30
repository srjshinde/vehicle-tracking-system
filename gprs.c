#include <LPC214x.h>

void _DelayMs(unsigned int count);
void uart0_init(void);
void Uart0PutCh (unsigned char ch);
void Uart1PutCh (unsigned char ch);
unsigned char Uart0GetCh (void);
unsigned char Uart1GetCh (void);
void  Uart0PutS(unsigned char *str);
void  Uart1PutS(unsigned char *str);
void  SendHexData1(unsigned char *str1, int StrLenCntr1);
void  SendHexData0(unsigned char *str0, int StrLenCntr0);
void _DelayMs(unsigned int count);


#define RDR      0x01
#define THRE     0x20
#define RDA      0x04
#define _9600_12_MHZ  78    //approximate
#define _9600_15_MHZ  97

#define UART0_CH_NUM   6     // dont change thiss 
#define VIC_UART0_CH   (0x01 << UART0_CH_NUM)    //don't change thiss
#define VIC_INT_CH_EN   (0x01 << 5)              //don't change thiss

#define UART1_CH_NUM   7     // dont change thiss 
#define VIC_UART1_CH   (0x01 << UART1_CH_NUM)    //don't change thiss

char InChar;
char RecWordsP1[60];
int RecWordsCntrP1 = 0;
char RecChar_Int;
   char CurrReChar;
   char PrevReChar;
   
   int GGAFound = 0,i;
   
   char WorkingString[50];
   int WorkStrWordCnt = 0;
   
   unsigned char CopyTheSring_lat[40],CopyTheSring_lon[40];
   int FillChar;

   unsigned char End[1] = {0x1A}; 


//*****************************************************************************
// Function:        int main(void)
//
// Note:            None
//*****************************************************************************
int main(void)
{
      uart0_init();               // Initialize UART module at 9600 bps
    _DelayMs(100);                  // Wait for UART module to stabilize
      _DelayMs(3000);                  

    
         Uart0PutS("AT\n"); 
         _DelayMs(50);
        Uart0PutS("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n");                                 
         _DelayMs(50);
        Uart0PutS("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"\n");                    
         _DelayMs(50);
         Uart0PutS("AT+SAPBR =1,1\n");                    
         _DelayMs(3000);
         Uart0PutS("AT+SAPBR=2,1\n");                    
         _DelayMs(1500);
         Uart0PutS("AT+HTTPINIT\n"); 
         _DelayMs(50);
          Uart0PutS("AT+HTTPSSL=1\n");                    
         _DelayMs(50);
          Uart0PutS("AT+HTTPPARA=\"CID\",1\n");                    
         _DelayMs(50);
         Uart0PutS("AT+HTTPPARA=\"URL\",\"firesimple-123.firebaseio.com/.json\"\n");
		            
         _DelayMs(50);
		  Uart0PutS("AT+HTTPPARA=\"CONTENT\",\"application/json\"\n");                    
         _DelayMs(50);

	 while (1){
		Uart0PutS("AT+HTTPDATA=33,16000\n"); 
      
					          
    
		  Uart0PutS("{\"lat\":");
   
          for(i= 0; i < 2; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 14];
         }
              SendHexData0(CopyTheSring_lat, 2);
                  _DelayMs(50);
		  
             Uart0PutS(".");
 
                for(i= 0; i < 2; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 16];
         }
               SendHexData0(CopyTheSring_lat, 2);
                  _DelayMs(50);
              for(i= 0; i < 4; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 19];
         }
               SendHexData0(CopyTheSring_lat, 4);
                  _DelayMs(50);
	 
       
             
         Uart0PutS(",\"lon\":");		 
         _DelayMs(50);
	
		 for(i= 0; i < 2; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 27];
         }
               SendHexData0(CopyTheSring_lat, 2);
                  _DelayMs(50);
	
	   
		
             Uart0PutS(".");
         _DelayMs(50);
                for(i= 0; i < 2; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 29];
         }
               SendHexData0(CopyTheSring_lat, 2);
                  _DelayMs(50);
              for(i= 0; i < 4; i++)
         {
             CopyTheSring_lat[i] = WorkingString[i + 32];
         }
               SendHexData0(CopyTheSring_lat, 4);
                  _DelayMs(50);
		  
		  Uart0PutS("}");
		
		 _DelayMs(500);
           Uart0PutS("AT+HTTPACTION=1\n"); 
          _DelayMs(3000);

	 }
		 // Uart0PutS("AT+HTTPREAD\n"); 
         // _DelayMs(2000);
		 // Uart0PutS("AT+HTTPTERM\n");
      
//_DelayMs(200);


                            
    }





 

void UART1_ISR(void) __irq
{

   if( U1IIR & RDA)              
   { 
         RecChar_Int = U1RBR;

    PrevReChar = CurrReChar;
    CurrReChar = RecChar_Int;
    
    
    if(PrevReChar == CurrReChar)
    {
      if(RecChar_Int == 'G')
      {
        GGAFound = 1;
      }
    }

   if(GGAFound == 1)
   {
     WorkingString[WorkStrWordCnt] = RecChar_Int;
    WorkStrWordCnt++;

    if(WorkStrWordCnt > 50)
    {
      GGAFound = 0;
      WorkStrWordCnt = 0;
    }

   }


   }
   VICVectAddr = 0x00;          
}



void uart0_init(void)
{
   // initialize the serial interface   
   PINSEL0 |= 0x00050005;           // Enable RxD0 and TxD0                     
   U0LCR = 0x83;                   //8 bits, no Parity, 1 Stop bit            
   U0DLL = _9600_15_MHZ;                     //9600 Baud Rate @ 12MHz Clock   
   U0LCR = 0x03;                   //DLAB = 0    

   //Enable Uart FIFO 
   U0FCR |= 0x01;

  

   U1LCR = 0x83;                   // 8 bits, no Parity, 1 Stop bit            
   U1DLL = 97;                     // 9600 Baud Rate @ 12MHz Clock   
   U1LCR = 0x03;                   // DLAB = 0    

   U1FCR |= 0x01;

   // Configure the VIC for Interrupt Handling 
   VICVectCntl7 |= (VIC_INT_CH_EN | UART1_CH_NUM);
   VICVectAddr7 = (unsigned int) UART1_ISR;
   VICIntSelect &= (~VIC_UART1_CH);   // optional : you can remove this line : This is to ensure the IRQ Mode 
   VICIntEnable |= VIC_UART1_CH;         // Enabel the Interrupt for listening request 
   // Enable UART Interrupt 
 
   U1IER   |= 0x01;      

}



void _DelayMs(unsigned int count)
{
     unsigned int j,k;
    for (j=0;j<count;j++)
   {
        for (k=0;k<6000;k++)
      {
            __asm
         {
                nop;
                nop;
            }
        }
    }
}
 void Uart0PutCh (unsigned char ch)   // Write character to Serial Port   
{    
  _DelayMs(100);                    
   while (!(U0LSR & 0x20));
     U0THR = ch;
}


unsigned char Uart0GetCh (void)      // Read character from Serial Port   
{            
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

void  Uart0PutS(unsigned char *str)    //A function to send a string on UART0
{  
   while(*str)
   {  
      Uart0PutCh(*str++);       
   }
}


void Uart1PutCh (unsigned char ch)   // Write character to Serial Port   
{                        
      U1THR = ch;
   while (!(U1LSR & 0x20));
}


unsigned char Uart1GetCh (void)      // Read character from Serial Port   
{            
  while (!(U1LSR & 0x01));
  return (U1RBR);
}

void  Uart1PutS(unsigned char *str)    //A function to send a string on UART1
{  
   while(*str)
   {  
      Uart1PutCh(*str++);       
   }
}


void  SendHexData1(unsigned char *str1, int StrLenCntr1)    //A function to send a string on UART1
{  
   int LenCntr1 = 0;
   while(LenCntr1 != StrLenCntr1)
   {  
     LenCntr1++;
      Uart1PutCh(*str1++);       
   }
}

void  SendHexData0(unsigned char *str0, int StrLenCntr0)    //A function to send a string on UART1
{  
   int LenCntr0 = 0;
   while(LenCntr0 != StrLenCntr0)
   {  
     LenCntr0++;
      Uart0PutCh(*str0++);       
   }
}