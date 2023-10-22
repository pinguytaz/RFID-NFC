/************************************************************************************************************
 * PoC        Tarjeta MiFare Classic con informacion de claves
 *
 *   Autor: Fco. Javier Rodriguez Navarro
 *   Web:   https:/www.pinguytaz.net
 *   gihub: https://github.com/pinguytaz
 *
 *     HASH MD5 trastocado inicial y longitud
 *     Encripta mediante XOR
 *     
 *     Estructura se inicia en la posicion XOR de la clave (0-255)
 *     Clave en MD5T
 *     Numero de registros.
 *     Reg1 .... Regm
 *         clave(MD5T long_valor valor_X
 *
 *  Externos
 *           LibNFC:    http://www.libnfc.org/api/index.html
 *           LibCrypto:
 *           Ficheros mifare.h y mafare.c de nfctools (https://github.com/nfc-tools/libnfc/tree/master/utils)
 *************************************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "codifica.h"
#include "Tarjeta.h"
#include "Funciones.h"


int main(int argc, const char *argv[])
{
   int salida = -1;
   unsigned char tarjeta[1024];
   char laclave[20];  
   int pos_inicio;
   unsigned char key[] = {0xff, 0xff, 0xff, 0xff, 0xff,0xFF};
   unsigned char md5_clave[16];
   int pos=0;
   unsigned char datos[16];  
   
   if (argc >= 3 && argc <= 5)
   {
      strcpy(laclave, argv[1]);
      md5T((char *) laclave,md5_clave);   /* MD5 de la clave */
      /* Calculamos la posicion de inicio */
      pos_inicio = (int) laclave[0];
      for (int i=1 ; i< strlen(laclave); i++) pos_inicio = pos_inicio ^ laclave[i]; /* Posicion inicio estructura */

      if (strcmp(argv[2], "-f")==0 && argc==3)
      {
         printf("Formateamos la tarjeta con la clave %s\n",laclave);
         salida = formato(laclave, key, pos_inicio, md5_clave);
      }
      else if (strcmp(argv[2], "-v")==0 && argc==4)
      {
         char registro[20];   
         char valor[20];  
         strcpy(registro, argv[3]);
         printf("Visualizamos el registro %s\n", registro);
         salida = visualiza(laclave, key, pos_inicio, md5_clave, registro, valor);
         if (salida == 0) printf("Resultado %s\n",valor);
         else printf("No encontramos el registro\n");
      }
      else if (strcmp(argv[2], "-a")==0 && argc==5)
      {
         char registro[20];   
         char valor[20];  
         strcpy(registro, argv[3]);
         strcpy(valor, argv[4]);
         printf("Añadimos datos %s -- %s \n",registro,valor);
         salida = anade(laclave, key, pos_inicio, md5_clave, registro, valor);
      }
   }

   if (salida == -1)
   {
     printf("Uso: %s  clave -f | -v <reg> | -a <reg> <valor>]\n", argv[0]);
     printf(" -c Formatear tarjeta\n");
     printf(" -v <dato> Visualiza la clave escrita en el registro \"reg\" especificado\n");
     printf(" -a <reg> <valor> se añade el registro y valor a la tarjeta.\n");
     return salida;
   }

   /*     Para Debugear  */
   if(salida == 0)
   {
      pos=0;
      for (int sector=SINI; sector < 16; sector++)
         for (int bloque=0; bloque < BFIN; bloque++)
         {
            if(leeB(datos, key,sector,bloque) == 0) memcpy(tarjeta+pos,datos, 16);
            pos = pos +16;
         }
               
    /*
     printf("DEBUG: Volcado del contenido\n");
     //for(int i=0;i<((16-SINI)*(BFIN))*16;i++) printf("%02x",tarjeta[i]);
     int it = 0;
     for(int is=SINI;is<16;is++)
     {
        //printf("Sector %d: \n",is);
        for(int ib=0;ib<BFIN;ib++)
        {
            printf("Sector: %d Bloque %d: ",is, ib);
            for(int i=0;i<16;i++) printf("%02X",tarjeta[it++]);
            printf("\n");
        }
     }
     */
     
  }
}


               
