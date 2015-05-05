#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

/**
 * Entrada:
 * qtd de m variaveis
 * qtd de n restricoes
 *
 * o vetor de pesos da funcao objetivo (m+n)
 * a matriz de restricoes expandida com as variaveis de folga(m+n X n)
 * o vetor b dos r-values das restricoes
 */
void solve_minimization_problem(int n, int m, double * c, double **A, double *b);
void solve_maximization_problem(int n, int m, double * c, double **A, double *b);
void print_resultado(double z, double *b, int *base_indx, int m);
int select_variavel_entrada(double * coef, int n, int m);

#endif
