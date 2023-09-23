#include <DoorLock.h>
#define LCD_RS_PIN      PIN_D1
#define LCD_RW_PIN      PIN_D2
#define LCD_ENABLE_PIN  PIN_D3
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7
#include <lcd.c>

int i;
char enter_password[16]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

void write_eeprom_pass()
{
   for(i=0;i<16;i++)
   {
      write_eeprom(i,enter_password[i]);
   }
}

char keypad()
{  
   output_low(pin_C4);
   output_high(pin_C5);
   output_high(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '7';
   else if(input(pin_B5)==0) return '4';
   else if(input(pin_B6)==0) return '1';
   else if(input(pin_B7)==0) return '*';
   
   output_high(pin_C4);
   output_low(pin_C5);
   output_high(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '8';
   else if(input(pin_B5)==0) return '5';
   else if(input(pin_B6)==0) return '2';
   else if(input(pin_B7)==0) return '0';
   
   output_high(pin_C4);
   output_high(pin_C5);
   output_low(pin_C6);
   output_high(pin_C7);
   if(input(pin_B4)==0) return '9';
   else if(input(pin_B5)==0) return '6';
   else if(input(pin_B6)==0) return '3';
   else if(input(pin_B7)==0) return '#';
   
   output_high(pin_C4);
   output_high(pin_C5);
   output_high(pin_C6);
   output_low(pin_C7);
   if(input(pin_B4)==0) return 'A';
   else if(input(pin_B5)==0) return 'B';
   else if(input(pin_B6)==0) return 'C';
   else if(input(pin_B7)==0) return 'D';
   
   return 0xff;
}

char key_debouncing() //chong doi nut nhan
{
   char key;
   do
   {
      key = keypad(); //key = 'A'
   }
   while(key==0xff); // doi nguoi dung nhap va giu phim
   while(keypad()!=0xff); //doi cho toi khi nha phim keypad() = oxff
   return key;
}

void enter_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = key_debouncing();
      if(enter_password[i]=='#') break;
      lcd_gotoxy(i+1,2);
      printf(lcd_putc,"%c",enter_password[i]);
   }
}

void empty_pw()
{
   for(i=0;i<16;i++)
   {
      enter_password[i] = '0';
   }
}

int compare()
{
   for(i=0;i<16;i++)
   {
      if(read_eeprom(i) != enter_password[i]) return 0; // sai mat khau
   }
   return 1; //dung mat khau
}

void open()
{
   lcd_gotoxy(5,1);
   printf(lcd_putc,"Welcome");
   output_high(pin_C0);
   for(i=5;i>0;i--)
   {
      lcd_gotoxy(1,2);
      printf(lcd_putc,"Can close in %d s",i);
      delay_ms(1000);
   }
   output_low(pin_C0);
}

void main()
{
   lcd_init();
   set_tris_b(0b11111111);
   port_b_pullups(true);
   while(TRUE)
   {
      lcd_putc('\f');
      lcd_gotoxy(1,1);
      printf(lcd_putc,"* to enter PW");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"# to change PW");
      char check = key_debouncing(); //nguoi dung nhan '*' hay '#'
      if(check=='*')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter password:");
         enter_pw();
         if(compare()==1)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Correct password");
            empty_pw();
            delay_ms(2000);
            lcd_putc('\f');
            open();
         }
         else if(compare()==0)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Wrong password");
            empty_pw();
            delay_ms(2000);
         }
         
      }
      else if(check=='#')
      {
         lcd_putc('\f');
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Enter old PW:");
         enter_pw();
         if(compare()==1)
         {
            lcd_putc('\f');
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Enter new PW:");
            empty_pw(); //rong mang enter_password
            enter_pw();
            write_eeprom_pass();
            empty_pw(); //rong mang enter_password
            lcd_putc('\f');
            lcd_gotoxy(1,1);
            printf(lcd_putc,"PW has been set");
            delay_ms(2000);
         }
         else if(compare()==0)
         {
            lcd_gotoxy(1,1);
            printf(lcd_putc,"Wrong password");
            empty_pw();
            delay_ms(2000);
         }
      }
   }
}
