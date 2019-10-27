
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

//Lista de Defines para os tokens

//Terminais
#define FIM_ARQUIVO 0
#define PORCENTAGEM 1
#define ABRE_PARENTESE 2
#define FECHA_PARENTESE 3
#define ASTERICO 4
#define SOMA 5
#define VIRGULA 6
#define SUBTRACAO 7
#define PONTO 8
#define BARRA 9
#define MENOR 10
#define MENOR_IGUAL 11
#define DIFERENTE 12
#define IGUAL 13
#define MAIOR 14
#define MAIOR_IGUAL 15
#define AS 16
#define DECLARE 17
#define DO 18
#define ELSE 19
#define FOR 20
#define FOREACH 21
#define FROM 22
#define IF 23
#define IN 24
#define LETTER 25
#define NUMBER 26
#define PRINT 27
#define PUT 28
#define READ 29
#define RESIZE 30
#define THEN 31
#define TO 32
#define ABRE_COLCHETE 33
#define FECHA_COLCHETE 34
#define CHAR 35
#define ID 36
#define NUM 37
#define STRING 38
//Não Terminais
#define S 39
#define SS 40
#define ALOCACAO 41
#define ATRIBUICAO 42
#define DECLARACAO 43
#define DESVIO 44
#define ELSE_NAO_TERMINAL 45
#define EXPR_ARIT 46
#define EXPR_ARIT2 47
#define EXPR_ARIT3 48
#define EXPR_ARIT4 49
#define EXPR_REL 50
#define ID1 51
#define ID2 52
#define IDVET 53
#define LER_IMRPIME 54
#define LISTA_IDS 55
#define LOOP 56
#define OPR_REL 57
#define TIPO 58


//Estruturas para Análise Léxica
typedef struct no
{
	struct no *prox;
	char caractere;
}No;

typedef struct lista
{
	No* Primeiro;
	struct lista *prox;
	int num;
	int tipo;
}Lista;

//Estrutura para Análise Sintática
typedef struct dado
{
	int elem;
	char *valor;
	struct dado *anterior;
}Dado;

typedef struct pilha {
	Dado *topo;
}Pilha;

//Estrutura para Análise Semântica
typedef struct listaVar
{
	int contexto, tamanho,vetAlocado;
	char *valor,*tipo;
	struct listaVar *prox;
	
}ListaVar;

Lista *iniciaLista();
Lista *listaTokens=NULL;

char palavrasReservadas[20][17];

int vazia(Pilha* p);
void libera(Pilha* p);
Pilha* cria(void) ;
void push(Pilha* p,int v, char *valor);
int pop(Pilha* p);
int popN(Pilha* p,int n,int atual);
void carregarPalavrasReservadas();
Lista *iniciaLista();
void adicionaCaractere(Lista *l, char c);
char* criaTokken(Lista *l);
int executaAnaliseLexica(char *programa, int fSize);
int verificaArquivo(char *programa, int fSize);
int converteTokenInteiro(char *token,int tipo);
int slr1(Pilha* aux, int tab[116][60], int red[53][2]);
Pilha* executaAnaliseSintatica();

//Estruturas da pilha

//Pilha vazia
int vazia(Pilha* p)
{
	return (p->topo == NULL);
}

void libera(Pilha* p)
{
	Dado* q = p->topo;
	while(q != NULL) {
		Dado *t = q->anterior;
		free(q);
		q = t;
	}
	free(p);
}

//Cria uma pilha
Pilha* cria(void) 
{
	Pilha* p;
	p = (Pilha*)malloc(sizeof(Pilha));
	p->topo = NULL;
	return p;	
}

//Insere um elemento na pilha
void push(Pilha* p,int v,char *vl){
	Dado* aux = (Dado*) malloc(sizeof(Dado));
	aux->elem = v;
	aux->valor = (char*)malloc(listaTokens->num*sizeof(char));
	strcpy(aux->valor,vl);
	aux->anterior = p->topo;
	p->topo = aux;
}	

//Desempilha um elemento da pilha
int pop(Pilha* p)
{
	int v;
	Dado* aux;
	if (vazia(p)) {
		exit(1);
	}
	v = p->topo->elem;
	
	aux = p->topo;
	p->topo = aux->anterior;
	free(aux);
	return v;
}

//Desempilha um valor da pilha
char* popValor(Pilha* p)
{
	char *v = (char*)malloc(listaTokens->num*sizeof(char));
	Dado* aux;
	if (vazia(p)) {
		exit(1);
	}
	strcpy(v,p->topo->valor);
	aux = p->topo;
	p->topo = aux->anterior;
	free(aux);
	return v;
}

//Desempilha n elementos da pilha
int popN(Pilha* p,int n,int atual)
{
	int v, i;
	Dado* aux;
	if(n==0) return atual;
	
	for(i=0;i<n;i++)
	{
		if (vazia(p)) {
			exit(1);
		}
		v = p->topo->elem;
		aux = p->topo;
		p->topo = aux->anterior;
		free(aux);
	}
	return v;
}

//Funções da Análise Léxica

//Coleção das palavras reservadas do programa
void carregarPalavrasReservadas() {
	
//		strcpy(palavrasReservadas[0], "DECLARE");
//		strcpy(palavrasReservadas[1], "AS");
//		strcpy(palavrasReservadas[2], "NUMBER");
//		strcpy(palavrasReservadas[3], "LETTER");
//		strcpy(palavrasReservadas[4], "PUT");
//		strcpy(palavrasReservadas[5], "IN");
//		strcpy(palavrasReservadas[6], "IF");
//		strcpy(palavrasReservadas[7], "THEN");
//		strcpy(palavrasReservadas[8], "ELSE");
//		strcpy(palavrasReservadas[9], "FOREACH");
//		strcpy(palavrasReservadas[10], "DO");
//		strcpy(palavrasReservadas[11], "FOR");
//		strcpy(palavrasReservadas[12], "FROM");
//		strcpy(palavrasReservadas[13], "TO");
//		strcpy(palavrasReservadas[14], "RESIZE");
//		strcpy(palavrasReservadas[15], "READ");
//		strcpy(palavrasReservadas[16], "PRINT");
		
		strcpy(palavrasReservadas[0], "programainicio");
		strcpy(palavrasReservadas[1], "execucaoinicio");
		strcpy(palavrasReservadas[2], "fimexecucao");
		strcpy(palavrasReservadas[3], "fimprograma");
		strcpy(palavrasReservadas[4], "definainstrucao");
		strcpy(palavrasReservadas[5], "como");
		strcpy(palavrasReservadas[6], "inicio");
		strcpy(palavrasReservadas[7], "fim");
		strcpy(palavrasReservadas[8], "repita");
		strcpy(palavrasReservadas[9], "vezes");
		strcpy(palavrasReservadas[10], "fimrepita");
		strcpy(palavrasReservadas[11], "enquanto");
		strcpy(palavrasReservadas[12], "faca");
		strcpy(palavrasReservadas[13], "fimpara");
		strcpy(palavrasReservadas[14], "se");
		strcpy(palavrasReservadas[15], "entao");
		strcpy(palavrasReservadas[16], "fimse");
		strcpy(palavrasReservadas[17], "senao");
		strcpy(palavrasReservadas[18], "fimsenao");
		strcpy(palavrasReservadas[19], "mova");
		strcpy(palavrasReservadas[20], "passos");
		strcpy(palavrasReservadas[21], "Vire Para");
		strcpy(palavrasReservadas[22], "Pare");
		strcpy(palavrasReservadas[23], "Finalize");
		strcpy(palavrasReservadas[24], "Apague Lampada");
		strcpy(palavrasReservadas[25], "Acenda Lampada");
		strcpy(palavrasReservadas[26], "Aguarde Ate");
		strcpy(palavrasReservadas[27], "Robo Pronto");
		strcpy(palavrasReservadas[28], "Robo Ocupado");
		strcpy(palavrasReservadas[29], "Robo Parado");
		strcpy(palavrasReservadas[30], "Robo Movimentando");
		strcpy(palavrasReservadas[31], "Frente Robo Bloqueada");
		strcpy(palavrasReservadas[32], "Direita Robo Bloqueada");
		strcpy(palavrasReservadas[33], "Esquerda Robo Bloqueada");
		strcpy(palavrasReservadas[34], "Lampada Acessa a Frente");
		strcpy(palavrasReservadas[35], "Lampada Apagada a Frente");
		strcpy(palavrasReservadas[36], "Lampada Acessa A Esquerda");
		strcpy(palavrasReservadas[37], "Lampada Apagada A Esquerda");
		strcpy(palavrasReservadas[38], "Lampada Acessa A Direita");
		strcpy(palavrasReservadas[39], "Lampada Apagada A Direita");
		strcpy(palavrasReservadas[40], "esquerda");
		strcpy(palavrasReservadas[41], "direita");
}	

Lista *iniciaLista() {
	Lista *l = (Lista*)malloc(sizeof(Lista));
	l->Primeiro = NULL;
	l->num = 0;
	l->tipo = 0;
	return l;
}

//Adiciona caractere na lista encadeada de tokens
void adicionaCaractere(Lista *l, char c) {
	No *n = (No*)malloc(sizeof(No));
	n->caractere = c;
	n->prox = NULL;
	
	if(l->Primeiro == NULL)	{
		l->Primeiro = n;
		return;
	}
	
	No *Aux = l->Primeiro;
	
	while(Aux->prox != NULL)
		Aux = Aux->prox;
	
	Aux->prox = n;
}

//cria um tokken e salva na estrutura
char* criaTokken(Lista *l) {
	No *caractere = l->Primeiro;
	char *token = (char*)malloc(l->num*sizeof(char));
	strcpy(token, "\0");
	
	while(caractere != NULL) {
		strcat(token,&caractere->caractere);
		strcat(token,"\0");
		caractere = caractere->prox;
	} 
	return token;
}

//Inicia análise lexica, recebendo como parâmetro o programada e o fsize(quantidade de caracteres).
int executaAnaliseLexica(char *programa, int fSize) {	
	
	Lista* lConectada=(Lista*)malloc(sizeof(Lista));
	int i=0;
	int j;
	int k;
	int linhaAtual = 1;
	int erroAnalise = 0;
	
	while(i < fSize) {
		//Verificando se é palavra reservada ou identificador
		if((programa[i] >= 65 && programa[i] <= 90) || (programa[i] >= 97 && programa[i] <= 122)) {
			Lista *l = iniciaLista();
			l->tipo = 1;
			j = i;
			
			while((programa[j] >= 65 && programa[j] <= 90) || (programa[j] >= 97 && programa[j] <= 122) || (programa[j] >= 48 && programa[j] <= 57)) {
				adicionaCaractere(l, programa[j]);
				l->num++;
				j++;
			}
			
			char *tokenImpresso = criaTokken(l);
			
			for(k=0; k < 17; k++) {
				if(strcasecmp(tokenImpresso, palavrasReservadas[k]) == 0) {
					l->tipo = 2;
					break;
				}
			}
		
			i = j-1;
			strcpy(tokenImpresso, "\0");
			
			if (listaTokens == NULL){
				listaTokens= l;
				lConectada= l;
			} else if (lConectada->prox==NULL){
				lConectada->prox=l;
				lConectada=l;
			}
		}			
		//Verificando se é um dos caracteres especiais: +-/%*()[],.
		else if (((programa[i] >= 40) && (programa[i] <= 47)) || (programa[i] == 37) || (programa[i] == 91 ) || (programa[i] == 93 )) {
			Lista *l = iniciaLista();
			l->tipo = 3;
			j = i;
			adicionaCaractere(l, programa[j]);
			l->num++;
			j++;
			i = j-1;
			
			if (listaTokens == NULL){
				listaTokens= l;
				lConectada= l;
			} 
			else if (lConectada->prox==NULL){
				lConectada->prox=l;
				lConectada=l;
			}
		}
		
		//Verificando se é numero 
		else if(programa[i] >= 48 && programa[i] <= 57) {				
			Lista *l = iniciaLista();
			l->tipo = 4;
			int erro = 0;
			int quant=0;
			j = i;
			
			while((programa[j] >=97 && programa[j] <= 122 ) || (programa[j] >= 48 && programa[j] <= 57) || (programa[j] >= 65 && programa[j] <= 90)) {
				if((programa[j] < 48 || programa[j] > 57))
					erro = 1;
				else
					quant++;
				
				adicionaCaractere(l, programa[j]);
				l->num++;
				j++;
			}
			
			char *tokenImpresso = criaTokken(l);
		
			if(erro==1 || quant > 10) {
				erroAnalise = 1;
				printf("LINHA %d: %s\n", linhaAtual, tokenImpresso);
			}
			i = j-1;
			strcpy(tokenImpresso, "\0");
			
			if (listaTokens == NULL){
				listaTokens= l;
				lConectada= l;
			} 
			else if (lConectada->prox==NULL){
				lConectada->prox=l;
				lConectada=l;
			}
		}
//		//Varre o programa para encontrar os símbolos <,=,>
//		else if (programa[i] >= 60 && programa[i] <=62) {
//				
//				Lista *l = iniciaLista();
//				l->tipo = 5;
//				j = i;
//				
//				adicionaCaractere(l, programa[j]);
//				l->num++;
//				
//				if(programa[j]==60) {
//					if(programa[j+1]==61 || programa[j+1]==62) {
//						
//						adicionaCaractere(l, programa[j+1]);
//						l->num++;
//						j++;
//					}
//				} 
//				else if(programa[j]==62) {
//					if(programa[j+1]==61) {
//						
//						adicionaCaractere(l, programa[j+1]);
//						l->num++;
//						j++;
//					}
//				}
//				j++;
//				
//				char *tokenImpresso = criaTokken(l);
//								
//				i = j-1;
//				strcpy(tokenImpresso, "\0");
//				
//				if (listaTokens == NULL){
//					listaTokens= l;
//					lConectada= l;
//				} 
//				else if (lConectada->prox==NULL){
//					lConectada->prox=l;
//					lConectada=l;
//				}
//		}
//		//Verificando constante do tipo caractere
//		else if (programa[i] == 39) {
//			
//			Lista *l = iniciaLista();
//			l->tipo = 6;
//			int quant=0;//quantidade de aspas simples.
//			int quant2=0;//quantidade de caracteres, se for uma excessão conta como apenas um caractere.
//			int erro=0;//0 se não encontrou erro, 1 não encontrou erro.
//			j = i;
//			
//			while((programa[j] >= 32 && programa[j] <= 126) && quant<2) {
//				adicionaCaractere(l, programa[j]);
//				l->num++;
//				quant2++;
//				
//				if(programa[j]==34)
//					erro=1;
//				else if(programa[j]==92) {
//					if(programa[j+1]==9 || programa[j+1]==10) {
//						j++;
//						break;	
//					}
//					else if(programa[j+1]==116 || programa[j+1]==110 || programa[j+1]==92 || programa[j+1]==39 || programa[j+1]==34);
//					else { 
//						erro=1;
//						quant2++;
//					}
//					adicionaCaractere(l, programa[j+1]);
//					l->num++;
//					j++;
//				}
//				else if(programa[j]==39)
//					quant++;
//				
//				j++;
//			}
//				
//			char *tokenImpresso = criaTokken(l);
//			
//			if(erro==1 || quant != 2 || quant2 != 3) {
//				erroAnalise = 1;
//				printf("LINHA %d: %s\n", linhaAtual, tokenImpresso);
//			}
//			
//			i = j-1;
//			strcpy(tokenImpresso, "\0");
//			
//			if (listaTokens == NULL){
//				listaTokens= l;
//				lConectada= l;
//			} 
//			else if (lConectada->prox==NULL){
//				lConectada->prox=l;
//				lConectada=l;
//			}
//		}
		//Verificando constante do tipo string
//		else if (programa[i] == 34) {
//			Lista *l = iniciaLista();
//			int quant=0; //Quantidade de aspas duplas(código 34) encontradas.
//			int quant2=0; //quantidade de caracteres, se for uma excessão conta como apenas um caractere.
//			int erro=0;
//			l->tipo = 7;
//			j = i;
//		
//			while((programa[j] >= 32 && programa[j] <= 126) && quant<2) {
//				adicionaCaractere(l, programa[j]);
//				l->num++;
//				quant2++;
//				
//				if(programa[j]==39)
//					erro=1;
//				else if(programa[j]==92) {
//					if(programa[j+1]==9 || programa[j+1]==10) {
//						j++;
//						break;	
//					}
//					else if(programa[j+1]==116 || programa[j+1]==110 || programa[j+1]==92 || programa[j+1]==39 || programa[j+1]==34);
//					else {
//						erro=1;
//						quant2++;
//					}
//					adicionaCaractere(l, programa[j+1]);
//					l->num++;
//					j++;
//				}
//				else if(programa[j]==34)
//					quant++;
//				
//				j++;
//			}
//		
//			char *tokenImpresso = criaTokken(l);
//			
//			if(erro == 1 || quant2-quant > 256 || quant != 2) {
//				erroAnalise = 1;
//				printf("LINHA %d: %s\n", linhaAtual, tokenImpresso);	
//			}
//			
//			i = j-1;
//			strcpy(tokenImpresso, "\0");
//		
//			if (listaTokens == NULL){
//				listaTokens= l;
//				lConectada= l;
//			} 
//			else if (lConectada->prox==NULL){
//				lConectada->prox=l;
//				lConectada=l;
//			}
//		}
		else if(programa[i] != 32 && programa[i] != '\t' && programa[i] !='\n' && programa[i] != '\r' && programa[i] > 0) {
			erroAnalise = 1;
			printf("LINHA %d: %c\n", linhaAtual, programa[i]);
		}
		
		else if(programa[i] == '\n')
			linhaAtual++;
		
		//incrementa para o próximo caractere do programa
		i++;
	}
	return erroAnalise;		
}

int verificaArquivo(char *programa, int fSize)
{
	int i=0;
	
	while(i < fSize){
		if((programa[i] !=  9) && (programa[i] != 10) && (programa[i] < 32 || programa[i] > 126))
			return -1;
		i++;
	}
	return 1;
}

//Funções da Análise Sintática

//int converteTokenInteiro(char *token,int tipo){
//	
//	if(strcasecmp(token ,"%") == 0) return PORCENTAGEM;
//	else if(strcasecmp(token ,"(") == 0) return ABRE_PARENTESE;
//	else if(strcasecmp(token ,")") == 0) return FECHA_PARENTESE;
//	else if(strcasecmp(token ,"*") == 0) return ASTERICO;
//	else if(strcasecmp(token ,"+") == 0) return SOMA;
//	else if(strcasecmp(token ,",") == 0) return VIRGULA;
//	else if(strcasecmp(token ,"-") == 0) return SUBTRACAO;
//	else if(strcasecmp(token ,".") == 0) return PONTO;
//	else if(strcasecmp(token ,"/") == 0) return BARRA;
//	else if(strcasecmp(token ,"<") == 0) return MENOR;
//	else if(strcasecmp(token ,"<=") == 0) return MENOR_IGUAL;
//	else if(strcasecmp(token ,"<>") == 0) return DIFERENTE;
//	else if(strcasecmp(token ,"=") == 0) return IGUAL;
//	else if(strcasecmp(token ,">") == 0) return MAIOR;
//	else if(strcasecmp(token ,">=") == 0) return MAIOR_IGUAL;
//	else if(strcasecmp(token ,"AS") == 0) return AS;
//	else if(strcasecmp(token ,"DECLARE") == 0) return DECLARE;
//	else if(strcasecmp(token ,"DO") == 0) return DO;
//	else if(strcasecmp(token ,"ELSE") == 0) return ELSE;
//	else if(strcasecmp(token ,"FOR") == 0) return FOR;
//	else if(strcasecmp(token ,"FOREACH") == 0) return FOREACH;
//	else if(strcasecmp(token ,"FROM") == 0) return FROM;
//	else if(strcasecmp(token ,"IF") == 0) return IF;
//	else if(strcasecmp(token ,"IN") == 0) return IN;
//	else if(strcasecmp(token ,"LETTER") == 0) return LETTER;
//	else if(strcasecmp(token ,"NUMBER") == 0) return NUMBER;
//	else if(strcasecmp(token ,"PRINT") == 0) return PRINT;
//	else if(strcasecmp(token ,"PUT") == 0) return PUT;
//	else if(strcasecmp(token ,"READ") == 0) return READ;
//	else if(strcasecmp(token ,"RESIZE") == 0) return RESIZE;
//	else if(strcasecmp(token ,"THEN") == 0) return THEN;
//	else if(strcasecmp(token ,"TO") == 0) return TO;
//	else if(strcasecmp(token ,"[") == 0) return ABRE_COLCHETE;
//	else if(strcasecmp(token ,"]") == 0) return FECHA_COLCHETE;
//	else{
//		if(tipo == 6) return CHAR;
//		else if(tipo == 1) return ID;
//		else if(tipo == 4) return NUM;
//		else if(tipo == 7) return STRING;
//	}
//	
//}

int slr1(Pilha* aux, int tab[116][60], int red[53][2]){
	
	int atual = 0,elem,acao;
	Pilha* p2 = cria();
	char *str = (char*)malloc(listaTokens->num*sizeof(char));
	
	for(;;){
		
		strcpy(str,aux->topo->valor);
		elem = pop(aux);
		
		acao = tab[atual][elem];
		
		if(acao == -INT_MAX)
			return 0;
		else if(acao == INT_MAX)
			return 1;
		else if(acao < 0){
			push(p2,atual,str);
			atual = -acao;
		}
		else{
			push(aux,elem,str);
			push(aux,red[acao][0],"");
			atual = popN(p2,red[acao][1],atual);
		}
	}
}

Pilha* executaAnaliseSintatica()
{
   Pilha* p = cria();
   Pilha* aux = cria();
   Pilha* aux2 = cria();
   int elem;
   
   Lista *l;
   char *token = (char*)malloc(listaTokens->num*sizeof(char));
   char *str;
   int tokenAux,prim=0;
   strcpy(token, "\0");
   	
   for (l = listaTokens; l != NULL; l = l->prox)
   {
		while(l->Primeiro != NULL){	
			strcat(token,&l->Primeiro->caractere);
			strcat(token,"\0");
			l->Primeiro = l->Primeiro->prox;
		}
		
		push(p,converteTokenInteiro(token,l->tipo),token);
		strcpy(token, "\0");
	}
	//Empilha final da da pilha
	push(p,0,"");
	
	do{
		strcpy(token,p->topo->valor);
		elem = pop(p);
		push(aux,elem,token);
		push(aux2,elem,token);
		
	} while(!vazia(p));
	libera(p);
	
	int tab[118][60] = {
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-2,-1,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{1,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-17,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-18,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-19,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-20,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-21,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-22,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-25,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-24,-INT_MAX,-23,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-26,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-28,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-27,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-38,-29,-30,-33,-37,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-39,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-41,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-42,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-47,-40,-46,-45,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-44,-43,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{2,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,2,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{3,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,3,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{4,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,4,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{5,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,5,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{7,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,7,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-49,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-48,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,10,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,10,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,12,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,12,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-50,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-51,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-52,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-55,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-58,-INT_MAX,19,-57,19,-INT_MAX,19,19,-56,19,19,19,19,19,19,-INT_MAX,-INT_MAX,19,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,19,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,19,19,-INT_MAX,19,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,23,-INT_MAX,23,23,23,-INT_MAX,23,23,23,23,23,23,23,23,23,-INT_MAX,-INT_MAX,23,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,23,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,23,23,-INT_MAX,23,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,24,-INT_MAX,24,24,24,-INT_MAX,24,24,24,24,24,24,24,24,24,-INT_MAX,-INT_MAX,24,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,24,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,24,24,-INT_MAX,24,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-59,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,26,-INT_MAX,26,26,26,-INT_MAX,26,26,26,26,26,26,26,26,26,-INT_MAX,-INT_MAX,26,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,26,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,26,26,-INT_MAX,26,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,27,-INT_MAX,27,27,27,-INT_MAX,27,27,27,27,27,27,27,27,27,-INT_MAX,-INT_MAX,27,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,27,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,27,27,-INT_MAX,27,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,28,-INT_MAX,28,28,28,-INT_MAX,28,28,28,28,28,28,28,28,28,-INT_MAX,-INT_MAX,28,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,28,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,28,28,-60,28,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-61,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-62,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-64,-66,-69,-68,-65,-67,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-63,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-70,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,33,-INT_MAX,33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-71,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-72,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-73,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-74,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,49,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,50,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,51,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,52,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-77,-76,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-75},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-25,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-78,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-79,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-80,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-81,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-82,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-83,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-84,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-85,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-86,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-87,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,25,-INT_MAX,25,25,25,-INT_MAX,25,25,25,25,25,25,25,25,25,-INT_MAX,-INT_MAX,25,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,25,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,25,25,-INT_MAX,25,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-88,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-89,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-90,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-91,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,36,36,36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,37,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,37,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,37,37,37,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,38,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,38,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,38,38,38,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,39,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,39,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,39,39,39,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,40,40,40,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,41,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,41,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,41,41,41,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-92,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-93,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-94,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{47,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,47,-INT_MAX,-INT_MAX,47,47,-INT_MAX,47,-INT_MAX,-INT_MAX,-INT_MAX,47,47,47,47,-INT_MAX,-INT_MAX,-INT_MAX,47,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{48,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,48,-INT_MAX,-INT_MAX,48,48,-INT_MAX,48,-INT_MAX,-INT_MAX,-INT_MAX,48,48,48,48,-INT_MAX,-INT_MAX,-INT_MAX,48,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-95,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,14,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,15,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,11,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,11,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,13,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,13,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-96,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-97,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-58,-INT_MAX,17,-57,17,-INT_MAX,17,17,-56,17,17,17,17,17,17,-INT_MAX,-INT_MAX,17,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,17,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,17,17,-INT_MAX,17,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-58,-INT_MAX,18,-57,18,-INT_MAX,18,18,-56,18,18,18,18,18,18,-INT_MAX,-INT_MAX,18,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,18,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,18,18,-INT_MAX,18,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-98,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,20,-INT_MAX,20,20,20,-INT_MAX,20,20,20,20,20,20,20,20,20,-INT_MAX,-INT_MAX,20,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,20,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,20,20,-INT_MAX,20,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,21,-INT_MAX,21,21,21,-INT_MAX,21,21,21,21,21,21,21,21,21,-INT_MAX,-INT_MAX,21,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,21,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,21,21,-INT_MAX,21,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,22,-INT_MAX,22,22,22,-INT_MAX,22,22,22,22,22,22,22,22,22,-INT_MAX,-INT_MAX,22,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,22,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,22,22,-INT_MAX,22,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-99,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,30,-INT_MAX,30,30,30,-INT_MAX,30,30,30,30,30,30,30,30,30,-INT_MAX,-INT_MAX,30,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,30,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,30,30,-INT_MAX,30,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-100,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,35,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-101,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-102,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-103,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{9,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,9,-INT_MAX,-INT_MAX,9,9,-INT_MAX,9,-INT_MAX,-INT_MAX,-INT_MAX,9,9,9,9,-INT_MAX,-INT_MAX,-INT_MAX,9,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{16,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,16,-INT_MAX,-INT_MAX,16,16,-INT_MAX,16,-INT_MAX,-INT_MAX,-INT_MAX,16,16,16,16,-INT_MAX,-INT_MAX,-INT_MAX,16,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{31,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,31,-INT_MAX,-INT_MAX,31,31,-INT_MAX,31,-INT_MAX,-INT_MAX,-INT_MAX,31,31,31,31,-INT_MAX,-INT_MAX,-INT_MAX,31,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,32,-INT_MAX,-INT_MAX,32,32,-INT_MAX,32,-INT_MAX,-INT_MAX,-INT_MAX,32,32,32,32,-INT_MAX,-INT_MAX,-INT_MAX,32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,29,-INT_MAX,29,29,29,-INT_MAX,29,29,29,29,29,29,29,29,29,-INT_MAX,-INT_MAX,29,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,29,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,29,29,-INT_MAX,29,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-104,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-36,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-32,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-31,-35,-34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-105,-29,-30,-33,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,34,-INT_MAX,34,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-106,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{44,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,44,-INT_MAX,-108,44,44,-INT_MAX,44,-INT_MAX,-INT_MAX,-INT_MAX,44,44,44,44,-INT_MAX,-INT_MAX,-INT_MAX,44,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-107,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-54,-INT_MAX,-53,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-109,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-110,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{42,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,42,-INT_MAX,-INT_MAX,42,42,-INT_MAX,42,-INT_MAX,-INT_MAX,-INT_MAX,42,42,42,42,-INT_MAX,-INT_MAX,-INT_MAX,42,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-111,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-112,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-113,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-114,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-9,-INT_MAX,-INT_MAX,-13,-14,-INT_MAX,-12,-INT_MAX,-INT_MAX,-INT_MAX,-16,-11,-15,-10,-INT_MAX,-INT_MAX,-INT_MAX,8,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-115,-INT_MAX,-4,-5,-3,-6,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-8,-INT_MAX,-7,-INT_MAX,-INT_MAX},
	{46,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,46,-INT_MAX,-INT_MAX,46,46,-INT_MAX,46,-INT_MAX,-INT_MAX,-INT_MAX,46,46,46,46,-INT_MAX,-INT_MAX,-INT_MAX,46,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-116,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-117,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{43,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,43,-INT_MAX,-INT_MAX,43,43,-INT_MAX,43,-INT_MAX,-INT_MAX,-INT_MAX,43,43,43,43,-INT_MAX,-INT_MAX,-INT_MAX,43,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX},
	{45,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,45,-INT_MAX,-INT_MAX,45,45,-INT_MAX,45,-INT_MAX,-INT_MAX,-INT_MAX,45,45,45,45,-INT_MAX,-INT_MAX,-INT_MAX,45,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX}
	};
	int red[54][2] = {
		{0,1},//Aceitação da gramática
		{40,1},{39,2},{39,2},{39,2},{39,2},{39,2},{39,2},{39,0},
		{43,5},{55,1},{55,3},{53,1},{53,3},{58,1},{58,1},{41,5},
		{46,3},{46,3},{46,1},{47,3},{47,3},{47,3},{47,1},{48,1},
		{48,2},{48,1},{49,1},{49,1},{49,4},{49,3},{42,5},{42,5},
		{51,1},{51,4},{50,3},{57,1},{57,1},{57,1},{57,1},{57,1},
		{57,1},{44,7},{45,4},{45,0},{56,10},{56,8},{54,3},{54,3},
		{52,1},{52,1},{52,1},{52,1}
	};
	
	if(slr1(aux,tab,red)==0) return NULL;
	else return aux2;
	
}

//Funções para Análise Semãntica

ListaVar* criaListaVar(void) 
{
	return NULL;
}

ListaVar* insereListaVar(ListaVar* lv, int contex, char *val, char *type,int tam) 
{
	 ListaVar* novo = (ListaVar*) malloc(sizeof(ListaVar)); 
	 novo->valor = (char*)malloc(listaTokens->num*sizeof(char));
	 novo->tipo = (char*)malloc(listaTokens->num*sizeof(char));
	 novo->contexto = contex;
	 novo->tamanho = tam;
	 novo->vetAlocado = 0;
	 strcpy(novo->valor,val);
	 strcpy(novo->tipo,type);
	 novo->prox = lv;
	 return novo;
}

int listaVazia(ListaVar* lv) 
{
	return (lv == NULL);
}

void imprimeVariaveis(ListaVar* lv) 
{
	ListaVar* p; 
	for (p = lv; p != NULL; p = p->prox)
	{
	}
	//	printf("info = %s %d %d %d\n", p->valor,p->tamanho,p->vetAlocado,p->contexto);
}

ListaVar* removeContexto(ListaVar* lv, int contex) 
{
	ListaVar *p,*q;
	for (p=lv; p!=NULL; p=p->prox) {
		if(p->contexto	== contex) {
			p->contexto = -1;
		} 
	}
	 	 
	return lv;
}	
	
int buscaTamanhoVar(ListaVar* lv, char *val) 
{ 
	 ListaVar* p;
	 for (p=lv; p!=NULL; p=p->prox) 
		if (strcasecmp(p->valor,val)==0 && (p->contexto != -1)) {
			return p->tamanho;
		}
	 	 
	return 0;
}
int verificaVetorAlocado(ListaVar* lv, char *val) 
{ 
	 ListaVar* p;
	 for (p=lv; p!=NULL; p=p->prox) {
		if (strcasecmp(p->valor,val)==0 && (p->contexto != -1)) {
			return p->vetAlocado;
		}
	 }
	return 0;
}

ListaVar* insereTipoEmListaVar(ListaVar* lv,char *type){
	 ListaVar* p;
	 p->tipo = (char*)malloc(1000*sizeof(char));
	 for (p=lv; p!=NULL; p=p->prox) {
		strcpy(p->tipo,type);
	}
	 	 
	return lv;
}

ListaVar* verificaDeclaracao(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	char *var = (char*)malloc(1000*sizeof(char));
	char *str = (char*)malloc(1000*sizeof(char));
	int tam;
	
	while(elem != PONTO) {
		
		if(elem == ABRE_COLCHETE) {
			tam++;
			elem = pop(aux);
			tam++;
		}
		else if(elem == VIRGULA || elem == AS) {
			
			if(buscaTamanhoVar(lst,var)==0)
				lst = insereListaVar(lst,contex,var,"",tam);
			else {
				//if(verificaContexto(lst,var,contex)==0) 
				return NULL;
			}
			strcpy(str,"\0");
			strcpy(var,"\0");
			tam=0;
		}
		else if(elem == ID) { 
			strcat(var,str);
			tam++;
		}
		
		strcpy(str,aux->topo->valor);
		elem = pop(aux);
	}	
	
	return lst;
}

//----Verifica atribuicao de variáveis----
ListaVar* verificaAtribuicao(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	
	char *var = (char*)malloc(1000*sizeof(char));
	int tamVar;
	
	while(elem != PONTO) {
		if(elem == ID) {
			tamVar = buscaTamanhoVar(lst,var);
			if(tamVar==0) return NULL;
			else if(tamVar==1){
				elem = pop(aux);
				if(elem == PONTO) break;
				else if(elem == ABRE_COLCHETE) return NULL;
			}
			else if(tamVar==3){ 
				if(verificaVetorAlocado(lst,var)==0) 
					return NULL;
				elem = pop(aux);
				if(elem != ABRE_COLCHETE) return NULL;
			}
		}
		strcpy(var,aux->topo->valor);
		elem = pop(aux);
	}
	//imprimeVariaveis(lst);	
	return lst;
	
}

//----Verifica alocação de vetores----
ListaVar* alocaVetor(ListaVar* lv,char *val){
	
	ListaVar* p;
	 for (p=lv; p!=NULL; p=p->prox) {
		
		if (strcasecmp(p->valor,val)==0){
			
			p->vetAlocado = 1;
	 	}
	}
	return lv;
}

ListaVar* verificaAlocacao(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	char *var = (char*)malloc(1000*sizeof(char));
	
	while(elem != PONTO) {
		if(elem == ID) {
			if(buscaTamanhoVar(lst,var)!=3)
				return NULL;
			else
				lst = alocaVetor(lst,var);
		}
		strcpy(var,aux->topo->valor);
		elem = pop(aux);
	}	
	
	return lst;
}

//----Verifica Leitura e impressão----
ListaVar* verificaIo(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	char *var = (char*)malloc(1000*sizeof(char));
	int tamVar;
	
	while(elem != PONTO) {
		if(elem == ID) {
			tamVar = buscaTamanhoVar(lst,var);
			if(tamVar==0) return NULL;
			else if(tamVar==1){
				elem = pop(aux);
				if(elem == PONTO) break;
				else if(elem == ABRE_COLCHETE) return NULL;
			}
			else if(tamVar==3){ 
				if(verificaVetorAlocado(lst,var)==0) 
					return NULL;
				elem = pop(aux);
				if(elem != ABRE_COLCHETE) return NULL;
			}
		}
		strcpy(var,aux->topo->valor);
		elem = pop(aux);
	}	
	
	return lst;
}

//----Verifica Desvio Condicional e Loop----
ListaVar* verificaDesvioLoop(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	char *var = (char*)malloc(1000*sizeof(char));
	int tamVar;
	
	while(elem != ABRE_COLCHETE) {
		if(elem == ID) {
			tamVar = buscaTamanhoVar(lst,var);
			if(tamVar==0) 
				return NULL;
			else if(tamVar==1){
				elem = pop(aux);
				if(elem == ABRE_COLCHETE) return NULL;
			}
			else if(tamVar==3){ 
				if(verificaVetorAlocado(lst,var)==0) 
					return NULL;
				elem = pop(aux);
				if(elem != ABRE_COLCHETE) return NULL;
			}
		}
		strcpy(var,aux->topo->valor);
		elem = pop(aux);
	}	
	
	return lst;
}

//----Verifica Loop Vetorial----
ListaVar* verificaLoopVetorial(Pilha* aux, int contex,int elem,ListaVar* lst) {
	
	char *var = (char*)malloc(1000*sizeof(char));
	int tamVar;
		
	while(elem != ABRE_COLCHETE) {
		if(elem == ID) {
			
			tamVar = buscaTamanhoVar(lst,var);
			if(tamVar==0) 
				return NULL;
			else if(tamVar==1){
				elem = pop(aux);
				if(elem == ABRE_COLCHETE) return NULL;
			}
			else if(tamVar==3){ 
				if(verificaVetorAlocado(lst,var)==0) return NULL;
				elem = pop(aux);
				if(elem != ABRE_COLCHETE) return NULL;
			}
		}
		if(elem == IN) {
			strcpy(var,aux->topo->valor);
			tamVar = buscaTamanhoVar(lst,var);
			if(tamVar!=3) return NULL;
			if(verificaVetorAlocado(lst,var)==0) return NULL;
			elem = pop(aux);
		}
		strcpy(var,aux->topo->valor);
		elem = pop(aux);
	}	
	return lst;
}

int executaAnaliseSemantica(Pilha* p)
{
	int contexto = 0,elem;
	ListaVar* lst; 
	lst = criaListaVar(); 
	
	do{
		elem = pop(p);
		if(elem == DECLAREnmjikn,lmjhgfgbhkjhytrrhjkjhyt) {
			lst = verificaDeclaracao(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == RESIZE) {
			lst = verificaAlocacao(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == PUT) {
			lst = verificaAtribuicao(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == IF) {
			contexto++;
			lst = verificaDesvioLoop(p,contexto,elem,lst);
			if(lst==NULL){ 
				return 0;
			}
		}
		else if(elem == ELSE) {
			contexto++;
			lst = verificaDesvioLoop(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == FECHA_COLCHETE) {
			removeContexto(lst,contexto); 
			contexto--;
		}
		else if(elem == FOR) {
			contexto++;
			lst = verificaDesvioLoop(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == FOREACH) {
			contexto++;
			lst = verificaLoopVetorial(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		else if(elem == READ || elem == PRINT) {
			lst = verificaIo(p,contexto,elem,lst);
			if(lst==NULL) return 0;
		}
		
	} while(!vazia(p));
	libera(p);
	
	return 1;
	
}

int main(int argc, char **argv) {
	int fp;
	int tamanho = (int)strlen(argv[1]); //Obtem tamanho do argumento argv[1] e realiza um cast pra int.
	char *fNome = (char*)malloc(sizeof(char)*tamanho);//Aloca na memória um espaço com o tamanho do argv[1].
	int fSize; 
	int fread;
	
	
	
	strcpy(fNome,argv[1]);//Copia o nome do argv[1] para o ponteiro fNome.
	fp = open(fNome, O_RDONLY);//Abre o arquivo de entrada.
	
	struct stat st;
	stat(fNome, &st);
	fSize =  (int)st.st_size;//Obtem tamanho do arquivo lido, que será a quantidade de caracteres do programa.
	
	char *programa = (char*)malloc(sizeof(char)*fSize);
	
	fread = (int)read(fp,programa, fSize);
	
	if(fread < 0)
		exit(1);
	
	close(fp);

	if(verificaArquivo(programa, fSize)==1)
	{
		carregarPalavrasReservadas();
		if(executaAnaliseLexica(programa, fSize)==0) {
			Pilha* aux = executaAnaliseSintatica();
			if(aux==NULL) printf("NAO\n");
			else {
				printf("SIM\n");
				if(executaAnaliseSemantica(aux)==0) printf("NAO\n");
				else printf("SIM\n");
			}
		}
	}
	else
		printf("ARQUIVO INVÁLIDO!\n");
		
	return 0;
}
