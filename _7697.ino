/* Receive the controlling message, turning on/off and pwm, and
 * than set the corresponding pin.
 */
#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>
#define SSID "2scream"
#define PASSWD "2017scream"
#define TCP_IP "192.168.0.102"
#define TCP_IP_PHONE "192.168.0.101"
#define TCP_PORT 5000

WiFiClient wifiClient;
WiFiClient wifiClientPh;

static char buf[48],bufPh[48],buf_send[32],buf_phsend[32];
static int messageLen,phmessageLen;
static char client_ID[] = "Sarah",Team[] = "B";
static char *recv_ID,*recv_buf;
static int MyPosX,MyPosY,DstPosX,DstPosY;


IPAddress ip;

enum MotorPinID {
    L_F = 0,
    L_B,
    R_F,
    R_B,
    NUM_OF_MOTOR_PIN
};
static const uint8_t motorPins[NUM_OF_MOTOR_PIN] = {14, 15, 16, 17};  //  L_F, L_B,R_F, R_B

void setup()
{ 
  int motorpins=0;
  while(motorpins<NUM_OF_MOTOR_PIN){
    pinMode(motorPins[motorpins],OUTPUT);
    motorpins++;
  }
    int status = WL_IDLE_STATUS;
    Serial.begin(115200);
    while (!Serial)
      ;      

    // set WiFi
   // WiFi.mode(WIFI_STA);
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
    while (!wifiClientPh.connect(TCP_IP_PHONE, TCP_PORT)){
      delay(300);
      Serial.print("Attempting to connect to PHONE SERVER: ");
        Serial.println(TCP_IP_PHONE);
    }

    reg_ID();

    delay(1000);
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
    strcpy(buf,"Register|");
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
void send_phone(int x,int y)
{
    sprintf(buf_phsend,"(%d,%d)",x,y);
    wifiClientPh.write(buf_phsend, strlen(buf_phsend));
    wifiClientPh.flush();
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
        Serial.print(recv_ID);
        Serial.print(":");
        recv_buf = strtok(NULL,"|\0");
        
        int cmp = strcmp(recv_buf,"Start");
        int cmp2 = strcmp(recv_buf,"Done");
        if(cmp==0){
            printf("Start!!!");
        }else if(cmp2==0){
            printf("Done!!!");           
        }else{
            recv_buf = strtok(NULL,"|\0");
             printf(recv_buf);
            sscanf(recv_buf,"(%d,%d)(%d,%d)",&MyPosX,&MyPosY,&DstPosX,&DstPosY);
            int ttt = strcmp(MyPosX,"a");
            if(ttt==0){
                printf("cut right");
            }
        }
        Serial.println(recv_buf);
        sscanf(recv_buf,"POS:(%d,%d)(%d,%d)",&MyPosX,&MyPosY,&DstPosX,&DstPosY);
        Serial.println(MyPosX);
        Serial.println(MyPosY);
        
    send_phone(MyPosX,MyPosY); 
        send_mes("Position","");
      }
      delay(100);
    }
    
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
 
}
void forward(int t)
{
        Serial.print("F");
        digitalWrite(motorPins[L_F], HIGH);
        digitalWrite(motorPins[L_B], LOW);
        digitalWrite(motorPins[R_F], HIGH);
        digitalWrite(motorPins[R_B], LOW);
        delay(t);
}
void backward(int t)
{
        Serial.print("B");
        digitalWrite(motorPins[L_F], LOW);
        digitalWrite(motorPins[L_B], HIGH);
        digitalWrite(motorPins[R_F], LOW);
        digitalWrite(motorPins[R_B], HIGH);
        delay(t);
}
void left(int t)
{
        Serial.print("L");
        digitalWrite(motorPins[L_F], LOW);
        digitalWrite(motorPins[L_B], HIGH);
        digitalWrite(motorPins[R_F], HIGH);
        digitalWrite(motorPins[R_B], LOW);
        delay(t);
}
void right(int t)
{
        
        Serial.print("R");
        digitalWrite(motorPins[L_F], HIGH);
        digitalWrite(motorPins[L_B], LOW);
        digitalWrite(motorPins[R_F], LOW);
        digitalWrite(motorPins[R_B], HIGH);
        delay(t);
}

void sstop(int t)
{
        
        Serial.print("P");
        digitalWrite(motorPins[L_F], LOW);
        digitalWrite(motorPins[L_B], LOW);
        digitalWrite(motorPins[R_F], LOW);
        digitalWrite(motorPins[R_B], LOW);
        delay(t);
}

void handleCommand()
{
    // Stop moving
    if (bufPh[1] == 'E') {
        sstop(100);
        return;
    }

    switch (bufPh[0]) {
    case 'F':   // Forward
        forward(100);
        break;
    case 'B':   // Backward
        backward(100);
        break;
    case 'L':   // Turn left
        left(100);
        break;
    case 'R':   // Turn right
        right(100);
        break;
    case 'Z':   // Report ultrasonic distance and color
    //    reportUltrasonic();
    //reportColorSensor();
        break;
    }
}
void loop()
{   
    if ((phmessageLen = wifiClientPh.available()) > 0) {
        for (int i = 0; i < phmessageLen; ++i)
            bufPh[i] = wifiClientPh.read();
        bufPh[phmessageLen] = '\0';
       // Serial.println(buf);
       handleCommand();
    }
    
    delay(100);
} 
