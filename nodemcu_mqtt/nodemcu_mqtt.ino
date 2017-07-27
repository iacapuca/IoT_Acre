/**
   Acre IoT Workshop - NodeMCU
   Autor: Pedro Luiz @iacapuca

 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <ArduinoJson.h>
#include <DHT.h>
#include <MQ135.h>
#include "config.h"


char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

//-------- DEBUG ONLY -------

// functions


//------- void setup() ------
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
        pinMode(PIRPIN, INPUT);
        pinMode(RAINPIN, INPUT);

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
        Serial.println("PIR: ");
        Serial.print(pirValue);
        Serial.println("Chuva: ");
        Serial.print(rainValue);
        Serial.println("MQ135: ");
        Serial.print(rzero);
        Serial.println("MQ2: ");
        Serial.print(mq2Value);



        String payload = "{\"acre1\":{\"Name\":\"5CCF7F248AA7\"";
        payload += ",\"temp\":";
        payload += temp;
        payload += ",\"umi\":";
        payload += umi;
        payload += ",\"pir\":";
        payload += pirValue;
        payload += ",\"rain\":";
        payload += rainValue;
        payload += ",\"mq2\":";
        payload += mq2Value;
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
        tempData();
        pirData();
        rainData();
        mq135Data();
        mq2Data();
}

void tempData(){
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

void pirData(){
        //Leitura do PIR
        pirValue = digitalRead(PIRPIN);
}

void rainData(){
    //Leitura do Sensor de Chuva
    rainValue = digitalRead(RAINPIN);
}

void mq135Data(){
  //BURN do MQ135
  float rzero = gasSensor.getRZero();
}

void mq2Data(){
  //Leitura digital do Sensor mq2Data
  mq2Value = digitalRead(MQ2PIN);
}
