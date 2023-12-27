
//---------- constantes pantalla--------------------------
#define SELECT_KEY   1
#define LEFT_KEY     2
#define DOWN_KEY     3
#define UP_KEY       4
#define RIGHT_KEY    5
#include <LiquidCrystal.h>
const int bl= 23, en=22, rs=1, d7=3, d6=21, d5=19, d4=18, btns = 34;
//const int rs=D1, en=D2, d4=D4, d5=D5, d6=D6, d7=D7;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7); //select the pins used on the LCD panel

//--------- constantes sensores---------------------------
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

// Variables Sensores
const int LC_DO_1 = 35;
const int LC_SCK_1 = 32;

HX711 scale_1;

int menuPos = 0;
int menuLev = 0;
int AUX = 0;

typedef void (*GenericFP)();
struct Menu {
  GenericFP funcion;
  const char * const Display;
};

//--------------------------------------------FUNCIONES GENERALES--------------------------------------------------------//

void LED_Toggle(int cicles){
  for (int i = 0; i == cicles; i++){
    digitalWrite(2, HIGH);  //LED PIN SET HIGH
    delay(250);              // 1 SEC DELAY
    digitalWrite(2, LOW);   //LED PIN SET LOW
    delay(2500);
  }
}

void write_lcd(int txt){
  lcd.clear();
  lcd.print(">"); lcd.print(txt);

}

//-------------------------------------------FIN FUNCIONES GENERALES-------------------------------------------------//
//---------------------------------------------- LECTURA DE BOTONES---------------------------------------------//
int btnValue = 0;

int getKeyID(){
  int aRead = analogRead(btns);
  int key = 0;
  if( aRead > 3900  ) return 0;                //no key is pressed 700
  if(aRead <= 3900) {
    key = antiRebote(aRead);
    return key;
    }
}

int antiRebote(int aRead){
  int newRead = analogRead(btns);
  int oldRead = aRead;
  int key = 0;
  int i = 0;

  while ((((oldRead * 1.1) <= newRead) || ((oldRead * 0.9) >= newRead)) && (newRead > 30)){
    oldRead = newRead;
    newRead = analogRead(btns);
  }

  if( newRead > 3000  ) key = SELECT_KEY;      //1 select key 600
  else if( newRead > 1800  ) key = LEFT_KEY;        //2 left key 500
  else if( newRead > 800   ) key = DOWN_KEY;        //3 right key 300
  else if( newRead > 100   ) key = UP_KEY;          //4 up key 150
  else key = RIGHT_KEY;       //5 right key

  while (analogRead(btns) < 3900){
    lcd.clear();
    delay(100);
  }
  return key;
}
//-------------------------------------------------FIN LECTURA BOTONES----------------------------------------------------//

//---------------------------------------- FUNCIONES DEL MENU-------------------------------------------------------//
void Settings(){
  lcd.clear();
  lcd.print(">"); lcd.print("Settings");
  lcd.setCursor(0, 1); lcd.print( "<<<<<>>>>>" );
  switch (getKeyID()){
    case LEFT_KEY:
      break;
    default:
      delay(100);
      Settings();
  };
}
void Reset(){
  lcd.clear();
  lcd.print(">"); lcd.print("Reset");
  lcd.setCursor(0, 1); lcd.print( "<<<<<>>>>>" );
  switch (getKeyID()){
    case LEFT_KEY:
      break;
    default:
      delay(100);
      Reset();
  };
}
void Main_State(){
  lcd.clear();
  lcd.print(">"); lcd.print("Main State");
  lcd.setCursor(0, 1); lcd.print( "<<<<<>>>>>" );
  switch (getKeyID()){
    case LEFT_KEY:
      break;
    default:
      delay(100);
      Main_State();
  };
}
void Values(){
  if (scale_1.is_ready()){

    scale_1.set_gain(128);
    long value_1a = scale_1.get_units(10)/10000;

    scale_1.set_gain(32);
    long value_1b = scale_1.get_units(10)/10000;

    lcd.clear();
    lcd.print(">"); lcd.print("Values");
    lcd.setCursor(0, 1); lcd.print(value_1a); lcd.print("<>"); lcd.print(value_1b);
  }
  else {
    lcd.clear();
    lcd.print(">"); lcd.print("Values");
    lcd.setCursor(0, 1); lcd.print("HX711 not found.");
  }

  switch (getKeyID()){
    case LEFT_KEY:
      break;
    default:
      delay(100);
      Values();
  };
}
//------------------------FIN FUNCIONES MENU----------------------------------//

Menu Main_Menu[] = {
  {&Settings, "Settings"},
  {&Reset, "Reset"},
  {&Values, "Values"},
  {&Main_State, "Main State"}
};

void setup() {

  lcd.begin( 16, 2 );
  scale_1.begin(LC_DO_1, LC_SCK_1);
}

void loop() {
  lcd.clear();

  lcd.print(">"); lcd.print( Main_Menu[ menuPos ].Display );
  lcd.setCursor(0, 1); lcd.print( Main_Menu[ menuPos + 1].Display );
  lcd.setCursor(12, 1); lcd.print(sizeof(Main_Menu));

  switch (getKeyID()){
    case UP_KEY:
      menuPos--;
      break;
    case DOWN_KEY:
      menuPos++;
      break;
    case SELECT_KEY:
      Main_Menu[menuPos].funcion();
      menuPos = 0;
      break;
    default:
      break;
  };

  if( menuPos < 0 ) menuPos = 0;
  if( menuPos > 3 ) menuPos = 3;
  delay(100);

}
