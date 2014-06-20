#ifndef _OLED_H_
#define _OLED_H_



/*Initialize the OLED module*/
void LCD_Init( void );

/*flush the whole screen*/
void LCD_Fill(unsigned char bmp_dat);

/*Display a character*/
void LCD_Dis_Char( unsigned char page, unsigned char column, char ch );

/*Display a string*/
void LCD_Dis_Str( unsigned char page, unsigned char column, char *str );

void LCD_Dis_Logo( void );

#endif //_OLED_H_
