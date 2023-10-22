/*******************************************************************************************************
 * Autor: Fco. Javier Rodriguez Navarro
 * Web:   https:/www.pinguytaz.net
 * gihub: https://github.com/pinguytaz
 
 * Funciones genericasde encriptado
 *********************************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>

void md5T(char valor[], unsigned char res[16])
{
   EVP_MD_CTX *context = EVP_MD_CTX_new();
   const EVP_MD* md = EVP_md5();
   unsigned char md_value[EVP_MAX_MD_SIZE];
   unsigned int  md_len;
   
   /*  Lo primero es calcular el MD5 correcto */
   EVP_DigestInit_ex2(context, md, NULL);
   EVP_DigestUpdate(context, valor, strlen(valor));
   EVP_DigestFinal_ex(context, md_value, &md_len);
   EVP_MD_CTX_free(context);

   for (unsigned int i = 0 ; i < md_len ; ++i)
     res[i] = (char) md_value[i];
   
   /* Realizamos cambio para trastocar MD5 */
   int p1 = strlen(valor) & 15;
   int p2 = valor[0] & 15;
   unsigned char v = res[p1];
   res[p1] = res[p2];
   res[p2] = v;
}

int encripta(char pwd[], char reg[], char valor[], unsigned char res[])
{
  char *semilla;
  semilla = malloc((strlen(reg)+strlen(pwd))*sizeof(char)*2);
  sprintf(semilla,"%s%s%s%s",reg,pwd,pwd,reg);   /* Creamos la semilla */
  
  for (int i=0; i < strlen(valor); i++) 
  {
    res[i] = valor[i] ^ semilla[i];  
  }

  free(semilla);
  return 0;
}
int desencripta(char pwd[], char reg[], unsigned char codifica[], char res[], int longitud)
{
  char *semilla;
  semilla = malloc((strlen(reg)+strlen(pwd))*sizeof(char)*2);
  sprintf(semilla,"%s%s%s%s",reg,pwd,pwd,reg);   /* Creamos la semilla */
  
  for (int i=0; i < longitud; i++) 
  {
    res[i] = codifica[i] ^ semilla[i];
  }    
  res[longitud] = 0;

  free(semilla);
  return 0;
}
