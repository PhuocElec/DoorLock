#include <16F877A.h>
#use delay(crystal=20000000)

#define LCD_RS_PIN      PIN_D1
#define LCD_RW_PIN      PIN_D2
#define LCD_ENABLE_PIN  PIN_D3
#define LCD_DATA4       PIN_D4
#define LCD_DATA5       PIN_D5
#define LCD_DATA6       PIN_D6
#define LCD_DATA7       PIN_D7
#include <lcd.c>

char pass_word[16]={'1','2','3','#','0','0','0','0','0','0','0','0','0','0','0','0'};

void main()
{
   lcd_init();
   while(true)
   {
      for(int i=0;i<16;i++)
      {
         write_eeprom(i,pass_word[i]);
      }
      for(int j=0;j<16;j++)
      {
         char a = read_eeprom(j);
         lcd_gotoxy(j+1,1);
         printf(lcd_putc,"%c",read_eeprom(j));
         delay_ms(100);
      }
   }
}
