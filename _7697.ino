#define ledpin1 10

void setup() {
   Serial.begin(9600);
 pinMode(ledpin1,OUTPUT);
}

void loop() {
 int i=0;
        for(i;i<1000;i+=10){
          Digital_PWM(ledpin1,i);
        }
        for(i;i>0;i-=10){
        Digital_PWM(ledpin1,i);
        }
}

void Digital_PWM(uint8_t pin, int duty){
    for(int i =0;i<100;i++){
      digitalWrite(pin, HIGH);
      delayMicroseconds(duty); // Approximately 10% duty cycle @ 1KHz
      digitalWrite(pin, LOW);
      delayMicroseconds(1000 - duty);
    }
  }
