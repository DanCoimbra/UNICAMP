#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char *msg;
} TK;

typedef struct binary_tree {
	int id;
	TK *token;
	struct binary_tree *esq;
	struct binary_tree *dir;
	struct binary_tree *pai;
} BT;

TK *set_TK(int id, char *msg);
TK *copy_TK(TK *original);

BT *set_BT(TK *token, BT *esq, BT *dir, BT *pai);
BT *get_TK(BT *tree, int id);
void free_BT(BT *tree);

BT *inserir_BT(BT *tree, TK *new_token);
void remover_BT(BT *tree, int id);

BT *prev_BT(BT *tree);
BT *next_BT(BT *tree);
BT *max_BT(BT *tree);
BT *min_BT(BT *tree);
int cmp_BT(BT *tree1, BT *tree2);

BT *get_triade_BT(BT *saco, int id_auth);
TK *merge_triade(BT *triade, int id_auth);
int remover_triade_BT(BT *saco, BT *triade);
void print_msg(BT *saco);