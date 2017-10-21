/* Receive the controlling message, turning on/off and pwm, and
 * than set the corresponding pin.
 */
//#include <WiFi.h>

#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>

#define SSID "CSIE-WLAN"
#define PASSWD "wificsie"
#define TCP_IP "192.168.208.242"
#define TCP_PORT 5000

WiFiClient wifiClient;

static char buf[32];
static int messageLen;
static char client_ID[] = "Sarah",Team[] = "B";
static char *recv_ID,*recv_buf;
static int pos[5];

int xf, yf;
float x_s, y_s;
int Dx, Dy;
float movm, posm, xmov, ymov, mov_v, pos_v;
int adj=200;


IPAddress ip;

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
void setup()
{
  int motorpins=0;
  while(motorpins<NUM_OF_MOTOR_PIN){
    pinMode(motorPins[motorpins],OUTPUT);
    motorpins++;
  }
    Serial.begin(115200);
    while (!Serial)
      ;       
    // set WiFi
   // WiFi.mode(WIFI_STA);
    int status = WL_IDLE_STATUS;
    WiFi.begin(SSID, PASSWD);
    while (status != WL_CONNECTED) {
        // Connect failed, blink 0.5 second to indicate
        // the board is retrying.
        delay(500);
        WiFi.begin(SSID, PASSWD);
        status =  WiFi.begin(SSID, PASSWD);
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(SSID);
        Serial.println(status);
    }
    
    // Conenct to AP successfully
   // wifiClient.connect(TCP_IP, TCP_PORT);
      while (!wifiClient.connect(TCP_IP, TCP_PORT)){
      delay(300);
      Serial.print("Attempting to connect to SERVER: ");
        Serial.println(TCP_IP);
    }

    reg_ID();

    delay(100);
    xTaskCreate(
                    askPos,          /* Task function. */
                    "askPos",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    NULL);            /* Task handle. */
}  

void reg_ID()
{
 strcpy(buf,"Register");
    strcat(buf,Team);
    strcat(buf,"|");
    strcat(buf,client_ID);
    wifiClient.write(buf, strlen(buf));
    wifiClient.flush();
}

void send_mes(char ID[],char mes[])
{
    sprintf(buf,"%s|%s",ID,mes);
    wifiClient.write(buf, strlen(buf));
    wifiClient.flush();
}

void askPos( void * parameter )
{
    while(1)
    {
      if ((messageLen = wifiClient.available()) > 0) {
        int i = 0;
        do
        {
            buf[i++] = wifiClient.read();
        }while(i<32 && buf[i-1]!='\r');
        
        buf[i-1] = '\0';
        send_mes("Position","");
      }
      delay(100);
    }
    
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
 
}
void get_buf(){
   char *pch;
         pch = strtok(buf,"|\0");
         int j =0;
         while (pch != NULL){
          int tmp = atoi(pch);
          pos[j]=tmp;
          j++;
          pch = strtok(NULL,"[,]() ");
          
          }
          for(int i=1;i<sizeof(buf);i++){ buf[i]={0}; } 
            xf = pos[1];yf = pos[2];Dx = pos[3];Dy = pos[4];
            Serial.println("pos[]:");
            Serial.println(pos[1]);
            Serial.println(pos[2]);
            Serial.println(pos[3]);
            Serial.println(pos[4]);
           
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

  
void D_first(float mx,float my,float posm){
  if(my==0){ // on the Axis
   if(mx >0){ right(adj); }else if(mx<0){ left(adj); }
    }else if(mx==0){
   if(my <0){ right(adj); }else if(my>0){ left(adj); }
  }
  else { // Quadrant
    if(mx>0 && my>0){ // first
      if(my/mx <= posm){ right(adj);}else{left(adj);}
    }else if(mx<0 && my>0){ // second
      left(adj);
    }else if(mx<0 && my<0){ // third
      if(my/mx <= posm){ left(adj);}else{right(adj);}
    }else if(mx<0 && my<0){ // fourth
      right(adj);
      }
    }
 }

void D_second(float mx,float my,float posm){
  if(my==0){ // on the X Axis
   if(mx >0){ right(adj); }else if(mx<0){ left(adj); }
    }else if(mx==0){ // on the Ｙ Axis
   if(my <0){ left(adj); }else if(my>0){ left(adj); }
  }
  else { // Quadrant
    if(mx>0 && my>0){ // first
      right(adj);
    }else if(mx<0 && my>0){ // second
      if(my/mx <= posm){ right(adj);}else{left(adj);}
    }else if(mx<0 && my<0){ // third
      left(adj);
      
    }else if(mx<0 && my<0){ // fourth
      if(my/mx <= posm){ left(adj);}else{right(adj);}
      }
    }
 }void D_third(float mx,float my,float posm){
  if(my==0){ // on the X Axis
   if(mx >0){ left(adj); }else if(mx<0){ right(adj); }
    }else if(mx==0){
   if(my <0){ left(adj); }else if(my>0){ right(adj); }
  }
  else { // Quadrant
    if(mx>0 && my>0){ // first
      if(my/mx <= posm){ left(adj);}else{right(adj);}
    }else if(mx<0 && my>0){ // second
      right(adj);
    }else if(mx<0 && my<0){ // third
      if(my/mx <= posm){ right(adj);}else{left(adj);}
    }else if(mx<0 && my<0){ // fourth
      left(adj);
      }
    }
 }

void D_fourth(float mx,float my,float posm){
  if(my==0){ // on the Axis
   if(mx >0){ left(adj); }else if(mx<0){ right(adj); }
    }else if(mx==0){
   if(my <0){ right(adj); }else if(my>0){ left(adj); }
  }
  else { // Quadrant
    if(mx>0 && my>0){ // first
      left(adj);
    }else if(mx<0 && my>0){ // second
      if(my/mx <= posm){ left(adj);}else{right(adj);}
    }else if(mx<0 && my<0){ // third
      right(adj);
    }else if(mx<0 && my<0){ // fourth
      if(my/mx <= posm){ right(adj);}else{left(adj);}
      }
    }
 }
void adjust_direction() {
    
      //adjust direction
      get_buf();
      x_s = xf;
      y_s = yf;
      float posx = Dx - x_s;
      float posy = Dy - y_s;
      float posm = posy / posx;
      Serial.println("x_s");
      Serial.println(x_s);
      Serial.println(y_s);
      delay(500);
      get_buf();
      Serial.println("f");
      Serial.println(xf);
      Serial.println(yf);
      xmov = xf - x_s;
      ymov = yf - y_s;
      if(posx>0 && posy>0){
          D_first(xmov,ymov,posm);
        }else if (posx<0 && posy>0)
        {  
          D_second(xmov,ymov,posm);
          }else if(posx<0 && posy<0)
        {  
          D_third(xmov,ymov,posm);
        }else{
          D_fourth(xmov,ymov,posm);
            }
            
      
  }
long *sense_obtacle(){
//  long dF, dL, dR;
  long ultra[3];
    ultra[0] = ultrasonicGetDistance(usTrigPins[U_F], usEchoPins[U_F]);
    ultra[1] = ultrasonicGetDistance(usTrigPins[U_L], usEchoPins[U_L]);
    ultra[2] = ultrasonicGetDistance(usTrigPins[U_R], usEchoPins[U_R]);
    return ultra;
    }
void loop()
{   
    
    adjust_direction();
    long *ultra_v=sense_obtacle();
    delay(100);
} 
