# define ledpin 7

void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin,OUTPUT);
}

void loop() {
  
 int i=0;
 for(i;i<255;i++){
  analogWrite(ledpin,i);
  delay(10);
  }
  for(i;i>0;i--){
  analogWrite(ledpin,i);
  delay(10);
  }
}
