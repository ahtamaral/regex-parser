#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Comprimento mínimo de string de regExp. Equivale a 1 caracter e 1 pula-linha.
#define MINIMUM_EXP_STR_SIZE    2

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
static RegExp *parse_basico(char buffer);

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
        printf("Erro: encontrado no nulo na arvore.\n\n"); 
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

RegExp *parse_basico(char basicExpChar)
{
    // printf("parse_basico() recebeu: %c\n", basicExpChar);
    return new_empty();

}

RegExp *parse_estrela(char starExpString[BUFFER_SIZE])
{
    // printf("parse_estrela() recebeu: %s\n", starExpString);
    return new_empty();
}

RegExp* parse_concat(char ConcatExpString[BUFFER_SIZE])
{
    printf("parse_concat() recebeu: %s -> TERMINAL\n", ConcatExpString);

    return new_empty();
}

RegExp* parse_uniao(char UnionExpString[BUFFER_SIZE])
{
    printf("parse_uniao() recebeu: %s\n", UnionExpString);

    int regExpStrLen = strlen(UnionExpString);

    int curIdx = 0;
    char curChar = UnionExpString[curIdx];

    char leftChildStr[BUFFER_SIZE];
    char rightChildStr[BUFFER_SIZE];

    RegExp *leftChild;
    RegExp *rightChild;

    RegExp *returnedExp;

    int unionOperatorFound = 0;

    while (curIdx < regExpStrLen)
    {
        printf("curChar: %c\n", curChar);

        if (isUnionOperator(curChar)) 
        {
            unionOperatorFound = 1;

            printf("\nUnion operator found at %d\n", curIdx);

            strncpy(leftChildStr, UnionExpString, curIdx);
            strncpy(rightChildStr, UnionExpString + curIdx + 1, regExpStrLen);
            
            printf("Dividing: %s and %s\n", leftChildStr, rightChildStr);


            leftChild = parse_concat(leftChildStr);
            rightChild = parse_uniao(rightChildStr);

            returnedExp = new_union(leftChild, rightChild);
        }
        curIdx += 1;
        curChar = UnionExpString[curIdx];
    }
    // Chegamos ao fim da expressão.

    // Caso nenhum operador de união tenha sido achado, joga a string inteira para um parse_concat().
    if (unionOperatorFound == 0) 
    {
        strncpy(leftChildStr, UnionExpString, regExpStrLen);
        returnedExp = parse_concat(leftChildStr);
    }

    return returnedExp;
}

RegExp * parse_regexp(char regExpString[BUFFER_SIZE])
{
    printf("parse_regexp() recebeu: %s\n", regExpString);

    RegExp *exp = parse_uniao(regExpString);
    return exp;
}

int main() {

    char buffer[BUFFER_SIZE];

    while(fgets(buffer, BUFFER_SIZE, stdin)){
        printf("RegExp: %s\n", buffer);

        RegExp *exp = parse_regexp(buffer);

        printTreePreOrder(exp, 0);

        printf("----------------------------------\n\n");
    }

    return 0;
}