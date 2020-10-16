#include "seeed_bme680.h"
#define IIC_ADDR  uint8_t(0x76)

Seeed_BME680 bme680(IIC_ADDR); /* IIC PROTOCOL */

byte choixbyte[10];
char choix;
int choixint;
int i = 0;
String inputString = "";  //string pour récupérer les données entrantes
bool choixReceived = false;//vérification de la réception

void setup() {
  Serial.begin(9600);
  while (!Serial);
  //Serial.println("Serial start!!!");
  delay(100);
  while (!bme680.init())
  {
    Serial.println("bme680 init failed !");
    delay(1000);
  }
}

void serialEvent() //tourne en continue en attente d'un event, ici la réception de charactère
{
  //Serial.println("En attente d'un choix\n");
  while (Serial.available() > 0) // En attente d'un charactère
  {
    delay(50);
    for(i=0;i<10;i++) //On parcourt les 10 bytes (?!!!!?) envoyés
    {
      choixbyte[i] = Serial.read();
    }
    choix = atoi(choixbyte[0]); //Conversion de binaire à char, on ne garde que le premier
    
    if (choixbyte[0] == 97 | choixbyte[0]== 98 | choixbyte[0] == 99 | choixbyte[0] == 100) 
    {
      choixReceived = true;
    }
  }
}

/*----------------------------------------------------------*/
// Définition des Unions pour l'envoie des valeurs en float //
/*----------------------------------------------------------*/
union
  {
     float t;
     char tArray[4];                 //Le contenu de tArray[0] est le LSByte
  }tU;                               //temperature Union

union
  {
     float h;
     char hArray[4];                 //Le contenu de hArray[0] est le LSByte
  }hU;                               //humidite Union

union
  {
     float p;
     char pArray[4];                 //Le contenu de pArray[0] est le LSByte
  }pU;                               //pression Union

union
  {
     float g;
     char gArray[4];                 //Le contenu de gArray[0] est le LSByte
  }gU;                               //gaz Union
  
void loop() 
{
  if (bme680.read_sensor_data())
  {
    Serial.println("Reading failed :(");
    return;
  }

  if (choixReceived) 
  {
    //Envoie de l'information demandée
    int selection = choixbyte[0];
    switch (selection) 
    {
      case 97:
        tU.t = bme680.sensor_result_value.temperature;

        if(Serial.availableForWrite())
        {
          Serial.write(tU.tArray, 4); //Write tArray, 4 bytes onto serial
        }

        choixReceived = false;
        choix = "";
        break;
      case 98:
        hU.h = bme680.sensor_result_value.humidity;

        if(Serial.availableForWrite())
        {
          Serial.write(hU.hArray, 4); //Write hArray, 4 bytes onto serial
        }

        choixReceived = false;
        choix = "";
        break;
      case 99:
        pU.p = (float)(bme680.sensor_result_value.pressure / 1000.0);

        if(Serial.availableForWrite())
        {
          Serial.write(pU.pArray, 4); //Write pArray, 4 bytes onto serial
        }

        choixReceived = false;
        choix = "";
        break;
      case 100:
        gU.g = (bme680.sensor_result_value.gas / 1000.0);

        if(Serial.availableForWrite())
        {
          Serial.write(gU.gArray, 4); //Write gArray, 4 bytes onto serial
        }
        
        choixReceived = false;
        choix = "";
        break;
    }
  }
}
