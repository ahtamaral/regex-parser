#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define QUEUE_MAX_SIZE 1024


enum RegExpTag {
    TAG_EMPTY,
    TAG_CHAR,
    TAG_STAR,
    TAG_CONCAT,
    TAG_UNION,
};
struct RegExp {
    char tag;
    union {
        struct {
            char c;
        } ch;
        struct {
            struct RegExp *filho;
        } un;
        struct {
            struct RegExp *filho1;
            struct RegExp *filho2;
        } bin;
    } u;
};
typedef struct RegExp RegExp;



RegExp *new_empty();
RegExp *new_char(char c);
RegExp *new_star(RegExp *filho);
RegExp *new_concat(RegExp *filho1, RegExp *filho2);
RegExp *new_union(RegExp *filho1, RegExp *filho2);

void printTree(RegExp *tree);


static RegExp *parse_regexp();
static RegExp *parse_uniao();
static RegExp *parse_concat();
static RegExp *parse_estrela();
static RegExp *parse_basico();


RegExp *new_empty(){
    RegExp* re = (RegExp*)malloc(sizeof(RegExp));
    if (re == NULL){
        exit(1);
    }
    re->tag = TAG_EMPTY;
    return re;
}


RegExp *new_char(char c){
    RegExp* re = (RegExp*)malloc(sizeof(RegExp));
    if (re == NULL){
        exit(1);
    }
    re->tag = TAG_CHAR;

    re->u.ch.c = c;

    return re; 
}


RegExp *new_star(RegExp *filho){
    RegExp* re = (RegExp*)malloc(sizeof(RegExp));
    if (re == NULL){
        exit(1);
    }
    re->tag = TAG_STAR;

    re->u.un.filho = filho;

    return re; 
}

RegExp *new_concat(RegExp *filho1, RegExp *filho2){
    RegExp* re = (RegExp*)malloc(sizeof(RegExp));
    if (re == NULL){
        exit(1);
    }
    re->tag = TAG_CONCAT;

    re->u.bin.filho1 = filho1;
    re->u.bin.filho2 = filho2;

    return re; 
}

RegExp *new_union(RegExp *filho1, RegExp *filho2){
    RegExp* re = (RegExp*)malloc(sizeof(RegExp));
    if (re == NULL){
        exit(1);
    }
    re->tag = TAG_UNION;

    re->u.bin.filho1 = filho1;
    re->u.bin.filho2 = filho2;

    return re; 
}

// Percorre a árvore em pré-ordem, seguindo o exemplo de output do enunciado
void printTreePreOrder(RegExp* root, int level) {
    
    if (root == NULL) {
        printf("\n"); 
        return;
    }

    // Identa output de acordo com o nível atual da árvore.
    for (int j = 0; j < level; j++) {
        printf("  ");
    }

    if (root->tag == TAG_CONCAT) {
        printf("CONCAT\n");
        printTreePreOrder(root->u.bin.filho1, level + 1);
        printTreePreOrder(root->u.bin.filho2, level + 1);
    } else if (root->tag == TAG_UNION) {
        printf("UNION\n");
        printTreePreOrder(root->u.bin.filho1, level + 1);
        printTreePreOrder(root->u.bin.filho2, level + 1);
    } else if (root->tag == TAG_STAR) {
        printf("STAR\n");
        printTreePreOrder(root->u.un.filho, level + 1);
    } else if (root->tag == TAG_CHAR) {
        printf("CHAR %c\n", root->u.ch.c);
    } else if (root->tag == TAG_EMPTY) {
        printf("EMPTY\n");
    }
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    while(fgets(buffer, BUFFER_SIZE, stdin)){
        //printf("%s", buffer);
    } 

    // Caso de teste 1: abc** <---> a(b(c**))
    
    printf("RegExp: abc**\n\n");

    RegExp* c = new_char('c');
    
    RegExp* star1 = new_star(c);
    RegExp* star2 = new_star(star1);
    
    RegExp* b = new_char('b');
    RegExp* concat1 = new_concat(b, star2);
    RegExp* a = new_char('a');
    RegExp* concat2 = new_concat(a, concat1);

    RegExp* root = concat2;

    printTreePreOrder(root, 0);

    printf("\n");

    // Caso 2: (ab||c)

    printf("RegExp: (ab||c)\n\n");
    RegExp * a1 = new_char('a');
    RegExp * b1 = new_char('b');

    RegExp * concat = new_concat(a1, b1);

    RegExp * c1 = new_char('c');
    RegExp * empty = new_empty();

    RegExp * union1 = new_union(c1, empty);

    RegExp * union2 = new_union(concat, union1);

    RegExp * root1 = union2;

    printTreePreOrder(root1, 0);

    // Caso 3: abc

    printf("RegExp: abc\n\n");

    RegExp* b2 = new_char('b');
    RegExp* c2 = new_char('c');
    RegExp* concat11 = new_concat(b2, c2);


    RegExp* a2 = new_char('a');
    RegExp* concat22 = new_concat(a2, concat11);

    RegExp* root2 = concat22;

    // printTree(root);
    printTreePreOrder(root2, 0);

    return 0;
}