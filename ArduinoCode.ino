#include <LiquidCrystal.h>
LiquidCrystal LCD_Dist(11,10,5,4,3,2); //LCD Screen that shows how far away the user is from the display
LiquidCrystal LCD_Wait(42,44,46,48,50,52); //LCD Screen that shows how many times the while loop has traversed

#define trigPin 13 //Distance sensor
#define echoPin 12 //Distance sensor

#define inputMotion 7
int pirState = LOW;
int motion = 0;

//LEDs are the lights for the while loop
#define led1 22
#define led2 24
#define led3 26
#define led4 28
#define led5 30
#define led6 32
#define led7 34
#define led8 36
#define led9 38
#define led10 40

//LEDs next to the while loop code
#define while_led1 31
#define while_led2 33
#define while_led3 35
#define while_led4 37
#define while_led5 39
#define while_led6 41
#define while_led7 43
#define while_led8 45

//LEDs that turn on at the top
#define top_led1 14
#define top_led2 15
#define top_led3 16
#define top_led4 17
#define top_led5 18
#define top_led6 19

int wait = 0; //How long the program has gone without any input
int inloop = 0; //How far in the loop is the user (shown with the while loop LEDs)
int checking = 0; //Checks to see if the user is still in the while loop (displayed on LCD_Wait row 1)
int iterations = 0; //Amount of iterations the while loop has gone through (displayed on LCD_Wait row 2)
bool go = false;
bool reset_program = false;
int noMotion = 0;
bool on = false;
bool iterating_loop = false;
bool in_dist = false; //Checks if the user is within the correct distance

int raspSignal = 0;

void setup() 
{  
  //Distance Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //While loop LEDs
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);

  //Top LEDs
  pinMode(top_led1, OUTPUT);
  pinMode(top_led2, OUTPUT);
  pinMode(top_led3, OUTPUT);
  pinMode(top_led4, OUTPUT);
  pinMode(top_led5, OUTPUT);
  pinMode(top_led6, OUTPUT);

  //While loop code LEDs
  pinMode(while_led1, OUTPUT);
  pinMode(while_led2, OUTPUT);
  pinMode(while_led3, OUTPUT);
  pinMode(while_led4, OUTPUT);

  //Motion Sensor
  pinMode(inputMotion, INPUT);
  
  Serial.begin(9600);

  /*
  LCD_Dist.begin(16,2);
  LCD_Dist.setCursor(0,0);
  LCD_Dist.print("Your Distance:  ");
  
  LCD_Wait.begin(16,2);
  LCD_Wait.setCursor(0,0);
  LCD_Dist.print("Checking Input");
  */
  
  LCD_Dist.begin(16,2);
  LCD_Dist.setCursor(0,0);
  LCD_Dist.print("                ");
  LCD_Dist.setCursor(0,1);
  LCD_Dist.print("                ");
  
  LCD_Wait.begin(16,2);
  LCD_Wait.setCursor(0,0);
  LCD_Dist.print("                ");
  LCD_Wait.setCursor(0,1);
  LCD_Dist.print("                ");

  Wire.begin(0x2A);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  /*
  digitalWrite(while_led1, HIGH);
  digitalWrite(while_led2, HIGH);
  digitalWrite(while_led3, HIGH);
  digitalWrite(while_led4, HIGH);
  */
  
  motion = digitalRead(inputMotion);

  readyPi = False;
  
  duration = pulseIn(echoPin, HIGH); //Distance the user is from the distance sensor, before conversion
  distance = (duration/2) / 74; //Converts duration to distance in inches (displayed on LCD_Dist row 2)

  if(motion == HIGH && distance > 2 && distance < 64) {
    go = true;
    noMotion = 0;
    if(on == false) {
      digitalWrite(while_led1, HIGH);
      digitalWrite(while_led2, LOW);
      digitalWrite(while_led3, LOW);
      digitalWrite(while_led4, LOW);
      delay(500);
      digitalWrite(while_led1, LOW);
      digitalWrite(while_led2, HIGH);
      digitalWrite(while_led3, LOW);
      digitalWrite(while_led4, LOW);
      delay(500);
      LCD_Dist.begin(16,2);
      LCD_Dist.setCursor(0,0);
      LCD_Dist.print("Your Distance:  ");
    }
    on = true;
  }
  else {
    noMotion++;
  }

  /*
  if(motion == HIGH && distance > 2 && distance < 64) {
    digitalWrite(while_led2, HIGH);
  }
  else {
    digitalWrite(while_led2, LOW);
  }
  if(noMotion > 1) {
    digitalWrite(while_led1, HIGH);
  }
  else {
    digitalWrite(while_led1, LOW);
  }
  */

  if(noMotion > 20) { //If the program has gone for more than 5 seconds without motion and without input (250ms * 8) that is less than 48 inches, then the program turns off
    go = false;
    if(on) {
      reset_program = true;
    }
  }

  if(go) {
    //readyPi = True;
    //raspSignal = 1;
    //Serial.println(raspSignal);
    digitalWrite(top_led1, HIGH);
    digitalWrite(top_led2, HIGH);
    digitalWrite(top_led3, HIGH);
    digitalWrite(top_led4, HIGH);
    digitalWrite(top_led5, HIGH);
    digitalWrite(top_led6, HIGH);
    if(distance < 36 && distance > 2) { //If the user is less than 36 inches from the distance sensor, then they are considered to be "in the while loop"
      in_dist = true;
      LCD_Dist.setCursor(0,1);
      LCD_Dist.print("                "); //Resets LCD_Dist, row 2, so it can print a new message
      LCD_Dist.setCursor(0,1);
      if(distance != 2) { //If the distance is 2 inches, than that is a warning that the person is too far from the sensor to get an accurate reading
        LCD_Dist.print(distance);
        LCD_Dist.print(" Inches");
      }
      wait = 0; //Input has occured so there is no need to wait
      if(inloop == 0) { //If the user was not in the while loop before, they are now
        inloop++;
      }
    }
    else {
      in_dist = false;
      if(wait > 8) { //If the program has gone for more than 2 seconds without input (250ms * 8) that is less than 36 inches, then the user is considered to be outside of the while loop
        LCD_Dist.setCursor(0,1);
        LCD_Dist.print("                ");
        LCD_Dist.setCursor(0,1);
        LCD_Dist.print("Come Closer"); //Creepy
        
        digitalWrite(while_led1, HIGH);
        digitalWrite(while_led2, LOW);
        digitalWrite(while_led3, LOW);
        digitalWrite(while_led4, LOW);
        
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH); //Distance the user is from the distance sensor, before conversion
        distance = (duration/2) / 74; //Converts duration to distance in inches (displayed on LCD_Dist row 2)

        /*
        digitalWrite(top_led1, HIGH);
        digitalWrite(top_led2, HIGH);
        digitalWrite(top_led3, HIGH);
        digitalWrite(top_led4, HIGH);
        digitalWrite(top_led5, HIGH);
        digitalWrite(top_led6, HIGH);
        */
    
        delay(500);
        digitalWrite(while_led1, LOW);
        digitalWrite(while_led2, HIGH);
        digitalWrite(while_led3, LOW);
        digitalWrite(while_led4, LOW);
        delay(500);
      }
      wait++;
    }
    
    if(inloop == 0) { //inLoop resets to 0 when the while loop LEDs have gone through 1 iteration, if that has happened then we need to check for input
      //Turns all the while loop LEDs off
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led5, LOW);
      digitalWrite(led6, LOW);
      digitalWrite(led7, LOW);
      digitalWrite(led8, LOW);
      digitalWrite(led9, LOW);
      digitalWrite(led10, LOW);
      
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("                ");
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("Checking Input"); //Shows the user that the program is checking to see if the user is less than 36 inches away
      digitalWrite(while_led1, LOW);
      digitalWrite(while_led2, LOW);
      digitalWrite(while_led3, HIGH);
      digitalWrite(while_led4, LOW);
      delay(20);
      //iterations = 0;
    }
    else if(inloop == 1) { //If the while loop just started then do the following
      digitalWrite(led10, LOW);
      digitalWrite(led1, HIGH);
      inloop++; //Increases inLoop by 1 so that the next light in the while loop can turn on
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("                ");
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("Traversing Loop"); //Shows the user that the loop has just begun to traverse
      delay(20);
      digitalWrite(while_led1, LOW);
      digitalWrite(while_led2, LOW);
      digitalWrite(while_led3, LOW);
      digitalWrite(while_led4, HIGH);
    }
    else if(inloop == 2) {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      inloop++;
    }
    else if(inloop == 3) {
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      inloop++;
    }
    else if(inloop == 4) {
      digitalWrite(led3, LOW);
      digitalWrite(led4, HIGH);
      inloop++;
    }
    else if(inloop == 5) {
      digitalWrite(led4, LOW);
      digitalWrite(led5, HIGH);
      inloop++;
    }
    else if(inloop == 6) {
      digitalWrite(led5, LOW);
      digitalWrite(led6, HIGH);
      inloop++;
    }
    else if(inloop == 7) {
      digitalWrite(led6, LOW);
      digitalWrite(led7, HIGH);
      inloop++;
    }
    else if(inloop == 8) {
      digitalWrite(led7, LOW);
      digitalWrite(led8, HIGH);
      inloop++;
    }
    else if(inloop == 9) {
      digitalWrite(led8, LOW);
      digitalWrite(led9, HIGH);
      inloop++;
    }
    else if(inloop == 10) {
      digitalWrite(led9, LOW);
      digitalWrite(led10, HIGH);
      inloop++;
      iterations++;
    }
    else {
      digitalWrite(led10, LOW);
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("                ");
      LCD_Wait.setCursor(0,0);
      LCD_Wait.print("Checking Input");
      digitalWrite(while_led1, LOW);
      digitalWrite(while_led2, LOW);
      digitalWrite(while_led3, HIGH);
      digitalWrite(while_led4, LOW);
      delay(750); //Waits .75 seconds to show the user that it is checking to see if the input is less than 36 inches
      inloop = 0;
      checking++;
      if(distance < 2 || distance > 36) {
        iterations = 0;
      }
    }
    if(checking >= 8) { //Checks if the program has been waiting for input for 2 seconds (250ms*8), if so then the program goes back to restarting the while loop
      inloop = 0;
      checking = 0;
    }
    delay(250); //Waits for .25 seconds to let the distance sensor settle
    
    LCD_Wait.setCursor(0,1);
    LCD_Wait.print("                ");
    LCD_Wait.setCursor(0,1);
    LCD_Wait.print(iterations); //Shows the number of iterations the user has gone through the while loop without leaving
    LCD_Wait.print(" Iterations");

    iterating_loop = true;
  }
  else if(reset_program) { //If the user has been away from the program (>= 64 inches) for more than 6 seconds
    iterating_loop = false; //Loop no longer iterates
    //readyPi = False;
    //raspSignal = 0;
    //Serial.println(raspSignal);
    digitalWrite(while_led1, HIGH); //Turn on the top line of the while loop code
    digitalWrite(while_led2, LOW);
    digitalWrite(while_led3, LOW);
    digitalWrite(while_led4, LOW);
    
    inloop = 0; //User is no longer in loop
    iterations = 0; //Iterations is reset to 0
    reset_program = false; //This part of the program runs once for every reset
    on = false; //The program is no longer on
    
    LCD_Dist.begin(16,2); //Clear LCD_Dist
    LCD_Dist.setCursor(0,0);
    LCD_Dist.print("                ");
    LCD_Dist.setCursor(0,1);
    LCD_Dist.print("                ");
    
    LCD_Wait.begin(16,2); //Clear LCD_Wait
    LCD_Wait.setCursor(0,0);
    LCD_Dist.print("                ");
    LCD_Wait.setCursor(0,1);
    LCD_Dist.print("                ");

    digitalWrite(led1, LOW); //Turn off loop LEDs
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);
    digitalWrite(led9, LOW);
    digitalWrite(led10, LOW);

    digitalWrite(top_led1, LOW); //Turn off top LEDs
    digitalWrite(top_led2, LOW);
    digitalWrite(top_led3, LOW);
    digitalWrite(top_led4, LOW);
    digitalWrite(top_led5, LOW);
    digitalWrite(top_led6, LOW);
  }
}
