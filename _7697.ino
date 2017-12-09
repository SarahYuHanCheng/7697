//#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>

#define SSID "1scream2.4G"
#define PASSWD "2017scream"
#define TCP_IP "192.168.0.69"
#define TCP_IP_PHONE "192.168.0.42"
#define TCP_PORT 5000

WiFiClient wifiClientPh;
xTaskHandle xHandle;
enum UltrasonicPinID {
    U_F = 0,
    U_L,
    U_R,
    NUM_OF_ULTRASONIC_PIN
};


IPAddress ip;
static char buf[48],bufPh[48],buf_send[32],buf_phsend[32];
static int messageLen,phmessageLen,state;
static char client_ID[] = "Sara",Team[] = "B";
static char *recv_ID,*recv_buf;
static int pos[5];
static const uint8_t usTrigPins[NUM_OF_ULTRASONIC_PIN] = {2, 4, 11 };  // F, L, R
static const uint8_t usEchoPins[NUM_OF_ULTRASONIC_PIN] = {3, 5, 12 };  // F, L, R
long ultrasonicGetDistance(uint8_t trig, uint8_t echo)
{
    long duration;

//    taskENTER_CRITICAL();
vTaskSuspend( xHandle );
    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    pinMode(echo, INPUT);
    duration = pulseIn(echo, HIGH, 5000000L);
//    delay(20);
//    taskEXIT_CRITICAL();
vTaskResume( xHandle );
    return duration / 29 / 2;
}
void send_phone(int x,long int* y)
{
    sprintf(buf_phsend,"(%d,%d)",x,y);
    wifiClientPh.write(buf_phsend, strlen(buf_phsend));
    wifiClientPh.flush();
}
 long ultra[3];
long *sense_obtacle(){
//  long dF, dL, dR;
 
    ultra[0] = ultrasonicGetDistance(usTrigPins[U_F], usEchoPins[U_F]);
    ultra[1] = ultrasonicGetDistance(usTrigPins[U_L], usEchoPins[U_L]);
    ultra[2] = ultrasonicGetDistance(usTrigPins[U_R], usEchoPins[U_R]);
   
    Serial.println("----------------");
    Serial.println(ultra[0]);
    Serial.println(ultra[1]);
    Serial.println(ultra[2]);
    return ultra;
    }

    void askPos( void * parameter )
{
  
    while(1)
    {
       send_phone('F',ultra);
    send_phone('L',ultra+1);
    send_phone('R',ultra+2);
    delay(80);
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
    
      while (!wifiClientPh.connect(TCP_IP_PHONE, TCP_PORT)){
      delay(300);
      Serial.print("Attempting to connect to SERVER: ");
        Serial.println(TCP_IP_PHONE);
    }

        xTaskCreate(
                    askPos,          /* Task function. */
                    "askPos",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    1,                /* Priority of the task. */
                    &xHandle);  
}

void loop() {
  // put your main code here, to run repeatedly:
sense_obtacle();
delay(50);
}
