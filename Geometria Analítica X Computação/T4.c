/*	Trabalho 4 de ICC - GA
	Aluno: Mateus Prado Santos
	Professores: Moacir Ponti e Miriam Garcia
	ICMC USP São Carlos

	Este programa confere se duas figuras geométricas se colidem no espaço ou não;	
*/
#include <stdio.h>
#include <stdlib.h>
// Utiliza-se uma struct para contornar os problemas nas operações entre números racionais;
// Note a definição de um novo tipo numero que é uma struct com inteiros que representam o numerador e denominador, respectivamente;
typedef struct numero{
	int num;
	int den;
}numero;

numero** aloca_matriz();
void desaloca_matriz();
int ache_o_absurdo();
void swap();
void copia_matriz();
int mmc();
void armazena_valores();
void organiza();

// Função aloca_matriz() recebe como argumentos as quantidades de linhas e colunas para, enfim, alocar uma matriz tipo numero na heap;
numero** aloca_matriz(int n, int m){
	numero **mat = NULL;
	mat = (numero**) malloc(n * sizeof(numero*));
	for(int i = 0; i < n; i++){
		mat[i] = (numero*) malloc(m * sizeof(numero));
	}
	return mat;
}

// Procedimento desaloca_matriz() recebe como argumentos a matriz e o número de linhas para desalocá-la;
void desaloca_matriz(numero **mat, int n){
	for(int i = 0; i < n; i++){
		free(mat[i]);
	}
	free(mat);
}

// Função ache_o_absurdo() possui os parâmetros: quantidade de linhas e colunas da matriz, duas matrizes tipo numero(uma sendo modificada
// a cada recursão e outra armazenando os resultados), a variável absurdo(que será retornada no final do processo) e uma variável 'faltando'
// que cresce na medida em que há redução de ordem na matriz(útil para armazenar resultados parciais de cada recursão);
int ache_o_absurdo(int linhas, int colunas, numero ***mat, numero ***original, int absurdo, int faltando){ 
	// Definição de casos bases:
	if(linhas == 1){
		return absurdo;
	}
	if(colunas == 2){
		return absurdo;
	}
	int linha_pivo;
	for(int i = 0; i < linhas; i++){
		if((*mat)[i][0].num != 0){
			linha_pivo = i;
			i = linhas; // Ao encontrar a linha pivo, sai do for;
		}
	}
	// Se necessário, realiza o swap:
	if(linha_pivo != 0){		
		swap(linhas, colunas, mat, linha_pivo);
	}
	numero p = (*mat)[0][0];   //Definindo o pivo p;
	int igual = 0;	// Variáveis utilizadas para conferir a existência do absurdo;
	int diferente = 0;	
	for(int i = 1; i < linhas; i++){
		numero a = (*mat)[i][0]; // Coeficiente a da linha atual;
		for(int j = 0; j < colunas; j++){
			int novo_den = mmc((*mat)[i][j].den, (*mat)[0][j].den * a.den * p.num); // Novo denominador proveniente do mmc;
			(*mat)[i][j].num = ((novo_den / (*mat)[i][j].den) * (*mat)[i][j].num) - ((novo_den / ((*mat)[0][j].den * a.den * p.num)) * (*mat)[0][j].num * a.num * p.den);
			(*mat)[i][j].den = novo_den;
			// Simplificação, se possível, dos novos valores;
			if((*mat)[i][j].num % (*mat)[i][j].den == 0){
				(*mat)[i][j].num /= (*mat)[i][j].den;
				(*mat)[i][j].den = 1;	
			}
			// Conferindo se há absurdo:
			if(j < colunas-1){
				if((float)(*mat)[i][j].num / (float)(*mat)[i][j].den == 0){
					igual++;
				}	
			}else{
				if((*mat)[i][j].num / (*mat)[i][j].den != 0){
					diferente = 1;
				}						
			}	
		}
		if(igual == colunas-1 && diferente == 1){
			absurdo = 1;
		}
		igual = 0;	
	}
	// Guarda-se, então, os valores obtidos dessa recursão:
	armazena_valores(linhas, colunas, original, mat, faltando);
	// Processo que organiza a matriz para que se ignore a linha correta na próxima recursão:
	organiza(linhas, colunas, mat);	
	// Para o efeito recursivo, diminui-se as linhas e as colunas, o que aumenta 'faltando';
	return ache_o_absurdo(--linhas, --colunas, mat, original, absurdo, ++faltando);
}

// Procedimento que recebe como argumentos o número de linhas e colunas de uma matriz, a própia matriz e a linha_pivo para, enfim, trocar
// as posições na matriz para que a linha_pivo troque de lugar com a primeira linha;
void swap(int linhas, int colunas, numero ***mat, int linha_pivo){
	numero **copia = NULL;
	copia = aloca_matriz(linhas, colunas);	// Faz uma copia da matriz para não perder valores na troca de linhas;
	copia_matriz(linhas, colunas, mat, &copia);
	// A linha 1 recebe a linha pivo
	for(int j = 0; j < colunas; j++){
		(*mat)[0][j] = copia[linha_pivo][j];
	}
	// A linha pivo recebe a antiga primeira linha
	for(int j = 0; j < colunas; j++){
		(*mat)[linha_pivo][j] = copia[0][j];
	}
	// Desaloco a matriz copia pois ja nao preciso mais dela:
	desaloca_matriz(copia, linhas);
}

// Procedimento que tem como argumentos numero de linhas e colunas das matrizes, juntamente com as mesmas, com a função de fazer a matriz
// copia se transformar, como seu próprio nome já diz, em uma cópia da matriz mat; 
void copia_matriz(int linhas, int colunas, numero ***mat, numero ***copia){
	for(int i = 0; i < linhas ; i++){
		for(int j = 0; j < colunas; j++){
			(*copia)[i][j] = (*mat)[i][j];
		}
	}
}

// Função mmc() recebe como argumentos dois números inteiros e retorna o mínimo múltiplo comum entre eles;
int mmc(int num1, int num2){
	int maior, menor;
	if(num1 > num2){
		maior = num1;
		menor = num2;
	}else{
		maior = num2;
		menor = num1;
	}
	if(maior % menor == 0){
		return maior;
	}
	int mult1, mult2, i = 1;
	while(mult1 != mult2){
		mult1 = maior * i;
		mult2 = menor * i;
		if(mult1 % mult2 == 0){
			return mult1;
		}
		i++;
	}
	return mult1;
}

// Procedimento que armazena, parcialmente, os valores obtidos no processo de escalonamento;
void armazena_valores(int linhas, int colunas, numero ***original, numero ***mat, int faltando){
	for(int i = faltando; i < linhas+faltando; i++){
		for(int j = faltando; j < colunas+faltando; j++){
			(*original)[i][j] = (*mat)[i-faltando][j-faltando];
		}
	}
}

// Procedimento responsável por manipular a matriz a fim de deixar a linha e a coluna que serão reduzidas no escalonamento como as últimas
// da matriz, com o intuito de facilitar as operações na mesma em futuras recursões;
void organiza(int linhas, int colunas, numero ***mat){
	numero **copia = NULL;
	copia = aloca_matriz(linhas, colunas);	
	copia_matriz(linhas, colunas, mat, &copia);	// Faz-se uma cópia para não perder valores;
	for(int i = 0; i < linhas-1; i++){	
		for(int j = 0; j < colunas; j++){
			(*mat)[linhas-1][j] = copia[0][j];	// A primeira linha vira a última;
			(*mat)[i][j] = copia[i+1][j];		// Desloca-se as outras linhas uma posição a menos na matriz;
		}
	}
	copia_matriz(linhas, colunas, mat, &copia);	//Copia-se novamente a matriz na copia para manter a estratégia nas colunas também;
	// O processo se repete para as colunas:
	for(int j = 0; j < colunas-1; j++){	
		for(int i = 0; i < linhas; i++){
			(*mat)[i][colunas-1] = copia[i][0];	
			(*mat)[i][j] = copia[i][j+1];		
		}
	}
	desaloca_matriz(copia, linhas);	// Desaloca-se a matriz copia, que já não é mais útil;
}

int main(int argc, char *argv[]){
	char fig_1, fig_2;				
	int linhas, colunas;
	scanf("%d%d", &linhas, &colunas);
	numero **mat = NULL;
	mat = aloca_matriz(linhas, colunas);
	int atual; // Para a leitura de matriz
	// Leitura das figuras:
	// Note que, de acordo com a figura, lê-se a matriz de maneira diferente;
	scanf(" %c", &fig_1);
	if(fig_1 == 'r'){
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < colunas; j++){
				scanf("%d%d", &mat[i][j].num, &mat[i][j].den);
			}
		}
		atual = 2;
	}
	else{
		for(int i = 0; i < 1; i++){
			for(int j = 0; j < colunas; j++){
				scanf("%d%d", &mat[i][j].num, &mat[i][j].den);
			}
		}
		atual = 1;
	}
	scanf(" %c", &fig_2);
	if(fig_2 == 'r'){
		for(int i = atual; i < (atual+2); i++){
			for(int j = 0; j < colunas; j++){
				scanf("%d%d", &mat[i][j].num, &mat[i][j].den);
			}
		}
	}
	else{
		for(int i = atual; i < (atual+1); i++){
			for(int j = 0; j < colunas; j++){
				scanf("%d%d", &mat[i][j].num, &mat[i][j].den);
			}
		}
	}
	int absurdo = 0; // Variável pra saber se tem intersecção:
	// 0 --> sem absurdo --> tem colisão;
	// 1 --> teve absurdo --> não tem colisão;
	numero **original = aloca_matriz(linhas, colunas);	
	copia_matriz(linhas, colunas, &mat, &original);
	int faltando = 0;
	absurdo = ache_o_absurdo(linhas, colunas, &mat, &original, absurdo, faltando);
	if(absurdo == 1){
		printf("nao\n");
	}
	else{
		printf("sim\n");
	}
	for(int i = 0; i < linhas; i++){
		for(int j = 0; j < colunas; j++){
			if(original[i][j].num % original[i][j].den == 0){
				printf("%d\t", original[i][j].num / original[i][j].den);
			}
			else{
				printf("%d/%d\t", original[i][j].num, original[i][j].den);
			}
		}
		printf("\n");
	}
	desaloca_matriz(original, linhas);
	desaloca_matriz(mat, linhas);
	return 0;
}