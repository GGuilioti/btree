#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "util.h"

#define ORDEM 5
#define MAXCHAVES ORDEM - 1

typedef struct
{
	int CONTACHAVES;
	int CHAVES[MAXCHAVES];
	int FILHOS[ORDEM];
} PAGINA;

typedef struct
{
	int CONTACHAVES;
	int CHAVES[MAXCHAVES + 1];
	int FILHOS[ORDEM + 1];
} PAGINA_AUX;

int contRrnPagina = 0;

void imprimePagina(PAGINA pag)
{
	for (int i = 0; i < MAXCHAVES; i++)
	{
		printf("%i |", pag.CHAVES[i]);
	}
	printf("\n");
	for (int i = 0; i < ORDEM; i++)
	{
		printf("%i |", pag.FILHOS[i]);
	}
	printf("\n");
}

int leCabecalho(FILE *arq)
{
	int buffer;
	fseek(arq, 0, SEEK_SET);
	fread(&buffer, sizeof(int), 1, arq);
	return buffer;
}

void inicializaPagina(PAGINA *pag)
{
	pag->CONTACHAVES = 0;

	for (int i = 0; i < MAXCHAVES; i++)
	{
		pag->CHAVES[i] = 0;
		pag->FILHOS[i] = -1;
	}
	pag->FILHOS[MAXCHAVES] = -1;

	// contRrnPagina++;
}

int rrnNovaPagina()
{
	FILE *arq = fopen("btree.dat", "r");

	int tamPagina = sizeof(PAGINA);
	int tamCabecalho = sizeof(int);
	fseek(arq, 0, SEEK_END);
	int byteOffset = ftell(arq);

	int novoRrn = (byteOffset - tamCabecalho) / tamPagina;

	return novoRrn;
}

void insereNaPagina(int chave, int filhoD, PAGINA *pag) // usar só quando tem espaço (CONTACHAVES < MAXCHAVES)
{
	int i = pag->CONTACHAVES;

	while (i > 0 && chave < pag->CHAVES[i - 1])
	{
		pag->CHAVES[i] = pag->CHAVES[i - 1];
		pag->FILHOS[i + 1] = pag->FILHOS[i];
		i--;
	}

	pag->CONTACHAVES++;
	pag->CHAVES[i] = chave;
	pag->FILHOS[i + 1] = filhoD;
}

void escrevePagina(int rrn, PAGINA pag)
{
	FILE *arq = fopen("btree.dat", "r+");
	int byteoffset = 0;
	byteoffset = rrn * sizeof(PAGINA) + sizeof(int);
	fseek(arq, byteoffset, SEEK_SET);
	fwrite(&pag, sizeof(PAGINA), 1, arq);
	fclose(arq);
}

void lePagina(int rrn, PAGINA *pag)
{
	FILE *arq = fopen("btree.dat", "r+");
	int byteoffset = 0;
	byteoffset = rrn * sizeof(PAGINA) + sizeof(int);
	fseek(arq, byteoffset, SEEK_SET);
	fread(pag, sizeof(PAGINA), 1, arq);
	fclose(arq);
}

int buscaNaPagina(int chave, PAGINA pag, int *pos)
{
	int i = 0;

	while (i < pag.CONTACHAVES && chave > pag.CHAVES[i])
	{
		i++;
	}

	*pos = i;

	if (*pos < pag.CONTACHAVES && chave == pag.CHAVES[*pos])
	{
		return 1; // encontrado
	}
	else
	{
		return -1; // não encontrado
	}
}

void divide(int chave, int filhoD, PAGINA *pag, int *chavePro, int *filhoDpro, PAGINA *novaPag)
{
	PAGINA_AUX pagAux;
	// PAGINA novaPag;
	int i;
	int meio;

	for (int i = 0; i < MAXCHAVES; i++) // copia chaves da pag para pagAux
	{
		pagAux.CHAVES[i] = pag->CHAVES[i];
	}

	for (int i = 0; i < MAXCHAVES + 1; i++) // copia filhos da pag para pagAux
	{
		int j = 0;
		while (j < MAXCHAVES + 1)
		{
			pagAux.FILHOS[j] = -1; // preenche tudo com -1 primeiro, pq no final o ultimo tem q ser -1
			j++;
		}

		pagAux.FILHOS[i] = pag->FILHOS[i]; // aqui sobreescreve deixando só o ultimo -1
	}

	//insere chave e filhoD nos lugares apropriados em pagAux
	i = MAXCHAVES;

	while (i > 0 && chave < pagAux.CHAVES[i - 1])
	{
		pagAux.CHAVES[i - 1] = pagAux.CHAVES[i - 1];
		pagAux.FILHOS[i + 1] = pagAux.FILHOS[i];
		i--;
	}

	pagAux.CHAVES[i] = chave;
	pagAux.FILHOS[i + 1] = filhoD;

	meio = ORDEM / 2;
	*filhoDpro = rrnNovaPagina();
	*chavePro = pagAux.CHAVES[meio];
	//Copie as chaves e ponteiros que vêm antes de chavePro para pag

	i = 0;
	inicializaPagina(pag);

	while (i < meio)
	{
		pag->CHAVES[i] = pagAux.CHAVES[i];
		pag->FILHOS[i] = pagAux.FILHOS[i];
		pag->CONTACHAVES++;
		i++;
	}

	pag->FILHOS[i] = pagAux.FILHOS[i];
	//Copie as chaves e ponteiros que vêm depois de chavePro para novaPag

	inicializaPagina(novaPag);
	i = meio + 1;

	while (i < MAXCHAVES + 1)
	{
		novaPag->CHAVES[novaPag->CONTACHAVES] = pagAux.CHAVES[i];
		novaPag->FILHOS[novaPag->CONTACHAVES] = pagAux.FILHOS[i];
		novaPag->CONTACHAVES++;
		i++;
	}

	novaPag->FILHOS[novaPag->CONTACHAVES] = pagAux.FILHOS[i];
}

int insere(int rrnAtual, int chave, int *filhoDpro, int *chavePro)
{
	PAGINA pag;
	PAGINA novaPag;
	int pos;
	int rrnPro = *filhoDpro;
	int chvPro = *chavePro;
	int encontrada = 0;

	int retorno = 0;
	//PROMOÇÃO        2
	//SEM PROMOÇÃO    1
	//ERRO           -1

	// printf("\n%i", rrnAtual);

	if (rrnAtual == -1)
	{
		*chavePro = chave;
		*filhoDpro = -1;
		retorno = 2;
		return retorno;
	}
	else
	{

		lePagina(rrnAtual, &pag);
		encontrada = buscaNaPagina(chave, pag, &pos);
	}

	if (encontrada == 1)
	{
		printf("Chave Duplicada\n");
		retorno = -1;
		// imprimePagina(pag);
		return retorno;
	}

	retorno = insere(pag.FILHOS[pos], chave, &rrnPro, &chvPro);

	if (retorno == 1 || retorno == -1)
	{
		return retorno;
	}
	else
	{
		if (pag.CONTACHAVES < MAXCHAVES)
		{
			insereNaPagina(chvPro, rrnPro, &pag);
			escrevePagina(rrnAtual, pag);
			return retorno = 1;
		}
		else
		{

			divide(chvPro, rrnPro, &pag, chavePro, filhoDpro, &novaPag);

			printf("\nRRN ATUAL: %i\n", rrnAtual);
			printf("\nFILHO DE PRO: %i\n\n", *filhoDpro);
			escrevePagina(rrnAtual, pag);
			imprimePagina(pag);
			escrevePagina(*filhoDpro, novaPag);
			imprimePagina(novaPag);

			contRrnPagina++;
			return retorno = 2;
		}
	}
}

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
		fread(&pag, sizeof(PAGINA), 1, arq);
		fclose(arq);
		encontrada = buscaNaPagina(chave, pag, &pos);

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

void gerenciador(char *argv)
{
	FILE *btree;
	FILE *arqOrigem = fopen(argv, "r+");
	int raiz;
	PAGINA novaPagina;
	int controle;
	int filhoDpro = -1;
	int chavePro = -1;

	btree = fopen("btree.dat", "w+");
	raiz = 0;
	fwrite(&raiz, sizeof(int), 1, btree);
	inicializaPagina(&novaPagina);
	escrevePagina(raiz, novaPagina);

	int chave;
	fscanf(arqOrigem, "%d|", &chave);

	while (!feof(arqOrigem))
	{
		if (chave == EOF)
		{
			break;
		}
		else
		{

			if ((controle = insere(raiz, chave, &filhoDpro, &chavePro)) == 2)
			{
				inicializaPagina(&novaPagina);
				novaPagina.CHAVES[0] = chavePro;
				novaPagina.FILHOS[0] = raiz;
				novaPagina.FILHOS[1] = filhoDpro;

				raiz = rrnNovaPagina(); 

				escrevePagina(raiz, novaPagina);
			}

			fscanf(arqOrigem, "%d|", &chave);
		}
		fseek(btree, 0, SEEK_SET);
		fwrite(&raiz, sizeof(int), 1, btree);
	}

	printf("Operacao bem sucedida, Arvore B criada com sucesso.");
	fclose(arqOrigem);
	fclose(btree);

	PAGINA p;
	lePagina(0, &p);
	printf("\n");
	imprimePagina(p);
}

int main(int argc, char *argv[])
{
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		// int rr = 7, pos = 1;
		printf("Modo de criacao ativado ... nome do arquivo = %s\n", argv[2]);
		gerenciador(argv[2]);
		char *beatriz = "melhor pessoa";
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

		//teste();
	}

	return 0;
}