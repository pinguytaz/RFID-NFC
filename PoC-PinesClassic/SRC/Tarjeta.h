/*******************************************************************************************************
 * Autor: Fco. Javier Rodriguez Navarro
 * Web:   https:/www.pinguytaz.net
 * gihub: https://github.com/pinguytaz
 
 * Funciones tratamiento de tarjeta mediante lector RFID para PC-Serial
 *********************************************************************************************************/
#include <stdlib.h>
#include <nfc/nfc.h>
#include <string.h>
#include "../../Auxiliares/mifare.h"

nfc_device *pnd;

/******************   Inicializa ********************************/
int init()
{
   nfc_context *context;
   nfc_init(&context);
   
   if (context == NULL) 
   {
      printf("No se puede inicializar libnfc\n");
      return EXIT_FAILURE;
   }
   const char *acLibnfcVersion = nfc_version();  // Obtenemos version d la libreria-
  
   printf("Utiliza la version de libnfc %s\n", acLibnfcVersion);

   pnd = nfc_open(context, NULL);     //  Primero abrimos el dispositivo lector NFC 
   if (pnd == NULL) 
   {
      printf("ERROR: %s\n", "No he podido abrir el dispositivo Lector");
      return EXIT_FAILURE;
   }

   if (nfc_initiator_init(pnd) < 0) //Inicializa dispositivo NFC
   {
      nfc_perror(pnd, "nfc_initiator_init");
      return EXIT_FAILURE;
   }

   return 0;
}

/***************  Lectura de un bloque  ***********/
int leeB(unsigned char datos[16],unsigned char key[6],int32_t sector, int32_t bloque)
{
   int res = 0;  
   const nfc_modulation nmMifare = { .nmt = NMT_ISO14443A, .nbr = NBR_106};
   nfc_target nt;
   int32_t bt = (sector * 4) + bloque;

   res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt); 
   if (res>0) 
   {
      mifare_param mp;
      memcpy(mp.mpa.abtAuthUid, nt.nti.nai.abtUid + nt.nti.nai.szUidLen - 4, 4);
      memcpy(mp.mpa.abtKey, key, sizeof(mp.mpa.abtKey));

       if (nfc_initiator_mifare_cmd(pnd, MC_AUTH_A, bt, &mp)) 
       {
          if (nfc_initiator_mifare_cmd(pnd, MC_READ, bt, &mp)) 
          {
             /*if ((bt+1) %4 == 0) printf("Bloque Trailer: ");
             else printf("Bloque Datos: ");*/
             memcpy(datos, mp.mpd.abtData, 16);
          } 
          else 
          {
             printf("ERROR Lectura\n");
             return -1;
          }
       } 
       else
       {
         printf("ERROR Aut\n");
         return -1;
       }
   } 
   else
   {
      printf("Tarjeta no encontrada\n");
      return -1;
   }
 
   return 0;
}

int grabaB(unsigned char datos[16],unsigned char key[6],int32_t sector, int32_t bloque)       
{
   int res = 0;  
   const nfc_modulation nmMifare = { .nmt = NMT_ISO14443A, .nbr = NBR_106};
   nfc_target nt;
   int32_t bt = (sector * 4) + bloque;

   if (sector == 0 && bloque ==0)
   {
      printf("Bloque no permitido\n");
      return -1;
   }

   res = nfc_initiator_select_passive_target(pnd, nmMifare, NULL, 0, &nt); 
   if (res>0) 
   {
      mifare_param mp;
      memcpy(mp.mpa.abtAuthUid, nt.nti.nai.abtUid + nt.nti.nai.szUidLen - 4, 4);
      memcpy(mp.mpa.abtKey, key, sizeof(mp.mpa.abtKey));

       if (nfc_initiator_mifare_cmd(pnd, MC_AUTH_A, bt, &mp)) 
       {
         if ((bt+1) %4 == 0) 
         {
            memcpy(mp.mpt.abtKeyA, datos, sizeof(mp.mpt.abtKeyA));
            memcpy(mp.mpt.abtAccessBits, datos+6, sizeof(mp.mpt.abtAccessBits));
            memcpy(mp.mpt.abtKeyB, datos+10, sizeof(mp.mpt.abtKeyB));
         }
         else
         {
            memcpy(mp.mpd.abtData,datos, 16);  /* Son Datos */
         }
         if (nfc_initiator_mifare_cmd(pnd, MC_WRITE, bt, &mp)) 
         {
         } else printf("ERROR Escritura\n");
         return 0;
       } else printf("ERROR Aut\n");

   } else printf("Tarjeta no encontrada\n");
   
   return -1;
}
