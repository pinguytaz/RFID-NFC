/*******************************************************************************************************
 * Autor: Fco. Javier Rodriguez Navarro
 * Web:   https:/www.pinguytaz.net
 * gihub: https://github.com/pinguytaz
 
 * Funciones genericas
 *********************************************************************************************************/


#define SINI 1
#define BFIN 3

struct reg
{
  unsigned char clave[16];  
  unsigned char longitud;
  unsigned char valor[20];
};

struct disco 
{
  unsigned char pwd[16];
  unsigned char n_reg;
  struct reg registros[];
};

typedef union {
  unsigned char tjt[1024] ;
  struct disco dsk;
} memoria;

/************ Salva disco a RFID **************/
int salvadisco (unsigned char key[], memoria disco, int pos_inicio)
{
   int pos;
   unsigned char datos[16];  
   unsigned char tarjeta[1024];

   /* Volcamos disco a formato tarjeta en la posicion inicio */
   for (int i=0; i < 720; i++) tarjeta[i+pos_inicio] = disco.tjt[i];
   for (int i = 0; i < pos_inicio; i++) tarjeta[i] = disco.tjt[i+450]; //Para tener aleatorios al inicio
   
   /* Volcamos los datos a la tarjeta RFID */
   pos = 0;
   for (int sector=SINI; sector < 16; sector++)
      for (int bloque=0; bloque < BFIN; bloque++)
      {
         memcpy(datos, tarjeta+pos, 16);
         pos=pos+16;
         grabaB(datos, key,sector,bloque) ;
      }

   return 0;
}

/******    Obtenemos los datos de la tarjeta  */
int cargadatos(unsigned char key[], unsigned char tjt[1024], int pos_inicio)
{
   int pos=0;
   int error=0;
   unsigned char tarjeta[1024];
   unsigned char datos[16];  

   for (int sector=SINI; sector < 16; sector++)
   {
      for (int bloque=0; bloque < BFIN; bloque++)
      {
        error =leeB(datos, key,sector,bloque);
        if(error == 0) 
        {
            memcpy(tarjeta+pos,datos, 16);
            pos = pos +16;
        }
      }
   }
   for (int i=0; i < 720; i++) tjt[i]= tarjeta[i+pos_inicio] ;
   
   return 0;
}

/*******************    Realiza un formateo de la tarjeta, añadiendo la clave de acceso *****/
int formato(char* clave, unsigned char key[], int pos_inicio, unsigned char md5_clave[16])
{
    memoria disco;  

    srand (time(NULL));
    int error = init();
    if(error != 0) return error;

    /* Aleatorizamos la tarjeta*/
    int pos=0;
    for (int i=0; i< 64; i++)
    {
       unsigned char tempo[16];
       char cadena[20];
       sprintf(cadena,"I%d-->%02xF",rand()%65,rand()%100);
       md5T(cadena,tempo);   
       //for(int acero=0; acero<16;acero++) disco.tjt[pos++] = 0x00;  // Para debugear
       memcpy(disco.tjt+pos, tempo, 16);
       pos=pos+16;
    }
    
    /* Ponemos la clave y el valor inicial de registros */
    memcpy(disco.dsk.pwd, md5_clave, 16);
    disco.dsk.n_reg=0;   

    salvadisco(key, disco, pos_inicio);
    
    return 0;
}

int visualiza(char* clave, unsigned char key[], int pos_inicio, unsigned char md5_clave[16], char registro[20], char valor[20])
{
   memoria disco;  
   unsigned char md5_registro[16];
   //unsigned char codifica[20];
   int num_registros = 0;
         
   //unsigned char tarjeta[1024];
   //int pos ;

   int error = init();
   if(error != 0) return error;

   cargadatos(key, disco.tjt, pos_inicio);
  
   /* Validamos la clave */
   for(int i=0;i<16;i++)
   {
      if(disco.dsk.pwd[i] != md5_clave[i])
      {
        printf("Clave incorrecta\n");
        return -3;
      }
   }
   
   md5T((char *) registro,md5_registro);  
   valor[0]=0;  /* En principio valor vacio no se encuentras*/
   num_registros = disco.dsk.n_reg;

   bool encontrado = false;
   for(int reg=0;reg<num_registros;reg++)
   {
      encontrado = true;
      for(int i=0;i<16;i++)
      {
         if(disco.dsk.registros[reg].clave[i] != md5_registro[i])
         {
           encontrado = false;
           break;
         }
      }
      if (encontrado)
      {
         desencripta(clave,registro, disco.dsk.registros[reg].valor ,valor,disco.dsk.registros[reg].longitud); 
         encontrado = true;
         break;
      }
   }

   if(encontrado)return 0;
   else return -2;
}

/********     Añade un registro ***********/
int anade(char* clave, unsigned char key[], int pos_inicio, unsigned char md5_clave[16], char registro[20], char valor[20])
{
   memoria disco;  
   unsigned char md5_registro[16];
   unsigned char xor_valor[20];
   int num_registros = 0; 
   int error = init();
   if(error != 0) return error;

   cargadatos(key, disco.tjt, pos_inicio);
  
   /* Validamos la clave */
   for(int i=0;i<16;i++)
   {
      if(disco.dsk.pwd[i] != md5_clave[i])
      {
        printf("Clave incorrecta\n");
        return -3;
      }
   }
   md5T((char *) registro,md5_registro);   // Generamos HASH del registro 
   encripta(clave,registro,valor,xor_valor); 
   num_registros = disco.dsk.n_reg++;

   /****    Añadimos el datos ****/   
   memcpy(disco.dsk.registros[num_registros].clave, md5_registro, 16);
   disco.dsk.registros[num_registros].longitud = (unsigned char) strlen(valor);
   memcpy(disco.dsk.registros[num_registros].valor, xor_valor, strlen(valor));

   salvadisco( key, disco,  pos_inicio);
   
   return 0;
}