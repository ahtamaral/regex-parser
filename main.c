#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Comprimento mínimo de string de regExp. Equivale a 1 caracter e 1 pula-linha.
#define MINIMUM_EXP_STR_SIZE    2

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

    RegExp *basicExp = new_char(basicExpChar);

    return basicExp;
}

RegExp *parse_estrela(char starExpString[BUFFER_SIZE])
{
    // printf("parse_estrela() recebeu: %s\n", starExpString);

    RegExp *basicExp = parse_basico(starExpString[0]);

    RegExp *starExp = new_star(basicExp);

    return starExp;
}

RegExp* parse_concat(char ConcatExpString[BUFFER_SIZE])
{
    // printf("parse_concat() recebeu: %s\n", ConcatExpString);

    int concatExpLen = strlen(ConcatExpString);

    // Condição de contorno: Expressão de concat tem tamanho mínimo.
    // Ação: Retorna um RegExp de caracter direto.

    if (concatExpLen <= MINIMUM_EXP_STR_SIZE)
    {
        RegExp* returnedExp = new_char(ConcatExpString[0]);
        return returnedExp;
    }

    int curIdx = 0;
    char nextChar = ConcatExpString[curIdx + 1];

    char leftChild[BUFFER_SIZE];
    char rightChild[BUFFER_SIZE];

    strncpy(leftChild, ConcatExpString, curIdx+1);

    int starOffset = 0;

    RegExp* child1;

    // Caso 1: Expressão de concat é apenas caracater seguido de asterisco.
    // Ação: Monta ponteiro para StarExp e retorna logo aqui.
    // printf("concatExpLen: %d\n", concatExpLen);
    if ( isStarOperator(nextChar) && concatExpLen <= MINIMUM_EXP_STR_SIZE + 1) {
            // printf("C\n");
            child1 = parse_estrela(leftChild);
            return child1;
    }

    if (isStarOperator(nextChar)) {
        starOffset = 1;
    }

    strncpy(rightChild, ConcatExpString + (curIdx+1) + starOffset, concatExpLen);

    RegExp* child2;

    // printf("Left child: %s\n", leftChild);
    // printf("Right child: %s\n", rightChild);

    // Caso Geral - Filho da esquerda: Se foi detectada uma estrela após o primeiro caracter.
    // O filho da esquerda é uma StarExp. Senão, é uma basicExp, um caracter.
    if (starOffset == 1) {
        child1 = parse_estrela(leftChild);
    } else {
        child1 = parse_basico(leftChild[0]);
    }

    // Caso geral - Filho da direita: Se tem comprimento maior que o mínimo (i.e, tem mais de um caractere), o filho esquerdo é uma ConcatExp.
    // Senão, é apenas um caracter, sendo uma basicExp.

    // printf("Right child len: %d\n", strlen(rightChild));
    if (strlen(rightChild) > MINIMUM_EXP_STR_SIZE) {
        child2 = parse_concat(rightChild);
    } else {
        child2 = parse_basico(rightChild[0]);
    }

    RegExp* concatExp = new_concat(child1, child2);

    return concatExp;
}

RegExp* parse_uniao(char UnionExpString[BUFFER_SIZE])
{
    // printf("parse_uniao() recebeu: %s\n", UnionExpString);

    int regExpLen = strlen(UnionExpString);
    
    int curIdx = 0;
    char curChar = UnionExpString[curIdx];

    char leftChild[BUFFER_SIZE];
    char rightChild[BUFFER_SIZE];

    int unionOperatorFound = 0;

    while (isNewLine(curChar) != 1) {
        
        if (isUnionOperator(curChar) == 1) {
            unionOperatorFound = 1;
            break;
        }

        curIdx += 1;
        curChar = UnionExpString[curIdx];
    }

    RegExp *child1;
    RegExp *child2;

    RegExp *returnedExp;

    if (unionOperatorFound == 0)
    {
        // printf("Left child: %s", UnionExpString);
        // printf("Right child: EMPTY\n");

        child1 = parse_concat(UnionExpString);
        child2 = new_empty(); // Será? Se não achar operador '|' nunca entra aqui, mas vou deixar por precaução.
        returnedExp = parse_concat(UnionExpString);
    }
    else {
        int unionIdx = curIdx;

        strncpy(leftChild, UnionExpString, unionIdx);
        strncpy(rightChild, UnionExpString + unionIdx + 1, regExpLen);

        // printf("Left child: %s\n", leftChild);
        // printf("Right child: %s\n", rightChild);

        child1 = parse_concat(leftChild);
        child2 = parse_uniao(rightChild);
        returnedExp = new_union(child1, child2);
    }  

    return returnedExp;
}

RegExp * parse_regexp(char regExpString[BUFFER_SIZE])
{
    // printf("parse_regexp() recebeu: %s\n", regExpString);

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