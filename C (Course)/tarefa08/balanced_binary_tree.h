#include <stdio.h>
#include <stdlib.h>

enum Cor {red, black};

typedef struct balanced_binary_tree {
	int val;
	int reps;
	struct balanced_binary_tree *esq;
	struct balanced_binary_tree *dir;
	enum Cor cor;
} BBT;

BBT *set_BBT(int val, BBT *esq, BBT *dir, enum Cor cor);
BBT *get_BBT(BBT *tree, int val);
void free_BBT(BBT *tree);

BBT *get_BST(BBT *tree, int val);
BBT* inserir_BBT(BBT *tree, int val);
BBT* inserir_BBT_rec(BBT *tree, int val);

BBT *mirror_left_BBT(BBT *tree);
BBT *mirror_right_BBT(BBT *tree);
void raise_color_BBT(BBT *tree);
 
int count_BBT(BBT *tree, int val);
int legalize_BBT(BBT *tree);

int isred_BBT(BBT *tree);
int isblack_BBT(BBT *tree);