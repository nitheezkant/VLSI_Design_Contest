#include <Wire.h> //This is for i2C
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

boolean newData = false;
char receivedChar;

double degAngle0, degAngle1, degAngle2, degAngle3, degAngle4; //raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber0, quadrantNumber1, quadrantNumber2, quadrantNumber3, quadrantNumber4;
int previousquadrantNumber0 = 0, previousquadrantNumber1 = 0, previousquadrantNumber2 = 0, previousquadrantNumber3 = 0, previousquadrantNumber4 = 0; //quadrant IDs
double numberofTurns0 = 0, numberofTurns1 = 0, numberofTurns2 = 0, numberofTurns3 = 0, numberofTurns4 = 0; //number of turns
double correctedAngle0 = 0, correctedAngle1 = 0, correctedAngle2 = 0, correctedAngle3 = 0, correctedAngle4 = 0; //tared angle - based on the startup value
double prevcorrectedAngle0 = 0, prevcorrectedAngle1 = 0, prevcorrectedAngle2 = 0, prevcorrectedAngle3 = 0, prevcorrectedAngle4 = 0; //tared angle - based on the startup value
double startAngle0 = 0, startAngle1 = 0, startAngle2 = 0, startAngle3 = 0, startAngle4 = 0; //starting angle
double totalAngle0 = 0, totalAngle1 = 0, totalAngle2 = 0, totalAngle3 = 0, totalAngle4 = 0, totalX = 0; //total absolute angular displacement
double previoustotalAngle0 = 0, previoustotalAngle1 = 0, previoustotalAngle2 = 0, previoustotalAngle3 = 0, previoustotalAngle4 = 0; //for the display printing
double initAngle0 = 0, initAngle1 = 0, initAngle2 = 0, initAngle3 = 0, initAngle4 = 0, initX = 0;

long w_angle = 0;
long s_angle = 0;
long d_angle = 0;
long a_angle = 0;
long cw_angle = 0;
long ccw_angle = 0;
long p_angle = 30;
long q_angle = 32;
long x_angle = 30;
long i_angle = 10;

int ratio = 6;

int RPWM_Output1 = 3; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output1 = 2; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output2 = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output2 = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output3 = 7; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output3 = 8; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output4 = 9; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output4 = 10; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

int RPWM_Output5 = 11; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output5 = 12;// Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

const int pingPin = 52; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 53; // Echo Pin of Ultrasonic Sensor

char ipt;
int X, Y, O;
int flag, upflag;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool setflag;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  //Serial.print(bus);
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds/29/2;
}

void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) {
//        Serial.print("This just in ... ");
//        Serial.println(receivedChar);
        newData = false;
    }
}

void setup()
{
  Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
  Wire.begin(); //start i2C  
  Wire.setClock(800000L); //fast clock
  Serial.println("Hello");
  setflag = true;

//  TCA9548A(4);
//  byte status = mpu.begin();
////  Serial.print(F("MPU6050 1 status: "));
////  Serial.println(status);
//  while(status!=0){ } // stop everything if could not connect to MPU6050
//  
////  Serial.println(F("Calculating offsets, do not move MPU6050 1"));
//  delay(500);
//  mpu.calcOffsets(true,true);
  
  checkMagnetPresence(0);
  checkMagnetPresence(1); //check the magnet (blocks until magnet is found)
  checkMagnetPresence(2);
  checkMagnetPresence(3);
//  checkMagnetPresence(4);

  ReadRawAngle(0); //update startAngle with degAngle - for taring
  startAngle0 = degAngle0; //update startAngle with degAngle - for taring

  ReadRawAngle(1); //update startAngle with degAngle - for taring
  startAngle1 = degAngle1; //update startAngle with degAngle - for taring

  ReadRawAngle(2); //update startAngle with degAngle - for taring
  startAngle2 = degAngle2; //update startAngle with degAngle - for taring

  ReadRawAngle(3); //update startAngle with degAngle - for taring
  startAngle3 = degAngle3; //update startAngle with degAngle - for taring

//  ReadRawAngle(4); //update startAngle with degAngle - for taring
//  startAngle4 = degAngle4; //update startAngle with degAngle - for taring

  initAngle0 = totalAngle0;
  initAngle1 = totalAngle1;
  initAngle2 = totalAngle2;
  initAngle3 = totalAngle3;
  initAngle4 = totalAngle4;
  initX = totalX;
  
  pinMode(RPWM_Output1, OUTPUT);
  pinMode(LPWM_Output1, OUTPUT);
  pinMode(RPWM_Output2, OUTPUT);
  pinMode(LPWM_Output2, OUTPUT);
  pinMode(RPWM_Output3, OUTPUT);
  pinMode(LPWM_Output3, OUTPUT);
  pinMode(RPWM_Output4, OUTPUT);
  pinMode(LPWM_Output4, OUTPUT);
  pinMode(RPWM_Output5, OUTPUT);
  pinMode(LPWM_Output5, OUTPUT);

  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);

  setMotor(511, LPWM_Output1, RPWM_Output1);
  setMotor(511, LPWM_Output2, RPWM_Output2);
  setMotor(511, LPWM_Output3, RPWM_Output3);
  setMotor(511, LPWM_Output4, RPWM_Output4);
  setMotor(511, LPWM_Output5, RPWM_Output5);
}

void loop(){
  ///////////////////////////////////////////////////////POSITION SETUP////////////////////////////////////////////////////////////////
  if(setflag){
    Serial.println("Enter '(X, Y)' Coordinates  and Orientation angle 'O' with spaces and w.r.t. current position");  
    while (Serial.available()==0){}
    X = Serial.parseInt();
    while (Serial.available()==0){}
    Y = Serial.parseInt();
    while (Serial.available()==0){}
    O = Serial.parseInt();
//    recvOneChar();
//    X = atoi(receivedChar);
//    showNewData();
//    recvOneChar();
//    Y = atoi(receivedChar);
//    showNewData();
//    recvOneChar();
//    O = atoi(receivedChar);
//    showNewData();
    Serial.print(X);
    Serial.print(" ");
    Serial.print(Y);
    Serial.print(" ");
    Serial.println(O);

    ///////////////////// X = Y = O = 6 means 1 turn of the wheel /////////////////////////////

    if(Y > 0)
      w_angle = 5 * Y;
    else
      s_angle = 5 * abs(Y);

    if(X > 0)
      d_angle = 5 * X;
    else
      a_angle = 5 * abs(X);

    if(O > 0)
      ccw_angle = 5 * O;
    else
      cw_angle = 5 * abs(O);
    
  if(w_angle > 0)
    {
      Serial.println("In case w");
      Serial.print("w_angle: ");
      Serial.println(2160*w_angle);
      Serial.print("TotInit: ");
      Serial.println(abs(totalAngle1-initAngle1));
      while(abs(totalAngle1-initAngle1) <= 2160*w_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);          
          setMotor(511, LPWM_Output1, RPWM_Output1);
          setMotor(600, LPWM_Output2, RPWM_Output2);
          setMotor(422, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0 = 0;
      initAngle1 = totalAngle1 = 0;
      initAngle2 = totalAngle2 = 0;
      w_angle = 0;
    }
    
  else if(s_angle > 0)
    { 
      Serial.println("In case s");
      Serial.print("s_angle: ");
      Serial.println(s_angle);
      Serial.print("TotInit: ");
      Serial.print(initAngle1); 
      Serial.print(" ");
      Serial.println(totalAngle1);
      while(abs(totalAngle1-initAngle1) <= 2160*s_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);
          setMotor(511, LPWM_Output1, RPWM_Output1);
          setMotor(422, LPWM_Output2, RPWM_Output2);
          setMotor(600, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0 = 0;
      initAngle1 = totalAngle1 = 0;
      initAngle2 = totalAngle2 = 0;
      s_angle = 0;
    }
    delay(100);
  
  if(a_angle > 0)
    {
      Serial.println("In case a");
      Serial.print("a_angle: ");
      Serial.println(a_angle);
      Serial.print("TotInit: ");
      Serial.print(initAngle1); 
      Serial.print(" ");
      Serial.println(totalAngle1);
      while(abs(totalAngle1-initAngle1) <= 2160*a_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);
          setMotor(333, LPWM_Output1, RPWM_Output1);
          setMotor(600, LPWM_Output2, RPWM_Output2);
          setMotor(600, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0 = 0;
      initAngle1 = totalAngle1 = 0;
      initAngle2 = totalAngle2 = 0;
      a_angle = 0;
    }

  else if(d_angle > 0)
    {
      Serial.println("In case d");
      Serial.print("d_angle: ");
      Serial.println(d_angle);
      Serial.print("TotInit: ");
      Serial.print(initAngle1); 
      Serial.print(" ");
      Serial.println(totalAngle1);
      while(abs(totalAngle1-initAngle1) <= 2160*d_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);
          setMotor(689, LPWM_Output1, RPWM_Output1);
          setMotor(422, LPWM_Output2, RPWM_Output2);
          setMotor(422, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0 = 0;
      initAngle1 = totalAngle1 = 0;
      initAngle2 = totalAngle2 = 0;
      d_angle = 0;
    }
    delay(100);

    if(cw_angle > 0){
      Serial.println("In case cw");
      Serial.print("cw_angle: ");
      Serial.println(cw_angle);
      Serial.print("TotInit: ");
      Serial.print(initAngle1); 
      Serial.print(" ");
      Serial.println(totalAngle1);
      while(abs(totalAngle1-initAngle1) <= 2160*cw_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);
          setMotor(600, LPWM_Output1, RPWM_Output1);
          setMotor(600, LPWM_Output2, RPWM_Output2);
          setMotor(600, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      cw_angle = 0;
    }
    else if(ccw_angle>0){
      Serial.println("In case ccw");
      Serial.print("ccw_angle: ");
      Serial.println(ccw_angle);
      Serial.print("TotInit: ");
      Serial.print(initAngle1); 
      Serial.print(" ");
      Serial.println(totalAngle1);
      while(abs(totalAngle1-initAngle1) <= 2160*ccw_angle)
        {
          Serial.print("In ");
          Serial.print(initAngle1); 
          Serial.print(" ");
          Serial.println(totalAngle1);
          setMotor(422, LPWM_Output1, RPWM_Output1);
          setMotor(422, LPWM_Output2, RPWM_Output2);
          setMotor(422, LPWM_Output3, RPWM_Output3);
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      
      Serial.println(" Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      ccw_angle = 0;
    }
    delay(100);

   if(X == 0 && Y == 0 && O == 0)                                                      // ** EMERGENCY STOP **
    {  
      Serial.println(" **STOP** ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);
    }
    delay(100);
    Serial.println("Do you wish to set this as the Base Rest Position? (Enter 1 for YES and 0 for NO)");
    Serial.println("1");
    Serial.println("Enter the exercise you want to perform (Exercise No. 1-8):");
    while (Serial.available()==0){}
    flag = Serial.parseInt();
    if(flag==1)
      setflag = false;
  }
  ////////////////////////////////////////////FIXED EXERCISES/////////////////////////////////////////////
  else{
//    if(Serial.available()){
//        ipt = Serial.read();
//    }
    while (Serial.available()==0){}
    ipt = Serial.parseInt();
    switch(ipt)
    {
      case '1':
      /////////////////////////UP-DOWN////////////////////////////////////////////
      upflag = 0;
      TCA9548A(4);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
      setMotor(800, LPWM_Output4, RPWM_Output4);
//      while(abs(totalAngle3-initAngle3) <= 2160*p_angle)
//        {
//          if(abs(totalAngle3-initAngle3)>=30240 && upflag==0){
//            setMotor(650, LPWM_Output5, RPWM_Output5);
//            upflag = 1;
//          }
////          Serial.print("Read start ");
////          Serial.println(totalAngle3);
//          ReadRawAngle(3); //update startAngle with degAngle - for taring
////          Serial.print("Correct start ");
////          Serial.println(totalAngle3);
//          correctAngle(3);
////          if(prevcorrectedAngle3!=0  && correctedAngle3 > prevcorrectedAngle3){
////            numberofTurns3--;
////          }
////          Serial.print("Check start ");
////          Serial.println(totalAngle3);
//          checkQuadrant(3);
////          Serial.print("End ");
////          Serial.println(totalAngle3);
////          delay(10);
//        }
      while(abs(round(totalX-initX)) <= p_angle){     //TODO
        if(abs(round(totalX-initX))>=(1/3)*p_angle && upflag==0){
            setMotor(650, LPWM_Output5, RPWM_Output5);
            upflag = 1;
          }
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
//      initAngle3 = totalAngle3;
//      initAngle4 = totalAngle4;
      delay(1000);
      
      upflag = 0;
      setMotor(222, LPWM_Output4, RPWM_Output4);
      setMotor(372, LPWM_Output5, RPWM_Output5);
//      while(abs(totalAngle3-initAngle3) <= 2160*p_angle)
//        {
//          if(abs(totalAngle3-initAngle3)>=56160 && upflag==0){
//            setMotor(511, LPWM_Output5, RPWM_Output5);
//            upflag = 1;
//          }
//          Serial.print("Read start ");
//          Serial.println(totalAngle3);
//          ReadRawAngle(3); //update startAngle with degAngle - for taring
//          Serial.print("Correct start ");
//          Serial.println(totalAngle3);
//          correctAngle(3);
////          if(prevcorrectedAngle3!=0  && correctedAngle3 < prevcorrectedAngle3){
////            numberofTurns3++;
////          }
//          Serial.print("Check start ");
//          Serial.println(totalAngle3);
//          checkQuadrant(3);
//          Serial.print("End ");
//          Serial.println(totalAngle3);
//        }
      while(abs(round(totalX-initX)) <= p_angle){     //TODO
        if(abs(round(totalX-initX))>=(2/3)*p_angle && upflag==0){
            setMotor(511, LPWM_Output5, RPWM_Output5);
            upflag = 1;
          }
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 2 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
//      initAngle3 = totalAngle3;
//      initAngle4 = totalAngle4;
      delay(500);
      break;

      case '2':
      //////////////////////////ARC///////////////////////////////////////////////
      setMotor(322, LPWM_Output1, RPWM_Output1);
      setMotor(542, LPWM_Output2, RPWM_Output2);
      setMotor(542, LPWM_Output3, RPWM_Output3);
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle)
        {
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      delay(500);
      /////////////////////////UP-DOWN////////////////////////////////////////////
      upflag = 0;
      TCA9548A(4);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
      setMotor(800, LPWM_Output4, RPWM_Output4);
      while(abs(totalX-initX) <= p_angle){     //TODO
        if(abs(totalX-initX)>=(1/3)*p_angle && upflag==0){
            setMotor(650, LPWM_Output5, RPWM_Output5);
            upflag = 1;
          }
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
//      initAngle3 = totalAngle3;
//      initAngle4 = totalAngle4;
      delay(1000);
      
      upflag = 0;
      setMotor(222, LPWM_Output4, RPWM_Output4);
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totalX-initX) <= p_angle){     //TODO
        if(abs(totalX-initX)>=(2/3)*p_angle && upflag==0){
            setMotor(511, LPWM_Output5, RPWM_Output5);
            upflag = 1;
          }
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 2 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
      delay(500);
      
      //////////////////////////DE-ARC///////////////////////////////////////////////
      setMotor(700, LPWM_Output1, RPWM_Output1);
      setMotor(480, LPWM_Output2, RPWM_Output2);
      setMotor(480, LPWM_Output3, RPWM_Output3);
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle)
        {
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      delay(500);
      break;

      case '3':
      //////////////////////////ARC///////////////////////////////////////////////
      Serial.println(2160*q_angle);
      setMotor(700, LPWM_Output1, RPWM_Output1);
      setMotor(480, LPWM_Output2, RPWM_Output2);
      setMotor(480, LPWM_Output3, RPWM_Output3); 
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle)
        {
//          ReadRawAngle(0); //update startAngle with degAngle - for taring
//          Serial.println("Read done 0");
//          correctAngle(0);
//          checkQuadrant(0);
          Serial.print("Read start ");
          Serial.println(totalAngle1);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle1);
          correctAngle(1);
          Serial.print("Check start ");
          Serial.println(totalAngle1);
          checkQuadrant(1);
          Serial.print("End ");
          Serial.println(totalAngle1);
//          ReadRawAngle(2); //update startAngle with degAngle - for taring
//          Serial.println("Read done 2");
//          correctAngle(2);
//          Serial.println("Correct done 2");
//          checkQuadrant(2);
//          Serial.println("Check done 2");
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
//      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
//      initAngle2 = totalAngle2;
      delay(1000);
      //////////////////////////DE-ARC///////////////////////////////////////////////
      Serial.println(2160*q_angle);
      setMotor(322, LPWM_Output1, RPWM_Output1);
      setMotor(542, LPWM_Output2, RPWM_Output2);
      setMotor(542, LPWM_Output3, RPWM_Output3);
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle)
        {  
//          ReadRawAngle(0); //update startAngle with degAngle - for taring
//          Serial.println("Read done 0");
//          correctAngle(0);
//          checkQuadrant(0);
          Serial.print("Read start ");
          Serial.println(totalAngle1);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle1);
          correctAngle(1);
          Serial.print("Check start ");
          Serial.println(totalAngle1);
          checkQuadrant(1);
          Serial.print("End ");
          Serial.println(totalAngle1);
//          ReadRawAngle(2); //update startAngle with degAngle - for taring
//          Serial.println("Read done 2");
//          correctAngle(2);
//          Serial.println("Correct done 2");
//          checkQuadrant(2);
//          Serial.println("Check done 2");
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
//      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
//      initAngle2 = totalAngle2;
      delay(500);
      break;

      case '4':
      //////////////////////////////////////EXTRUDE////////////////////////////////////////////////
      long initdist, totaldist;
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
      initdist = totaldist;

      setMotor(650, LPWM_Output5, RPWM_Output5);
      while(abs(totaldist-initdist) <= x_angle)
        {
          digitalWrite(pingPin, LOW);
          delayMicroseconds(2);
          digitalWrite(pingPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(pingPin, LOW);
          totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
//          ReadRawAngle(4); //update startAngle with degAngle - for taring
//          correctAngle(4);
//          checkQuadrant(4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
//      initAngle4 = totalAngle4;
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
      initdist = totaldist;
      delay(500);
      //////////////////////////ARC///////////////////////////////////////////////
      setMotor(322, LPWM_Output1, RPWM_Output1);
      setMotor(542, LPWM_Output2, RPWM_Output2);
      setMotor(542, LPWM_Output3, RPWM_Output3);
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle*(0.6666))
        {
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      delay(500);

      //////////////////////////DE-ARC///////////////////////////////////////////////
      setMotor(700, LPWM_Output1, RPWM_Output1);
      setMotor(480, LPWM_Output2, RPWM_Output2);
      setMotor(480, LPWM_Output3, RPWM_Output3);
      while(abs(totalAngle1-initAngle1) <= 2160*q_angle*(0.6666))
        {
          ReadRawAngle(0); //update startAngle with degAngle - for taring
          correctAngle(0);
          checkQuadrant(0);
          ReadRawAngle(1); //update startAngle with degAngle - for taring
          correctAngle(1);
          checkQuadrant(1);
          ReadRawAngle(2); //update startAngle with degAngle - for taring
          correctAngle(2);
          checkQuadrant(2);
        }
      Serial.println("Done!! ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle0 = totalAngle0;
      initAngle1 = totalAngle1;
      initAngle2 = totalAngle2;
      delay(500);
      
      //////////////////////////////////////DE-EXTRUDE///////////////////////////////////////////
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
      initdist = totaldist;
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totaldist-initdist) <= x_angle)
        {
          digitalWrite(pingPin, LOW);
          delayMicroseconds(2);  
          digitalWrite(pingPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(pingPin, LOW);
          totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
//          ReadRawAngle(4); //update startAngle with degAngle - for taring
//          correctAngle(4);
//          checkQuadrant(4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
//      initAngle4 = totalAngle4;
      digitalWrite(pingPin, LOW);
      delayMicroseconds(2);
      digitalWrite(pingPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(pingPin, LOW);
      totaldist = microsecondsToCentimeters(pulseIn(echoPin, HIGH));
      initdist = totaldist;
      delay(500);
      break;

      case '5':
      ///////////////////////////////////////////////UP//////////////////////////////////////////
      TCA9548A(4);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
      setMotor(800, LPWM_Output4, RPWM_Output4);
      while(abs(totalX-initX) <= i_angle){     //TODO
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
//      initAngle3 = totalAngle3;
//      initAngle4 = totalAngle4;
      delay(500);
//      setMotor(700, LPWM_Output4, RPWM_Output4);
//      while(abs(totalAngle3-initAngle3) <= 2160*i_angle)
//        {
//          ReadRawAngle(3); //update startAngle with degAngle - for taring
//          correctAngle(3);
//          checkQuadrant(3);
//        }
//      Serial.println("Part 1 Done!! ");
//      setMotor(511, LPWM_Output3, RPWM_Output3);
//
//      //Serial.print("Current Position = ");
//      //Serial.println(totalAngle0, 2);
//      initAngle3 = totalAngle3;
//      delay(500);
      
      //////////////////////////////////////EXTRUDE////////////////////////////////////////////////
      setMotor(650, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 2160*x_angle)
        {
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          correctAngle(4);
          checkQuadrant(4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);

      //////////////////////////////////////DE-EXTRUDE///////////////////////////////////////////
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 2160*x_angle)
        {
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          correctAngle(4);
          checkQuadrant(4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);

      ///////////////////////////////////DOWN/////////////////////////////////////////////
      setMotor(222, LPWM_Output4, RPWM_Output4);
      while(abs(totalX-initX) <= i_angle){     //TODO
        mpu.update();
        totalX = mpu.getAngleX();
      }
      Serial.println("Part 2 Done!! ");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      mpu.update();
      totalX = mpu.getAngleX();
      initX = totalX;
      delay(500);
//      setMotor(322, LPWM_Output4, RPWM_Output4);
//      while(abs(totalAngle3-initAngle3) <= 2160*i_angle)
//        {
//          ReadRawAngle(3); //update startAngle with degAngle - for taring
//          correctAngle(3);
//          checkQuadrant(3);
//        }
//      Serial.println("Part 1 Done!! ");
//      setMotor(511, LPWM_Output3, RPWM_Output3);
//
//      //Serial.print("Current Position = ");
//      //Serial.println(totalAngle0, 2);
//      initAngle3 = totalAngle3;
//      delay(500);
      break;

      case '6':
     
      //////////////////////////////////////EXTRUDE////////////////////////////////////////////////
      setMotor(650, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);

      //////////////////////////////////////DE-EXTRUDE///////////////////////////////////////////
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);
      break;

      case '7':
     
      //////////////////////////////////////EXTRUDE////////////////////////////////////////////////
      setMotor(650, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);

      //////////////////////////////////////DE-EXTRUDE///////////////////////////////////////////
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);
      break;

      case '8':
     
      //////////////////////////////////////EXTRUDE////////////////////////////////////////////////
      setMotor(650, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);

      //////////////////////////////////////DE-EXTRUDE///////////////////////////////////////////
      setMotor(372, LPWM_Output5, RPWM_Output5);
      while(abs(totalAngle4-initAngle4) <= 7000)
        {
          Serial.print("Read start ");
          Serial.println(totalAngle4);
          ReadRawAngle(4); //update startAngle with degAngle - for taring
          Serial.print("Correct start ");
          Serial.println(totalAngle4);
          correctAngle(4);
          Serial.print("Check start ");
          Serial.println(totalAngle4);
          checkQuadrant(4);
          Serial.print("End ");
          Serial.println(totalAngle4);
        }
      Serial.println("Part 1 Done!! ");
      setMotor(511, LPWM_Output5, RPWM_Output5);

      //Serial.print("Current Position = ");
      //Serial.println(totalAngle0, 2);
      initAngle4 = totalAngle4;
      delay(500);
      break;
    }
  }
}

void setMotor(int Motorval, int LPWM_Output, int RPWM_Output)
{
  if (Motorval <= 512)
  {
    // reverse rotation
    int reversePWM = -(Motorval - 511) / 2;
    //Serial.print("Reverse ");
    //Serial.println(reversePWM);
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, reversePWM);
  }
  else
  {
    // forward rotation
    int forwardPWM = (Motorval - 512) / 2;
    //Serial.print("Forward ");
    //Serial.println(forwardPWM);
    analogWrite(RPWM_Output, 0);
    analogWrite(LPWM_Output, forwardPWM);
  }
}

void ReadRawAngle(int sensor)
{
  TCA9548A(sensor);
  int lowbyte; //raw angle 7:0
  word highbyte; //raw angle 7:0 and 11:8
  int rawAngle; //final raw angle 
  
  //7:0 - bits
  Wire.beginTransmission(0x36); //connect to the sensor
  Wire.write(0x0D); //figure 21 - register map: Raw angle (7:0)
  Wire.endTransmission(); //end transmission
  Wire.requestFrom(0x36, 1); //request from the sensor
  
  while(Wire.available() == 0); //wait until it becomes available 
  lowbyte = Wire.read(); //Reading the data after the request
 
  //11:8 - 4 bits
  Wire.beginTransmission(0x36);
  Wire.write(0x0C); //figure 21 - register map: Raw angle (11:8)
  Wire.endTransmission();
  Wire.requestFrom(0x36, 1);
  
  while(Wire.available() == 0);  
  highbyte = Wire.read();
  //Serial.println(lowbyte, HEX);
  //Serial.println(highbyte, HEX);
  
  
  //4 bits have to be shifted to its proper place as we want to build a 12-bit number
  highbyte = highbyte << 8; //shifting to left
  //What is happening here is the following: The variable is being shifted by 8 bits to the left:
  //Initial value: 00000000|00001111 (word = 16 bits or 2 bytes)
  //Left shifting by eight bits: 00001111|00000000 so, the high byte is filled in
  
  //Finally, we combine (bitwise OR) the two numbers:
  //High: 00001111|00000000
  //Low:  00000000|00001111
  //      -----------------
  //H|L:  00001111|00001111
  rawAngle = highbyte | lowbyte; //int is 16 bits (as well as the word)

  //We need to calculate the angle:
  //12 bit -> 4096 different levels: 360° is divided into 4096 equal parts:
  //360/4096 = 0.087890625
  //Multiply the output of the encoder with 0.087890625
  if(sensor==0){
    degAngle0 = rawAngle * 0.087890625;
  }
  else if(sensor==1){
    degAngle1 = rawAngle * 0.087890625;  
  }
  else if(sensor==2){
    degAngle2 = rawAngle * 0.087890625;  
  }
  else if(sensor==3){
    degAngle3 = rawAngle * 0.087890625;  
  }
  else if(sensor==4){
    degAngle4 = rawAngle * 0.087890625;  
  }
  //Serial.print("Deg angle: ");
  //Serial.println(degAngle, 2); //absolute position of the encoder within the 0-360 circle
  
}

void correctAngle(int sensor)
{
  //recalculate angle
  if(sensor==0){
    correctedAngle0 = degAngle0 - startAngle0; //this tares the position

    if(correctedAngle0 < 0) //if the calculated angle is negative, we need to "normalize" it
    {
      correctedAngle0 = correctedAngle0 + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
      //Eat 5 star do nothing
    }
  }
  else if(sensor==1){
    correctedAngle1 = degAngle1 - startAngle1; //this tares the position

    if(correctedAngle1 < 0) //if the calculated angle is negative, we need to "normalize" it
    {
      correctedAngle1 = correctedAngle1 + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
      //Eat 5 star do nothing
    }
  }
  else if(sensor==2){
    correctedAngle2 = degAngle2 - startAngle2; //this tares the position

    if(correctedAngle2 < 0) //if the calculated angle is negative, we need to "normalize" it
    {
      correctedAngle2 = correctedAngle2 + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
      //Eat 5 star do nothing
    }
  }
  else if(sensor==3){
    correctedAngle3 = degAngle3 - startAngle3; //this tares the position

    if(correctedAngle3 < 0) //if the calculated angle is negative, we need to "normalize" it
    {
      correctedAngle3 = correctedAngle3 + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
      //Eat 5 star do nothing
    }
  }
  else if(sensor==4){
    correctedAngle4 = degAngle4 - startAngle4; //this tares the position

    if(correctedAngle4 < 0) //if the calculated angle is negative, we need to "normalize" it
    {
      correctedAngle4 = correctedAngle4 + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
      //Eat 5 star do nothing
    }
  }
  //Serial.print("Corrected angle: ");
  //Serial.println(correctedAngle, 2); //print the corrected/tared angle  
}


void checkQuadrant(int sensor)
{
  /*
  //Quadrants:
  4  |  1
  ---|---
  3  |  2
  */
  if(sensor==0){
    //Quadrant 1
    if(correctedAngle0 >= 0 && correctedAngle0 <=90)
    {
      quadrantNumber0 = 1;
    }
  
    //Quadrant 2
    if(correctedAngle0 > 90 && correctedAngle0 <=180)
    {
      quadrantNumber0 = 2;
    }
  
    //Quadrant 3
    if(correctedAngle0 > 180 && correctedAngle0 <=270)
    {
      quadrantNumber0 = 3;
    }
  
    //Quadrant 4
    if(correctedAngle0 > 270 && correctedAngle0 <360)
    {
      quadrantNumber0 = 4;
    }
  }
  else if(sensor==1){
    //Quadrant 1
    if(correctedAngle1 >= 0 && correctedAngle1 <=90)
    {
      quadrantNumber1 = 1;
    }
  
    //Quadrant 2
    if(correctedAngle1 > 90 && correctedAngle1 <=180)
    {
      quadrantNumber1 = 2;
    }
  
    //Quadrant 3
    if(correctedAngle1 > 180 && correctedAngle1 <=270)
    {
      quadrantNumber1 = 3;
    }
  
    //Quadrant 4
    if(correctedAngle1 > 270 && correctedAngle1 <360)
    {
      quadrantNumber1 = 4;
    }
  }
  else if(sensor==2){
    //Quadrant 1
    if(correctedAngle2 >= 0 && correctedAngle2 <=90)
    {
      quadrantNumber2 = 1;
    }
  
    //Quadrant 2
    if(correctedAngle2 > 90 && correctedAngle2 <=180)
    {
      quadrantNumber2 = 2;
    }
  
    //Quadrant 3
    if(correctedAngle2 > 180 && correctedAngle2 <=270)
    {
      quadrantNumber2 = 3;
    }
  
    //Quadrant 4
    if(correctedAngle2 > 270 && correctedAngle2 <360)
    {
      quadrantNumber2 = 4;
    }
  }
  else if(sensor==3){
    //Quadrant 1
    if(correctedAngle3 >= 0 && correctedAngle3 <=90)
    {
      quadrantNumber3 = 1;
    }
  
    //Quadrant 2
    if(correctedAngle3 > 90 && correctedAngle3 <=180)
    {
      quadrantNumber3 = 2;
    }
  
    //Quadrant 3
    if(correctedAngle3 > 180 && correctedAngle3 <=270)
    {
      quadrantNumber3 = 3;
    }
  
    //Quadrant 4
    if(correctedAngle3 > 270 && correctedAngle3 <360)
    {
      quadrantNumber3 = 4;
    }
  }
  else if(sensor==4){
    //Quadrant 1
    if(correctedAngle4 >= 0 && correctedAngle4 <=90)
    {
      quadrantNumber4 = 1;
    }
  
    //Quadrant 2
    if(correctedAngle4 > 90 && correctedAngle4 <=180)
    {
      quadrantNumber4 = 2;
    }
  
    //Quadrant 3
    if(correctedAngle4 > 180 && correctedAngle4 <=270)
    {
      quadrantNumber4 = 3;
    }
  
    //Quadrant 4
    if(correctedAngle4 > 270 && correctedAngle4 <360)
    {
      quadrantNumber4 = 4;
    }
  }
  //Serial.print("Quadrant: ");
  //Serial.println(quadrantNumber); //print our position "quadrant-wise"
  if(sensor==0){
    if(quadrantNumber0 != previousquadrantNumber0) //if we changed quadrant
    {
      if(quadrantNumber0 == 1 && previousquadrantNumber0 == 4)
      {
        numberofTurns0++; // 4 --> 1 transition: CW rotation
      }
  
      if(quadrantNumber0 == 4 && previousquadrantNumber0 == 1)
      {
        numberofTurns0--; // 1 --> 4 transition: CCW rotation
      }
      //this could be done between every quadrants so one can count every 1/4th of transition
  
      previousquadrantNumber0 = quadrantNumber0;  //update to the current quadrant
    }
    totalAngle0 = (numberofTurns0*360) + correctedAngle0; //number of turns (+/-) plus the actual angle within the 0-360 range
  }
  else if(sensor==1){
    if(quadrantNumber1 != previousquadrantNumber1) //if we changed quadrant
    {
      if(quadrantNumber1 == 1 && previousquadrantNumber1 == 4)
      {
        numberofTurns1++; // 4 --> 1 transition: CW rotation
      }
  
      if(quadrantNumber1 == 4 && previousquadrantNumber1 == 1)
      {
        numberofTurns1--; // 1 --> 4 transition: CCW rotation
      }
      //this could be done between every quadrants so one can count every 1/4th of transition
  
      previousquadrantNumber1 = quadrantNumber1;  //update to the current quadrant
    }
    totalAngle1 = (numberofTurns1*360) + correctedAngle1; //number of turns (+/-) plus the actual angle within the 0-360 range
  }
  else if(sensor==2){
    if(quadrantNumber2 != previousquadrantNumber2) //if we changed quadrant
    {
      if(quadrantNumber2 == 1 && previousquadrantNumber2 == 4)
      {
        numberofTurns2++; // 4 --> 1 transition: CW rotation
      }
  
      if(quadrantNumber2 == 4 && previousquadrantNumber2 == 1)
      {
        numberofTurns2--; // 1 --> 4 transition: CCW rotation
      }
      //this could be done between every quadrants so one can count every 1/4th of transition
  
      previousquadrantNumber2 = quadrantNumber2;  //update to the current quadrant
      
    }
    
      totalAngle2 = (numberofTurns2*360) + correctedAngle2; //number of turns (+/-) plus the actual angle within the 0-360 range
  }
  else if(sensor==3){
    if(quadrantNumber3 != previousquadrantNumber3) //if we changed quadrant
    {
      if(quadrantNumber3 == 1 && previousquadrantNumber3 == 4)
      {
        numberofTurns3++; // 4 --> 1 transition: CW rotation
      }
  
      if(quadrantNumber3 == 4 && previousquadrantNumber3 == 1)
      {
        numberofTurns3--; // 1 --> 4 transition: CCW rotation
      }
      //this could be done between every quadrants so one can count every 1/4th of transition
  
      previousquadrantNumber3 = quadrantNumber3;  //update to the current quadrant
      
    }
      if(prevcorrectedAngle3!=0  && correctedAngle3 > prevcorrectedAngle3){
        numberofTurns3--;
      }
      totalAngle3 = (numberofTurns3*360) + correctedAngle3; //number of turns (+/-) plus the actual angle within the 0-360 range
      Serial.print("Check Quadrant: ");
      Serial.println(correctedAngle3);
      Serial.println(totalAngle3);
      Serial.println(numberofTurns3);
      prevcorrectedAngle3 = correctedAngle3;
  }
  else if(sensor==4){
    if(quadrantNumber4 != previousquadrantNumber4) //if we changed quadrant
    {
      if(quadrantNumber4 == 1 && previousquadrantNumber4 == 4)
      {
        numberofTurns4++; // 4 --> 1 transition: CW rotation
      }
  
      if(quadrantNumber4 == 4 && previousquadrantNumber4 == 1)
      {
        numberofTurns4--; // 1 --> 4 transition: CCW rotation
      }
      //this could be done between every quadrants so one can count every 1/4th of transition
  
      previousquadrantNumber4 = quadrantNumber4;  //update to the current quadrant
      
    }
    
      totalAngle4 = (numberofTurns4*360) + correctedAngle4; //number of turns (+/-) plus the actual angle within the 0-360 range
  }
}


void checkMagnetPresence(int sensor)
{  
  //This function runs in the setup() and it locks the MCU until the magnet is not positioned properly
  TCA9548A(sensor);
  int magnetStatus = 0;
  while((magnetStatus & 32) != 32) //while the magnet is not adjusted to the proper distance - 32: MD = 1
  {
    magnetStatus = 0; //reset reading

    Wire.beginTransmission(0x36); //connect to the sensor
    Wire.write(0x0B); //figure 21 - register map: Status: MD ML MH
    Wire.endTransmission(); //end transmission
    Wire.requestFrom(0x36, 1); //request from the sensor

    while(Wire.available() == 0); //wait until it becomes available 
    magnetStatus = Wire.read(); //Reading the data after the request

    //Serial.print("Magnet status: ");
    //Serial.println(magnetStatus, BIN); //print it in binary so you can compare it to the table (fig 21)      
  }      
  
  //Status register output: 0 0 MD ML MH 0 0 0  
  //MH: Too strong magnet - 100111 - DEC: 39 
  //ML: Too weak magnet - 10111 - DEC: 23     
  //MD: OK magnet - 110111 - DEC: 55

  Serial.println("Magnet found!");
  delay(500);  
}
