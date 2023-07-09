#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Interface.h"

struct Regiao
{
	unsigned short cod_regiao;
	char nome[40];
	int divisao[400];
	int num_divisoes;
};

struct Div_admin
{
	long tempo;
	unsigned short cod_regiao;
	unsigned int cod_div_admin;
	char Nome_Div[141];
	int coordx;
	int coordy;
	int num_habitantes;
	short num_ligacoes;
	int ligacoes[10];	
};

int size_regiao()
{
	return sizeof(struct Regiao);
}

int regiao_code(struct Regiao * regiao)
{
	return regiao -> cod_regiao;
}

char * regiao_nome(struct Regiao *regiao)
{
	return regiao -> nome;
}

int regiao_divisao(struct Regiao * regiao, int i)
{
	return regiao -> divisao[i];
}

int regiao_num_divisoes(struct Regiao * regiao)
{
	return regiao -> num_divisoes;
}

int size_admin()
{
	return sizeof(struct Div_admin);
}

unsigned short admin_cod_regiao(struct Div_admin * admin)
{
	return admin -> cod_regiao;
}


unsigned int admin_code(struct Div_admin * admin)
{
	return admin -> cod_div_admin;
}

char * admin_nome(struct Div_admin * admin)
{
	return admin -> Nome_Div;
}

int admin_coordx(struct Div_admin * admin)
{
	return admin -> coordx;
}

int admin_coordy(struct Div_admin * admin)
{
	return admin -> coordy;
}

int admin_habitantes(struct Div_admin * admin)
{
	return admin -> num_habitantes;
}

short admin_num_ligacoes(struct Div_admin * admin)
{
	return admin -> num_ligacoes;
}

int admin_ligacao(struct Div_admin * admin, int i)
{
	return admin -> ligacoes[i];
}

int * admin_ligacoes(struct Div_admin * admin)
{
	return admin -> ligacoes;
}

long tempo_admin(struct Div_admin * admin)
{
	return admin -> tempo;
}

/////////////   SETTERS   ////////////////////

void regiao_new_nome(struct Regiao *regiao, char *nome)
{
	strcpy(regiao -> nome, nome);
}

void new_regiao(struct Regiao *regiao, int code, char *nome)
{
	regiao_new_nome(regiao, nome);
	regiao -> cod_regiao = code;
	regiao -> num_divisoes = 0;
}


void regiao_new_ligacao(int cod_div_admin, struct Regiao * regiao)
{
	regiao -> divisao[regiao -> num_divisoes] = cod_div_admin;
	regiao -> num_divisoes++;
}

void bridge(struct Div_admin * admin, int code)
{
	admin -> ligacoes[admin -> num_ligacoes] = code;
	admin -> num_ligacoes++;
}

void ID(struct Div_admin * admin, char * str_i, char * str)
{
	int ch;

	for(int j = 3; j < 9; j++)
		str_i[j - 3] = str[j];

	admin -> cod_div_admin = atoi(str_i);

	while ((ch = getchar()) != EOF && ch != '\n');

	scanf(" %[^\n]", admin -> Nome_Div);

	scanf(" %hu", &admin -> cod_regiao);

	scanf(" %d %d", &admin -> coordx, &admin -> coordy);

	scanf(" %d", &admin -> num_habitantes);
}

void reset_ligacoes(struct Div_admin *admin)
{
	admin -> num_ligacoes = 0;
	for (int i = 0; i < 10; ++i)
	{
		admin -> ligacoes[i] = -1;
	}
}

void Div_ligacoes(struct Div_admin * admin, struct Div_admin * admin_disco)
{
	admin -> num_ligacoes = admin_disco -> num_ligacoes;
	for(int i = 0; i < admin_disco -> num_ligacoes; i++)
	{
		admin -> ligacoes[i] = admin_disco -> ligacoes[i];
	}
}

void admin_new_tempo(struct Div_admin * admin, int t)
{
	admin -> tempo = t;
}

void initialization(struct Div_admin ** admin, int code_start, int i)
{
	if (admin[i] -> cod_div_admin == code_start)
    {
        admin[i] -> tempo = 0;
    }
    else
    {
    	admin[i] -> tempo = 565685425; // distancia maxima 
    }
}

void time_adjust(struct Div_admin * admin1, struct Div_admin * admin2, long tempo)
{
	admin1 -> tempo = admin2 -> tempo + tempo;
}

void time_total(struct Div_admin * admin)
{
	if(admin -> num_habitantes >= 5000)
	{
		if(admin -> tempo < 0)
			admin -> tempo += admin -> num_habitantes / 5000;

		else if(admin -> tempo > 0)
			admin -> tempo -= admin -> num_habitantes / 5000;
	}
	else
	{
		if(admin -> tempo < 0)
			admin -> tempo += 1; 

		else if(admin -> tempo > 0)
			admin -> tempo -= 1;
	}
}

//////
int cmpfunc (const void * a, const void * b)
{
	struct Div_admin *adminA = *(struct Div_admin **)a;
	struct Div_admin *adminB = *(struct Div_admin **)b;

	if (adminA -> tempo < adminB -> tempo)
        return -1;

	else if (adminA -> tempo > adminB -> tempo)
        return 1;

	else
        return 0;
}

struct Div_admin ** sort(int n, struct Div_admin * tosort[n])
{	
	qsort(tosort, n, sizeof(struct Div_admin*), cmpfunc);

	return tosort;
}