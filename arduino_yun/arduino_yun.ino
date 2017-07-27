/**
   Acre IoT Workshop - Arduino Yun
   Autor: Pedro Luiz @iacapuca

 */

 #include <YunClient.h>
 #include <PubSubClient.h>
 #include <Console.h>
 #include <DHT.h>
 #include "config.h"

 float sensor_volt;
 float RS_gas; // Get value of RS in a GAS
 float ratio; // Get ratio RS_GAS/RS_air
 float lgPPM;
 int ppm;

 char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
 char topic[] = "iot-2/evt/status/fmt/json";
 char authMethod[] = "use-token-auth";
 char token[] = TOKEN;
 char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

 YunClient yunClient;
 PubSubClient client(server, 1883, NULL, yunClient);

void setup() {
    Serial.begin(115200);
    Bridge.begin();
    Console.begin();
    Serial.print("Calibrating...\n");
    Ro = MQCalibration(MQ5PIN);                       //Calibrating the sensor. Please make sure the sensor is in clean air
                                                    //when you perform the calibration
  Serial.print("Calibration is done...\n");
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
}

void loop() {

  if (!client.connected()) {
          Serial.print("Reconnecting client to ");
          Serial.println(server);
          while (!client.connect(clientId, authMethod, token)) {
                  Serial.print(".");
                  delay(500);
          }
          Serial.println();
  }

          /*-----------------------------------------------------------------------*/

          String payload = "{\"acre2\":{\"Name\":\"B4218AF04AD4\"";
          payload += ",\"LPG PPM\":";
          payload += (MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_LPG));
          payload += "}}";

          Serial.print("LPG:");
          Serial.print(MQGetGasPercentage(MQRead(MQ5PIN)/Ro,GAS_LPG) );

    Serial.print("Sending payload: ");
    Serial.println(payload);

    if (client.publish(topic, (char*) payload.c_str())) {
            Serial.println("Publish ok");
    } else {
            Serial.println("Publish failed");
    }

    delay(1000);

}

float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE_MQ5*(1023-raw_adc)/raw_adc));
}

float MQCalibration(int mq_pin)
{
  int i;
  float RS_AIR_val=0,r0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {                     //take multiple samples
    RS_AIR_val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  RS_AIR_val = RS_AIR_val/CALIBARAION_SAMPLE_TIMES;              //calculate the average value

  r0 = RS_AIR_val/RO_CLEAN_AIR_FACTOR_MQ5;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro
                                                                 //according to the chart in the datasheet

  return r0;
}

float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;
}

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function uses different equations representing curves of each gas to
         calculate the ppm (parts per million) of the target gas.
************************************************************************************/
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,((-3.986*(log10(rs_ro_ratio))) + 3.075)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.513*(log10(rs_ro_ratio))) + 1.878)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,((-2.554*(log10(rs_ro_ratio))) + 2.265 )));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,((-6.900*(log10(rs_ro_ratio))) + 6.241)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,((-4.590*(log10(rs_ro_ratio))) + 4.851)));
  }
}

  else if ( accuracy == 1 ) {
    if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,(-22.89*pow((log10(rs_ro_ratio)), 3) + 8.873*pow((log10(rs_ro_ratio)), 2) - 3.587*(log10(rs_ro_ratio)) + 2.948)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.513*(log10(rs_ro_ratio))) + 1.878)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,(-0.428*pow((log10(rs_ro_ratio)), 2) - 2.867*(log10(rs_ro_ratio)) + 2.224)));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,(1401*pow((log10(rs_ro_ratio)), 4) - 2777*pow((log10(rs_ro_ratio)), 3) + 2059*pow((log10(rs_ro_ratio)), 2) - 682.5*(log10(rs_ro_ratio)) + 88.81)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,(14.90*pow((log10(rs_ro_ratio)), 3) - 19.26*pow((log10(rs_ro_ratio)), 2) + 3.108*(log10(rs_ro_ratio)) + 3.922)));
  }
}
  return 0;
}
