/*
 Модуль перистальтического насоса с LCD (16х2) RTC и отдельным БП
 Время включения, и время работы программируется в скетче. 
 На экране высвечивается дата и время, срабатывание насоса и объем.
 Created 30 Dec. 2015
 by Andrey Chernyshov
 */

#include <Wire.h>
#define DS1307_I2C_ADDRESS 0x68
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
byte decToBcd(byte val)
{
return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
return ( (val/16*10) + (val%16) );
}

void setDateDs1307(
byte second, 
byte minute, 
byte hour, 
byte dayOfWeek, 
byte dayOfMonth, 
byte month, 
byte year) 
{
Wire.beginTransmission(DS1307_I2C_ADDRESS);
Wire.write(0);
Wire.write(decToBcd(second)); 
Wire.write(decToBcd(minute));
Wire.write(decToBcd(hour)); 
Wire.write(decToBcd(dayOfWeek));
Wire.write(decToBcd(dayOfMonth));
Wire.write(decToBcd(month));
Wire.write(decToBcd(year));
Wire.endTransmission();
}

void getDateDs1307(
byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{

Wire.beginTransmission(DS1307_I2C_ADDRESS);
Wire.write(0);
Wire.endTransmission();
Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

*second = bcdToDec(Wire.read() & 0x7f);
*minute = bcdToDec(Wire.read());
*hour = bcdToDec(Wire.read() & 0x3f); 
*dayOfWeek = bcdToDec(Wire.read());
*dayOfMonth = bcdToDec(Wire.read());
*month = bcdToDec(Wire.read());
*year = bcdToDec(Wire.read());
}

int motorPin1 = 10; 

int actled = 13; 

void setup() 
{
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
pinMode(motorPin1, OUTPUT);

pinMode(actled, OUTPUT);
Wire.begin();
lcd.begin(16,2);


}


void loop()
{
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
lcd.backlight();
getDateDs1307(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
lcd.print("Date: ");
lcd.print(dayOfMonth, DEC);
lcd.print("/");
lcd.print(month, DEC);
lcd.print("/");
lcd.print(year, DEC);
lcd.print(" ");

lcd.setCursor(0, 1);

lcd.print("Time: ");
lcd.print(hour, DEC);
lcd.print(":");
lcd.print(minute, DEC);
lcd.print(":");
lcd.print(second, DEC);
lcd.print("");

delay(500);
analogWrite(actled, 255);
delay(500);
lcd.clear();

if((hour == 14)&&(minute == 07)&&(second == 00))//задаем время начала работы насоса
{

lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Reeline Co Ltd");
lcd.setCursor(0, 1);
lcd.print("Pump 100ml");
lcd.print(" ");
lcd.print("*On*");
analogWrite(motorPin1, 255);
delay(10000); // 1000 = aprox 1 ml//задаем время работы насоса то есть объем жидкости
analogWrite(motorPin1, 0);
lcd.clear();
}

else{
lcd.clear();
analogWrite(motorPin1, 0);
analogWrite(actled, 0);

}

}
