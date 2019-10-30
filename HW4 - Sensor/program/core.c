#include <mega16.h>

#include <delay.h>
#include <stdlib.h>
// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here

// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

char String[16];
int value;
int limit0=410,limit1=410,limit2=410;
int mode = 0; //0 --> normal ,,, 1 ---> calibrate
// new
void ADC_init(){
    // initialize ADC
    DDRA=0x0;            /* Make ADC port as input */
    ADCSRA = 0x87;            /* Enable ADC, fr/128  */
    ADMUX = 0x40;            /* Vref: Avcc, ADC channel: 0 */
}
int ADC_Read(char channel)
{
    int Ain,AinLow;
    
    ADMUX=ADMUX|(channel & 0x0f);    /* Set input channel to read */

    ADCSRA |= (1<<ADSC);        /* Start conversion */
    while((ADCSRA&(1<<ADIF))==0);    /* Monitor end of conversion interrupt */
    
    delay_us(10);
    AinLow = (int)ADCL;        /* Read lower byte*/
    Ain = (int)ADCH*256;        /* Read higher 2 bits and 
                    Multiply with weight */
    Ain = Ain + AinLow;                
    return(Ain);            /* Return digital value*/
}
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here
 mode = mode ^ 1;
 PORTB = 0x00;
}
// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}

int lastMode=-1;
void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (1<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Rising Edge
// INT1: Off
// INT2: Off
GICR|=(0<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);
#asm("sei");
// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 500.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 2
// D4 - PORTB Bit 4
// D5 - PORTB Bit 5
// D6 - PORTB Bit 6
// D7 - PORTB Bit 7
// Characters/line: 16
lcd_init(16);




//ADC_Init();
//LCD_Init();            /* Initialization of LCD */
lcd_clear();
lcd_gotoxy(0,0);
lcd_puts("ADC value");    /* Write string on 1st line of LCD */

ADC_init();

//DDRC=0x07;

lcd_clear();
lcd_gotoxy(0,0);
lcd_puts("    LCD TEST    ");
lcd_gotoxy(0,1);
lcd_puts(" <<<< DONE >>>> ");
lcd_gotoxy(0,1);
//lcd_puts("START");
delay_ms(1000);

while (1)
      {
      // Place your code here
      int v0=0,v1=0,v2=0;
      ADC_init();
      v0=ADC_Read(0);
      ADC_init();
      v1=ADC_Read(1);
      ADC_init();
      v2=ADC_Read(2);
      //int v2=ADC_Read(2);
      if(mode == 0){
        if(v0>=limit0)
            PORTD.4=1;
        else
            PORTD.4=0;
        
        if(v1>=limit1)
            PORTD.5=1;
        else
            PORTD.5=0; 
            
        if(v2>=limit2)
            PORTD.6=1;
        else
            PORTD.6=0;
        if(lastMode != 0){   
            lcd_clear();            
            lcd_gotoxy(0,0);
            lcd_puts("ADC value");
        }
        lcd_gotoxy(0,1);
        lcd_puts("                ");   //cleen lcd line 0                  
        lcd_gotoxy(0,1);
        itoa(v0,String);    /* Integer to string conversion */ 
        lcd_gotoxy(0,1);
        lcd_puts(String);
        itoa(v1,String);    /* Integer to string conversion */ 
        lcd_gotoxy(5,1);
        lcd_puts(String);  
        itoa(v2,String);    /* Integer to string conversion */ 
        lcd_gotoxy(12,1);
        lcd_puts(String);                       
        delay_ms(500);
        lastMode=0;
       // mode=0;
      }else{
        if(lastMode!=1){      
            lcd_gotoxy(0,0);
            lcd_puts("                ");   //cleen lcd line 0
            lcd_gotoxy(0,0);
            lcd_puts("  Calibrating   ");
        }
        lcd_gotoxy(0,1);
        lcd_puts("                ");   //cleen lcd line 0                  
        lcd_gotoxy(0,1);         
        ADC_init();
        limit0 = ADC_Read(0);
        
        itoa(limit0,String);    /* Integer to string conversion */ 
        lcd_gotoxy(0,1);
        lcd_puts(String);
        
        ADC_init();
        limit1 = ADC_Read(1);
        
        itoa(limit1,String);    /* Integer to string conversion */ 
        lcd_gotoxy(5,1);
        lcd_puts(String);
        
        ADC_init();
        limit2 = ADC_Read(2);
        
        itoa(limit2,String);    /* Integer to string conversion */ 
        lcd_gotoxy(12,1);
        lcd_puts(String);
        
        ADC_init();
        lastMode=1;
        delay_ms(150);
        
      }
      
      
      }
}
