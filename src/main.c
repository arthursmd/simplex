#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simplex.h"


char *problema_exemplo = "#Este eh o Template do arquivo de entrada do problema de \n\
#minimizacao para o simplex resolver.\n\
#Deve ser respeitada a ordem em que os dados aparecem. Os comentarios iniciados com jogo da velha, como esse, sao ignorados.\n\
\n\
#Numero de variaveis da funcao objetivo\n\
4\n\
#Numero de restricoes do problema\n\
3\n\
#Tipo do problema padrao (1 para maximizacao, 0 para minimizacao)\n\
1\n\
#Pesos das variaveis da funcao objetivo\n\
4 5 9 11\n\
#Restricoes do problema + valores resultado (considera todos como '>=' caso minimizacao e '<=' caso maximizacao)\n\
1 1 1 1 15\n\
7 5 3 2 120\n\
3 5 10 15 100\n\
\n";

void imprimir_problema(int qtd_variaveis, int qtd_restricoes, int tipo_problema, double *funcao_objetivo, double **restricoes, double *b){

	//Imprimindo o valor das variaveis===============================================================
		printf("Funcao objetivo: ");
		if (tipo_problema == 1)
				printf(" Maximizar z= ");
			else
				printf(" Minimizar z= ");
		for (int i = 0; i < qtd_variaveis; i++){
			if (i == 0 && funcao_objetivo[i] < 0)
				printf("%.2fx_%i", funcao_objetivo[i], i+1);
			else
				printf("%.2fx_%i", fabs(funcao_objetivo[i]), i+1);
			if (i < qtd_variaveis -1){
				if (funcao_objetivo[i+1] >=0)
					printf (" + ");
				else
					printf(" - ");
			}
		}

		printf("\nSujeito a:\n");
		for (int i = 0; i < qtd_restricoes; i++){
			for (int j = 0; j < qtd_variaveis; j++){
				if (restricoes[i][j] == 0)
					continue;
				printf("%.2fx_%i", restricoes[i][j], j+1);
				if (j < qtd_variaveis - 1){
					printf(" + ");
				}
			}
			if (tipo_problema == 1)
				printf(" <= %.2f\n", b[i]);
			else
				printf(" >= %.2f\n", b[i]);
		}
}


/*
 * Configura o tableau inicial
 */
void init(FILE *fp){
	//printf(problema_exemplo);
	int qtd_variaveis;
	int qtd_restricoes;
	int tipo_problema;
	double *funcao_objetivo;
	double **restricoes;
	double *b;

	int passo = 1;
	while (!feof(fp)){
		char linha[1000];
		fgets(linha, 1000, fp);

		if (linha[0] == '#' || linha[0] == '\n'){
			continue;
		}

		int jump = 0;
		if (passo == 1){
			//printf("1Linha lida: %s\n", linha);
			sscanf(linha, "%i\n", &qtd_variaveis);
			jump = 1;
		}
		if (passo == 2){
			//printf("2Linha lida: %s\n", linha);
			sscanf(linha, "%i\n", &qtd_restricoes);
			jump = 1;
		}
		if (passo == 3){
			//printf("3Linha lida: %s\n", linha);
			sscanf(linha, "%i\n", &tipo_problema);
			jump = 1;
		}
		if (passo == 4){
			//printf("4Linha lida: %s\n", linha);
		 	funcao_objetivo = (double *) calloc((qtd_variaveis + qtd_restricoes), sizeof(double));

			int i = 0;
			int deslocamento = 0;
			char *linha_p = linha;
			while (i < qtd_variaveis && sscanf(linha_p, "%lf%n", &funcao_objetivo[i], &deslocamento)==1){
				i++;
				linha_p +=deslocamento;
			}
			for (int i = qtd_variaveis; i < qtd_variaveis + qtd_restricoes; i++){
				funcao_objetivo[i] = 0;
			}
			jump = 1;
		}
		if (passo == 5){
			//printf("5Linha lida: %s\n", linha);

			//Inicializando as variaveis
			b = (double *) calloc(qtd_restricoes, sizeof(double));
			restricoes = (double**) calloc(qtd_restricoes, sizeof(double*));
			for (int i = 0; i < qtd_restricoes; i++){
				restricoes[i] = (double*) calloc((qtd_variaveis+qtd_restricoes), sizeof(double));
			}

			for (int i = 0; i < qtd_restricoes; i++){
				int pos = 0;
				int deslocamento = 0;
				char *linha_restricao_p = linha;
				while (pos < qtd_variaveis && sscanf(linha_restricao_p, "%lf%n", &restricoes[i][pos], &deslocamento)==1){
					pos++;
					linha_restricao_p +=deslocamento;
				}
				//Lendo a ultima posicao da linha para o vetor b
				sscanf(linha_restricao_p, "%lf%n", &b[i], &deslocamento);
				if (i < qtd_restricoes + 1)
					fgets(linha, 100, fp);
			}
			jump = 1;
		}
		passo +=jump;
	}


	imprimir_problema(qtd_variaveis, qtd_restricoes, tipo_problema, funcao_objetivo, restricoes, b);


	if (tipo_problema == 0)
		solve_minimization_problem(qtd_variaveis, qtd_restricoes, funcao_objetivo, restricoes, b);
	else
		solve_maximization_problem(qtd_variaveis, qtd_restricoes, funcao_objetivo, restricoes, b);

}

void init_from_local(char *content){
	FILE *fp = fopen("tempfile", "w+");
	fputs(content, fp);
	fflush(fp);
	rewind(fp);
	init(fp);
}


int main(int argc, char* argv[]){
	char* file_name1 = NULL;
	if (argc < 2) {// Se nao passar o nome do arquivo, usa a string local
		printf("Usando o problema local.\n");
		init_from_local(problema_exemplo);
	}
	else{
		file_name1 = argv[1];
		FILE *fp = fopen(file_name1, "r");
		if (fp != NULL)
			init(fp);
		else
			printf("Arquivo informado nao encontrado.\n");
	}
	return 0;
}
