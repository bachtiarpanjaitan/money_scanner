#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//define value color of money
//1k,2k,5k,10k,20k,50k,100k
const char* moneyValue[] = {"112233","000000","576065","000000","000000","665055", "535560"};
int arrayMoneylen = 7;

#define S0 3
#define S1 4
#define S2 5
#define S3 6
#define sensorOut 2

//df player define
SoftwareSerial mySoftwareSerial(7, 8); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int m1 = 12;
int m2 = 13;
int led = 11;

int uv = A0;

int frequency = 0;
int maxScanCounter = 10;
int scanCounter;
int maxColorCounter = 50;
unsigned int r,g,b;
unsigned int tempr, tempg, tempb;
int toleranceColor = 5;
boolean hasMoneyValue = false;
//unsigned int moneyValue;

//variable for uv
unsigned uvValue;
int motorForward = 90;
int motorReverse = 90;


void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(uv, INPUT);
  pinMode(led, OUTPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  //dfplayer setup
  Serial.println("Initialize DFPlayer");
  myDFPlayer.volume(28);  //Set volume value. From 0 to 30
  mySoftwareSerial.begin(38400);
  
  //Serial.begin(115200);
  Serial.begin(9600);
}

void loop() {
 unsigned colorSensorCounter;
 //start scanning money use color sensor
 do {
    //hasMoneyValue = false;
    do {
        colorSensorCounter += 1;
        //printColorSerial();
        colorSensor();
        delay(10);
     }while(colorSensorCounter < maxColorCounter );
     tempr = r / colorSensorCounter - 1;
     tempg = g / colorSensorCounter - 1;
     tempb = b / colorSensorCounter - 1;
     getMoneyValue();
     printAverageColor();  
     scanCounter += 1;
     colorSensorCounter = 0;
     //start scanning with uv
     uvMotorScan(); 
    //end scanning with uv
    Serial.println("====================");
    Serial.print("======= ");
    Serial.print(scanCounter);
    Serial.print(" ");
    Serial.print(hasMoneyValue);
    Serial.println(" ========");
    Serial.println("====================");
 }while(hasMoneyValue != true);
  scanCounter = 0;
  hasMoneyValue = false;
  delay(10000);
//end scanning money use color sensor
}

//for detect value of color sensor has on range with result counter
boolean calculateRangeColor(String money){
  String _r = "";
  String _g = "";
  String _b = "";

  int _intr = 0;
  int _intg = 0;
  int _intb = 0;
  
  _r = money.substring(0,2);
  _g = money.substring(4,2);
  _b = money.substring(4,-2);

  Serial.print(tempr);
  Serial.print(" ");
  Serial.print(tempg);
  Serial.print(" ");
  Serial.print(tempb);
  Serial.print(" | ");
  
  Serial.print(_r);
  Serial.print(" ");
  Serial.print(_g);
  Serial.print(" ");
  Serial.print(_b);
  Serial.print(" res : ");
  _intr = _r.toInt();
  _intg = _g.toInt();
  _intb = _b.toInt();

 boolean _restr, _restg, _restb;

  if((tempr >= _intr - toleranceColor) && (tempr <= _intr + toleranceColor)){_restr = true;}else {_restr = false;}
  if((tempg >= _intg - toleranceColor) && (tempg <= _intg + toleranceColor)){_restg = true;}else {_restg = false;}
  if((tempb >= _intb - toleranceColor) && (tempb <= _intb + toleranceColor)){_restb = true;}else {_restb = false;}
  if(_restr == true && _restg == true && _restb == true){
    return true;
  }else {
    return false;
  }
}


void getMoneyValue(){
  for(int i = 0; i <= arrayMoneylen - 1; i++){
    String _val = moneyValue[i];
    boolean _res = calculateRangeColor(_val);
    Serial.println(_res);
    if( _res ){
       //static unsigned long timer = millis();
      int mp3file = i + 1;
      switch(mp3file){
        case 1 :
        Serial.println("Rp.1000");
        hasMoneyValue = true;
        break;
        case 2 :
        Serial.println("Rp.2000");
        hasMoneyValue = true;
        break;
        case 3 :
        Serial.println("Rp.5000");
        hasMoneyValue = true;
        break;
        case 4 :
        Serial.println("Rp.10000");
        hasMoneyValue = true;
        break;
        case 5 :
        Serial.println("Rp.20000");
        hasMoneyValue = true;
        break;
        case 6 :
        Serial.println("Rp.50000");
        hasMoneyValue = true;
        break;
        case 7 :
        Serial.println("Rp.100000"); 
        playMp3(i + 1);
        hasMoneyValue = true;
        break;
        default :
        hasMoneyValue = false;
      }
      Serial.print("MP3 ke: ");
      Serial.println(mp3file); 
         
    }
  }
  Serial.println("----------------------------------");
}

void colorSensor(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW); //R color frequency
  r += frequency;

  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW); //G color frequency
  g += frequency;

  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW); //B color frequency
  b += frequency;
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 20;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

void printColorSerial(){
   Serial.print(r);
   Serial.print(" ");
   Serial.print(g);
   Serial.print(" ");
   Serial.print(b);
   Serial.println(" ");
}
void printAverageColor(){
   Serial.print(tempr);
   Serial.print(" ");
   Serial.print(tempg);
   Serial.print(" ");
   Serial.print(tempb);
   Serial.println();
   r= 0; g=0; b=0;
   tempr=0;tempg=0;tempb=0;
}

void uvMotorScan(){
  digitalWrite(led, HIGH);
  for(int i = 0; i <= motorForward; i++){
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    uvValue = averageAnalogRead(uv);
    //Serial.println(uvValue);
    delay(10);
  }
  digitalWrite(led, LOW);
  digitalWrite(m1, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  for(int i = 0; i <= motorReverse; i++){
    digitalWrite(m2, HIGH);
    uvValue = averageAnalogRead(uv);
    //Serial.println(uvValue);
    delay(10);
  }
  digitalWrite(led, LOW);
  digitalWrite(m2, LOW);
  uvValue = averageAnalogRead(uv);
}

void playMp3(int mp3){
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  } 
  myDFPlayer.setTimeOut(1000); //Set serial communictaion time out 500ms
  if (myDFPlayer.available()) {
    myDFPlayer.playMp3Folder(mp3);
    //myDFPlayer.play(mp3);
  }
}

