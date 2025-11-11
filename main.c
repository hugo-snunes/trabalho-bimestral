#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "cidades.h"

int main(int argc, char const *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    const char *arquivo = (argc > 1) ? argv[1] : "teste03.txt";

    Estrada *e = getEstrada(arquivo);

    if (e == NULL) {
        printf("\n❌ Erro ao carregar dados da estrada.\n");
        return 1;
    }

    printf("\n----------------------------\n");
    printf("\tInformações da Estrada\n");
    printf("----------------------------\n\n");
    printf("Tamanho total (T): %d\n", e->T);
    printf("Número de cidades (N): %d\n\n", e->N);

    printf("\tLista de Cidades:\n\n");
    Cidade *atual = e->Inicio;
    while (atual != NULL) {
        printf("Cidade: %-20s | Posição: %d\n", atual->Nome, atual->Posicao);
        atual = atual->Proximo;
    }

    double menorViz = calcularMenorVizinhanca(arquivo);
    char *cidadeMenor = cidadeMenorVizinhanca(arquivo);

    printf("\n----------------------------\n");
    printf("Menor vizinhança: %.2f km\n", menorViz);
    printf("Cidade com menor vizinhança: %s\n", cidadeMenor);
    printf("----------------------------\n\n");

    free(cidadeMenor);

    // Liberação de memória
    Cidade *tmp = e->Inicio;
    while (tmp != NULL) {
        Cidade *prox = tmp->Proximo;
        free(tmp);
        tmp = prox;
    }
    free(e);

    return 0;
}
