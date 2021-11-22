#ifndef  UTIL_H
#define  UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 5
#define MAXCHAVES ORDEM - 1

typedef struct
{
  int CONTACHAVES;
  int CHAVES[MAXCHAVES];
  int FILHOS[MAXCHAVES + 1];
} PAGINA;

void lePagina(int rrn, PAGINA *pag)
{
    FILE *arq = fopen("btree.dat", "r");
    int byteoffset = 0;
    byteoffset = rrn*(MAXCHAVES+ORDEM) + 4;
    fseek(arq, byteoffset, SEEK_SET);
}

void escrevePagina(int rrn, PAGINA pag)
{
    FILE *arq = fopen("btree.dat", "w");
    int byteoffset = 0;
    byteoffset = rrn*(MAXCHAVES+ORDEM) + 4;

    fwrite(&pag, sizeof(PAGINA), 1, arq);
}

void inicializaPagina(PAGINA *pag)
{
    pag->CONTACHAVES = 0;

    for(int i = 0; i < MAXCHAVES; i++)
    {
        pag->CHAVES[i] = 0;
        pag->FILHOS[i] = -1;
    }
    pag->FILHOS[MAXCHAVES] = -1;
}

int buscaNaPagina(int chave, PAGINA *pag, int *pos)
{
  int i = 0;

  while (i < pag->CONTACHAVES && chave > pag->CHAVES[i])
  {
    i++;
    *pos = i;

    if (*pos < pag->CONTACHAVES && chave == pag->CHAVES[*pos])
    {
      return 1; // encontrado
    }
    else
    {
      return 0; // não encontrado
    }
  }
}

void insereNaPagina(int chave, int filhoD, PAGINA *pag)
{
    int i = pag->CONTACHAVES;
    while(i > 0 && chave < pag->CHAVES[i-1])
    {
        pag->CHAVES[i] = pag->CHAVES[i-1];
        pag->FILHOS[i+1] = pag->FILHOS[i];
        i--;
    }
    pag->CONTACHAVES++;
    pag->CHAVES[i] = chave;
    pag->FILHOS[i+1] = filhoD;

}

void divide(int chave, int filhoD, PAGINA *pag, int *proxchave, int *filhoDpro, PAGINA *novaPag)
{
  
}

int leCabecalho(FILE *arq)
{
  int buffer;
  fseek(arq, 0, SEEK_SET);
  fread(&buffer, sizeof(int), 1, arq);
  return buffer;
}

void mostraPagina(FILE *arq, int rrn, PAGINA *buffer)
{
  //PAGINA buffer;
  fseek(arq, 4+(rrn*MAXCHAVES+ORDEM), SEEK_SET);
  fread(&buffer, sizeof(PAGINA), 1, arq);

  printf("|");
  for(int i = 0; i < MAXCHAVES; i++)
  {
    printf(" %i |", buffer->CHAVES[i]);
  }

  printf("\n|");
  for(int i = 0; i < MAXCHAVES; i++)
  {
    printf(" %i |", buffer->FILHOS[i]);
  }
  printf(" %i |", buffer->FILHOS[MAXCHAVES]);
}

#endif