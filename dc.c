#include<reg51.h>
#include<stdio.h>
#include<string.h>
#define LCD_data P2
#define ADC_input P1
sbit RS = P0^6;
sbit RW = P0^5;
sbit EN = P0^4;
sbit outa=P3^5;
sbit outb=P3^6;
sbit READ = P3^2;
sbit WRITE = P3^3;
sbit INTR = P3^4;

char abc[10];
int adc_in,input=0;

void delay(int ms){
   unsigned int i, j;
   for(i = 0; i<ms; i++){ // Outer for loop for given milliseconds value
      for(j = 0; j< 1275; j++){
         //execute in each milliseconds;
       }
   }
}

void LCD_cmd(unsigned char command)
{
  LCD_data = command;
  RS = 0;
	RW = 0;
	EN = 1;
	delay(100);
	EN = 0;
}

void LCD_data_write(unsigned char Data)
{
  LCD_data = Data;
  RS = 1; // Data mode
	RW = 0;
  EN = 1;
  delay(100);
	EN = 0;
    // Delay for the LCD write operation to complete
   
}

void LCD_init()
{
    LCD_cmd(0x38); // 2 lines, 5x7 matrix
    LCD_cmd(0x0E); // Display on, cursor blinking
    LCD_cmd(0x01); // Clear display
    LCD_cmd(0x80); // Move cursor to beginning of first line

}

void LCD_string(unsigned char *str)
{
  unsigned int i = 0;
	while (str[i] != '\0')
	{
		LCD_data_write(str[i]);
		i++;
	}
}

unsigned char ADC_read()
{
    unsigned char result;
    READ = 1;
    WRITE = 0;
    WRITE = 1;
    while(INTR == 1);
    READ = 0;
    result = ADC_input;
    WRITE = 0;
    return result;
}


void main(){
	int day_night=0,start=0;
	P2 = 0xFF;
	LCD_init();
	input=0;

	outa=0;
	outb=0;
	do {		 
		adc_in=ADC_read();

		if(adc_in<150){
			input=1;
		}
		else{
			input=0;
		}

		if(start!=0){
			if(day_night==1 && input==0){
				continue;
			}

			if(day_night==0 && input==1){
				continue;
			}
		}

		if(input==0){ //opens curtains
			LCD_string("Curtain Open");
			outa=1;
			outb=0;
			day_night=1;
			start=1;
			delay(1000);
			LCD_cmd(0x01);
		}
		
		if(input==1){ //close curtains
			LCD_string("Curtain close");
			outa=0;
			outb=1;
			day_night=0;
			start=1;
			delay(1000);
			LCD_cmd(0x01);
		}
		
		outa=0;
		outb=0;

	}while(1);
}