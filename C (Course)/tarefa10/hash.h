#define KEYSIZE 26
#define TABLESIZE 43891

typedef unsigned long long int ULL;

typedef struct _hlist {
	char key[KEYSIZE];
	struct _hlist *next;
} hlist;

typedef struct {
	hlist *keylist[TABLESIZE];
} htable;

hlist *cria_hlist(char *key, hlist *next);

htable *cria_htable();

void free_hlist(hlist *hashlist);

void free_htable(htable *hashtable);

void insere_htable(htable *hashtable, char *key);

int hash(char *key);