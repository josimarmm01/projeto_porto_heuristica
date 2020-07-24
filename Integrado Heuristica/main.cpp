/* 
 * File:   main.cpp
 * Author: Josimar de Moraes Mistura
 *
 * Created on 2 de janeiro de 2020, 12:49
 */

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define B_ 100
#define P_ 100
#define N_ 100
#define T_ 500
#define R_ 100
#define S_ 100

//Tempo de execucao
static int segundos = 120;
clock_t hi, hf;

//Dados referente as embaracoes
static int Numero_navio;
static int Navios[N_];                               // Necessario para ordenação dos navios pois necesita do id
static int Prioridade_navio[N_];
static int Tempo_chegada_navio[N_];
static int Comprimento_navios[N_];

//Dados referente aos bercos
static int Numero_berco;
static int Comprimento_berco[B_];

//Dados referente aos periodos de mare alta
static int Numero_mare;
static int Tempo_inicial_mare[N_];
static int Tempo_final_mare[N_];

//Dados referente as tarefas
static int Numero_tarefa;
static int Tempo_procesamento_tarefa[100];
static int Comprimento_tarefa_patio[100];
static int Tarefa_relacionada_navio[100];

//O caminho de ferro no qual cada recuperador está montado
static int Numero_recuperador;
static int Recuperador_montado_trilho[100];

//Matriz distancia recuperador ao berço
static int Matriz_distancia[B_][R_];

//const int N_Patio =  3;
//const int N_Posisoes = 20;
//const int T = 200;
//const int R = 3;
//const int W = (N_Patio - 1);

int N_Patio;
int N_Posisoes;
int T;
int W;

// Dados para teste
//static int horario_teste[3] = {6, 4, 2};

typedef struct instantePatio {

    int Rec[R_];
    int Pat[S_][P_];

}InstantePatio;

typedef struct tBerco {
	int N[N_];
    int indice = 0;
    int horario = 0;
}Berco;

typedef struct tSolucao {

	int FO_;
    
    Berco  B[B_];

    int     N[N_];
	int     C[N_];
	int     S[N_];
    int     Y[N_];
    
    int    SR[N_];
    int    CR[N_];
    int    Re[N_];

    int    PI[N_];
    int    PF[N_]; 
    int    Ps[N_]; 

    int    PM[N_];

}Solucao;

static InstantePatio InstantePatioTempo[T_];

//Variaveis auxiliares globais para preencher e verificar se tem opsicao no patio de estocagem 
int patio_global_aux = 0;
int inicio_posisao_global_aux = 0;
int fim_posisao_global_aux = 0;

int patio_horario_final[P_] = {0, 0, 0, 0};

//Variavel auxiliar global para preencher e verificar se ha recuperador disponivel
int recuperador_global_aux = 0;


void setarDadosConfig(int nPatio, int nPosicoes, int nTempo) {
    N_Patio =  nPatio;
    N_Posisoes = nPosicoes;
    T = nTempo;
    W = (N_Patio - 1);
}

void limparSolucao() {
    for (int t = 1; t <= T; t++) {
        for (int s = 1; s <= N_Patio; s++) {
            for (int p = 1; p <= N_Posisoes; p++) {
                InstantePatioTempo[t].Pat[s][p] = 0;
            }
        }
    }

    for (int t = 1; t <= T; t++) {
        for (int r = 1; r <= Numero_recuperador; r++) {
            InstantePatioTempo[t].Rec[r] = 0;
        }
    }

}   

void imprimirInstantePatio () {

    for (int t = 1; t <= T; t++) {
        printf("\nTempo = %d\n", t);
        for (int i = 1; i <= N_Patio; i++) {
            printf("\nPatio = %d  ", i); 
            for (int j = 1; j <= N_Posisoes; j++) {
                printf("%d ", InstantePatioTempo[t].Pat[i][j]);
            }
        }

        printf("\n");

        for (int r = 1; r <= Numero_recuperador; r++) {
            printf("\nRecuperador = %d  ", InstantePatioTempo[t].Rec[r]);
        }

        printf("\n");    

    }
}

void caregarDados(char * arq) {
    
	FILE* Arquivo;
    Arquivo = fopen(arq, "r");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo");
    } else {
    
        fscanf(Arquivo, "%d %d %d %d %d\n",  &Numero_navio, &Numero_berco, &Numero_recuperador, &Numero_tarefa, &Numero_mare);
        
        for (int i = 1; i <= Numero_navio; i++) {
            fscanf(Arquivo, "%d ", &Prioridade_navio[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_navio; i++) {
            fscanf(Arquivo, "%d ", &Tempo_chegada_navio[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_navio; i++) {
            fscanf(Arquivo, "%d ", &Comprimento_navios[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_berco; i++) {
            fscanf(Arquivo, "%d ", &Comprimento_berco[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_mare; i++) {
            fscanf(Arquivo, "%d ", &Tempo_inicial_mare[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_mare; i++) {
            fscanf(Arquivo, "%d ", &Tempo_final_mare[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_tarefa; i++) {
            fscanf(Arquivo, "%d ", &Tempo_procesamento_tarefa[i]);
        }

        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_tarefa; i++) {
            fscanf(Arquivo, "%d ", &Comprimento_tarefa_patio[i]);
        }
        
        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_tarefa; i++) {
            fscanf(Arquivo, "%d ", &Tarefa_relacionada_navio[i]);
        }
        
        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_recuperador; i++) {
            fscanf(Arquivo, "%d ", &Recuperador_montado_trilho[i]);
        }
        
        fscanf(Arquivo, "\n");

        for (int i = 1; i <= Numero_recuperador ; i++) {
            for (int j = 1; j <= Numero_berco; j++) {
                fscanf(Arquivo, "%d ", &Matriz_distancia[i][j]);
            }
            fscanf(Arquivo, "\n");
        }
        fscanf(Arquivo, "\n");

        fclose(Arquivo);
    }   
}       

void testarDados() {

	printf("Numero de Navios = %d \n", Numero_navio);
    printf("Numero de Berços = %d \n", Numero_berco);
    printf("Numero de Recuperadores = %d \n", Numero_recuperador);
    printf("Numero de Tarefas = %d \n", Numero_tarefa);
    printf("Numero de Maré = %d \n", Numero_mare);

    printf("Prioridade Navios = ");
	for (int i = 1; i <= Numero_navio; i++) {
		printf("%d ", Prioridade_navio[i]);
	}

    printf("\nTempo Chegada = ");
	for (int i = 1; i <= Numero_navio; i++) {
		printf("%d ", Tempo_chegada_navio[i]);
	}
    
    printf("\nComprimento Navios = ");
	for (int i = 1; i <= Numero_navio; i++) {
		printf("%d ", Comprimento_navios[i]);
	}

    printf("\nComprimento Berços = ");
	for (int i = 1; i <= Numero_berco; i++) {
		printf("%d ", Comprimento_berco[i]);
	}

    printf("\nInicio Mare Alta = ");
	for (int i = 1; i <= Numero_mare; i++) {
		printf("%d ", Tempo_inicial_mare[i]);
	}

    printf("\nFim Mare Alta = ");
	for (int i = 1; i <= Numero_mare; i++) {
		printf("%d ", Tempo_final_mare[i]);
	}

    printf("\nTempo Procesamento das Tarefas = ");
	for (int i = 1; i <= Numero_tarefa; i++) {
		printf("%d ", Tempo_procesamento_tarefa[i]);
	}

    printf("\nQuantidade que a Terefa Ocupa Pátio = ");
	for (int i = 1; i <= Numero_tarefa; i++) {
		printf("%d ", Comprimento_tarefa_patio[i]);
	}

    printf("\nNavio Associado a Cada Tarefa = ");
	for (int i = 1; i <= Numero_tarefa; i++) {
		printf("%d ", Tarefa_relacionada_navio[i]);
	}

    printf("\nTrilho Cada Recuperador Esta Montado = ");
	for (int i = 1; i <= Numero_recuperador; i++) {
		printf("%d ", Recuperador_montado_trilho[i]);
	}

    printf("\nMatriz Distancia = ");
	for (int i = 1; i <= Numero_recuperador; i++) {
        for (int j = 1; j <= Numero_berco; j++) {
		    printf("%d ", Matriz_distancia[i][j]);
        }
        printf("\n");
	}

}   	    

int Ud(int rangeLow, int rangeHigh) {
	double myRand = rand() / (1.0 + RAND_MAX);
	int range = rangeHigh - rangeLow + 1;
	int myRand_scaled = (myRand * range) + rangeLow;
	return myRand_scaled;
}

void ordernar() {

    for (int i = 1; i <= Numero_navio; i++) {
        Navios[i] = i;
    }
    
    int i, j, aux_navio, aux_tempo, aux_prioridade, aux_comprimento;
        for (i = 1; i <= Numero_navio; i++) {
            for (j = i + 1; j <= Numero_navio; j++) {
                if (Tempo_chegada_navio[i] > Tempo_chegada_navio[j]) {
                    
                    aux_navio = Navios[i];
                    Navios[i] = Navios[j];
                    Navios[j] = aux_navio;
                    
                    
                    aux_tempo = Tempo_chegada_navio[i];
                    Tempo_chegada_navio[i] = Tempo_chegada_navio[j];
                    Tempo_chegada_navio[j] = aux_tempo;
                    
                    aux_prioridade = Prioridade_navio[i];
                    Prioridade_navio[i] = Prioridade_navio[j];
                    Prioridade_navio[j] = aux_prioridade;
                    
                    aux_comprimento = Comprimento_navios[i];
                    Comprimento_navios[i] = Comprimento_navios[j];
                    Comprimento_navios[j] = aux_comprimento;
                    
                }   
            }       
        }

        for (int i = 1; i <= Numero_navio; i++) {
            printf("%d ", Tempo_chegada_navio[i]);
        }

        printf("\n");   
        for (int i = 1; i <= Numero_navio; i++) {
            printf("%d ", Navios[i]);
        }
                   
} 

bool verificaSeHaRecuperadorTempoT(int t) {

    int w_aux_inicio;
    int w_aux_fim;

    if (patio_global_aux == 1) {
        w_aux_inicio = 1;
        w_aux_fim = 1;
    }

    if (patio_global_aux == 2) {
        w_aux_inicio = 1;
        w_aux_fim = 2;
    }
    
    if (patio_global_aux == 3) {
        w_aux_inicio = 2;
        w_aux_fim = 2;
    }
    

    for (int r = 1; r <= Numero_recuperador; r++) {

        for (int w = w_aux_inicio; w <= w_aux_fim; w++) {
            
            if (Recuperador_montado_trilho[r] == w) {

                if(InstantePatioTempo[t].Rec[r] == 0) {

                    printf("Recuperador e tempo %d %d ", r, t);    
                    recuperador_global_aux = r;    
                    return true;
                }   
            }      
        }       

    }   

    return false;
}   

bool verificaSeHaRecuperadorTemposMesmaConfiguracao(int tempoInicial, int tempoFinal) {
    for (int t = tempoInicial; t <= tempoFinal; t++) {
        printf("Mesma config tempo %d ", t);    
        if (InstantePatioTempo[t].Rec[recuperador_global_aux] == 1) {
            return false;
        }                
    }
        
    return true;
}   

bool verificaSeHaPosicaoTempoT(int t, int posicoes_patio) {
    
    int cont;
    
    for (int i = 1; i <= N_Patio; i++) {
        
        cont = 0;

        if (t >= patio_horario_final[i]) {

            for (int j = 1; j <= N_Posisoes; j++) {

                if (InstantePatioTempo[t].Pat[i][j] == 0) {
                        
                    cont++;

                } else {
                    cont = 0;
                }

                if (cont == posicoes_patio) {
                    
                    patio_global_aux = i;
                    inicio_posisao_global_aux = (j - posicoes_patio) + 1;
                    fim_posisao_global_aux = j;
                    
                    printf("Patio = %d, Inicio = %d e fim = %d \n", patio_global_aux, inicio_posisao_global_aux, fim_posisao_global_aux);
                    
                    return true;
                }
            }
        }       
    }
    printf("Falso");
    return false;
}

bool verificaSeHaPosicaoTemposMesmaConfiguracao(int tempoInicial, int tFinal) {
    
    for (int t = tempoInicial; t <= tFinal; t++) {

        for (int i = inicio_posisao_global_aux; i <= fim_posisao_global_aux; i++) {

            if (InstantePatioTempo[t].Pat[patio_global_aux][i] == 1) {
                return false;
            }
        }
    }
    return true;
}   

void setarPatioInstanteTempo (int tempo_inicial, int tempo_final) {
    
    for (int t = tempo_inicial; t <= tempo_final; t++) {
        for (int i = inicio_posisao_global_aux; i <= fim_posisao_global_aux; i++) {
            InstantePatioTempo[t].Pat[patio_global_aux][i] = 1;    
        }
    }
}

void setarRecuperadorInstanteTempo(int tempo_inicial, int tempo_final) {
    
    for (int t = tempo_inicial; t <= tempo_final; t++) {
        InstantePatioTempo[t].Rec[recuperador_global_aux] = 1;
    }
}

void zerarVariaveisAuxiliares() {
    patio_global_aux = 0;
    inicio_posisao_global_aux = 0;
    fim_posisao_global_aux = 0;
    recuperador_global_aux = 0;
}

void contrutivo(Solucao &s) {

    int berco_atual = 1;
    
    //Alocar navios
    for(int nav = 1; nav <= Numero_navio; nav++) {

        int tamanhoTarefaOcupa = 0;
        int tempoProcesamento  = 0;
        
        printf("percorendo o navio %d\n", nav);
        
        if(Comprimento_navios[nav] <= Comprimento_berco[berco_atual]) {

                    printf("Comprimento do navio = %d\n", Comprimento_navios[nav]);
                    printf("Comprimento do berco = %d\n", Comprimento_berco[berco_atual]);    

                    int tempoInicialParaRecuperacao = MAX(s.B[berco_atual].horario, Tempo_chegada_navio[nav]);                                      //Define o horario inicial de atracação  
                    
                    s.S[nav] = tempoInicialParaRecuperacao;
                    s.Y[nav] = berco_atual;                                                                                                     //Define o berço que a embarcação vai alocar
                    
                    printf("Indice = %d\n", berco_atual);
                    printf("Tempo Inical Para Inicio da Recuperação = %d\n", tempoInicialParaRecuperacao);
                    
                    for (int tarefa = 1; tarefa <= Numero_tarefa; tarefa++) {                                                                       //Achar posicoes no patio para a tarefa do navio 'nav'
                        
                        printf("Tarefa = %d\n", tarefa);
                        //printf("Tarefa relacionada navio = %d\n", Tarefa_relacionada_navio[tarefa]);
                        
                        if (Navios[nav] == Tarefa_relacionada_navio[tarefa]) {

                            printf("Tarefa relacionada ao navio %d tarefa = %d\n", nav,  Tarefa_relacionada_navio[tarefa]);
                            printf("Comprimento tarefa = %d\n", Comprimento_tarefa_patio[tarefa]);
                            printf("Tempo tarefa = %d\n", Tempo_procesamento_tarefa[tarefa]);

                            tamanhoTarefaOcupa = tamanhoTarefaOcupa + Comprimento_tarefa_patio[tarefa];
                            tempoProcesamento  = tempoProcesamento  + Tempo_procesamento_tarefa[tarefa];

                        }
                    }

                    printf("Comprimento total tarefa %d \n", tamanhoTarefaOcupa);
                    printf("Tempo total tarefa %d \n", tempoProcesamento);

                    //Buscar posicoes para alocagem
                    //Tempo t e posições ocupadas
                    for (int tempo = tempoInicialParaRecuperacao; tempo <= T; tempo++) {

                        printf("Tempo = %d\n", tempo);
                        int instanteTempo = tempo;

                        //Alocar patio e Recuperador
                        if (verificaSeHaPosicaoTempoT(instanteTempo, tamanhoTarefaOcupa) &&
                            verificaSeHaRecuperadorTempoT(instanteTempo) && 
                            verificaSeHaPosicaoTemposMesmaConfiguracao(instanteTempo, ((tempoProcesamento + instanteTempo) - 1) + Matriz_distancia[recuperador_global_aux][berco_atual] +  1) &&
                            verificaSeHaRecuperadorTemposMesmaConfiguracao(instanteTempo, ((tempoProcesamento + instanteTempo) - 1) + Matriz_distancia[recuperador_global_aux][berco_atual] +  1)) {  

                                int tempoProcesamentoTotal = ((tempoProcesamento + instanteTempo) - 1) + Matriz_distancia[recuperador_global_aux][berco_atual] +  1;   
                                printf(" %d, %d  tempoProcesamentoTotal = %d\n",(tempoProcesamento + instanteTempo) - 1,Matriz_distancia[recuperador_global_aux][berco_atual], tempoProcesamentoTotal);
        
                                setarPatioInstanteTempo(instanteTempo, tempoProcesamentoTotal);  
                                setarRecuperadorInstanteTempo(instanteTempo, tempoProcesamentoTotal);
                                        
                                //Setar variaveis de decisao na Solucao s
                                //int horario_auxiliar = ((tempoProcesamento + instanteTempo) - 1) + Matriz_distancia[recuperador_global_aux][berco_atual] +  1;
                                
                                s.SR[nav] = instanteTempo;
                                s.CR[nav] = tempoProcesamentoTotal;
                                s.Re[nav] = recuperador_global_aux;

                                //Setar variaveis de alocação de patios
                                s.PI[nav] = inicio_posisao_global_aux ;
                                s.PF[nav] = fim_posisao_global_aux;
                                s.Ps[nav] = patio_global_aux;

                                //Restricao de horio de mare alta
                                //Buscar horario para atracação                                    
                                for (int z = 1; z <= Numero_mare; z++) {
                                    if (tempoProcesamentoTotal <= Tempo_final_mare[z]) {
                                        
                                        s.PM[nav] = z;
                                        printf("\nPriodo nare%d\n", z);
                                        
                                        if (tempoProcesamentoTotal < Tempo_inicial_mare[z]) {
                                            tempoProcesamentoTotal = Tempo_inicial_mare[z];
                                        }            
                                        break;
                                    }
                                }

                                s.B[berco_atual].horario = tempoProcesamentoTotal;                                                                                    //Horario gasto para procesamento do agendamento e da recuperacao
                                printf("\n Matriz de distancia do berco ateh o recuperador %d \n", Matriz_distancia[recuperador_global_aux][berco_atual]);

                                s.C[nav] = s.B[berco_atual].horario;        
                                //tempo = ((tempoProcesamento + instanteTempo) - 1);                                                                              //Horario de atracação da embarcação apos o caregamento das tarefas    
                                
                                //patio_horario_final[patio_global_aux] = tempoProcesamentoTotal;
                                //printf("\n\npatio horario final = %d", patio_horario_final[patio_global_aux]);
                                tempo = tempoProcesamentoTotal;

                                zerarVariaveisAuxiliares();
                                //imprimirInstantePatio();

                                break;
                        }  
                    }
                                                                                                                                                                        //Pula para o proximo berço em ordem crecente 
                    if (berco_atual == Numero_berco) {                                                                                                      //Indice chema ao ultimo berço reinicia para inicial                
                        berco_atual = 1;                                                                                                         
                    } else {
                        berco_atual++;                                                                                                                      //Responsavel para pular o berco que nao suporta a embarcação autal        
                    }

        } else {                                                                                                                  
            nav--; 
            if (berco_atual == Numero_berco) {
                printf("\n\nAki\n\n");
                berco_atual = 1;
            } else {
                berco_atual++;    
            }
        }
    }
}       

void gerarArquivoInstancia(Solucao &s, char* diretorio) {
	
	FILE* arq = fopen(diretorio, "a");

         s.FO_ = 0;

        for (int i = 1; i <= Numero_navio; i++) {
            s.FO_ = s.FO_ + s.C[i] * Prioridade_navio[i];    
        }

        fprintf(arq, "\nFO = %d \n S ", s.FO_);
        
        for(int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.S[i]);
        }

        fprintf(arq, "\n C ");

        for(int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.C[i]);
        }

        fprintf(arq, "\n Y ");

        for(int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.Y[i]);
        }

        fprintf(arq, "\n Recuperador ");
        for(int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.Re[i]);
        }

        fprintf(arq, "\n SR ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.SR[i]);
        }

        fprintf(arq, "\n CR ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.CR[i]);
        }

        fprintf(arq, "\n Patio ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.Ps[i]);
        }

        fprintf(arq, "\n PI ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.PI[i]);
        }

        fprintf(arq, "\n PF ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.PF[i]);
        }
        
        fprintf(arq, "\n PM ");
        for (int i = 1; i <= Numero_navio; i++) {
            fprintf(arq, "%d ", s.PM[i]);
        }

    fprintf(arq, "\n");    
        
	fclose(arq);

}

void gerarArquivoTabela(Solucao &s, char* diretorio, double tempo) {
	
	FILE* arq = fopen(diretorio, "a");
	
	fprintf(arq, "$%d$	& ", s.FO_);
	fprintf(arq, "$%f$	& \n", tempo);
	
	fclose(arq);

}

void imprimirSolucao(Solucao &s) {

    s.FO_ = 0;

    for (int i = 1; i <= Numero_navio; i++) {
        s.FO_ = s.FO_ + s.C[i] * Prioridade_navio[i];   
    }

    printf("\nFO = %d \n N ", s.FO_);

    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", Navios[i]);
    }

    printf("\n");


    for(int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.S[i]);
    }

    printf("\n C ");

    for(int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.C[i]);
    }

    printf("\n Y ");

    for(int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.Y[i]);
    }

    printf("\n Recuperador ");
    for(int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.Re[i]);
    }

    printf("\n SR ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.SR[i]);
    }

    printf("\n CR ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.CR[i]);
    }

    printf("\n Patio ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.Ps[i]);
    }

    printf("\n PI ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.PI[i]);
    }

    printf("\n PF ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.PF[i]);
    }
    
    printf("\n PM ");
    for (int i = 1; i <= Numero_navio; i++) {
        printf("%d ", s.PM[i]);
    }

}       

int main(int argc, char** argv) {

    for (int i = 1; i < argc; i++) {

        double tempo = 0;

        char * caminho = argv[i];

        Solucao s;

        hi = clock();    

        int TotalTempo      = 300;
        int TotalPatio      =   3;
        int TotalPosicoes   =  20;
        setarDadosConfig(TotalPatio, TotalPosicoes, TotalTempo);

        caregarDados(caminho);
        testarDados();
        limparSolucao();
        
        ordernar();

        contrutivo(s);
        imprimirSolucao(s);
        
        hf = clock();
		tempo = (((double)hf - (double)hi) / CLOCKS_PER_SEC);

        //char  *inst = "";
        //char aux[100]  = "4_3/";
        //char aux[100]  = "4_3/";
        //inst  = argv[i];
        //strcat(aux, inst);
        //char c2[100];
        //strcpy(c2, caminho);

        //gerarArquivoInstancia(s, aux);
        gerarArquivoTabela(s, "Tabela_4_3_30.txt", tempo);
    
    }

    return 0;
}   

