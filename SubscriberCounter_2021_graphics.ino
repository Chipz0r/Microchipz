//
//	Microchipz 2021
//
//  Original code From https://www.youtube.com/watch?v=yfDWCga7iP4 Many thanks.
//
//  Youtube logo borrowed from https://www.youtube.com/watch?v=2knWVQivX_k Many thanks.
//
//  Edit the Wifi to suit your own wifi access point.
//
//  Enter your Channel API key and channelId.
//
//	Change your i2c address for your LCD.
//
//	Have fun



#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <stdio.h>

////////////////////////////////////   EDIT INFO HERE

const char* ssid     = "CHANGEME";       // Wireless Router NAME
const char* password = "CHANGEME";   // Wireless Router Password
String apiKey = "ENTER API HERE";  // YouTube Data API v3 key generated here: https://console.developers.google.com


///// Change this if you want different channels

String channelId = "ENTER CHANNEL ID HERE";  // 



////////////////////////////////////////////////////////////////

byte LT[8] = 
{
  B01111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111
};

byte UB[8] =
{
  B11111,  
  B11111,  
  B11111,  
  B00000,  
  B00000,  
  B00000,  
  B00000,  
  B00000
};
byte RT[8] =
{
  B11110,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111
};

byte LL[8] =
{
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B01111
};

byte LB[8] =
{
  B00000,  
  B00000,  
  B00000,  
  B00000,  
  B00000,  
  B11111,  
  B11111,  
  B11111
};

byte LR[8] =
{
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11111,  
  B11110
};

byte UMB[8] =
{
  B11111,  
  B11111,  
  B11111,  
  B00000,  
  B00000,  
  B00000,  
  B11111,  
  B11111
};

byte LMB[8] =
{
  B11111,  
  B00000,  
  B00000,  
  B00000,  
  B00000,  
  B11111,  
  B11111,  
  B11111
};


const char *host = "www.googleapis.com";
long subscribers,subscribersBefore = 0;


int Cycle = 10; // How many seconds before it cycles between channels


int Seconds = Cycle*1000;


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void setup()  
{
  Serial.begin(9600);
  int cursorPosition=0;

  lcd.begin(16,2); 
  lcd.setCursor(1,0);
  lcd.print("Connecting ....");

  createCustomChars();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(cursorPosition,1); 
    lcd.print(".");
    cursorPosition++;
  }
  
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Wifi");
  lcd.setCursor(3,1);
  lcd.print("Connected!");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  int length;
  String subscribersString = String(getSubscribers());
    //lcd.clear();
    DrawYoutube();
    lcd.setCursor(5, 1);
    lcd.print("   ");
    length = subscribersString.length();
    lcd.setCursor(5, 0);
    lcd.print("DISPLAY NAME"); // DISPLAY NAME HERE
    lcd.setCursor(9, 1);
    lcd.print(subscribersString);
      long x = subscribers - subscribersBefore;
      if (x > 0){
        lcd.setCursor(5, 1); 
        lcd.print("+");
        lcd.setCursor(6, 1);
        lcd.print(x);
        
    } else if ( x < 0){
      
        lcd.setCursor(5, 1);
        lcd.print(x);
      
      }
 
    //printSubscribers(length,subscribersString);
    subscribersBefore = subscribers;

  delay(Seconds);
    
}

void printSubscribers(int length, String subscribersString)
{
  switch(length)
  {
    case 5: printDigits(subscribersString,5);printDigits(subscribersString,4);printDigits(subscribersString,3);printDigits(subscribersString,2);printDigits(subscribersString,1);break;
    case 4: printDigits(subscribersString,4);printDigits(subscribersString,3);printDigits(subscribersString,2);printDigits(subscribersString,1);break;
    case 3: printDigits(subscribersString,3);printDigits(subscribersString,2);printDigits(subscribersString,1);break;
    case 2: printDigits(subscribersString,2);printDigits(subscribersString,1);break;
    case 1: printDigits(subscribersString,1);break;
    default: break;
  }
}

void printDigits(String subscribersString,int digit)
{
  int length = subscribersString.length();
  char digitToPrint = subscribersString.charAt(length-digit);
  Serial.println(digitToPrint);
  switch(digit)
  {
    case 1: printDigitToScreen(digitToPrint,17); break;
    case 2: printDigitToScreen(digitToPrint,13); break;
    case 3: printDigitToScreen(digitToPrint,9); break;
    case 4: printDigitToScreen(digitToPrint,5); break;
    case 5: printDigitToScreen(digitToPrint,1); break;
  } 
}

void printDigitToScreen(char digit,int position)
{
  int digitToPrint = digit - '0'; //Convertion from char to int
  switch(digitToPrint)
  {
    case 1:digit1(position); break;
    case 2:digit2(position); break;
    case 3:digit3(position);break;
    case 4:digit4(position); break;
    case 5:digit5(position); break;
    case 6:digit6(position); break;
    case 7:digit7(position); break;
    case 8:digit8(position); break;
    case 9:digit9(position); break;    
    case 0:digit0(position); break;    
  }
}

void digit0(int position_x)
{ 
  lcd.setCursor(position_x, 1); 
  lcd.write((byte)0);  
  lcd.write(1); 
  lcd.write(2);
  lcd.setCursor(position_x, 2);
  lcd.write(3);  
  lcd.write(4);  
  lcd.write(5);
}

void digit1(int position_x) 
{
  lcd.setCursor(position_x,1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(position_x+1,2);
  lcd.write(5);
}

void digit2(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(position_x, 2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(7);
}

void digit3(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write(6);
  lcd.write(6);
  lcd.write(2);
  lcd.setCursor(position_x,2);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5); 
}

void digit4(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write(3);
  lcd.write(4);
  lcd.write(2);
  lcd.setCursor(position_x+2,2);
  lcd.write(5);
}

void digit5(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write((byte)0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(position_x,2);
  lcd.write(7);
  lcd.write(7);
  lcd.write(5);
}

void digit6(int position_x) 
{
  lcd.setCursor(position_x,1);
  lcd.write((byte)0);
  lcd.write(6);
  lcd.write(6);
  lcd.setCursor(position_x,2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void digit7(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write(1);
  lcd.write(1);
  lcd.write(2);
  lcd.setCursor(position_x+1,2);
  lcd.write((byte)0);
}

void digit8(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write((byte)0);
  lcd.write((byte)6);
  lcd.write(2);
  lcd.setCursor(position_x,2);
  lcd.write(3);
  lcd.write(7);
  lcd.write(5);
}

void digit9(int position_x)
{
  lcd.setCursor(position_x,1);
  lcd.write((byte)0);
  lcd.write((byte)6);
  lcd.write((byte)2);
  lcd.setCursor(position_x+2,2);
  lcd.write((byte)5);
}

int getSubscribers()
{
  WiFiClientSecure client;
  Serial.print("connecting to "); Serial.println(host);
  if (!client.connect(host, 443)) {
    Serial.println("connection failed");
    return -1;
  }
  String cmd = String("GET /youtube/v3/channels?part=statistics&id=") + channelId + "&key=" + apiKey+ " HTTP/1.1\r\n" +
                "Host: " + host + "\r\nUser-Agent: ESP8266/1.1\r\nConnection: close\r\n\r\n";
  client.print(cmd);

  int repeatCounter = 10;
  while (!client.available() && repeatCounter--) {
    delay(500);
  }
  String line,buf="";
  int startJson=0;
  
  while (client.connected() && client.available()) {
    line = client.readStringUntil('\n');
    if(line[0]=='{') startJson=1;
    if(startJson) 
    {
      for(int i=0;i<line.length();i++)
        if(line[i]=='[' || line[i]==']') line[i]=' ';
      buf+=line+"\n";
    }
  }
  client.stop();

  DynamicJsonBuffer jsonBuf;
  JsonObject &root = jsonBuf.parseObject(buf);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    delay(10);
    return -1;
  }
  
  subscribers = root["items"]["statistics"]["subscriberCount"];
  Serial.println(subscribers);
  return subscribers;
}

void printfcomma (int n) {
    if (n < 0) {
        printf ("-");
        printfcomma (-n);
        return;
    }
    if (n < 1000) {
        printf ("%d", n);
        return;
    }
    printfcomma (n/1000);
    printf (",%03d", n%1000);
}

void createCustomChars()
{
  lcd.createChar(0,LT);
  lcd.createChar(1,UB);
  lcd.createChar(2,RT);
  lcd.createChar(3,LL);
  lcd.createChar(4,LB);
  lcd.createChar(5,LR);
  lcd.createChar(6,UMB);
  lcd.createChar(7,LMB);
}


void DrawYoutube()
{
  //initiate custom char for youtube
  youtubelcdcharinitiate();
  //print youtube logo
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.write(byte(1));
  lcd.setCursor(1,0);
  lcd.write(byte(2));
  lcd.setCursor(1,1);
  lcd.write(byte(3));
  lcd.setCursor(2,0);
  lcd.write(byte(4));
  lcd.setCursor(2,1);
  lcd.write(byte(5));
  lcd.setCursor(3,0);
  lcd.write(byte(6));
  lcd.setCursor(3,1);
  lcd.write(byte(7));
  
}

void youtubelcdcharinitiate()
{
    //number 0 charaters 2x2 decimals
    byte C0[8] = {0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C1[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07,0x03};
    byte C2[8] = {0x1F,0x1F,0x1B,0x19,0x18,0x18,0x18,0x18};
    byte C3[8] = {0x18,0x18,0x18,0x18,0x19,0x1B,0x1F,0x1F};
    byte C4[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07,0x03};
    byte C5[8] = {0x03,0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C6[8] = {0x18,0x1C,0x1E,0x1F,0x1F,0x1F,0x1F,0x1F};
    byte C7[8] = {0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C,0x18};
    
    lcd.createChar(0 , C0);
    lcd.createChar(1 , C1);
    lcd.createChar(2 , C2);
    lcd.createChar(3 , C3);
    lcd.createChar(4 , C4);
    lcd.createChar(5 , C5);
    lcd.createChar(6 , C6);
    lcd.createChar(7 , C7);
}
