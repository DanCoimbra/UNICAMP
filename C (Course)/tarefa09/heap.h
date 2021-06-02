typedef struct {
	char *nome;
	double nota;
	int *origem;
	int *destino;
} pessoa;

typedef struct {
	pessoa *atual;
	pessoa **itens;
	int qtd;
} heap;

void free_pessoa(pessoa *passageiro);

void free_heap(heap *fila);

pessoa *get_pessoa(void);

heap *cria_heap();

heap *insere_heap(heap *fila, pessoa *passageiro);

heap *sobe_heap(heap *fila, int index_alvo);

heap *remove_heap(heap *fila, char *nome);

heap *desce_heap(heap *fila, int topo);