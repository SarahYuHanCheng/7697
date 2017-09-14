/* Receive the controlling message, turning on/off and pwm, and
 * than set the corresponding pin.
 */

enum UltrasonicPinID {
    U_F = 0,
    U_L,
    U_R,
    NUM_OF_ULTRASONIC_PIN
};

/* Pin assignment */
static const uint8_t usTrigPins[NUM_OF_ULTRASONIC_PIN] = {6, 8, 10 };  // F, L, R
static const uint8_t usEchoPins[NUM_OF_ULTRASONIC_PIN] = {7, 9, 11 };  // F, L, R

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




void setup()
{

  Serial.begin(9600);
  while (!Serial)
    ;
}
void loop()
{

  long dF, dL, dR;
  
    dF = ultrasonicGetDistance(usTrigPins[U_F], usEchoPins[U_F]);
    dL = ultrasonicGetDistance(usTrigPins[U_L], usEchoPins[U_L]);
    dR = ultrasonicGetDistance(usTrigPins[U_R], usEchoPins[U_R]);
    Serial.println("Serial.println: ");
    Serial.println(dF);
    Serial.println(dR);
    Serial.println(dL);
    delay(100);
      
}
