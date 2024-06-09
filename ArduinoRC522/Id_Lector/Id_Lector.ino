/*******************************************************************************************
 *  Autor: Fco. Javier Rodriguez Navarro
 *  WEB: www.pinguytaz.net
 *
 *  Descripción:  Verificación RFC522, identificando la versión
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

MFRC522DriverPinSimple ss_pin(10); 
MFRC522DriverSPI driver{ss_pin}; // Crea el Driver SPI.
MFRC522 mfrc522{driver};  // Crea la instancia

void setup() 
{
  Serial.begin(9600);  
  mfrc522.PCD_Init();  // Inicia la placa.
  
  Serial.println("************");
  Serial.println("test MFRC522");
  Serial.println("************");
  byte version = mfrc522.PCD_GetVersion();
  Serial.print("Version de Firmware: 0x");
  Serial.println(version,HEX);
  Serial.println("Test de la placa: ");
  bool resultado = mfrc522.PCD_PerformSelfTest(); 
  if (resultado)
    Serial.println("OK");
  else
    Serial.println("Defectuoso o desconocido");
  Serial.println();

  byte ganancia = mfrc522.PCD_GetAntennaGain();
  Serial.print("Mascara ganancia de la antena: 0x");
  Serial.println(ganancia,HEX);
}

void loop() {} // No realiza nada 
