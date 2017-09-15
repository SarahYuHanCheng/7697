/* Receive the controlling message, turning on/off and pwm, and
 * than set the corresponding pin.
 */
#include <WiFiClient.h>
#include <LWiFi.h>
#include <task.h>

#define SSID "CSIE-WLAN"
#define PASSWD "wificsie"
#define TCP_IP "192.168.208.204"
#define TCP_PORT 5000

WiFiClient wifiClient;

static char buf[32];
static int messageLen;
static char client_ID[] = "Man";
static char *recv_ID,*recv_buf;



IPAddress ip;
void setup()
{
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
        Serial.println(recv_buf);
        send_mes("Position","");
      }
      delay(100);
    }
    
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
 
}

void loop()
{   
    
    
    delay(100);
} 
