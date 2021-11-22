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
    encontrada = buscaNaPagina(chave, &pag, &pos);

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

//int insereChave(int rrnAtual, int chave, int *filhoDpro, int *proxChave)
//{
//  PAGINA pag;
//  PAGINA novaPag;
//  int pos;
//  int rrnPro = filhoDpro;
//  int chavePro = proxChave;
//
//  int retorno = 0;
//  //PROMOÇÃO        2
//  //SEM PROMOÇÃO    1
//  //ERRO           -1
//
//  if (rrnAtual = -1)
//  {
//    proxChave = chave;
//    filhoDpro = -1;
//    return retorno = 2;
//
//  }else
//  {
//    lePagina(rrnAtual, &pag);
//    //ENCONTRADA = int buscaNaPagina(chave, pag, pos);
//  }
//  if(/*ENCONTRADA == 1*/)
//  {
//    printf("Chave Duplicada");
//    return retorno = -1;
//  }
//
//  retorno = insere(pag->filhos[pos], chave, rrnPro, chavePro);
//
//  if(retorno == 1 || -1)
//  {
//    return retorno;
//  }else
//  {
//    if(/*EXISTE ESPAÇO EM PAG PARA INSERIR CHAVEPRO*/)
//    {
//      insereNaPagina(chavePro, rrnPro, &pag);
//      escrevePagina(rrnAtual, pag);
//      return retorno = 1; 
//      
//    }else
//    {
//      //divide(chavePro, rrnPro, &pag, chavePro, filhoDpro, &novaPag);
//      escrevePagina(rrnAtual, pag);
//      escrevePagina(filhoDpro, novaPag);
//      return retorno = 2; 
//    }
//  }
//}

void gerenciador(char *argv)
{
  FILE *btree = fopen("btree.dat", "r");
  FILE *arqOrigem = fopen(argv, "r");
  int raiz;
  PAGINA novaPagina;
  int controle;

  if(!(btree == NULL))
  {
    btree = fopen("btree.dat", "r+w");
    raiz = leCabecalho(btree);

  }else
  {
    btree = fopen("btree.dat", "w");
    raiz = 0;
    fwrite(&raiz, sizeof(int), 1, btree);
    inicializaPagina(&novaPagina);
    escrevePagina(raiz, novaPagina);
    mostraPagina(btree, raiz, &novaPagina);
  }

  int chave;
  fscanf(arqOrigem, "%d|", &chave);

  while(!feof(arqOrigem))
  {
    if(chave == EOF)
    {
      break;
    }
    else
    {
      if(controle = insereChave(raiz, chave, /*&filhoDpro, &chavePro*/) == 2)
      {
        inicializaPagina(&novaPagina);
        novaPagina.CHAVES[0] = chavePro;
        novaPagina.FILHOS[0] = raiz;
        novaPagina.FILHOS[1] = filhoDpro;
        escrevePagina(raiz, novaPagina);
        raiz; // raiz recebe rrn da nova pagina
      }
    }
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
    gerenciador(argv[2]);
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