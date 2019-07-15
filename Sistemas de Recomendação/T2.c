/*

	Trabalho 2 de ICC	/   ICMC USP SÃO CARLOS
	
	Aluno: Mateus Prado Santos

	Nº USP: 10851707
	
	Programa que estima a nota de um usúario para um filme de acordo com a similaridade com outros usuários envolvendo o mesmo filme:

*/


#include <stdio.h>			
#include <math.h>
#include <time.h>
clock_t begin = clock();

int main(int argc, char* argv[]){
	
	int n, i, A[15][15];    // Declara variáveis inteiras n(quantidade de usuários), i(quantidade de filmes) e a matriz A[n x i];
	double t, MEDIAS[15], somatoria_1 = 0, somatoria_2 = 0, nota;			
	double S[15][15];			// Declara a matriz S[u * v] para armazenar as similaridades entre os usuários;
	int count = 0, mod_u = 0, mod_v = 0, p_esc = 0, tem_zero;				
	
	
	// Lê as entradas para as dimensões da matriz e para a limiar, respectivamente;
	scanf("%d%d", &n, &i);
	scanf("%lf", &t);	
		
	
	// Utiliza os contadores x(linha) e y(coluna) para "percorrer" a matriz, guarda os valores e faz as médias dos usuários, guardando
	// os valores na matriz MEDIAS:	
	for(int x = 0; x < n; x++){		
		for(int y = 0; y < i; y++){
			scanf("%d", &A[x][y]);	
			MEDIAS[x] +=  A[x][y];	  
			if(A[x][y] != 0){		
				count++;				
			}
		}
		MEDIAS[x] /= count;					
		count = 0; 
	}
	
	
	
	
	// Percorre pelos usuários e vai comparando de dois em dois(u e v); 
	// Note que v inicializa como u + 1 pois não queremos comparar um usuario com ele mesmo;
	// Calcula o módulo de u(sem a raiz quadrada), o módulo de v(sem a raiz quadrada) e o produto escalar de u e v;
	// Calcula a similaridade entre u e v e armazena na matriz das similaridades;	
	for(int u = 0; u < n; u++){	
		for(int v = 0; v < n; v++){				
			for(int y = 0; y < i; y++){			   
				mod_u += pow(A[u][y], 2);						
				mod_v += pow(A[v][y], 2);
				p_esc += A[u][y] * A[v][y];					
			}
			S[u][v] = p_esc / (sqrt(mod_u) * sqrt(mod_v));									
			S[v][u] = S[u][v];	
			mod_u = 0;	
		    mod_v = 0;	
			p_esc = 0;																						
		}
	}
	
	
	
	// Percorre as linhas e as colunas da matriz A;
	// Se valor na matriz for igual a zero, ou seja, se a nota não existir, então percorre a matriz das similaridades;
	// Se a similaridade for maior que a limiar, então é similar;
	// Calcula a somatoria_1, a somatoria_2, verifica se o resultado é DI, se não, estima a nota;
	for(int x = 0; x < n; x++){					
		for(int y = 0; y < i; y++){
			if(A[x][y] == 0){
				tem_zero = 1;		
				for(int v = 0; v < n; v++){										
					if(S[x][v] > t  && v != x){	
						somatoria_1 += S[x][v] * (A[v][y] - MEDIAS[v]);
						somatoria_2 += fabs(S[x][v]);
					}
				}
				if(somatoria_2 == 0){	
					printf("DI ");
				}	
				else {	
					nota = MEDIAS[x] + (somatoria_1 / somatoria_2);
					printf("%.2lf ", nota);
				}
			}	
			somatoria_1 = 0;
			somatoria_2 = 0;
			nota = 0;
		}
		if(tem_zero == 1){
		printf("\n");
		}
		tem_zero = 0;				
	}			
	return 0;
}
clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
