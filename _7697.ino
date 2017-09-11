#include <WiFiClient.h>
#include <LWiFi.h>

#define SSID "Biigtree"
#define PASSWD "screamlab"
#define TCP_IP "172.20.10.2"
#define TCP_PORT 8888

int status = WL_IDLE_STATUS;
static unsigned long currentTimer = millis();

WiFiClient wifiClient;

void setup() {
    Serial.begin(9600);
    while (!Serial)
      ;
    while (status != WL_CONNECTED) { // 3
        delay(500);
        status =  WiFi.begin(SSID, PASSWD);
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(SSID);
        Serial.println(status);
    }
    // Conenct to AP successfully
    while (!wifiClient.connect(TCP_IP, 8888)){
      delay(300);
      Serial.print("Attempting to connect to SERVER: ");
        Serial.println(TCP_IP);
    }

   
}

void loop() {
  
  wifiClient.println("good connection");
  delay(2000);
  currentTimer = millis();
  if(millis() - currentTimer >= 10000) {
    if(!wifiClient.connected()){
      //reconnect
      setup();
    } else {
      currentTimer = millis();
    }
  }

}
