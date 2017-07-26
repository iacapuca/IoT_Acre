//-------- Wi-Fi -----------
const char* ssid = "";
const char* password = "";

//-------- MQTT ------------
#define ORG "2kcheh"
#define DEVICE_TYPE "nodemcu"
#define DEVICE_ID "" // MAC ADDRESS
#define TOKEN "" // TOKEN DO BLUEMIX

//-------- GEOLOCATION ------
float lat;
float lng;

//-------- DHT11 ------------
#define DHTPIN 5 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float umi;
float temp;

//-------- PIR --------------
#define PIRPIN 16
int pirValue;

//-------- CHUVA ------------
#define RAINPIN 4
int rainValue;

//-------- MQ135 ------------
#define ANALOGPIN A0
MQ135 gasSensor = MQ135(ANALOGPIN);
float rzero;

//-------- MQ2 --------------
#define MQ2PIN 2
int mq2Value;
