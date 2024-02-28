#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <RCSwitch.h>
#include <RH_ASK.h>

int star=0;
int com =0;
char msg[6];

bool sc1 = true;
bool sc433 = false;

int numbers[7];

int stepWrite=0;

long sspb=0;

String code,code1="",code2="",code3="",code4="";
Adafruit_SSD1306 display(-1);
RCSwitch mySwitch = RCSwitch();
RH_ASK rf_driver;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("BabakStars");
  display.display();
  delay(4000);
  display.clearDisplay();
  display.display();
  intscreen();

  pinMode(2,INPUT_PULLUP);        //Down
  //pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);        //Enter
  pinMode(5,INPUT_PULLUP);        //Ne - pr
  pinMode(6,INPUT_PULLUP);        //Up

  
  mySwitch.enableReceive(1);
  rf_driver.init();

  for(int j = 0 ; j<10 ; j++)
  {
    numbers[j]=0;
    delay(5);
  }
}

void loop() {
if(sc1)
{
    if(digitalRead(2) == LOW)
    {
      delay(60);
      star++;
      if(star > 1)
      {
        star = 0;
      }
    }
    if(digitalRead(6) == LOW)
    {
      delay(60);
      star--;
      if(star < 0)
      {
        star = 1;
      }
    }
    intscreen();
   if(digitalRead(4) == LOW)
    { delay(60);
      if(star == 0)
      {
        sc433 = true;
        //sc315 = false;
        sc1   = false;
        screen433();
      }
      if(star == 1)
      {
        //sc315 = true;
        sc433 = false;
        sc1   = false;
      }
    }
}
if(sc433)
{
  if(digitalRead(2) == LOW)
    {
      delay(60);
      star++;
      if(star > 2)
      {
        star = 0;
      }
    }
  if(digitalRead(6) == LOW)
    {
      delay(60);
      star--;
      if(star < 0)
      {
        star = 2;
      }
    }
    screen433();
  if(digitalRead(4) == LOW)
  {
    if(star == 0)
    { 
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("I'm listening");
      display.display();
      while(digitalRead(5) == HIGH)
      {
        L1:
          if (mySwitch.available())
          {
            code = String(mySwitch.getReceivedValue());
            mySwitch.resetAvailable();
            if(code1=="" & code2=="" & code3=="" & code4=="")
            {
              code1 = code;
              star = 0;
            }
            else if (code2=="" & code3=="" & code4=="" & code != code1)
            {
              code2 = code;
            }
            else if (code3=="" & code4=="" & code != code1 & code != code2)
            {
              code3 = code;
            }
            else  if (code4=="" & code != code1 & code != code2 & code != code3)
            {
              code4 = code;
            }
            else
            {
               
            }
            L2:
            display.setCursor(10,8);
            display.println(code1);
            display.setCursor(10,16);
            display.println(code2);
            display.setCursor(10,24);
            display.println(code3);
            display.setCursor(80,8);
            display.println(code4);
            RTX();
          }
          if(digitalRead(4) == LOW)
          {
            mySwitch.enableTransmit(12);
            if(code1 != "" & star == 0)
            {
              mySwitch.send(code1.toInt(), 24);
              delay(1000);
            }
            if(code2 != "" & star == 1)
            {
              mySwitch.send(code2.toInt(), 24);
              delay(1000);
            }
           if(code3 != "" & star == 2)
           {
              mySwitch.send(code3.toInt(), 24);
              delay(1000);
           }
           if(code4 != "" & star == 3)
           {
              mySwitch.send(code4.toInt(), 24);
              delay(1000);
           } 
          }
          if(digitalRead(2) == LOW)
          {
              delay(60);
              if(code1 == ""){goto L1;}
              if(code1 != "" & code2 == ""){com = 1;}
              if(code1 != "" & code2 != "" & code3 == ""){com= 2;}
              if(code1 != "" & code2 != "" & code3 != "" & code4 ==""){com= 3;}
              if(code1 != "" & code2 != "" & code3 != "" & code4 !=""){com= 4;}
              star++;
              display.clearDisplay();
              display.setCursor(0,0);
              display.println("I'm listening");
              goto L2;
          }
          if(digitalRead(6) == LOW)
          {
              delay(60);
              if(code1 == ""){goto L1;}
              if(code1 != "" & code2 == ""){com = 1;}
              if(code1 != "" & code2 != "" & code3 == ""){com= 2;}
              if(code1 != "" & code2 != "" & code3 != "" & code4 ==""){com= 3;}
              if(code1 != "" & code2 != "" & code3 != "" & code4 !=""){com= 4;}
              star--;
              display.clearDisplay();
              display.setCursor(0,0);
              display.println("I'm listening");
              goto L2;
          }
      }
    }
    if(star == 1)
    {
      sc433=false;
      writingDs();
      mySwitch.enableTransmit(12);
      S3:
        while(digitalRead(5) == HIGH)
        {
          if(digitalRead(6) == LOW)
              {
                delay(60);
                 numbers[stepWrite]++;
                if(numbers[stepWrite] > 9)
                {
                  numbers[stepWrite] = 0;
                }
                writingDs();
              }
          if(digitalRead(2) == LOW)
              {
                delay(60);
                numbers[stepWrite]--;
                if(numbers[stepWrite] < 0)
                {
                  numbers[stepWrite] = 9;
                }
                writingDs();
 
              }
              if(digitalRead(4) == LOW)
              {
                //sending
                mySwitch.send(sspb, 24);
                delay(1000);
              }
          }
          while(digitalRead(5) == LOW)
          {
            delay(60);
            if(digitalRead(4) == LOW)
            {
              sc433=true;
              return;
            }
          }
          stepWrite++;
          writingDs();
          if(stepWrite >= 7)
          {
            stepWrite = 0;
            writingDs();
            goto S3;
          }
          else
          {
            goto S3;
          }
    }
    if(star == 2)
    {
      //jamming
       tone(12,200);
       display.clearDisplay();
       display.setTextSize(1);
       display.setTextColor(WHITE);
       display.setCursor(0,0);
       display.println("Jamming");
       display.display();
       while(digitalRead(5) == HIGH)
       {
          
       }
       noTone(12);
    }
  }
}
  

}
void writingDs(){
      display.clearDisplay();
      for(int i=0;i<7;i++)
      {
        display.setCursor(5+(i*7),16);
        display.println(numbers[i]);
      }
      sspb = numbers[0]*1000000 + numbers[1] *100000 + numbers[2] *10000 + numbers[3] *1000 + numbers[4] *100 + numbers[5] *10 + numbers[6] *1;
      display.setCursor(5+(stepWrite*7),24);
      display.println("*");
      display.display();
}
void RTX(){
  if(star >= com)
  {
     star = 0;
  }
  if(star < 0)
  {
     star = com-1;
  }
  if(star == 0)
  {
    display.setCursor(0,8);
    display.println("*");
  }
  else if(star == 1)
  {
    display.setCursor(0,16);
    display.println("*");    
  }
  else if(star == 2)
  {
    display.setCursor(0,24);
    display.println("*");    
  }
  else if (star == 3)
  {
    display.setCursor(70,8);
    display.println("*");    
  }
  else
  {
    
  }
  
  display.display();
}

void screen433() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Operation");
  display.setCursor(10,8);
  display.println("Reading");
  display.setCursor(10,16);
  display.println("Writing");
  display.setCursor(10,24);
  display.println("Jamming");
  if(star == 0)
  {
    display.setCursor(0,8);
    display.println("*"); 
  }
  if(star == 1)
  {
    display.setCursor(0,16);
    display.println("*"); 
  }
  if(star == 2)
  {
    display.setCursor(0,24);
    display.println("*"); 
  }  
  display.display();
}

void intscreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Select the frequency");
  display.setCursor(10,10);
  display.println("433 MHz");
  display.setCursor(10,20);
  display.println("315 MHz"); 
  if(star == 0)
  {
    display.setCursor(0,10);
    display.println("*"); 
  }
  if(star == 1)
  {
    display.setCursor(0,20);
    display.println("*"); 
  }
  display.display();

}
