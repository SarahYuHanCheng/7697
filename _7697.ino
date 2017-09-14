# define ledpin 10
# define btn 12

void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin,OUTPUT);
 pinMode(btn,INPUT);
}


void loop() {

 int btn_value = digitalRead(btn);
 Serial.println(btn_value);
 
 if(digitalRead(btn)){
  digitalWrite(ledpin,HIGH);
  delay(100);
 }else{
  digitalWrite(ledpin,LOW);
  }
}
