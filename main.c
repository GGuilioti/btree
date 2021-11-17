#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define ORDEM 5
#define MAXCHAVES ORDEM - 1

int busca(int rrn, int chave, int *rrnEncontrado, int *posEncontrada)
{
  // printf("RRN: %i\n CAHVE: %i\n RRN_ENCONTRADO: %i\n POS_ENCONTRADA: %i\n", rrn, chave, *rrnEncontrado, *posEncontrada);
  int encontrada = 0;
  int pos = 0;
  PAGINA pag;

  FILE *arq = fopen("btree.dat", "r");

  if (rrn == -1)
  {
    return 0;
  }
  else
  {
    // leia a página armazenada no RRN para PAG
    fseek(arq, sizeof(PAGINA), SEEK_SET);
    fread(&pag, sizeof(PAGINA), 1, arq);
    encontrada = busca_na_pagina(chave, &pag, &pos);

    if (encontrada == 1)
    {
      *rrnEncontrado = rrn;
      *posEncontrada = pos;
      return 1; // encontrado
    }
    else
    {
      return busca(pag.FILHOS[pos], chave, rrnEncontrado, posEncontrada);
    }
  }
}

void insereChave(int rrnAtual, int chave, int *filhoDpro, int *chavePro)
{
}

void iniciaBtree(char *argv)
{
  FILE *btree;
  FILE *arqOrigem;

  if ((btree = fopen("btree.dat", "w")) == NULL || (arqOrigem = fopen(argv, "r")) == NULL)
  {
    printf("\nErro ao abrir o arquivo !");
    return;
  }

  int rrn = 0; // cabeçalho inicia com o rrn da pagina raiz, a primeira vez é 0
  fwrite( &rrn, sizeof(int), 1, btree);

  int chave;

  while (!feof(arqOrigem))
  {

    if(fscanf(arqOrigem, "%d|", &chave) == EOF)
    break;

    //insereChave(rrnAtual, chave, &filhoDpro, &chavePro);
  }

  printf("Operação bem sucedida, Arvore B criada com sucesso.");
  fclose(btree);
}

int main(int argc, char *argv[])
{

  if (argc == 3 && strcmp(argv[1], "-c") == 0)
  {
    int rr = 7, pos = 1;
    printf("Modo de criacao ativado ... nome do arquivo = %s\n", argv[2]);
    iniciaBtree(argv[2]);
    // busca(2, 35, &rr, &pos);
  }
  else if (argc == 2 && strcmp(argv[1], "-p") == 0)
  {

    printf("Modo de impressao ativado ...\n");
    //imprime();
  }
  else if (argc == 2 && strcmp(argv[1], "-k") == 0)
  {

    printf("Modo de impressao em ordem crescente ativado ...\n");
    //imprime_crescente();
  }
  else
  {

    fprintf(stderr, "Argumentos incorretos!\n");
    fprintf(stderr, "Modo de uso:\n");
    fprintf(stderr, "$ %s -c nome_arquivo\n", argv[0]);
    fprintf(stderr, "$ %s (-p|-k)\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  return 0;
}