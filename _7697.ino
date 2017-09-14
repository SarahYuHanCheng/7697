# define ledpin1 10
# define ledpin2 11
# define ledpin3 12
# define sw_btn 13


int _status;
void setup() {
  
 Serial.begin(9600);
 pinMode(ledpin1,OUTPUT);
 pinMode(ledpin2,OUTPUT);
 pinMode(ledpin3,OUTPUT);
 pinMode(sw_btn,INPUT);
}

void loop() {

 int btn_value1, btn_value2 ;
 Serial.println(btn_value1);
 Serial.println(btn_value2);
 Serial.println(_status);
 Serial.println("-------------------");
 btn_value1 = digitalRead(sw_btn);
 delay(200);  
 btn_value2 = digitalRead(sw_btn);
    
 if(btn_value1!=btn_value2){
   _status ++;
   switch(_status){
   case 1:
      {
        int i=0;
        for(i;i<255;i++){
          analogWrite(ledpin1,i);
          analogWrite(ledpin2,i);
          analogWrite(ledpin3,i);
          delay(10);
        }
        for(i;i>0;i--){
        analogWrite(ledpin1,i);
        analogWrite(ledpin2,i);
        analogWrite(ledpin3,i);
        delay(10);
        }
         
        break;
    }
        
    case 2:
    {
      for(int j=0;j<3;j++){
        analogWrite(ledpin1,255);
        analogWrite(ledpin3,0);
      delay(500);
        analogWrite(ledpin2,255);
        analogWrite(ledpin1,0);
      delay(500);
        analogWrite(ledpin3,255);
        analogWrite(ledpin2,0);
      delay(500);
      }
      break;
    }
       
   default:
   {_status=0;
   analogWrite(ledpin1,0);
   analogWrite(ledpin2,0);
   analogWrite(ledpin3,0);
   }
    break;
    }
 }
    delay(300);
}
