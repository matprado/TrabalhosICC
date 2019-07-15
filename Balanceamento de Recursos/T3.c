/* Trabalho 3 - Balanceamento de Recursos
Introdução à Ciência de Computação
Prof. Moacir Ponti
Aluno: Mateus Prado Santos
Nº USP: 10851707
Este código verifica uma condição de igualdade na distribuição de recursos para dois jogadores em um mapa de um jogo:*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*	Função 'aloca' recebe como parâmetro o inteiro 'nRec' para alocar, na memória heap, e retornar uma matriz 'posRec' de nRec linhas e 
	2 colunas(1 para cada coordenada dos recursos);*/
int** aloca(int nRec){
	int** posRec;
	posRec = (int**) malloc(nRec * sizeof(int*));
	for(int i = 0; i < nRec; i++){
		posRec[i] = (int*) malloc(2 * sizeof(int));	
	}
	return posRec;
}
/*	Função 'desaloca' recebe como parâmetros o número de recursos e a matriz das coordenadas de cada recurso para, assim, desalocar
	a mesma da memória heap; */
void desaloca(int nRec, int** posRec){
	for(int i = 0; i < 2; i++){
		free(posRec[i]);
	}	
	free(posRec);
}	
/*	Função 'reset_matriz' recebe como parâmetro a matriz 'MAPA'	e reseta os valores modificados no floodfill para que o mesmo
	possa ser calculado para um recurso ou jogador diferente;*/
void reset_matriz(int MAPA[9][9]){
	int i, j;
	for(i = 0; i < 9; i++){
		for(j = 0; j < 9; j++){
			if(MAPA[i][j] == 2) MAPA[i][j] = 0;
		}	
	}	
}
/*	Função 'floodfill' recebe como parâmetros as coordenadas do jogador e do recurso, a matriz 'MAPA' e a variável 'percurso' para, enfim,
	retorná-la nos cálculos de exploração(com o intuito de terminar a execução e desempilhar corretamente, retorna 'percurso' com valor negativo);
	Note que foi necessária a utilização de recursão para tal tarefa, com o cuidado de não contar fora do mapa ou células intransponíveis; */
double floodfill(int x, int y, int* posRec, int MAPA[9][9], double percurso){
	percurso++;
	MAPA[x][y] = 2;	
	if(x == posRec[0] && y == posRec[1]) return percurso * (-1);	 
	if((x + 1) < 9 && MAPA[x + 1][y] != 1 && MAPA[x + 1][y] != 2){
		percurso = floodfill(x + 1, y, posRec, MAPA, percurso);
		if(percurso < 0) return percurso;
	}
	if((x - 1) >= 0 && MAPA[x - 1][y] != 1 && MAPA[x - 1][y] != 2){
		percurso = floodfill(x - 1, y, posRec, MAPA, percurso);
		if(percurso < 0) return percurso;
	}	
	if((y - 1) >= 0 && MAPA[x][y - 1] != 1 && MAPA[x][y-1] != 2){
		percurso = floodfill(x, (y - 1), posRec, MAPA, percurso);
		if(percurso < 0) return percurso;		
	} 
	if((y + 1) < 9 && MAPA[x][y + 1] != 1 && MAPA[x][y+1] != 2){	
		percurso = floodfill(x, (y + 1), posRec, MAPA, percurso);
		if(percurso < 0) return percurso;
	}		
	return percurso;
}
int main(int argc, char* argv[]){
	int P = 0, jogador1_x, jogador1_y, jogador2_x, jogador2_y, nRec;
	int MAPA[9][9];
	//Leitura do mapa, com contagem do número de células transponíveis, e das coordenadas dos jogadores:	
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			scanf("%d", &MAPA[i][j]);
			if(MAPA[i][j] == 0)	P++;
		}
	}
	scanf("%d%d%d%d", &jogador1_x, &jogador1_y, &jogador2_x, &jogador2_y);
	// Lê as coordenadas dos recursos e armazena as mesmas numa matriz alocada na heap pela função 'aloca()';
	scanf("%d", &nRec);
	int** posRec = aloca(nRec);  
	for(int k = 0; k < nRec; k++){
		for(int l = 0; l < 2; l++){
			scanf("%d", &posRec[k][l]);
		}
	}
	// Calcula as somatorias de cada jogador usando floodfill para, enfim, calcular os valores de exploração;
	// Usa a função 'fabs()' pois floodfill retorna 'percurso' com valor negativo;
	// Usa a função 'reset_matriz()' para retornar os valores do mapa modificados pelo floodfill;
	double percurso = 0, somatoria1 = 0.0, somatoria2 = 0.0;
	for(int m = 0; m < nRec; m++){
		somatoria1 += fabs(floodfill(jogador1_x, jogador1_y, posRec[m], MAPA, percurso)) / P;
		reset_matriz(MAPA);	
	}
	for(int n = 0; n < nRec; n++){
		somatoria2 += fabs(floodfill(jogador2_x, jogador2_y, posRec[n], MAPA, percurso)) / P;
		reset_matriz(MAPA);
	}
	double exploracao1, exploracao2, N = nRec + 1, diferenca;
	exploracao1 = (1 / (N - 1)) * somatoria1;
	exploracao2 = (1 / (N - 1)) * somatoria2;
	// Calcula a diferença e imprime se o mapa está balanceado ou não, caso não esteja balanceado, imprime qual jogador tem vantagem; 
	diferenca = fabs(exploracao1 - exploracao2);
	printf("%lf\n", exploracao1);
	printf("%lf\n", exploracao2);
	if(diferenca >= 0.001){
		if(exploracao1 > exploracao2) printf("O jogador 2 possui vantagem\n");
		else printf("O jogador 1 possui vantagem\n");
	}
	else printf("O mapa eh balanceado\n");
	//Por fim, usa a função 'desaloca()' para desalocar a matriz das coordenadas dos recursos da heap;
	desaloca(nRec, posRec);
	return 0;
}