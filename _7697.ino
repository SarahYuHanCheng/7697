//#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>

#define SSID "1scream2.4G"
#define PASSWD "2017scream"
#define TCP_IP "192.168.0.50"
#define TCP_IP_PHONE "192.168.0.42"
#define TCP_PORT 5000

WiFiClient wifiClient;
WiFiClient wifiClientPh;
xTaskHandle xHandle;
enum UltrasonicPinID {
    U_F = 0,
    U_L,
    U_R,
    NUM_OF_ULTRASONIC_PIN
};
enum MotorPinID {
    L_F = 0,
    L_B,
    R_F,
    R_B,
    NUM_OF_MOTOR_PIN
};
static const uint8_t motorPins[NUM_OF_MOTOR_PIN] = {14, 15, 16, 17};  //  L_F, L_B,R_F, R_B

int cnt;
IPAddress ip;
static char buf[48],bufPh[48],buf_send[32],buf_phsend[32];
static int messageLen,phmessageLen,state;
static char client_ID[] = "Zeo",Team[] = "B";
static char *recv_ID,*recv_buf;
static int pos[5];
static const uint8_t usTrigPins[NUM_OF_ULTRASONIC_PIN] = {2, 4, 11 };  // F, L, R
static const uint8_t usEchoPins[NUM_OF_ULTRASONIC_PIN] = {3, 5, 12 };  // F, L, R
static int MyPosX,MyPosY,DstPosX,DstPosY,DstPos1x,DstPos1y,DstPos3,DstPos4;

long ultrasonicGetDistance(uint8_t trig, uint8_t echo)
{
    long duration;

//    taskENTER_CRITICAL();
//vTaskSuspend( xHandle );
//    Serial.println("in suspend task");

    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    pinMode(echo, INPUT);
    duration = pulseIn(echo, HIGH, 5000000L);
//    taskEXIT_CRITICAL();
//vTaskResume( xHandle ); 
    return duration / 29 / 2;
}
void send_mes(char ID[],char mes[])
{
    sprintf(buf,"%s|%s",ID,mes);
    wifiClient.write(buf, strlen(buf));
    wifiClient.flush();
}

void reg_ID()
{
    strcpy(buf_send,"Register|");
    strcat(buf_send,client_ID);
    wifiClient.write(buf_send, strlen(buf_send));
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
        recv_ID = strtok(buf,"|\0");
        recv_buf = strtok(NULL,"|\0");
        Serial.println(recv_buf);
        int cmp = strcmp(recv_buf,"Start");
        int cmp2 = strcmp(recv_buf,"Done");
        recv_ID = strtok(recv_buf,":");
        int cmp3 = strcmp(recv_ID,"Treasure");
        int cmp4 = strcmp(recv_ID,"False");
        
        if(cmp==0){
            Serial.println("Start!!");
            cnt=1;
            send_mes("Treasure","");
            
        }else if(cmp2==0){
            Serial.println("Done!!!");           
        }else if(cmp3==0){
            Serial.println("Treasure!!!");   
                 sscanf(recv_buf,"(%d, %d)",&DstPosX,&DstPosY);
                  Serial.println("Dst:");
                 Serial.println(DstPosX);
                  Serial.println(DstPosY);
        }else if(cmp4==0){
          recv_buf = strtok(NULL,"|\0");
            Serial.println("Whose:");
            Serial.println(recv_buf);

char partner[] = {'123', '132'};
            send_mes(recv_buf,partner);
                
        }else{
            
            recv_buf = strtok(NULL,":\0");
            sscanf(recv_buf,"POS:(%d, %d)",&MyPosX,&MyPosY);
           
        }
        
//    send_phone(MyPosX,MyPosY); 
        send_mes("Position","");
      }
      delay(100);
    }
    
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
 
}

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
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

      while (!wifiClient.connect(TCP_IP, TCP_PORT)){
      delay(300);
      Serial.print("Attempting to connect to SERVER: ");
        Serial.println(TCP_IP);
    }
reg_ID();

//    
//      while (!wifiClientPh.connect(TCP_IP_PHONE, TCP_PORT)){
//      delay(300);
//      Serial.print("Attempting to connect to SERVER: ");
//        Serial.println(TCP_IP_PHONE);
//    }

        xTaskCreate(
                    askPos,          /* Task function. */
                    "askPos",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    0,                /* Priority of the task. */
                    &xHandle);  
                    int motorpins=0;

  while(motorpins<NUM_OF_MOTOR_PIN){
    pinMode(motorPins[motorpins],OUTPUT);
    motorpins++;
  }

}

void loop() {
  // put your main code here, to run repeatedly:
//cnt +=1;
long dF, dL, dR;
  if(cnt>0){
//if(cnt>1000){
//  Serial.println("suspend");
   dF = ultrasonicGetDistance(usTrigPins[U_F], usEchoPins[U_F]);
    dL = ultrasonicGetDistance(usTrigPins[U_L], usEchoPins[U_L]);
     dR = ultrasonicGetDistance(usTrigPins[U_R], usEchoPins[U_R]);
Serial.println("loop ULTRA:");
Serial.println(dF);
Serial.println(dL);
Serial.println(dR);     
//  cnt = 0;}//delay(50);
  delay(300);
  }
}
