//Code By RADEARSR
//rsrprojek.epizy.com

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Password.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;

byte currentPasswordLength = 0;
byte maxPasswordLength     = 6;
int cursorKey              = 5;
const byte ROWS            = 4;
const byte COLS            = 4;
int buzz                   = 10; 
bool status_log            = false;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPin[ROWS] = {9, 8, 7, 6};
byte colPin[COLS] = {5, 4, 3, 2};


Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPin, colPin, ROWS, COLS);

Password password = Password("1234");

//Code Reset Arduino
void(* reset_prog) (void) = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buzz, OUTPUT);
  myservo.attach(11);
  lcd.init();
  lcd.backlight();
}

void loop() {
 char key = customKeypad.getKey();

 if ( status_log == false ){
  
   lcd.setCursor(1, 0);
   lcd.print("Enter Password");
   
 }

 if ( key != NO_KEY ){
  tone(buzz, 1500);
  delay(100); 
  noTone(buzz);
  switch ( key ){
    
    case 'A': reset_prog();break;
    case 'B': break;
    case 'C': checkPassword(); break;
    case 'D': relock(); break;
    default: prosesNumKeys( key );

    }
  
  }
}

// Tampilan LCD untuk OUTPUT Keypad
void prosesNumKeys(char key){
  lcd.setCursor(cursorKey, 1);
  lcd.print("*");
  currentPasswordLength++;
  password.append(key);
  cursorKey++;
}

// Cek Password                                                            
void checkPassword(){
  if ( password.evaluate() ){
   status_log = true;
   LcdReset(); 
   lcd.setCursor(1, 0);
   lcd.print("Password Benar");
   delay(1000);
   lcd.clear();
   myservo.write(0);
   lcd.setCursor(1, 0);
   lcd.print("Selamat Datang");
   tone(buzz, 1800);
   delay(200);
   tone(buzz, 2000);
   delay(300);
   tone(buzz, 1800);
   delay(200);
   noTone(buzz);
   lcd.setCursor(0, 1);
   lcd.print("Brankas  Terbuka");
     
  } else {
    tone(buzz, 2400);
    delay(1500);
    noTone(buzz);
    lcd.setCursor(1, 1);
    lcd.print("Password Salah");
    delay(2000);
    LcdReset();
    status_log = false;
  }
}


// Reset LCD
void LcdReset(){
  lcd.clear();
  cursorKey = 5;
  lcd.setCursor(cursorKey, 1);
  password.reset();
  currentPasswordLength = 0;
}

// Relock
void relock(){
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Terkunci Kembali");
 delay(500);
 myservo.write(90);
 tone(buzz, 1800);
 delay(200);
 noTone(buzz);
 LcdReset();
 status_log = false;
}
