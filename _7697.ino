# define ledpin1 9
# define ledpin2 10

void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin1,OUTPUT);
 pinMode(ledpin2,OUTPUT);
}

void loop() {
digitalWrite(ledpin2,LOW);

  for(int i=0;i<255;i++){
 analogWrite(ledpin1,i);
delay(50);
}
  
for(int i=255;i>0;i--){
 analogWrite(ledpin1,i);
delay(50);
}
}
