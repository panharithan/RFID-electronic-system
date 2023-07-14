#include <Keypad.h>
#include <RFID.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Servo.h> // reserved

Servo theServo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
bool start = true;
void Servo_access()
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        theServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      delay(2000);
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        theServo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// System DataStructure
const int infinity = 1000;
String members [6]= {"Siet Sophort","Mean Sareth","AN Panharith","Sam Sarom","Phan Theara","Chim Socheat"};

struct Stuff
{
  public:
  int theId;  
  String theName;
  String theGender;
  String thePosition;
  String thePass;
  Stuff()
  {
    theId=1000;
    theName="Unknown";
    theGender="none";
    thePosition="none";   
    thePass=""; 
  }
  // initial members 
   void initStuff(int ID,String Name,String  Gender,String Pos,String P )
  {
    theId = ID; theName = Name; theGender = Gender; thePosition = Pos; thePass=P;
    Serial.println("Member is added");
  }

// LCD print feature
  void PrintStuff(Stuff s, LiquidCrystal lcd)
  {
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ID ");
    lcd.setCursor(0,1);
    lcd.print(s.theId);
    delay(1000);
    lcd.clear();
  
    lcd.setCursor(0,0);
    lcd.print("Name ");
    lcd.setCursor(0,1);
    lcd.print(s.theName);
    delay(1500);
    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("Position ");
    lcd.setCursor(0,1);
    lcd.print(s.thePosition);
    delay(1500);
    lcd.clear();
  }
  
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void PrintEnterCard ( LiquidCrystal lcd)
{
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("Enter ID card   ");
    lcd.setCursor(0,1);
    lcd.print("                ");

}
void PrintText( LiquidCrystal lcd , String s)
{
  
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("S");
    lcd.setCursor(0,1);
    lcd.print("                ");
}



Stuff s1,s2,s3;
  Stuff Company[3]={};  

void AddStuff()
{
  s1.initStuff(1,"Siet Sophort","F","Leader","1111");

  s2.initStuff(2,"Mean Sareth","M","Co-leader","2222");

  s3.initStuff(3,"AN Panharith","M","Coder","3333");
  Company[0] = s1;
  Company[1] = s2;
  Company[2] = s3;
  }

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// RFID feature with LCD display
#define SS_PIN 10
#define RST_PIN 9
RFID rfid(SS_PIN,RST_PIN);

LiquidCrystal lcd (A0, A1, A2, A3, A4, A5); // pins of the LCD. (RS, E, D4, D5, D6, D7)
//String serNum;
int cards[][5] = {
  {53,33,105,208,173},
  {34,95,131,16,238}  
};

bool onceAgain = false;
int correct = 0;
int memberID = 10000;
// declare some variable for the Input Password feature
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

int count =1;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {5,4, 3,2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,8, 7};

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); 
String pass = "";

void PrintHastag(LiquidCrystal lcd)
{
  
    lcd.clear();
    lcd.setCursor(0, 0); 
      String Hastag = "";
      for (int k=0;k<count;++k)
      {
        Hastag +="#";
      }
      lcd.print(Hastag);
      count ++;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Keypad feature
void InputPassword(String P)
{
  
  char customKey = customKeypad.getKey();
  if (customKey){
    lcd.clear();
    if (customKey == '#') // input
    {
            
        // compare pass to s.thePass
        if (pass == P)
        {
          
          lcd.clear();
           lcd.begin(16,2);
          lcd.setCursor(0,0);  
          lcd.print("Access Granted");
          Servo_access(); // Access Granted => activate the servo
        }
        else
        {
          lcd.clear();
           lcd.begin(16,2);
          lcd.setCursor(0,0);  
          lcd.print("Access Denied");
          delay(2000);
          
        }
        lcd.clear();
        pass="";
        count =1;
        onceAgain = true;
        return;
    }
    else if (customKey=='*')// clear password
      {
        lcd.clear();
        pass = "";
        count = 1;
    }
    else
      {
        pass = String (pass +customKey);    
        // display # on LCD 
         PrintHastag(lcd);
    }
  }// end of if key
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
      theServo.attach(1);  // attaches the servo on pin 1 to the servo object
      Serial.begin(9600);
      SPI.begin();
      rfid.init();  
      AddStuff();
      lcd.print("Enter ID card   ");

}

void loop() {
  
  if(rfid.isCard())
      {
              Serial.print("card scanned");
  
          if(rfid.readCardSerial())
          {      
              for(int x = 0; x < sizeof(cards); x++)
              {
                for(int i = 0; i < sizeof(rfid.serNum); i++ )
                {
                    if(rfid.serNum[i] == cards[x][i])
                    {
                      correct ++;                   
                    }// end of (rfid.serNum[i] == cards[x][i])           
                    if (correct == 5)
                    {
                              lcd.begin(16,2);
                              lcd.setCursor(0,0);  
                              lcd.println("Card ID:       ");
                              lcd.setCursor(0,1);
                              memberID =x;
                              Company[x].PrintStuff(Company[x],lcd);
                              delay(500);
                              lcd.clear();
                              correct =0;  
                              
                              lcd.setCursor(0, 0);
                              lcd.print("Password");
                              lcd.setCursor(0, 1);
                              lcd.print("# Enter * Clear");
                                  while (1)
                                  {
                                            InputPassword(Company[x].thePass);
                                            if (onceAgain == true)
                                            {
                                              onceAgain = false;
                                              break;
                                            }
                                  }
                               PrintEnterCard(lcd);
                               rfid.halt(); 
                               return;
                    }// end of correct ==5
                }// end of for(int i = 0; i < sizeof(rfid.serNum); i++ )
              }// end of for(int x = 0; x < sizeof(cards); x++)
                             
                    // otherwise it's wrong card
                    lcd.begin(16,2);
                    lcd.setCursor(0,0);  
                    lcd.print(" Not Allowed");
                    delay(1000);
                    lcd.clear();
                          
          }
                   PrintEnterCard(lcd);
          }// end of if(rfid.isCard())
  
      rfid.halt(); 
         
}// end of loop()
