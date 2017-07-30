#include <NewPing.h>
#include <YunClient.h>
#include <PubSubClient.h>
#include <Console.h>
#include "config.h"

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

YunClient yunClient;
PubSubClient client(server, 1883, NULL, yunClient);

int y;
float echoTime;
int nivel;

void setup() {
  Serial.begin(115200);
  Bridge.begin();
  Console.begin();
}

void loop() {                    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  if (!client.connected()) {
          Serial.print("Reconnecting client to ");
          Serial.println(server);
          while (!client.connect(clientId, authMethod, token)) {
                  Serial.print(".");
                  delay(500);
          }
          Serial.println();
  }
  echoTime = sonar.ping_median(10);
  y = sonar.convert_cm(echoTime);
  Serial.print("Ping: ");
  nivel = map(y,25,0,min_rio,max_rio);
  Serial.print(nivel); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
  String payload = "{\"acre1\":{\"Name\":\"\"";
  payload += ",\"nivel\":";
  payload += nivel;
  payload += "}}";

  Serial.print("Nivel do Rio:");
  Serial.print(nivel);

Serial.print("Sending payload: ");
Serial.println(payload);

if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
} else {
    Serial.println("Publish failed");
}
delay(1000);
}
