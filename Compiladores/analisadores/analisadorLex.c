#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
char leitura[514];
char leituraAux[514];
int indiceLeitura;
int posicao = 0;
char palavra[1000000];
char reservadas[42] [27] = {"programainicio","execucaoinicio","fimexecucao","fimprograma","definainstrucao","como","inicio","fim","repita","vezes","fimrepita","enquanto","faca","fimpara","se","entao","fimse","senao","fimsenao","mova","passos","Vire,Para","Pare","Finalize","Apague,Lampada","Acenda,Lampada","Aguarde,Ate","Robo Pronto","Robo Ocupado","Robo Parado","Robo Movimentando","Frente Robo Bloqueada","Direita Robo Bloqueada","Esquerda Robo Bloqueada","Lampada Acessa a Frente","Lampada Apagada a Frente","Lampada Acessa A Esquerda","Lampada Apagada A Esquerda","Lampada Acessa A Direita","Lampada Apagada A Direita","esquerda","direita"};
int linha = 1;
int coluna = 1;
char tokens[10000][514];
int indexToken = 0;
int final;
int inicioCarac;
int inicioCarac2;
int QtdErros =0;
int tamanhoMaximo;

int teste =0;

bool rese(int indice){
	//printf("-----%d",indice);
	//scanf("%i", &indice);
	int i,j,match;
	for(i=0;i<43;i++){
		match = 0;
		for(j=0;j<indice;j++){
			if(leituraAux[j] == reservadas[i][j] ){
				match++;
			}
			else{
				j=indice;
			}
		}
		if(match == indice && reservadas[i][j+1]=='\0'){
			return true;
		}
	}
	return false;
}

void verificador(){
	//printf("\nverificador");
	int a;
	int i = 0;
	int i2 = 0;
	bool mudou = false;
	int virgulaAtivada = 0;
	char tipo = 'v';
	char erro = 'v';
	int inicioString;
	int colunaAux = coluna - indiceLeitura; // inicio da palavra no verificador
	int estouro= 0;
	while(leitura[i] != '\0' &&teste<50){
		teste++;
		//colunaAux++;
	    //printf("\nleitura[ %c ]",leitura[i]);
		//[a-z]
		if(leitura[i]>= 97 && leitura[i]<= 122){
			if(i2<512){
				printf(" => [a-z] -%c- ",leitura[i]);
				if(tipo == 'v' || tipo == 'i' || tipo == 'r'){
					leituraAux[i2] = leitura[i];
					i2++;
					tipo = 'i';
				}
				else{
					if(tipo == 'n'){
						erro = 'n';
					}
					mudou = true;
				//	printf(" => MUDEI");
				}
			}
			else{
				mudou =true;
			}
		}
		//[A-Z]
		else if(leitura[i]>= 65 && leitura[i]<= 90){
			printf(" => [A-Z] ");
			if(tipo == 'v'){
				leituraAux[i2] = leitura[i];
				i2++;
				tipo = 'r';
			}
			else if(tipo == 'i'){
				leituraAux[i2] = leitura[i];
				i2++;
			}
			else if(tipo == 'r'){
				leituraAux[i2] = leitura[i];
				i2++;
			}
			else{
				if(tipo == 'n'){
					erro = 'n';
				}
				mudou = true;
				//printf(" => MUDEI");
			}
		}
		//[0-9]
		else if((leitura[i]>= 48 && leitura[i]<= 57) || (leitura[i]== 44 && i2 != 0) ){
			if(i2<512){
				if(leitura[i]== 44){
					printf(" => virgulaAtivada %d",virgulaAtivada);
					virgulaAtivada++;
				}
				if(virgulaAtivada<2){
					printf(" => [0-9] %c ",leitura[i]);
					if(tipo == 'v'|| tipo == 'n'){
						leituraAux[i2] = leitura[i];
						i2++;
						tipo = 'n';
					}
					else if(tipo == 'i'){
						leituraAux[i2] = leitura[i];
						i2++;
					}
					else{
						mudou = true;
						//printf(" => MUDEI");
					}
				}
				else{
					mudou =true;
					erro = 'n';
					virgulaAtivada = 0;
					i--;
				}
			}
			else{
				mudou =true;
			}
		}
		// Caracteres especiais
		else if(
			(leitura[i] == 33) ||
			(leitura[i] >= 37 && leitura[i] <= 38) ||
			(leitura[i] >= 40 && leitura[i] <= 43) ||
			(leitura[i] >= 45 && leitura[i] <= 47) ||
			(leitura[i] >= 58 && leitura[i] <= 62) ||
			(leitura[i] == 91) ||
			(leitura[i] == 93) ||
			(leitura[i] == 111) ||
			(leitura[i] == 124)
		){
			if(i2<512){
				printf(" => Caracteres especiais = %c =\n",leitura[i]);
				if(tipo == 'v' || tipo == 'c'){
					leituraAux[i2] = leitura[i];
					i2++;
					if(tipo == 'v'){
						//printf(" => Primeiro Carac \n");
						inicioCarac = i;
						inicioCarac2 = colunaAux;
					}
					else{
						//printf(" => Nao primeiro \n");
						if(rese(i2)){
							//printf(" => Match \n");
						}
						else{
							//printf(" => Nao deu match \n");
							i2--;
							leituraAux[i2] = '\0';
							//i--;
							mudou = true;
						}
					}
					tipo = 'c';
					if(leitura[i+1] == '\0'){
						mudou = true;
						//printf(" => MUDEI ESPECIAL %d   Carac: %d   Carac: %d",i2, inicioCarac ,inicioCarac2);
					}
				}
				else{
					/*if(tipo == 'n' && leitura[i] == 59){ // Erro no topico 2.2
						erro = 'n';
					}*/
					mudou = true;
					//printf(" => MUDEI");
				}
			}
			else{
				mudou =true;
			}
		}
		//String
		else if(leitura[i] == 34){
			//printf(" => Aqui STRING %c ",tipo);
			inicioString = i;
			if(tipo == 'v'){
				//printf(" INICIO");
				//printf(" %d - %c \n",inicioString,leitura[i]);
				leituraAux[i2] = leitura[i];
				i2++;
				i++;
				int x= i;
				estouro= 0;
				while((x - i<=512) && (leitura[x] != 34) && leitura[x] != '\0') {
					leituraAux[i2] = leitura[x];
					colunaAux++;
					i2++;
					x++;
					estouro++;
					printf("AQUI %d - %c ",i2,leitura[x]);
					if(leitura[x] == 34 && estouro<=512) {
						leituraAux[i2] = leitura[x];
						i2++;
						mudou = true;
						printf(" => FIM STRING");
						i = x+1;
					}
				}
				if(estouro>512){
					printf(" => Estourou (%d) %d *",estouro,colunaAux);
					//Estourou Erro!
					//i = estouro;
					i2 = 0;
					erro = 's';
					mudou = true;
				}
				if(leitura[x] == '\0'){

					//printf("==linha %d colunaAux %d posicao %d\n", linha, colunaAux, posicao);
					int linhaProvisoria = linha;
					int colunaProvisoria = colunaAux;
					int indiceProvisorio = posicao+1;
					while(indiceProvisorio<tamanhoMaximo && (palavra[indiceProvisorio] !=34 )){
						//printf(" indiceProvisorio=%d ",indiceProvisorio);
						if(palavra[indiceProvisorio] ==10){
							linhaProvisoria++;
							colunaProvisoria =1;
						}
						else{
							colunaProvisoria++;
						}
						indiceProvisorio++;
					}
					if(palavra[indiceProvisorio] ==34 ){
						posicao = indiceProvisorio;
						linha = linhaProvisoria;
						coluna = colunaProvisoria;
						i =x;
					}
					else{
						estouro = x+1;
						erro = 's';
					}
					mudou = true;
				}
				tipo = 's';
			}
			else{
				mudou = true;
				//printf(" => MUDEI");
			}
		}
		else{
			//ERRO Caracter não identificado
			printf(" ___> %c \n",leitura[i]);
			printf("%d %d\n",linha,colunaAux);
			QtdErros++;
			for(a=0;a<i2;a++){
				leituraAux[a] ='\0';
			}
			//i++;
			//colunaAux++;
		}
		if(mudou){
			//printf(" => MUDANDO");
			//grava token
			if(i2<512){

				if(erro != 'v'){
					if(erro == 's'){
						printf("%d %d\n",linha,estouro);
						QtdErros++;
						for(a=0;a<514;a++){
							leituraAux[a] ='\0';
						}
						i = estouro;
						erro = 'v';
						mudou = false;
						i2 = 0;
					}
					if(erro == 'n'){
						printf("AQUY %d %d\n",linha,colunaAux-1);
						QtdErros++;
						for(a=0;a<i2;a++){
							leituraAux[a] ='\0';
						}
						i++;
						colunaAux++;
						erro = 'v';
						mudou = false;
						i2 = 0;
					}
				}
				else{
					if(tipo == 'r'||tipo == 'c'){
						if(rese(i2)){
							// inserir nos tokens
							for(a=0;a<i2;a++){
								//printf(" %c",leituraAux[a]);
								tokens[indexToken][a] = leituraAux[a];
								leituraAux[a] ='\0';
							}
							indexToken++;
							if(leitura[i+1] == 0){
								i++;
							}
						}
						else{
							//printf(" => NAO EH RESERVADA");
							if(tipo == 'r'){
								for(a=0;a<i2;a++){
									//printf(" %c",leituraAux[a]);
									tokens[indexToken][a] = leituraAux[a];
									leituraAux[a] ='\0';
								}
								indexToken++;
								if(leitura[i+1] == 0){
									i++;
								}
							}
							else{
								printf("%d %d AQUI\n",linha,colunaAux-i2-1);
								QtdErros++;
								for(a=0;a<i2;a++){
									leituraAux[a] ='\0';
								}
								i = inicioCarac+1;
								colunaAux = inicioCarac2 +1;
								erro = 'v';
							}
						}
					}
					else{
						if(i2>512){
							//estouro
						}
						for(a=0;a<i2;a++){
							//printf(" %c",leituraAux[a]);
							tokens[indexToken][a] = leituraAux[a];
							leituraAux[a] ='\0';
						}
						indexToken++;
					}
					i2 = 0;
					mudou = false;
					tipo = 'v';
				}
			}
			else{
				printf("=3 %d %d\n",linha,colunaAux-1);
				QtdErros++;
				for(a=0;a<i2;a++){
					leituraAux[a] ='\0';
				}
				i++;
				//colunaAux++;
				tipo = 'v';
				erro = 'v';
				i2=0;
			}
		}
		else{
			i++;
			colunaAux++;
			printf("<<<%d>>>",colunaAux);
		}
	}
	if(i2>0){
		int a;
		//printf(" ULTIMO ITEM");
		for(a=0;a<i2;a++){
			//printf(" %c",leituraAux[a]);
			tokens[indexToken][a] = leituraAux[a];
			leituraAux[a] ='\0';
		}
		indexToken++;
	}
}


char* reservada(){
	int i,j,match;
	for(i=0;i<43;i++){
		match = 0;
		for(j=0;j<indiceLeitura;j++){
			if(leitura[j] == reservadas[i][j] ){
				match++;
			}
			else{
				j=indiceLeitura	;
			}
		}
		if(match == indiceLeitura){
			return reservadas[i];
		}
	}
	return "";
}

bool identificador(){
	if(((leitura[0]>=65 &&leitura[0]<=90)||(leitura[0]>=97 &&leitura[0]<=122)) && indiceLeitura <= 512 ){
		int i;
		printf("%d",indiceLeitura);
		for(i=1;i<indiceLeitura;i++){
			if((leitura[i]>=65 &&leitura[i]<=90)||(leitura[i]>=97 &&leitura[i]<=122)||(leitura[i]>=48 &&leitura[i]<=57)){
				// Ainda é válido
			}
			else{
				return false;
			}
		}
		return true;
	}
	else{
		return false;
	}
}
bool constanteNumerica(){
	int i;
	for(i=0;i<indiceLeitura;i++){
		if(leitura[i]>=48 &&leitura[i]<=57 && indiceLeitura<512){
			// Ainda é válido
		}
		else{
			return false;
		}
	}
	return true;
}

bool eString(){
	int i=0;
	printf("%d",indiceLeitura);
	if(leitura[i]==34 && leitura[indiceLeitura-1]==34 && (indiceLeitura) <=(512+2)){
		return true;
	}
	else{
		return false;
	}
}

void procurarToken(){
	indiceLeitura = 0;
	int aux = 0;
	int a,b;
	while(posicao < final){
		//printf("procurarToken \n ");
		indiceLeitura = 0;
		while(palavra[posicao] !=' ' && palavra[posicao] !=9 && palavra[posicao] !=10 && posicao<515){

			leitura[indiceLeitura] = palavra[posicao];
			//printf("%d ",posicao);
			//printf("%c\n",palavra[posicao]);
			posicao++;
			coluna++;
			indiceLeitura++;
		}

			//printf("IDENTIFICANDO =>  %d",posicao);
			if(reservada()!=""){
				//printf(" reservada =>  ");
				for(b=0;b<indiceLeitura;b++){
					//printf(" %c",leitura[b]);
					tokens[indexToken][b] = leitura[b];
					leitura[b] ='\0';
				}
				indexToken++;
			}
			else if(constanteNumerica()){
				printf(" constanteNumerica =>  ");
				for(b=0;b<indiceLeitura;b++){
					//printf(" %c",leitura[b]);
					tokens[indexToken][b] = leitura[b];
					leitura[b] ='\0';
				}
				indexToken++;
			}
			else if(eString()){
				printf(" String =>  ");
				for(b=0;b<indiceLeitura;b++){
					//printf(" %c",leitura[b]);
					tokens[indexToken][b] = leitura[b];
					leitura[b] ='\0';
				}
				indexToken++;
			}
			else if(identificador()){
				printf(" identificador =>  ");
				for(b=0;b<indiceLeitura;b++){
					//printf(" %c",leitura[b]);
					tokens[indexToken][b] = leitura[b];
					leitura[b] ='\0';
				}
				indexToken++;
			}
			else{
				printf(" foi para o verificador =>  ");
				verificador();
			}
			if(palavra[posicao] ==10){
				linha++;
				coluna =1;
			}
			posicao++;
			coluna++;

	}

	int d;
	for(a=0;a<indexToken;a++){
		//printf("\nFor %d \n",a);
		d=0;
		while(tokens[a][d] !='\0'){
			//printf("\nWHILE ");
			printf("%c \n",tokens[a][d]);
			d++;
		}
	}
	//printf("\nFIM");
}

int main(int argc, char* argv[])
{
	bool invalido= false;
	reservadas[1][0] ='"';
    int i = 0;
    int numPalavras = 0;
    char line[50],c;

    FILE *p;
   	p = fopen("teste.txt", "r");	//ler arquivo
    //p = fopen(argv[1], "r");		//Ler argumento
    if (p == NULL)
        return EXIT_FAILURE;
   while (!feof(p))
    {
       fscanf(p,"%c",&c);
       if(!feof(p)){
       		//printf("%c",c);
       		if(!((c>=9 && c<=10)|| (c>=32 && c<=126))){
       			invalido= true;
			}
       		palavra[i]= c;
       		i++;
	   }
    }
    tamanhoMaximo = i;
   fclose(p);
   if(!invalido){
		//printf("\n");
		final = i;
		palavra[i]= ' ';
		procurarToken();
		if(QtdErros == 0){
			printf("OK\n");
		}
   }
   else{
   		printf("ARQUIVO INVALIDO\n");
   }

    return EXIT_SUCCESS;
}
