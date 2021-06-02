#define MAX 600

typedef struct {
	int size;
	int **mtx_dst;
	double **mtx_pos;
} grafo;

grafo *cria_grafo();

void free_grafo(grafo *gr);

void insere_nodulo(grafo *gr, double x, double y, char *tipo);