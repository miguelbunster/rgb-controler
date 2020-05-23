// PINS OF THE RGB LIGHT

const byte redLedPin = 11;
const byte greenLedPin = 10;
const byte blueLedPin = 9;


// PINS FOR THE PUSH BUTTONS
const byte upSwitchPin = 2;
const byte downSwitchPin = 4;
const byte menuSwitchPin = 7;

// DEFINED VALUE / INTERGER FOR EACH CHANNEL
byte red = 0 ;
byte green = 0;
byte blue = 0;
byte redstored;
byte greenstored;
byte bluestored;
byte counter = 0;
byte colorcounter = 0;

//Time variables for push and hold press
const long debounce = 25;
const long holdTime = 300;
const long holdTime2 = 1000;

// Button variables
//MENU
byte MenuButtonVal = HIGH;
byte MenuButtonLast = HIGH;
unsigned long MenuBtnDownTime;
unsigned long MenuBtnUpTime;
bool ignoreMenu = false;

//UP
byte UpButtonVal = HIGH;
byte UpButtonLast = HIGH;
unsigned long UpBtnDownTime;
unsigned long UpBtnUpTime;
bool ignoreUp = false;

//DOWN
byte DownButtonVal = HIGH;
byte DownButtonLast = HIGH;
unsigned long DownBtnDownTime;
unsigned long DownBtnUpTime;
bool ignoreDown = false;


// include the library code FOR LCD:
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

void setup()
{

 //HOW MANY ROWS AND COLUMNS LCD
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  
  //WHAT EACH PIN DOES WRITE OR READ
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(upSwitchPin, INPUT_PULLUP);
  pinMode(downSwitchPin, INPUT_PULLUP);
  pinMode(menuSwitchPin, INPUT_PULLUP);

//PWM HIGH SPEED SETTINGS. FIX to have 100 resolution. and find HZ.
//PIN 9-10 TIMER 1
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10) | _BV(WGM10);
  TCCR1B = _BV(CS10);
  OCR1A = 1599;
  OCR1B = 1598;

//PIN 11-3 TIMER 2
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS10);
  OCR2A = 100;
  OCR2B = 99;

/* ESTE CODIGO LO HACE REALMENTE MAS FLICKER FREE INVESTIGARLO   
 // put your setup code here, to run once:
  //Set Timer1 for around 109Khz signal
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  //Set register bits 
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(CS10) | _BV(WGM12) | _BV(WGM13);
  ICR1 = 73; // frequency = 8000000/73 = 109.589 kHz
  OCR1A = 0; //0% Duty Cycle or Full Off.
  sei();//allow interrupts
*/

}

void loop() {

  MenuButtonVal = digitalRead (menuSwitchPin);

  // Test for button pressed and store the down time
  if (MenuButtonVal == LOW && MenuButtonLast == HIGH) { // && (millis() - MenuBtnUpTime) > debounce) {
    MenuBtnDownTime = millis();
  }

  // Test for button release and store the up time quick release
  if (MenuButtonVal == HIGH && MenuButtonLast == LOW && (millis() - MenuBtnDownTime) > debounce) {
    if (ignoreMenu == false) menuEvent1();
    else ignoreMenu = false;
    MenuBtnUpTime = millis();
  }

  // Test for button held down for longer than the hold time
  if (MenuButtonVal == LOW && (millis() - MenuBtnDownTime) > holdTime)
  {
    menuEvent2();
    ignoreMenu = true;
  }

  // Test for button held down for longer than the hold time2
  if (MenuButtonVal == LOW && (millis() - MenuBtnDownTime) > holdTime2)
  {
    menuEvent3();
    ignoreMenu = true;
  }

  MenuButtonLast = MenuButtonVal;



  //Up Button Code

  UpButtonVal = digitalRead (upSwitchPin);

  // Test for button pressed and store the down time
  if (UpButtonVal == LOW && UpButtonLast == HIGH && (millis() - UpBtnUpTime) > debounce) {
    UpBtnDownTime = millis();
  }

  // Test for button release and store the up time quick release
  if (UpButtonVal == HIGH && UpButtonLast == LOW && (millis() - UpBtnDownTime) > debounce ) {
    if (ignoreUp == false) menuEvent4Up();
    else ignoreUp = false;
    UpBtnUpTime = millis();
  }

  // Test for button held down for longer than the hold time
  if (UpButtonVal == LOW && (millis() - UpBtnDownTime) > holdTime)
  {
    menuEvent5Up();
    ignoreUp = true;
  }

  // Test for button held down for longer than the hold time2
  if (UpButtonVal == LOW && (millis() - UpBtnDownTime) > holdTime2)
  {
    menuEvent6Up();
    ignoreUp = true;
  }

  UpButtonLast = UpButtonVal;

  //Down Button Code

  DownButtonVal = digitalRead (downSwitchPin);

  // Test for button pressed and store the down time
  if (DownButtonVal == LOW && DownButtonLast == HIGH && (millis() - DownBtnUpTime) > debounce) {
    DownBtnDownTime = millis();
  }

  // Test for button release and store the up time quick release
  if (DownButtonVal == HIGH && DownButtonLast == LOW && (millis() - DownBtnDownTime) > debounce ) {
    if (ignoreDown == false) menuEvent4Down();
    else ignoreDown = false;
    UpBtnUpTime = millis();
  }

  // Test for button held down for longer than the hold time
  if (DownButtonVal == LOW && (millis() - DownBtnDownTime) > holdTime)
  {
    menuEvent5Down();
    ignoreDown = true;
  }

  // Test for button held down for longer than the hold time2
  if (DownButtonVal == LOW && (millis() - DownBtnDownTime) > holdTime2)
  {
    menuEvent6Down();
    ignoreDown = true;
  }

  DownButtonLast = DownButtonVal;

  // LCD VALUE / MENU TOP LINE

  //CORRECT COLOR COUNTER
  if (counter != 4) {
    colorcounter = 0;
  }
  if (counter == 0) {
    lcd.setCursor (0, 0);
    lcd.print("RGB             ");
  }

  else if (counter == 1) {
    lcd.setCursor(0, 0);
    lcd.print("RED             ");
  }

  else if (counter == 2) {
    lcd.setCursor(0, 0);
    lcd.print("GREEN           ");
  }

  else if (counter == 3) {
    lcd.setCursor(0, 0);
    lcd.print("BLUE            ");
  }

  else if (counter == 4) {

    if (colorcounter == 0) {
      lcd.setCursor(0, 0);
      lcd.print("COLOR USER    ");

      //STORE RGB VALUES
      redstored = red;
      greenstored = green;
      bluestored = blue;
    }
    else if (colorcounter == 1) {
      lcd.setCursor(0, 0);
      lcd.print("COLOR YELLOW  ");
      red = 255;
      green = 200;
      blue = 0;
    }
    else if (colorcounter == 2) {
      lcd.setCursor(0, 0);
      lcd.print("COLOR PURPLE  ");
      red = 255;
      green = 0;
      blue = 255;
    }

    else if (colorcounter == 3) {
      lcd.setCursor(0, 0);
      lcd.print("COLOR CYAN    ");
      red = 0;
      green = 255;
      blue = 255;
    }

    else {
      colorcounter = 0;
      red = redstored;
      green = greenstored;
      blue = bluestored;
    }
  }

  else {
    counter = 0;
  }
  //LCD RGB VALUES CODE PRINTING AND ROLL OVER
  lcd.setCursor(0, 1);

  //RED
  lcd.print("R");

  if (red < 10) {
    lcd.print("00"), (red);
  }
  else if (red < 100) {
    lcd.print("0"), (red);
  }
  lcd.print(red);
  lcd.print(" ");

  //GREEN
  lcd.print("G");
  if (green < 10) {
    lcd.print("00"), (green);
  }
  else if (green < 100) {
    lcd.print("0"), (green);
  }
  lcd.print(green);
  lcd.print(" ");

  //BLUE
  lcd.print("B");
  if (blue < 10) {
    lcd.print("00"), (blue);
  }
  else if (blue < 100) {
    lcd.print("0"), (blue);
  }
  lcd.print(blue);
  lcd.print(" ");


  //LED ANALOGE WRITE
  analogWrite(redLedPin, red);
  analogWrite(greenLedPin, green);
  analogWrite(blueLedPin, blue);
  //delay(10);
}

//=================================================
// Events to trigger by click and press+hold

void menuEvent1()
{
  counter++;
  if (counter > 4) {
    counter = 0;
  }
}

void menuEvent2()
{
  red = 0;
  green = 0;
  blue = 0;
  counter = 0;
}

void menuEvent3()
{
  red = 255;
  green = 255;
  blue = 255;
  counter = 0;
}

void menuEvent4Up()
{
  if (counter == 0) {
    red++;
    green++;
    blue++;
  }

  if (counter == 1) {
    red++;
  }

  if (counter == 2) {
    green++;
  }

  if (counter == 3) {
    blue++;
  }

  if (counter == 4) {
    colorcounter++;
  }
}

void menuEvent4Down()
{
  if (counter == 0) {
    red--;
    green--;
    blue--;
  }

  else if (counter == 1) {
    red--;
  }

  else if (counter == 2) {
    green--;
  }

  else if (counter == 3) {
    blue--;
  }
  if (counter == 4) {
    colorcounter--;
  }
}

///////////////////////

void menuEvent5Up()
{
  if (counter == 0) {
    red = red + 1;
    green = green + 1;
    blue = blue + 1;
  }

  else if (counter == 1) {
    red = red + 1;
  }

  else if (counter == 2) {
    green = green + 1;
  }

  else if (counter == 3) {
    blue = blue + 1;
  }
}

void menuEvent5Down()
{
  if (counter == 0) {
    red = red - 1;
    green = green - 1;
    blue = blue - 1;
  }

  else if (counter == 1) {
    red = red - 1;
  }

  else if (counter == 2) {
    green = green - 1;
  }

  else if (counter == 3) {
    blue = blue - 1;
  }
}

////////////////////

void menuEvent6Up()
{
  if (counter == 0) {
    red = red + 2;
    green = green + 2;
    blue = blue + 2;
  }

  else if (counter == 1) {
    red = red + 2;
  }

  else if (counter == 2) {
    green = green + 2;
  }

  else if (counter == 3) {
    blue = blue + 2;
  }
}

void menuEvent6Down()
{
  if (counter == 0) {
    red = red - 2;
    green = green - 2;
    blue = blue - 2;
  }

  else if (counter == 1) {
    red = red - 2;
  }

  else if (counter == 2) {
    green = green - 2;
  }

  else if (counter == 3) {
    blue = blue - 2;
  }
}

//////////////////////
