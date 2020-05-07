#include <string>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>

byte LT[8] =
    {
        B00111,
        B01111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};
byte UB[8] =
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B00000,
        B00000};
byte RT[8] =
    {
        B11100,
        B11110,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111};
byte LL[8] =
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B01111,
        B00111};
byte LB[8] =
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111};
byte LR[8] =
    {
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11111,
        B11110,
        B11100};
byte UMB[8] =
    {
        B11111,
        B11111,
        B11111,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111};
byte LMB[8] =
    {
        B11111,
        B00000,
        B00000,
        B00000,
        B00000,
        B11111,
        B11111,
        B11111};
byte DOT[8] =
    {
        B00000,
        B00000,
        B00000,
        B00000,
        B01111,
        B01111,
        B01111,
        B01111};
byte UP[8] =
    {
        B11111,
        B10001,
        B10001,
        B10001,
        B11111,
        B00000,
        B00000,
        B00000};

void custom0(LiquidCrystal_I2C lcd, int x)
{ // uses segments to build the number 0
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void custom1(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x + 1, 1);
  // lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x + 1, 2);
  lcd.write(255);
}

void custom2(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(7);
}

void custom3(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void custom4(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
  lcd.setCursor(x + 2, 2);
  lcd.write(255);
}

void custom5(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 2);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void custom6(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void custom7(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x + 1, 2);
  lcd.write(8);
}

void custom8(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void custom9(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x + 2, 2);
  lcd.write(255);
}

void customA(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(255);
}

void customB(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(5);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(7);
  lcd.write(2);
}

void customC(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(4);
}

void customD(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(4);
  lcd.write(5);
}

void customE(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(7);
  lcd.write(7);
}

void customF(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 2);
  lcd.write(255);
}

void customG(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(1);
  lcd.write(1);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
}

void customH(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(4);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(255);
}

void customI(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(x, 2);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4);
}

void customJ(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x + 2, 1);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(4);
  lcd.write(4);
  lcd.write(5);
}

void customK(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(2);
}

void customL(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(4);
  lcd.write(4);
}

void customM(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(3);
  lcd.write(5);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
}

void customN(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(2);
  lcd.write(254);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(3);
  lcd.write(5);
}

void customO(LiquidCrystal_I2C lcd, int x)
{ // uses segments to build the number 0
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void customP(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(255);
}

void customQ(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(255);
  lcd.write(4);
}

void customR(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(x, 2);
  lcd.write(255);
  lcd.write(254);
  lcd.write(2);
}

void customS(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(8);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(x, 2);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void customT(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(1);
  lcd.write(255);
  lcd.write(1);
  lcd.setCursor(x, 2);
  lcd.write(254);
  lcd.write(255);
}

void customU(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
}

void customV(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(3);
  lcd.write(254);
  lcd.write(254);
  lcd.write(5);
  lcd.setCursor(x + 1, 2);
  lcd.write(2);
  lcd.write(8);
}

void customW(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(255);
  lcd.write(254);
  lcd.write(254);
  lcd.write(255);
  lcd.setCursor(x, 2);
  lcd.write(3);
  lcd.write(8);
  lcd.write(2);
  lcd.write(5);
}

void customX(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(x, 2);
  lcd.write(8);
  lcd.write(254);
  lcd.write(2);
}

void customY(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.setCursor(x + 1, 2);
  lcd.write(255);
}

void customZ(LiquidCrystal_I2C lcd, int x)
{
  lcd.setCursor(x, 1);
  lcd.write(1);
  lcd.write(6);
  lcd.write(5);
  lcd.setCursor(x, 2);
  lcd.write(8);
  lcd.write(7);
  lcd.write(4);
}

void int_to_custom(int i, int x, LiquidCrystal_I2C lcd)
{
  switch (i)
  {
  case 0:
    custom0(lcd, x);
    break;
  case 1:
    custom1(lcd, x);
    break;
  case 2:
    custom2(lcd, x);
    break;
  case 3:
    custom3(lcd, x);
    break;
  case 4:
    custom4(lcd, x);
    break;
  case 5:
    custom5(lcd, x);
    break;
  case 6:
    custom6(lcd, x);
    break;
  case 7:
    custom7(lcd, x);
    break;
  case 8:
    custom8(lcd, x);
    break;
  case 9:
    custom9(lcd, x);
    break;
  }
}

bool temperature(String t, LiquidCrystal_I2C lcd, int x)
{
  const char *buffer = t.c_str();
  //for(int i = 0; i < strlen)
  float temp = atof(buffer);
  temp *= 10;
  temp = (int)temp;
  int a, b, c;
  a = temp / 100;
  b = (temp - a * 100) / 10;
  c = temp - a * 100 - b * 10;
  int_to_custom(a, x, lcd);
  int_to_custom(b, x + 4, lcd);
  lcd.setCursor(x + 8, 2);
  lcd.write(4);
  int_to_custom(c, x + 10, lcd);
  lcd.setCursor(x + 14, 1);
  lcd.write(1);
  customC(lcd, x + 15);
  if (temp >= 375)
    return true;
  else
    return false;
}
void display_ready(LiquidCrystal_I2C lcd)
{
  lcd.setCursor(0, 0);
  lcd.print("********************");
  //lcd.setCursor(4,1);
  customR(lcd, 1);
  customE(lcd, 5);
  customA(lcd, 9);
  customD(lcd, 13);
  customY(lcd, 17);
  lcd.setCursor(0, 3);
  lcd.print("********************");
}
void display_card(LiquidCrystal_I2C lcd)
{
  lcd.setCursor(0, 0);
  lcd.print("Please attach your");
  //lcd.setCursor(4,1);
  customC(lcd, 2);
  customA(lcd, 6);
  customR(lcd, 10);
  customD(lcd, 14);
  lcd.setCursor(0, 3);
  lcd.print("on the right side");
}

void display_temp(LiquidCrystal_I2C lcd)
{
  lcd.setCursor(0, 0);
  lcd.print("Please measure your");
  //lcd.setCursor(4,1);
  customT(lcd, 2);
  customE(lcd, 6);
  customM(lcd, 10);
  customP(lcd, 15);
  lcd.setCursor(0, 3);
  lcd.print("on the upper right");
}

void display_err(LiquidCrystal_I2C lcd)
{
  customE(lcd, 0);
  customR(lcd, 4);
  customR(lcd, 8);
  customO(lcd, 12);
  customR(lcd, 16);
}

void lcd_display(String input, char mode, LiquidCrystal_I2C lcd, int row, int col)
{
  lcd.createChar(8, LT);
  lcd.createChar(1, UB);
  lcd.createChar(2, RT);
  lcd.createChar(3, LL);
  lcd.createChar(4, LB);
  lcd.createChar(5, LR);
  lcd.createChar(6, UMB);
  lcd.createChar(7, LMB);
  if (mode == 'm')
  {
    lcd.setCursor(col, row);
    lcd.print(input);
  }
  else if (mode == 't')
  {
    lcd.setCursor(0, 0);
    lcd.print("Body Temperature:");
    if (temperature(input, lcd, 1))
    {
      lcd.setCursor(0, 3);
      lcd.print("Fever!");
    }
  }
  else if (mode == 'c')
  {
    display_card(lcd);
  }
  else if (mode == 'b')
  {
    display_temp(lcd);
  }
  else if (mode == 'e')
  {
    display_err(lcd);
  }
}

void lcd_display_tmp(float tmp, LiquidCrystal_I2C lcd)
{
  char tmp_str[4];
  snprintf(tmp_str, 5, "%.1f", tmp);
  lcd_display(tmp_str, 't', lcd, 0, 0);
}