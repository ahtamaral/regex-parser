#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct parserState
{
    char buffer[BUFFER_SIZE];
    char curChar;
    char nextChar;
    int curIndex;
} ParserState;


// ------------------------------------------------
// Código proposto no enunciado.

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

static RegExp *parse_regexp(char buffer[BUFFER_SIZE]);
static RegExp *parse_uniao(char buffer[BUFFER_SIZE]);
static RegExp *parse_concat(char buffer[BUFFER_SIZE]);
static RegExp *parse_estrela(char buffer[BUFFER_SIZE]);
static RegExp *parse_basico(char buffer[BUFFER_SIZE]);

// ------------------------------------------------

void printTreePreOrder(RegExp *tree, int level);

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
        printf("\n\n"); 
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

unsigned short isStarOperator (char c){
    if (c == '*')
        return 1;
    return 0;
}

unsigned short isUnionOperator (char c){
    if (c == '|')
        return 1;
    return 0;
}

unsigned short isNewLine (char c){
    if (c == '\n')
        return 1;
    return 0;
}

unsigned short isEndOfFile(char c){
    if (c == '\0')
        return 1;
    return 0;
}

unsigned short isOpenParethesis(char c){
    if (c == '(')
        return 1;
    return 0;
}

unsigned short isCloseParethesis(char c){
    if (c == ')')
        return 1;
    return 0;
}

unsigned short isSpecialCharacter(char c) {
    // Se qualquer um desses retornos for verdadeiro, o caracter é especial (OR lógico).
    return isStarOperator(c) ||
            isUnionOperator(c) ||
            isNewLine(c) ||
            isEndOfFile(c) ||
            isOpenParethesis(c) ||
            isCloseParethesis(c);
}

unsigned short isChar(char c){
    if (isSpecialCharacter(c) == 0)
        return 1;
    return 0;
}

RegExp* parse_concat(char buffer[BUFFER_SIZE])
{
    printf("parse_concat(): %s\n", buffer);
}

RegExp* parse_uniao(char UnionExpString[BUFFER_SIZE])
{
    printf("parse_uniao(): %s\n", UnionExpString);

    int regExpLen = strlen(UnionExpString);
    
    int curIdx = 0;
    char curChar = UnionExpString[curIdx];

    char leftChild[BUFFER_SIZE];
    char rightChild[BUFFER_SIZE];

    int unionOperatorFound = 0;

    while (isNewLine(curChar) != 1){
        
        if (isUnionOperator(curChar) == 1) {
            unionOperatorFound = 1;
            break;
        }

        curIdx += 1;
        curChar = UnionExpString[curIdx];
    }

    if (unionOperatorFound == 0)
    {
        parse_concat(UnionExpString);
    }
    else {
        int unionIdx = curIdx;

        strncpy(leftChild, UnionExpString, unionIdx);
        strncpy(rightChild, UnionExpString + unionIdx + 1, regExpLen);

        printf("Left child: %s\n", leftChild);
        printf("Right child: %s\n", rightChild);

        parse_concat(leftChild);
        parse_regexp(rightChild);
    }

}

RegExp * parse_regexp(char regExpString[BUFFER_SIZE])
{

    printf("parse_regexp(): %s\n", regExpString);

    parse_uniao(regExpString);
}

/*
    parse_regexp recebe uma string que É uma união.

    abc|def|ghi

    é dividida em "abc" e "def|ghi".
    O da esquerda vai para um parse_concat, equanto o da direita vai para um parse_union.

    Se não acha um '|', o parse_union manda a string toda pro parse_concat.

 */

// ab | c

int main(int argc, char *argv[]) {

    char buffer[BUFFER_SIZE];

    while(fgets(buffer, BUFFER_SIZE, stdin)){
        printf("RegExp: %s\n\n", buffer);

        parse_regexp(buffer);

    }

    return 0;
}

// Caso de teste 1: abc** <---> a(b(c**))
    
    // printf("RegExp: abc**\n\n");

    // RegExp* c = new_char('c');
    
    // RegExp* star1 = new_star(c);
    // RegExp* star2 = new_star(star1);
    
    // RegExp* b = new_char('b');
    // RegExp* concat1 = new_concat(b, star2);
    // RegExp* a = new_char('a');
    // RegExp* concat2 = new_concat(a, concat1);

    // RegExp* root = concat2;

    // printTreePreOrder(root, 0);

    // printf("\n");

    // // Caso 2: (ab||c)

    // printf("RegExp: (ab||c)\n\n");
    // RegExp * a1 = new_char('a');
    // RegExp * b1 = new_char('b');

    // RegExp * concat = new_concat(a1, b1);

    // RegExp * c1 = new_char('c');
    // RegExp * empty = new_empty();

    // RegExp * union1 = new_union(c1, empty);

    // RegExp * union2 = new_union(concat, union1);

    // RegExp * root1 = union2;

    // printTreePreOrder(root1, 0);

    // // Caso 3: abc

    // printf("RegExp: abc\n\n");

    // RegExp* b2 = new_char('b');
    // RegExp* c2 = new_char('c');
    // RegExp* concat11 = new_concat(b2, c2);


    // RegExp* a2 = new_char('a');
    // RegExp* concat22 = new_concat(a2, concat11);

    // RegExp* root2 = concat22;

    // // printTree(root);
    // printTreePreOrder(root2, 0);