#include <stdbool.h>

struct Regiao *regiao;

struct Div_admin *admin1;

struct Div_admin *admin2;

struct ContainerReg *container_regiao;

struct ContainerDivAdmin *container_div;

struct Div_admin ** admin;


/////////////// GETTERS //////////////////////////////
int size_container_regiao();

int size_container_admin();

int size_admin();

int regiao_code(struct Regiao *regiao);

char * regiao_nome(struct Regiao *regiao);

int regiao_divisao(struct Regiao *regiao, int i);

int regiao_num_divisoes(struct Regiao *regiao);



int size_regiao();

long tempo_admin(struct Div_admin *admin);

unsigned short admin_cod_regiao(struct Div_admin *admin1);

unsigned int admin_code(struct Div_admin *admin1);

char * admin_nome(struct Div_admin *admin1);

int admin_coordx(struct Div_admin *admin1);

int admin_coordy(struct Div_admin *admin1);

int admin_habitantes(struct Div_admin *admin1);

short admin_num_ligacoes(struct Div_admin *admin1);

int admin_ligacao(struct Div_admin *admin1, int i);

int * admin_ligacoes(struct Div_admin *admin1);

/////////////////////////SETTERS//////////////////////////////////

void admin_new_tempo(struct Div_admin * admin, int t);

void Div_ligacoes(struct Div_admin * admin1, struct Div_admin * admin2);

void regiao_new_nome(struct Regiao *regiao, char *nome);

void reset_ligacoes(struct Div_admin * admin1);

void new_regiao(struct Regiao *regiao, int code, char *nome);

void regiao_new_ligacao(int cod_div_admin, struct Regiao * regiao);

void bridge(struct Div_admin *admin1, int code);

void ID(struct Div_admin *admin1, char *str_i, char * str);

void initialization(struct Div_admin ** admin, int code_start, int i);

void time_adjust(struct Div_admin *admin1, struct Div_admin *admin2, long tempo);

void time_total(struct Div_admin *admin1);

//--//

int cmpfunc (const void * a, const void * b);

struct Div_admin ** sort(int n, struct Div_admin * tosort[n]);

char * int_to_string(int size, int value);

void print_admin(int code, struct ContainerDivAdmin *containerdiv, 
				 struct ContainerReg *container_regiao);

void open_files(char * name, int i, struct ContainerReg *container_regiao, 
				 struct ContainerDivAdmin *container_div);

void close_files(struct ContainerDivAdmin *container_div, struct ContainerReg *container_regiao);





int find_regiao(int cod_regiao, struct ContainerReg *container_regiao, bool intro);

void intro_regiao(unsigned short code_regiao, char *nome, struct ContainerReg *container_regiao);

int intro_div_admin(struct Div_admin *admin1, struct ContainerDivAdmin *container_div, struct ContainerReg *container_regiao);

int write_bridge(int cod_admin1, int cod_admin2, struct ContainerDivAdmin *container_div);




void read_admin(int posicao, struct Div_admin *admin1, 
				struct ContainerDivAdmin *container_div);

void read_regiao(int posicao, struct Regiao *regiao, 
				 struct ContainerReg *container_div);

int find_div_admin(int cod_div_admin , struct ContainerDivAdmin *container_div,
				   bool intro);

int Tempo_Viagem(int code_start, int code_end, struct ContainerDivAdmin *container_div,
				 struct ContainerReg *container_regiao);
