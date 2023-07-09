#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "Interface.h"


#define sizeHashtableDiv 1200000
#define sizeHashtableReg 3000
#define PAGESIZE 4096



struct ContainerReg
{
	int fdRegiaoHashtable;
	int fd_regiao;
	struct RegiaoHashtable * regioeshash[sizeHashtableReg];
	struct header *header;
	struct Div_admin ** mem_admin;
};

struct ContainerDivAdmin
{
	int fdAdminHashtable;
	int fd_div_admin;
	struct AdminHashtable * divisoeshash[sizeHashtableDiv];
	struct header *header;
};

struct header
{
	int tail;
};

struct RegiaoHashtable
{
	unsigned short cod_regiao;					   
	short tposicao;
};

struct AdminHashtable
{
	int cod_div_admin;                  
	int tposicao;
};



struct header* new_head()
{
	struct header* header = malloc(sizeof(struct header));
	header -> tail = 1;
	return header;
}

int size_container_regiao()
{
	return sizeof(struct ContainerReg);
}

int size_container_admin()
{
	return sizeof(struct ContainerDivAdmin);
}

int posicao_bytesRegiaoHashTable(int posicao) 
{
	
	if(posicao == 0)
	{
		return 0;	
	}
	else
	{
		int bytes = (PAGESIZE / (sizeof(struct RegiaoHashtable)));   // bytes = numero de structs Div_admin podemos colocar numa pagina
		return (posicao / bytes) * PAGESIZE + 
			   (posicao % bytes) * sizeof(struct RegiaoHashtable);
	}
}
int posicao_bytesAdminHashtable(int posicao) 
{
	if(posicao == 0)
	{
		return 0;	
	}
	else
	{
		int bytes = (PAGESIZE / (sizeof(struct AdminHashtable)));   // bytes = numero de structs Div_admin podemos colocar numa pagina
		return (posicao / bytes) * PAGESIZE + 
			   (posicao % bytes) * sizeof(struct AdminHashtable);
	}
}

void open_files(char * name, int i, struct ContainerReg *container_regiao, 
				struct ContainerDivAdmin *container_div) 
{
	container_regiao -> header = new_head();
	container_div -> header = new_head();
	int fd;

	fd = open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);


	if (fd == -1)
	{
		perror("open");
		return;
	}

	switch(i)
	{
		case 0:
			container_regiao -> fdRegiaoHashtable = fd;

			struct RegiaoHashtable regiao;
			memset(&regiao,0,sizeof(struct RegiaoHashtable));
			lseek(container_regiao -> fdRegiaoHashtable, posicao_bytesRegiaoHashTable(3000),
	     		 SEEK_SET);
			write(container_regiao -> fdRegiaoHashtable, &regiao, sizeof(struct RegiaoHashtable));

			break;
		case 1:
			container_div -> fdAdminHashtable = fd;
			struct AdminHashtable admin;
			memset(&admin,0,sizeof(struct AdminHashtable));
			lseek(container_div -> fdAdminHashtable, posicao_bytesAdminHashtable(1200000),
	     			 SEEK_SET);
			write(container_div -> fdAdminHashtable, &admin, sizeof(struct AdminHashtable));
			break;
		case 2:
			container_regiao  -> fd_regiao = fd;
			lseek(fd, 0, SEEK_SET);
			write(fd, container_regiao -> header, sizeof(struct header));
			break;
		case 3:
			container_div -> fd_div_admin = fd;
			lseek(fd, 0, SEEK_SET);
			write(fd, container_div -> header, sizeof(struct header));
			break;
	}

}

void close_files(struct ContainerDivAdmin *containerdiv, 
				 struct ContainerReg *container_regiao)
{
	int status = close(container_regiao -> fdRegiaoHashtable);
	int status2 = close(containerdiv -> fdAdminHashtable);
	int status3 = close(container_regiao -> fd_regiao);
	int status4 = close(containerdiv -> fd_div_admin);

	for(int i = 0; i < 3000; i++)
	{
		free(container_regiao -> regioeshash[i]);
	}

	for(int i = 0; i < 1200000; i++)
	{
		free(containerdiv -> divisoeshash[i]);
	}
	free(containerdiv);
	free(container_regiao);

  	if(status == -1 || status2 == -1 || status3 == -1 || status4 == -1)
   		perror("close");
}

char * int_to_string(int size, int value)
{
	char * new_code = malloc(size + 1);
	int n = sprintf(new_code, "%d", value);
	char to_concat[size + 1];
	int j = 0;

	for(int i = 0; i < size; i++)
	{
		if(i < size - n)
			to_concat[i] = (char) 48;
		else
		{
			to_concat[i] = new_code[j];
			j++;
		}

	}
	to_concat[size] = '\0';

	strcpy(new_code, to_concat);
	
	return new_code;

}

int posicao_bytesRegiao(int posicao) 
{
	int bytes = (PAGESIZE / size_regiao());   // bytes = numero de structs Div_admin podemos colocar numa pagina
	return (posicao / bytes) * PAGESIZE + 
		   (posicao % bytes) * size_regiao();
}



int posicao_bytesAdmin(int posicao) 
{
	int bytes = (PAGESIZE / size_admin());   // bytes = numero de structs Div_admin podemos colocar numa pagina
	return (posicao / bytes) * PAGESIZE +
		   (posicao % bytes) * size_admin();
}

int read_headRegiao(struct ContainerReg *container)
{
	int tail = container -> header -> tail;
	container -> header -> tail++;
	lseek(container -> fd_regiao, 0, SEEK_SET);
	write(container -> fd_regiao, container -> header, 
		  sizeof(struct header));
	return tail;
}

int read_headDivAdmin(struct ContainerDivAdmin* container)
{
	int tail = container -> header -> tail;
	container -> header -> tail++;
	lseek(container -> fd_div_admin, 0, SEEK_SET);
	write(container -> fd_div_admin, container -> header, 
		  sizeof(struct header));
	return tail;
}

void writeRegiaoHashtable(struct RegiaoHashtable *regiao, int posicao, 
						  struct ContainerReg *container)
{
	regiao -> tposicao =  read_headRegiao(container);
	lseek(container -> fdRegiaoHashtable, posicao_bytesRegiaoHashTable(posicao),
	      SEEK_SET);
	write(container -> fdRegiaoHashtable, regiao, sizeof(struct RegiaoHashtable));
}

int find_regiao(int cod_regiao, struct ContainerReg *container,
				bool intro)
{
	int posicao = (cod_regiao % sizeHashtableReg);
	int contador = 0;
	bool in_hashtable = true;
	struct RegiaoHashtable *ler;
	while(contador <= sizeHashtableReg)
	{
		if (in_hashtable) // estamos a procurar na hashtable em memoria
		{
			if(container -> regioeshash[posicao] == NULL)
			{
				contador--;
				posicao--;
				ler =  malloc(sizeof(struct RegiaoHashtable));
				in_hashtable = false;
			}
			
			else if(container -> regioeshash[posicao] -> cod_regiao == cod_regiao) // vamos renomear a struct Regiao
			{
				return container -> regioeshash[posicao] -> tposicao;				
			}
		}
		else
		{
			lseek(container -> fdRegiaoHashtable, posicao_bytesRegiaoHashTable(posicao), SEEK_SET);
			read(container -> fdRegiaoHashtable, ler, sizeof(struct RegiaoHashtable));

			if(ler -> cod_regiao == cod_regiao) // vamos renomear a struct Regiao
			{
				container -> regioeshash[posicao] = ler; 
				return ler -> tposicao;   
			}

			else if (ler -> cod_regiao  == 0) // temos de criar uma nova
			{
				if (intro)
				{
					ler -> cod_regiao = cod_regiao;
					writeRegiaoHashtable(ler, posicao, container);
					container -> regioeshash[posicao] = ler;
				}

				return -(ler -> tposicao);				 
			}
		}

		posicao++;
		contador++;
		if(posicao > sizeHashtableReg)
		{
			posicao = 0;
		}
		
	}
	return 0;
}
	


void writeAdminHashtable(struct AdminHashtable *admin, int posicao, 
						 struct ContainerDivAdmin *container)
{
	admin -> tposicao =  read_headDivAdmin(container);
	lseek(container -> fdAdminHashtable, posicao_bytesAdminHashtable(posicao), 
		  SEEK_SET);
	write(container -> fdAdminHashtable, admin, sizeof(struct AdminHashtable));
}

int find_div_admin(int cod_div_admin , struct ContainerDivAdmin *container,
				   bool intro)
{
	int posicao = (cod_div_admin % sizeHashtableDiv);
	int contador = 0;
	bool in_hashtable = true;

	struct AdminHashtable *ler;
	

	while(contador <= sizeHashtableDiv)
	{

		if (in_hashtable) // estamos a procurar na hashtable em memoria
		{
			if( container -> divisoeshash[posicao] == NULL)
			{
				contador--;
				posicao--;
				ler =  malloc(sizeof(struct AdminHashtable));
				in_hashtable = false;
			}
			
			else if(container -> divisoeshash[posicao] -> cod_div_admin == cod_div_admin) // vamos renomear a struct Regiao
			{
				return container -> divisoeshash[posicao] -> tposicao;				
			}

		}
		
		else // estamos a procura no disco
		{
			lseek(container -> fdAdminHashtable, posicao_bytesAdminHashtable(posicao), SEEK_SET);
			read(container -> fdAdminHashtable, ler, sizeof(struct AdminHashtable));

			if(ler -> cod_div_admin == cod_div_admin) // vamos renomear a struct Regiao
			{
				container -> divisoeshash[posicao] = ler; 
				// atualizar a hashtable em memória com aquilo que está em disco
				return ler -> tposicao;				 
			}

			else if (ler -> cod_div_admin  == 0) // temos de criar uma nova
			{
				if (intro)
				{
					ler -> cod_div_admin = cod_div_admin;
					writeAdminHashtable(ler, posicao, container);
					container -> divisoeshash[posicao] = ler;
				}

				return -(ler -> tposicao);				 
			}
		}

		posicao++;
		contador++;

		if(posicao > sizeHashtableDiv)
		{
			posicao = 0;
		}
	}
	return 0;
}




void read_regiao(int posicao, struct Regiao *regiao, struct ContainerReg *container) 
{
	int posicao_bytes = posicao_bytesRegiao(posicao);
	lseek(container -> fd_regiao, posicao_bytes, SEEK_SET); 
	read(container -> fd_regiao, regiao, size_regiao());
}

void read_admin(int posicao, struct Div_admin *admin, 
				struct ContainerDivAdmin *container)
{
	int posicao_bytes = posicao_bytesAdmin(posicao);
	lseek(container -> fd_div_admin, posicao_bytes, SEEK_SET);
	read(container -> fd_div_admin, admin, size_admin());
}

void write_regiao(struct Regiao *reg, int posicao, 
				  struct ContainerReg *container)
{
	int posicao_bytes = posicao_bytesRegiao(posicao);
	lseek(container -> fd_regiao, posicao_bytes ,SEEK_SET); 
	write(container -> fd_regiao, reg, size_regiao());
}

void write_admin(struct Div_admin *admin, int posicao, 
				 struct ContainerDivAdmin *container)
{
	int posicao_bytes = posicao_bytesAdmin(posicao);
	lseek(container -> fd_div_admin ,posicao_bytes ,SEEK_SET); // espaço ocupado por uma divisao adminstrativa
	write(container -> fd_div_admin, admin, size_admin());
}

int write_bridge(int code_admin1, int code_admin2, struct ContainerDivAdmin *container)
{
	int posicao_admin1 = find_div_admin(code_admin1, container, false);
	int posicao_admin2 = find_div_admin(code_admin2, container, false);

	if(posicao_admin1 <= 0 && code_admin1 == code_admin2)
	{
		char *j = int_to_string(6, code_admin1);
		printf("+ divisao %s inexistente\n", j);
		free(j);
		return 0;
	}

	if(posicao_admin1 <= 0)
	{
		char *j = int_to_string(6, code_admin1);
		printf("+ divisao %s inexistente\n", j);
		free(j);
	}

	if(posicao_admin2 <= 0)
	{
		char *j = int_to_string(6, code_admin2);
		printf("+ divisao %s inexistente\n", j);
		free(j);
	}

	if(code_admin1 == code_admin2)
	{
		char *j = int_to_string(6, code_admin1);
		printf("+ ligacao de %s a si propria\n", j);
		free(j);
		return 0;
	}

	if(posicao_admin1 <= 0 || posicao_admin2 <= 0)
		return 0;

	struct Div_admin *admin1 = malloc(size_admin());
	struct Div_admin *admin2 = malloc(size_admin());

	read_admin(posicao_admin1, admin1, container);
	read_admin(posicao_admin2, admin2, container);

	if (admin_cod_regiao(admin1) != admin_cod_regiao(admin2))
	{
		printf("+ ligacao de divisoes de regioes distintas\n");
		free(admin1);
		free(admin2);
		return 0;
	}

	else
	{
	
		for (int i = 0; i < admin_num_ligacoes(admin1); ++i)
		{
			if (admin_ligacao(admin1, i) == code_admin2)
			{
				char *j1 = int_to_string(6, code_admin1);
				char *j2 = int_to_string(6, code_admin2);
			   	printf("+ ligacao %s - %s existente\n", j1, j2);
			   	free(j1);
			   	free(j2);
			   	return 0;
			}
		}

		bridge(admin1 , code_admin2);
		bridge(admin2 , code_admin1);

		write_admin(admin1, posicao_admin1, container);
		write_admin(admin2, posicao_admin2, container);

		char *j1 = int_to_string(6, code_admin1);
		char *j2 = int_to_string(6, code_admin2);

		printf("+ ligacao %s - %s criada\n", j1, j2);

		free(admin1);
		free(admin2);
		free(j1);
		free(j2);

		return 0;
	}
}

void intro_regiao(unsigned short code_regiao, char *nome, 
				  struct ContainerReg *container)
{	
	int posicao = find_regiao(code_regiao, container, true);
	struct Regiao *regiao =  malloc(size_regiao());

	if (posicao > 0)
	{
		char *j = int_to_string(4, code_regiao);
		printf("+ regiao %s renomeada\n", j);

		read_regiao(posicao, regiao, container);
		regiao_new_nome(regiao, nome);
		write_regiao(regiao, posicao, container);
		
		free(j);
	}

	else
	{
		char *j = int_to_string(4, code_regiao);

		printf("+ regiao %s criada\n", j);
		posicao = -(posicao);
		new_regiao(regiao, code_regiao, nome);
		write_regiao(regiao, posicao, container);

		free(j);
	}
	
}

int intro_div_admin(struct Div_admin *div_admin, 
					struct ContainerDivAdmin *containerdiv, 
					struct ContainerReg *container_regiao)
{
	int code_regiao = admin_cod_regiao(div_admin);
	int regiao_posicao = find_regiao(code_regiao, container_regiao, false);

	if (regiao_posicao <= 0)
	{
		char *j = int_to_string(4, code_regiao);
		printf("+ regiao %s inexistente\n", j);
		free(j);
		return 0;
	}

	int posicao = find_div_admin(admin_code(div_admin), containerdiv, true);
	struct Div_admin *admin_disco = malloc(size_admin());;
	if (posicao > 0) 
	{
		read_admin(posicao, admin_disco, containerdiv);

		if(code_regiao != admin_cod_regiao(admin_disco))
		{
			char *j = int_to_string(6, admin_code(div_admin));
			printf("+ divisao %s pertence a outra regiao\n", j);

			free(admin_disco);
			free(j);
			return 0;
		}
		else
		{
			char *j = int_to_string(6, admin_code(div_admin));
			printf("+ divisao %s alterada\n", j);
			Div_ligacoes(div_admin, admin_disco);

			free(admin_disco);
			free(j);
		}
			
	}

	else
	{
		reset_ligacoes(div_admin);

		char *j = int_to_string(6, admin_code(div_admin));
		printf("+ divisao %s criada\n", j);
		posicao = -(posicao);

		struct Regiao *regiao = malloc(size_regiao()); 
		read_regiao(regiao_posicao, regiao, container_regiao);

		regiao_new_ligacao(admin_code(div_admin), regiao);
		write_regiao(regiao, regiao_posicao, container_regiao);

		free(regiao);
		free(j);
	}
	
	write_admin(div_admin, posicao, containerdiv);
	return 0;
}

int cmpf (const void * a, const void * b)
{
	int A = *(int*)a;
	int B = *(int*)b;

	if (A < B)
        return -1;

	else if (A > B)
        return 1;

	else
        return  0;
}

int * sortint(int n, int tosort[n])
{
	qsort(tosort, n, sizeof(int), cmpf);

	return tosort;

}

void print_admin(int code, struct ContainerDivAdmin *containerdiv, 
				 struct ContainerReg *container_regiao) 
{
	int posicao_div = find_div_admin(code, containerdiv, false);

	if (posicao_div <= 0)
	{
		char *j = int_to_string(6, code);
		printf("+ divisao %s inexistente\n", int_to_string(6, code));
		free(j);
	}

	else
	{
		struct Div_admin *admin = malloc(size_admin());
		struct Regiao *regiao = malloc(size_regiao());
		
		read_admin(posicao_div, admin, containerdiv);

		int posicao_reg = find_regiao(admin_cod_regiao(admin), container_regiao, false);
		read_regiao(posicao_reg, regiao, container_regiao);

		char *j = int_to_string(4, admin_cod_regiao(admin));
		int * ligacoes_sorted;

		printf("VD %s\n", int_to_string(6, admin_code(admin)));
		printf("%s\n", admin_nome(admin));
		printf("%s (%s)\n", j, regiao_nome(regiao));
		printf("%d %d\n", admin_coordx(admin), admin_coordy(admin));
		printf("%d\n", admin_habitantes(admin));
		
		ligacoes_sorted = sortint(admin_num_ligacoes(admin), admin_ligacoes(admin));

		for(int i = 0; i < admin_num_ligacoes(admin); i++)
			printf("%s", int_to_string(6,ligacoes_sorted[i]));

		printf("\n");

		free(admin);
		free(regiao);
		free(j);
	}
}