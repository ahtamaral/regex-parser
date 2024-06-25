=============================
= Linguagens Formais 2024.1 =
=============================

# Trabalho 2: Analisador sintático de expressões regulares.

Autores:
	- Artur Amaral (DRE: 122032113)
	- Tales Moreira (DRE:)
Uso:

```bash
gcc main.c -Wall -Wextra --std=c99 -o parser
parser < regex.txt
```

## Explicação do algoitmo implementado

O algoritmo implementado se baseia em um "recursive descent parser", que tenta fazer a análise sintática de maneira top-down,
 descendo em chamadas recursivas para os elementos não-terminais, até o ponto em que atingem-se os terminais, construindo a árvore
na "volta" da recursão.

A expressão inteira é lida. O operador de menor precedência é UNIÃO ('|'). Então o primeiro '|' da expressão é lido, de forma que
tudo à sua esquerda é parseado como uma concatenação e tudo à sua direita é parseado como união novamente, até o ponto em que a 
string da direita não possuir mais '|', de modo que será parseada como concatenação.

OBS: Casos que ainda não funcionam:

1 - Estrelas consecutivas.
2 - Parêntesis no geral.
3 - Concatenação antes de união (Ex: ab|c)
