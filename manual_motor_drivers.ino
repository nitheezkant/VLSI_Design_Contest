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

char input;

void setup()
{
  Serial.begin(115200); //start serial - tip: don't use serial if you don't need it (speed considerations)
  Serial.println("HelloThere");

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

  setMotor(511, LPWM_Output1, RPWM_Output1);
  setMotor(511, LPWM_Output2, RPWM_Output2);
  setMotor(511, LPWM_Output3, RPWM_Output3);
  setMotor(511, LPWM_Output4, RPWM_Output4);
  setMotor(511, LPWM_Output5, RPWM_Output5);
}

void loop()
{
   
    if(Serial.available()){
        input = Serial.read();
    }
    
  switch(input)
    {
      case 'w':    
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(600, LPWM_Output2, RPWM_Output2);
      setMotor(422, LPWM_Output3, RPWM_Output3);
      break;

      case 's':
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(422, LPWM_Output2, RPWM_Output2);
      setMotor(600, LPWM_Output3, RPWM_Output3);
      break;

      case 'a':
      setMotor(333, LPWM_Output1, RPWM_Output1);
      setMotor(600, LPWM_Output2, RPWM_Output2);
      setMotor(600, LPWM_Output3, RPWM_Output3);
      break;

      case 'd':
      setMotor(689, LPWM_Output1, RPWM_Output1);
      setMotor(422, LPWM_Output2, RPWM_Output2);
      setMotor(422, LPWM_Output3, RPWM_Output3);
      break;

      case '[':
      setMotor(600, LPWM_Output1, RPWM_Output1);
      setMotor(600, LPWM_Output2, RPWM_Output2);
      setMotor(600, LPWM_Output3, RPWM_Output3);
      break;

      case ']':
      setMotor(422, LPWM_Output1, RPWM_Output1);
      setMotor(422, LPWM_Output2, RPWM_Output2);
      setMotor(422, LPWM_Output3, RPWM_Output3);
      break;

      case 'q':
      setMotor(272, LPWM_Output1, RPWM_Output1);
      setMotor(542, LPWM_Output2, RPWM_Output2);
      setMotor(542, LPWM_Output3, RPWM_Output3);
      break;

      case 'e':
      setMotor(750, LPWM_Output1, RPWM_Output1);
      setMotor(480, LPWM_Output2, RPWM_Output2);
      setMotor(480, LPWM_Output3, RPWM_Output3);
      break;

      case 'i':
      Serial.println("In case i");
      setMotor(800, LPWM_Output4, RPWM_Output4);
      break;
 
      case 'k':
      Serial.println("In case k");
      setMotor(222, LPWM_Output4, RPWM_Output4);
      break;

      case 'l':
      Serial.println("In case l");
      setMotor(700, LPWM_Output5, RPWM_Output5);
      break;

      case 'j':
      Serial.println("In case j");
      setMotor(322, LPWM_Output5, RPWM_Output5);
      break;

      case 'p':
      Serial.println("In case p");
      setMotor(222, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);
      break;

      case 'u':
      Serial.println("In case u");
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(750, LPWM_Output5, RPWM_Output5);
      break;

      case 'o':                                                      // ** EMERGENCY STOP **
      Serial.println(" **STOP** ");
      setMotor(511, LPWM_Output1, RPWM_Output1);
      setMotor(511, LPWM_Output2, RPWM_Output2);
      setMotor(511, LPWM_Output3, RPWM_Output3);
      setMotor(511, LPWM_Output4, RPWM_Output4);
      setMotor(511, LPWM_Output5, RPWM_Output5);
      break;

      case 'v':
      Serial.println("In case m");
      setMotor(800, LPWM_Output4, RPWM_Output4);
      setMotor(700, LPWM_Output5, RPWM_Output5);
      break;

      case 'b':
      Serial.println("In case n");
      setMotor(222, LPWM_Output4, RPWM_Output4);
      setMotor(322, LPWM_Output5, RPWM_Output5);
      break;

      case 'n':
      Serial.println("In case m");
      setMotor(222, LPWM_Output4, RPWM_Output4);
      setMotor(750, LPWM_Output5, RPWM_Output5);
      break;

      case 'm':
      Serial.println("In case n");
      setMotor(800, LPWM_Output4, RPWM_Output4);
      setMotor(322, LPWM_Output5, RPWM_Output5);
      break;
    }
    delay(100);
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
