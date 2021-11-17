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
    int byteoffset = 0;
    byteoffset = rrn*(MAXCHAVES+ORDEM) + 4;
    fseek(/*arquivo aqui*/, byteoffset, SEEK_SET);
}

void escrevePagina(int rrn, PAGINA pag)
{
    int byteoffset = 0;
    byteoffset = rrn*(MAXCHAVES+ORDEM) + 4;

    fwrite(&pag, sizeof(PAGINA), 1, /*arquivo aqui*/);
}

void inicializaPagina(PAGINA *pag)
{
    pag->CONTACHAVES = 0;

    for(int i = 0; i < MAXCHAVES-1; i++)
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

#endif