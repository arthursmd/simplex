#include "simplex.h"
#include <stdio.h>
#include <stdlib.h>

/** Seleciona o indice da variavel
 *  de maior custo reduzido que nao esta na base
 * -1 caso nao exista
 * 0 == x1
 * 1 == x2
 * ...*/
int select_variavel_entrada(double * coef, int n, int m){
	int index = -1;
	double valor = 0;

	for (int i = 0; i < n + m; i++){
		if (coef[i] > 0 && index == -1)
			index = i;
		if (coef[i] > valor){
			valor = coef[i];
			index = i;
		}
	}

	return index;
}

void print_resultado(double z, double *b, int *base_indx, int m){

	printf("Terminou, valor final da funcao objetivo = %f\n", z);
	for (int i = 0; i < m; i++){
		printf("x_%i=%f; ", base_indx[i]+1, b[i]);
	}
}

/**
 * Indica a linha i do pivo atual, e atualiza a variavel de entrada e saida na lista de variaveis basicas (base_indx)
 */
int select_linha_pivo(int indice_entrada, double* b, double **A, int *base_index, int m){
	int posicao_saida_base = 0;//posicao da variavel de saida no vetor base_index
	double menor_razao = -1;

	for (int i = 0; i < m; i++){
		if (A[i][indice_entrada] <= 0)
			continue;
		else if (menor_razao == -1)
			menor_razao = b[i] / A[i][indice_entrada];

		double razao_atual = b[i] / A[i][indice_entrada];

		if (razao_atual < menor_razao){
			menor_razao = razao_atual;
			posicao_saida_base = i;
		}
	}

	base_index[posicao_saida_base] = indice_entrada;

	return posicao_saida_base;
}

/**
 * Atualiza o tableau, zerando a coluna correspondente ao pivo.
 */
void zerar_coluna(int linha_pivo, int indice_entrada, double *c, double *b, double **A, int m, int n){
	//tornando == 1 o pivo, e atualizando toda a linha
	double divisor = A[linha_pivo][indice_entrada];
	for (int j = 0; j < n + m; j++){
		A[linha_pivo][j] /= divisor;
	}
	b[linha_pivo] /= divisor;

	//zerando a coluna na funcao objetivo
	double valor_mult = c[indice_entrada];
	for (int j = 0; j < n+m; j++){
		c[j] -= A[linha_pivo][j] * valor_mult;
	}

	//zerando a coluna em A
	for (int i = 0; i < m; i++){
		if (i == linha_pivo) continue;

		valor_mult = A[i][indice_entrada];
		for (int j = 0; j < n+m; j++){
			A[i][j] -= A[linha_pivo][j] * valor_mult;
		}
		b[i] -= b[linha_pivo] *valor_mult;
	}

}

double calcula_z(double *c, double *b, int *base_indx, int m, int n){
	double z = 0;

	for (int j = 0; j < m; j++){
		z += b[j] * c[base_indx[j]];
	}
	return z;
}


/* Resolve um problema de minimizacao */
void solve_minimization_problem(int n, int m, double * c, double **A, double *b){

}

/* Problema padrao de maximizacao
Encontre o n-vetor x  que maximize
cx = c1x1 + c2x2 + ... + cnxn


Sujeito as restricoes:
	a11x1 + a12x2 + ... + a1nxn <= b1
	a21x1 + a22x2 + ... + a2nxn <= b2
	...
	am1x1 + am2x2 + ... + amnxn <= bn
E x >= 0

Parametros: tamanho do vetor - n
n - Tamanho do vetor solucao
m - qtd de restricoes
max - 1 se maximizacao, 0 se minimizacao
c - pesos da funcao objetivo
A - matriz de restricoes
b - valor resultado das restricoes*/
void solve_maximization_problem(int n, int m, double * c, double **A, double *b){
	//Valor inicial da funcao objetivo (o valor de b ja esta configurado)
	double z = 0;
	//Indices das variaveis basicas
	int base_indx[m];

	for (int i = 0; i < m; i++)
		base_indx[i] = n+i;

	//matriz identidade das variaveis de folga
	for (int i = 0; i < m; i++){
		A[i][i+n] = 1;
	}

	double * funcao_objetivo_original = malloc(sizeof(double)*(n+m));
	for (int i = 0; i < n+m; i++)
		funcao_objetivo_original[i] = c[i];

	while (1){
		int indice_entrada = select_variavel_entrada(c, n, m);
		if (indice_entrada == -1) break;
		int linha_pivo = select_linha_pivo(indice_entrada, b, A, base_indx, m);
		zerar_coluna(linha_pivo, indice_entrada, c, b, A, m, n);
		z = calcula_z(funcao_objetivo_original, b, base_indx, m, n);
	}

	print_resultado(z, b, base_indx, m);
}
