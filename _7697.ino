# define ledpin 10
# define sw_btn 12

void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin,OUTPUT);
 pinMode(sw_btn,INPUT);
}

int _status;
void loop() {

 int btn_value1 = digitalRead(sw_btn);
 delay(200);
 int btn_value2 = digitalRead(sw_btn);
 Serial.println(btn_value1);
 Serial.println(btn_value2);
 Serial.println(_status);
 Serial.println("-------------------");
 
 if(btn_value1!=btn_value2){
  if(!_status){
    _status = 1;
  
 }else{
  _status = 0;
  }
  }
  digitalWrite(ledpin,_status);
  delay(500);
}
