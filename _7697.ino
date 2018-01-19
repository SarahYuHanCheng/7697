#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>
#include <LRTC.h>
#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

//
//#define SSID "CSIE-WLAN"
//#define PASSWD "wificsie"
//#define TCP_IP "192.168.208.124"
//#define SSID "1scream2.4G"
//#define PASSWD "2017scream"
#define SSID "scream"
#define PASSWD "s741852scream"
#define TCP_IP "192.168.0.50"
#define TCP_PORT 5000

xTaskHandle xHandle;

WiFiClient wifiClient;
static char buf[128],buf_send[128];

static int messageLen,phmessageLen;
static char client_ID[] = "aaa",P_ID[]="bbb",Team[] = "B";
static char *recv_ID,*recv_buf;
boolean start=true;

unsigned long time;
unsigned long ask_time;
char buffer[64];


void reg_ID()
{
    strcpy(buf,"Register");
    strcat(buf,Team);
    strcat(buf,"|");
    strcat(buf,client_ID);
    wifiClient.write(buf, strlen(buf));
    wifiClient.flush();
//    getPCtime();   // try to get time sync from pc        
//  if(DateTime.available()) { // update clocks if time has been synced
//    unsigned long prevtime = DateTime.now(); 
//    Serial.print("reg_ID time: ");
//    Serial.println(prevtime);
//  }
  
}
 
void setup() {  int status = WL_IDLE_STATUS;
    Serial.begin(9600);
    while (!Serial)
      ;      
     
    // set WiFi
//    WiFi.mode(WIFI_STA);
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
    
    delay(500);
     sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
    LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());

  Serial.println(buffer);
    xTaskCreate(
                    askPos,          /* Task function. */
                    "askPos",        /* String with name of task. */
                    10000,            /* Stack size in words. */
                    NULL,             /* Parameter passed as input of the task */
                    0,                /* Priority of the task. */
                    &xHandle);      
}
void send_mes(char ID[],char mes[])
{
    sprintf(buf_send,"%s|%s",ID,mes);
    wifiClient.write(buf_send, strlen(buf_send));
    wifiClient.flush();
}
void askPos( void * parameter )
{
  
  
    while(1)
    {
      if ((messageLen = wifiClient.available()) > 0) {
//        getPCtime();   // try to get time sync from pc  
      
//  if(DateTime.available()) { // update clocks if time has been synced
////    unsigned long prevtime = DateTime.now(); 
if(start){
    Serial.print("get msg from server : ");
//    Serial.println(prevtime);}
LRTC.get();

  // display the time
  sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
    LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());

  Serial.println(buffer);    
  Serial.print("Interval: ");
  time=millis()-ask_time;
      Serial.println(time);
        int i = 0;
        do
        {
            buf[i++] = wifiClient.read();
        }while(i<128 && buf[i-1]!='\r');
        
        buf[i-1] = '\0';
        Serial.print(buf);
        recv_ID = strtok(buf,"|\0");
        send_mes("AskPos",P_ID);
}else{
  int i = 0;

        do
        {
            buf[i++] = wifiClient.read();
        }while(i<128 && buf[i-1]!='\r');
        
        buf[i-1] = '\0';
        Serial.print(buf);
        if (strncmp("Master|Start\0", buf, 10)==0){
          start = true;
              LRTC.set(2018, 1, 16, 14, 30, 00);
                   delay(1000);
            LRTC.get();
            
              // display the time
              sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
                LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());
            Serial.println(client_ID);
              Serial.println(buffer);
//          Serial.println("test");
////          send_mes("Treasure","");
        }
        
//          getPCtime();   // try to get time sync from pc        
//  if(DateTime.available()) { // update clocks if time has been synced
//    unsigned long prevtime = DateTime.now(); 
//    Serial.print("reg_ID time: ");
//    Serial.println(prevtime);
//  }
//  char toPartner=client_ID+" in "+prevtime;
//        send_mes(P_ID,toPartner);
      }
      delay(100);
    }
  
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
//  }
 
}

}

void loop() {
//  vTaskSuspend( xHandle );
  delay(100);
  LRTC.get();

  // display the time
  sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
    LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());
Serial.print("SEND TO PARTNER: ");
  Serial.println(buffer);
//    char toPartner=client_ID+" in "+buffer;
        send_mes(P_ID,buffer);
//  if ((messageLen = wifiClient.available()) > 0) {
//        int i = 0;
//        do
//        {
//            buf[i++] = wifiClient.read();
//        }while(i<128 && buf[i-1]!='\r');
//        
//        buf[i-1] = '\0';
        if (strncmp("Master|Start\0", buf, 10)==0){
              LRTC.set(2018, 1, 16, 14, 30, 00);
                   delay(1000);
            LRTC.get();
            
              // display the time
              sprintf(buffer, "%ld/%ld/%ld %.2ld:%.2ld:%.2ld",
                LRTC.year(), LRTC.month(), LRTC.day(), LRTC.hour(), LRTC.minute(), LRTC.second());
            Serial.println(client_ID);
              Serial.println(buffer);
//          Serial.println("test");
////          send_mes("Treasure","");
        }//else{
//        Serial.println(buf);
//        recv_ID = strtok(buf,"|\0");
//        recv_buf = strtok(NULL,"|\0");
////        Serial.println(recv_buf);
////Serial.println("got");
////        Serial.print(recv_ID);
////        Serial.print(":");
////        recv_buf = strtok(NULL,"|\0");
////        Serial.println(recv_buf);
////       
//        
//        }
       
//      }else{
//        Serial.println("No msg");
//        
//        }
delay(1000);
 send_mes("Position","");
  ask_time = millis();
    Serial.print("ask_time: ");
    Serial.println(ask_time);
//       vTaskResume( xHandle );
      delay(100);
      
    
}
