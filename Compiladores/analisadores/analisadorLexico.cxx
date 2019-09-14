#include <stdio.h>
#include <bits/stdc++.h>

using namespace std;

//tamanho de caracteres por linha
int TAM = 128;
char buf[555555];

bool ok = true;
int lin=1, col=1, alin=1, acol=1;
int idx=0;

void inc_lin(){
	alin=lin;
	acol=col;
	lin++;
	col=1;
}

void inc_col(){
	alin=lin;
	acol=col;
	col++;
}

void print_atual(){
	cout << lin << " " << col << endl;
	ok = false;
}

void print_ant(){
	cout << alin << " " << acol << endl;
		ok = false;
}

void parsear_id(){
	int tam = 1;
	inc_col();
	while(tam < TAM && (buf[idx] >= 'a' && buf[idx] <= 'z' ||
			buf[idx] >= 'A' && buf[idx] <= 'Z' || buf[idx] >= '0' && buf[idx] <= '9')){
		inc_col();
		tam++;
	}
	if(tam == TAM && (buf[idx] >= 'a' && buf[idx] <= 'z' ||
				buf[idx] >= 'A' && buf[idx] <= 'Z' || buf[idx] >= '0' && buf[idx] <= '9')){
			print_ant();
		}
}


void parsear_num(){
	int tam = 1;
		inc_col();
		bool flag = false;
		while(tam < TAM && (buf[idx] == '.' && !flag || buf[idx] >= '0' && buf[idx] <= '9')){
			if(buf[idx] == '.'){
				flag = true;
			}
			inc_col();
			tam++;
		}
		if(tam == TAM && (buf[idx] >= 'a' && buf[idx] <= 'z' ||
					buf[idx] >= 'A' && buf[idx] <= 'Z' || buf[idx] >= '0' && buf[idx] <= '9')){
			print_ant();
		}
}

void parsear_str(){
	int tam = 1;
	inc_col();
	while(tam < TAM && buf[idx] !=  '"' && buf[idx] != "\0" ){
		if(buf[idx] == '\n'){
			inc_lin();
		}else{
			inc_col();
		}
		tam++;
	}
	if(buf[idx] !=  '"'){
		print_ant();
	} else {
		inc_col();
	}
}


int main(int argc, char **argv)
{
	int n = fread(buf, sizeof(char), 555554, stdin);
	buf[n] = '\0';

	for(int i=0; i < n; i++){
		if(buf[i] < 9 || buf[i] > 10 && buf[i] < 32 || buf[i] > 126 ){
			cout << "ARQUIVO INVALIDO" << endl;
			return 0;
		}
	}
	while(idx < n){
		if(buf[idx] >= 'a' && buf[idx] <= 'z' ||  buf[idx] >= 'A' && buf[idx] <= 'Z'){
			parsear_id();
		}else if (buf[idx] >= '0' && buf[idx] <= '9'){
			parsear_num();
		}else if(buf[idx] == '"'){
			parsear_str();
		}else if(buf[idx] == '.' || buf[idx] == ':' || buf[idx] == ';' || buf[idx] == '<' || buf[idx] == '-' ||
				buf[idx] == '+' || buf[idx] == '*' || buf[idx] == '/' || buf[idx] == '%' || buf[idx] == '(' ||
				buf[idx] == ')' || buf[idx] == '[' || buf[idx] == ']' || buf[idx] == '>' || buf[idx] == '&' ||
				buf[idx] == '|' || buf[idx] == '!' || buf[idx] == ' ' || buf[idx] == '\t'){
			idx++;
			inc_col();
		}
	}

	return 0;
}
