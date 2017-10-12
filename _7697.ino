#include "TCS3200.h"

TCS3200 colorSensor(6, 7, 8, 9, 10);  // S0, S1, S2, S3, Out
static char buf[32];
void setup() {
  colorSensor.setOutFreqScaling(PERCENT_100);
 Serial.begin(9600);

}
void reportColorSensor()
{
  sprintf(buf,"R: %ld, G: %ld, B: %ld",
      colorSensor.getPeriod(RED),
      colorSensor.getPeriod(GREEN),
      colorSensor.getPeriod(BLUE));
      Serial.println(buf);
}
void loop() {
reportColorSensor();
delay(1000);
}
