# define ledpin1 9
# define ledpin2 10

void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin1,OUTPUT);
 pinMode(ledpin2,OUTPUT);
}

void loop() {

 analogWrite(ledpin1,HIGH);
 digitalWrite(ledpin2,LOW);
 delay(500);
 digitalWrite(ledpin1,LOW);
 digitalWrite(ledpin2,HIGH);
 delay(500);

}
