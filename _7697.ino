enum MotorPinID {
    L_F = 0,
    L_B,
    R_F,
    R_B,
    NUM_OF_MOTOR_PIN
};
enum UltrasonicPinID {
    U_F = 0,
    U_L,
    U_R,
    NUM_OF_ULTRASONIC_PIN
};

/* Pin assignment */
static const uint8_t usTrigPins[NUM_OF_ULTRASONIC_PIN] = {2, 4, 11 };  // F, L, R
static const uint8_t usEchoPins[NUM_OF_ULTRASONIC_PIN] = {3, 5, 12 };  // F, L, R
static const uint8_t motorPins[NUM_OF_MOTOR_PIN] = {14, 15, 16, 17};  //  L_F, L_B,R_F, R_B

long ultrasonicGetDistance(uint8_t trig, uint8_t echo)
{
    long duration;

    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);

    pinMode(echo, INPUT);
    duration = pulseIn(echo, HIGH, 5000000L);
    return duration / 29 / 2;
}
void forward(int t)
{
        digitalWrite(motorPins[L_F], HIGH);
        digitalWrite(motorPins[L_B], LOW);
        digitalWrite(motorPins[R_F], HIGH);
        digitalWrite(motorPins[R_B], LOW);
        delay(t);
}
void backward(int t)
{

        digitalWrite(motorPins[L_F], LOW);
        digitalWrite(motorPins[L_B], HIGH);
        digitalWrite(motorPins[R_F], LOW);
        digitalWrite(motorPins[R_B], HIGH);
        delay(t);
}
void left(int t)
{

        digitalWrite(motorPins[L_F], LOW);
        digitalWrite(motorPins[L_B], HIGH);
        digitalWrite(motorPins[R_F], HIGH);
        digitalWrite(motorPins[R_B], LOW);
        delay(t);
}
void right(int t)
{
        digitalWrite(motorPins[L_F], HIGH);
        digitalWrite(motorPins[motorPins[L_B]], LOW);
        digitalWrite(motorPins[R_F], LOW);
        digitalWrite(motorPins[R_B], HIGH);
        delay(t);
}

void setup()
{ 
  int motorpins=0;
  while(motorpins<NUM_OF_MOTOR_PIN){
    pinMode(motorPins[motorpins],OUTPUT);
    motorpins++;
  }
  Serial.begin(9600);
  while (!Serial)
    ;
}
void loop()
{

  long dF, dL, dR;
  
    dF = ultrasonicGetDistance(usTrigPins[U_F], usEchoPins[U_F]);

    if(dF<15){
      right(150);
      }else{
        forward(100);
        }
      
}
