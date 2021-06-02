typedef struct doubly_linked_list {
	int val;
	struct doubly_linked_list *prev;
	struct doubly_linked_list *next;
} DLL;

typedef struct paciente {
	int id;
	char *nome;
	char *prioridade;
	DLL *atendimentos;
	struct paciente *prev;
	struct paciente *next;
} pac;

typedef struct deque {
	pac *ini;
	pac *fim;
} dq;

DLL *criar_DLL(void);
void free_DLL(DLL *dll);
DLL *copiar_DLL(DLL *prev, DLL *dll);

pac *criar_pac(int id);
void free_pac(pac *paciente);
pac *copiar_pac_isolado(pac *paciente);
pac *copiar_pac_isolado_id(pac *paciente, int id);

dq *criar_deque(void);
void free_deque(dq *deque);

void insere_inicio(dq *deque, pac *paciente);
void insere_fim(dq *deque, pac *paciente);
void remove_inicio(dq *deque);
void remove_fim(dq *deque);