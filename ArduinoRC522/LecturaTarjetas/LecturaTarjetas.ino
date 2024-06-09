/*******************************************************************************************
 *  Autor: Fco. Javier Rodriguez Navarro
 *  WEB: www.pinguytaz.net
 *
 *  Descripción:  Lee e identifica el tipo de tarjeta.
 *
 * ---------------------------------------
 *             MFRC522      Arduino        
 * ---------------------------------------
 * SPI SS      SDA(SS)      D10            
 * SPI MOSI    MOSI         D11 / ICSP-4  
 * SPI MISO    MISO         D12 / ICSP-1  
 * SPI SCK     SCK          D13 / ICSP-3  
 *
 **************************************************************************************************/
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_pin(10); 
MFRC522DriverSPI driver{ss_pin}; // Crea el Driver SPI.
MFRC522 mfrc522{driver};  // Crea la instancia

void setup() 
{
  Serial.begin(9600);  
  mfrc522.PCD_Init();  // Inicia la placa.
  
  byte version = mfrc522.PCD_GetVersion();
  Serial.print("Version de Firmware: 0x");
  Serial.println(version,HEX);
  
  Serial.println("*******************");
  Serial.println("Lectura de tarjetas");
  Serial.println("*******************");
}

void loop() 
{
 	if (mfrc522.PICC_IsNewCardPresent())
  {
    Serial.println("Tarejeta Presente por lo tanto se lee");
    if (mfrc522.PICC_ReadCardSerial()) //Leemos realizando un Select de la tarjeta
    { 
      Serial.println("Tarjeta leida");
      Serial.print("Tamaño del UID: ");
      Serial.println(mfrc522.uid.size);
      Serial.print("UID: 0x");
      for (int i =0 ; i<mfrc522.uid.size; i++ ) 
      {
        if(mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
        Serial.print(mfrc522.uid.uidByte[i],HEX);
      }
      Serial.println("");
      Serial.print("SAK: 0x");
      if(mfrc522.uid.sak < 0x10) Serial.print("0");
      Serial.println(mfrc522.uid.sak,HEX);

      // Analizamos SAK para ver el tipo de tarjeta.
      Serial.print("Tipo de tarjeta: ");
      byte tipo = mfrc522.uid.sak;
      Serial.print(tipo,HEX);
      Serial.print(" --> ");

      switch(tipo) // Uniendolo a tamaño UID Tabla5 https://www.nxp.com/docs/en/application-note/AN10833.pdf
      {
          case 0x00: 
             Serial.println(" MiFARE UltraLigth / NTAG 2xx");
             break;
          case 0x04: 
             Serial.println(" UID incompleto ");
             break;
          case 0x08: 
             Serial.println(" MiFARE Classic 1K ");
             break;
          case 0x09: 
             Serial.println(" MiFARE Mini ");
             break;
          case 0x10: 
             Serial.println(" MiFARE Plus 2K SL2 ");
             break;
          case 0x11: 
             Serial.println(" MiFARE Plus 4K SL2 ");
             break;
          case 0x18: 
             Serial.println(" MiFARE Classic 4K ");
             break;
          case 0x19: 
             Serial.println(" MiFARE Classic 2K ");
             break;
          case 0x20: 
             Serial.println(" MiFARE Desfire / Plus / NTAG 4xx");
             break;
          default:
             Serial.println(" Desconocida ");
             
      }
    }
    else Serial.println("Error en la lectura de la tarjeta");
	}
  
  delay(3000);
  

  /*
 Serial.print(F("PICC type: "));
MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
 Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));
    */  


 }  
