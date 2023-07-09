#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Interface.h"

#define POINTERSIZE 4

typedef struct queue
{
    struct Div_admin ** sorted;
    int first;
    int last;
    int size;
}queue;

struct Hashfind
{
	struct Div_admin ** divisoes;
};

struct Hashfind * new_hashfind()
{
	struct Hashfind * hash = malloc(sizeof(struct Hashfind));
	hash -> divisoes = malloc(sizeof(struct Div_admin*) * 400 * 3);

	return hash;
}

struct queue *new_queue()
{
    struct queue * queue = malloc(sizeof(struct queue));

    queue -> sorted = malloc(sizeof(struct Div_admin*) * 400);
    queue -> last = 0;
    queue -> first = 0;
    queue -> size = 0;

    return queue;
}


bool is_empty(queue * queue)
{
    return queue -> size == 0;
}

bool is_full(queue * queue)
{
    return queue -> size == 400;
}


void enqueue(int size, struct Div_admin * data[size], queue *queue)
{
    if(!is_full(queue))
    {
        if(queue -> last == 400 - 1)
            queue -> last = 0;

        queue -> size = size;

        for(int i = 0; i < size; i++)
        	queue -> sorted[i] = data[i];

        queue -> sorted = sort(size, queue -> sorted);
        queue -> first = 0;
        queue -> last = queue -> size - 1;
    }
}

struct Div_admin * dequeue(queue * queue)
{
    struct Div_admin * data = queue -> sorted[queue -> first++];

 	admin_new_tempo(queue -> sorted[queue -> first - 1], 
 					-tempo_admin(queue -> sorted[queue -> first - 1]));

    if (queue -> first == 400)
        queue -> first = 0;

    queue -> size --;

    return data;
}

struct Div_admin ** initial_state(int code_start, struct Regiao * regiao,
								  struct ContainerDivAdmin *container,
								  struct Hashfind * hashfind)
{
	struct Div_admin ** admin = malloc(sizeof(struct Div_admin*) * regiao_num_divisoes(regiao));
	int posicao;
	for (int i = 0; i < regiao_num_divisoes(regiao); ++i)
	{
		if (regiao_divisao(regiao, i) == 0)
		{
			return admin;
		}
		admin[i] = malloc(size_admin());
		posicao = find_div_admin(regiao_divisao(regiao, i), container, false);
		read_admin(posicao, admin[i], container);
        initialization(admin, code_start, i); 					//if code_start == cod_div_admin => tempo = 0 |-| else => tempo = max

        int posicaohash = regiao_divisao(regiao, i) % 1200;

		while(hashfind -> divisoes[posicaohash] != NULL)
			posicaohash++;

		hashfind -> divisoes[posicaohash] = admin[i];

	}

	return admin;
}

long tempo(struct Div_admin *admin1, struct Div_admin *admin2)
{
	long distancia = sqrtl(pow((admin_coordx(admin1)) - (admin_coordx(admin2)), 2) + pow((admin_coordy(admin1)) - (admin_coordy(admin2)), 2));

	long tempo = (distancia * 60) / 70000;

	if (admin_habitantes(admin2) > 5000)
	{
		tempo += admin_habitantes(admin2) / 5000;
	}
	else
	{
		tempo += 1;
	}

	return tempo;
}


void relax(struct Div_admin *admin1, struct Div_admin *admin2)
{
	bool admin1neg = false;
	bool admin2neg = false;

	if(tempo_admin(admin1) < 0)
	{
		admin1neg = true;
		admin_new_tempo(admin1, -tempo_admin(admin1));
	}

	if(tempo_admin(admin2) < 0)
	{
		admin2neg = true;
		admin_new_tempo(admin2, -tempo_admin(admin2));
	}

	long time = tempo(admin1, admin2);

	if (tempo_admin(admin2) > tempo_admin(admin1) + time)
	{
		time_adjust(admin2, admin1, time);	//admin -> tempo = admin2 -> tempo + tempo
	}

	if(admin1neg == true)
		admin_new_tempo(admin1, -tempo_admin(admin1));

	if(admin2neg == true)
		admin_new_tempo(admin2, -tempo_admin(admin2));
}


struct Div_admin * find(int cod_admin, struct Hashfind * hashfind)
{
	int posicaohash = cod_admin % 1200;
	int count = 0;

	while(count < 1200 && admin_code(hashfind -> divisoes[posicaohash]) != cod_admin)
	{
		count++;
		posicaohash++;
	}

	return hashfind -> divisoes[posicaohash];
}

int Tempo_Viagem(int code_start, int code_end, 
				 struct ContainerDivAdmin *container_div,
				 struct ContainerReg *container_regiao)
{	
	int posicao_start = find_div_admin(code_start, container_div, false);
	int posicao_end = find_div_admin(code_end, container_div, false);

	char *j1 = int_to_string(6, code_start);
	char *j2 = int_to_string(6, code_end);

	if(posicao_start <= 0 && code_start == code_end)
	{
		printf("+ divisao %s inexistente\n", j1);

		free(j1);
		free(j2);
		return 0;
	}
	
	if(posicao_start <= 0)
	{
		printf("+ divisao %s inexistente\n", j1);

		free(j1);
	}

	if(posicao_end <= 0)
	{
		printf("+ divisao %s inexistente\n", j2);

		free(j2);
	}

	if (posicao_end <= 0 || posicao_start <= 0)
	{
		return 0;
	}

	if (code_start == code_end)
	{
		printf("+ viagem %s a %s demora 0 minutos\n", j1, j2);

		free(j1);
		free(j2);
		return 0;
	}

	struct Div_admin * admin_start = malloc(size_admin());
	read_admin(posicao_start, admin_start, container_div);

	struct Div_admin * admin_end = malloc(size_admin());
	read_admin(posicao_end, admin_end, container_div);

	if (admin_cod_regiao(admin_start) != admin_cod_regiao(admin_end))
	{
		printf("+ divisoes de regioes distintas\n");
		free(admin_start);
		free(admin_end);
		return 0;
	}

	struct Regiao * regiao = malloc(size_regiao());

	int posicao_regiao = find_regiao(admin_cod_regiao(admin_start), container_regiao, false);

	read_regiao(posicao_regiao, regiao, container_regiao);

	struct Hashfind * hashfind = new_hashfind(); 

	struct Div_admin ** mem_admin = initial_state(code_start, regiao, container_div, hashfind);		
	
	
	queue* admin_queue = new_queue();

	int num_divisoes = regiao_num_divisoes(regiao);
	
	enqueue(num_divisoes, mem_admin, admin_queue);
	
	struct Div_admin * current;
	
	while(!is_empty(admin_queue))
	{
		/*printf("////////////\n");
		for (int i = 0; i < num_divisoes; ++i)
		{
			printf("%d cod_admin %d tempo\n", admin_code(admin_queue -> sorted[i]), tempo_admin(admin_queue -> sorted[i]));
		}
		printf("////////////\n");*/
		current = dequeue(admin_queue);		//DEQUEUE DA DIV_ADMIN COM O MENOR TEMPo
		//printf("%d admin_ligacao %d code %d tempo\n", admin_num_ligacoes(current), admin_code(current), tempo_admin(current));
		for (int i = 0; i < admin_num_ligacoes(current); ++i) 
		{
			relax(current, find(admin_ligacao(current, i), hashfind));
		}
		admin_queue -> sorted = sort(num_divisoes, admin_queue -> sorted);	
	}

	admin_end = find(admin_code(admin_end), hashfind);
	
	time_total(admin_end);//if habitante >= 10000 => tempo -= habitantes % 5000 |-| else => tempo -=1

	if(-tempo_admin(admin_end) >= 565685400)
		printf("+ sem caminho de %s a %s\n", j1, j2);

	else
	{
		if(tempo_admin(admin_end) > 0)
			printf("+ viagem %s a %s demora %ld minutos\n", j1, j2, tempo_admin(admin_end));

		else if(tempo_admin(admin_end) < 0)
			printf("+ viagem %s a %s demora %ld minutos\n", j1, j2, -tempo_admin(admin_end));
	}

	for (int i = 0; i < num_divisoes ; ++i)
	{
		free(mem_admin[i]);
	}
	free(mem_admin);
	free(admin_queue);
	free(j1);
	free(j2);
	free(admin_start);
	return 0;
}

