/**
  Acre IoT Workshop - NodeMCU
  Autor: Pedro Luiz @iacapuca

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <ArduinoJson.h>
#include <DHT.h>


//-------- Wi-Fi -----------
const char* ssid = "Euler";
const char* password = "e^(i*pi)+1=0";

//-------- MQTT ------------
#define ORG "2kcheh"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "5CCF7F248AA7"
#define TOKEN "v6lDrq0kqLWUzLFmHs"

char server[] = ORG".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

//-------- DEBUG ONLY -------

//-------- DHT11 ------------
#define DHTPIN 5 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float temp;

// functions
void getData();

//------- void setuo() ------
void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to "); Serial.print(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
   }
   Serial.println("");

   Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
   dht.begin();

}

void loop(){
  if (!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }

getData();

   //Logs na porta serial
     Serial.print("Temperatura: ");
     Serial.print(temp);
     Serial.print(" Umidade: ");
     Serial.println(umi);


 String payload = "{\"acre1\":{\"Name\":\"5CCF7F248AA7\"";
 payload += ",\"temp\":";
 payload += temp;
 payload += ",\"umi\":";
 payload += umi;
 payload += "}}";

 Serial.print("Sending payload: ");
 Serial.println(payload);

 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }

 delay(300000);
}

void getData(){
  //Leitura de umidade
  float umi = dht.readHumidity();
  //Leitura de temperatura
  float temp = dht.readTemperature();
  //Se não for um numero retorna erro de leitura
  if (isnan(umi) || isnan(temp)) {
    Serial.println("Erro ao ler o sensor!");
    return;
  }
}
