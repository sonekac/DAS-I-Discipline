#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Interface.h"

int main()
{
	char typeop[3] = ""; 
	char *names[] = {"RegiaoHashtable.dat", "DivisaoHashtable.dat",
					  "Regiao.dat", "Divisao.dat", NULL};
	int ch;			
	int c;
	int i;

	struct ContainerReg *container_regiao = malloc(size_container_regiao());
	struct ContainerDivAdmin *container_div = malloc(size_container_admin());

	memset(container_regiao, 0, size_container_regiao());
	memset(container_div, 0, size_container_admin());

	for(int j = 0; names[j]!=NULL; j++)
		open_files(names[j], j, container_regiao, container_div);

	char str[48];
	char str2[40];
	char str_i[7];


	while(scanf(" %[^\n]", str) != EOF)
	{
		for(int j = 0; j < 2; j++)
			typeop[j] = str[j];

		if (strcmp(typeop, "IR") == 0)
		{
			int j = 3;
			for(; j < 7; j++)
				str_i[j - 3] = str[j];

			for(j = j + 1; j < 48 || str[j] != '\0'; j++)
				str2[j - 8] = str[j];

 			intro_regiao(atoi(str_i), str2, container_regiao);
		}	
		
		else if (strcmp(typeop, "ID") == 0)
		{
			struct Div_admin * div_admin = malloc(size_admin());
			ID(div_admin, str_i, str);
			intro_div_admin(div_admin, container_div, container_regiao);  //ID does everything that was here before
			free(div_admin);
		}
		
		else if (strcmp(typeop, "IL") == 0)
		{	
			int j = 3;
			for(; str[j] != (char) 32; j++)
				str_i[j - 3] = str[j];

			c = atoi(str_i);
			int k = j + 1;

			for(j = k; str[j] != '\0'; j++)
				str_i[j - k] = str[j];

			i = atoi(str_i);
			write_bridge(c, i, container_div);
		}
		
		else if (strcmp(typeop, "VD") == 0)
		{
			for(int j = 3; j < 9; j++)
				str_i[j - 3] = str[j];

			print_admin(atoi(str_i), container_div, container_regiao);
		}

		else if (strcmp(typeop, "TV") == 0)
		{
			int j = 3;
			for(; j < 9; j++)
				str_i[j - 3] = str[j];

			c = atoi(str_i);

			for(j = j + 1; j < 16; j++)
				str_i[j - 10] = str[j];

			Tempo_Viagem(c, atoi(str_i), container_div, container_regiao);
		}

		else if(strcmp(typeop, "XX\n"))
		{
			close_files(container_div, container_regiao);
			return 0;
		}
		
		for(int j = 0; j < 7; j++)
			str_i[j] = '\0';

		while ((ch = getchar()) != EOF && ch != '\n'); 
		fflush(stdout);
	}
	close_files(container_div, container_regiao);
	return 0;
}
	

