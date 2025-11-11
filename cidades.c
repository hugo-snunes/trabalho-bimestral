#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

/*
RESTRIÇÕES 
    • 3 ≤ T ≤ 10^6 e 2 ≤ N ≤ 10^4
    • 0 < Xi < T, para 1 ≤ i ≤ N
    • Xi ≠ Xj, para todo par 1 ≤ i, j ≤ N.
*/

/*
OBSERVAÇÕES 
    - Caso ocorra erro na inicialização da estrada, getEstrada() deve retornar ponteiro NULL.
    - É importante construir casos de teste adicionais para validar sua solução.
*/

// Função auxiliar para criar uma cidade
Cidade *criarCidade(int posicao, const char *nome) {
    Cidade *nova = (Cidade *) malloc(sizeof(Cidade));
    if (!nova) return NULL;

    strcpy(nova->Nome, nome);
    nova->Posicao = posicao;
    nova->Proximo = NULL;
    return nova;
}

// Estrutura temporária usada na leitura e ordenação
typedef struct {
    int Posicao;
    char Nome[256];
} TempCidade;

// Função de comparação para qsort
int compararCidades(const void *a, const void *b) {
    const TempCidade *c1 = (const TempCidade *)a;
    const TempCidade *c2 = (const TempCidade *)b;
    return c1->Posicao - c2->Posicao;
}

// ==============================
// 1. getEstrada()
// ==============================
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) {
        printf("ERRO: não foi possível abrir o arquivo '%s'.\n", nomeArquivo);
        return NULL;
    }

    int T, N;
    if (fscanf(fp, "%d", &T) != 1 || fscanf(fp, "%d", &N) != 1) {
        printf("ERRO: formato incorreto na leitura de T ou N.\n");
        fclose(fp);
        return NULL;
    }

    // Verifica restrições
    if (T < 3 || T > 1000000 || N < 2 || N > 10000) {
        printf("ERRO: restrições de T ou N violadas.\n");
        fclose(fp);
        return NULL;
    }

    TempCidade *temp = malloc(sizeof(TempCidade) * N);
    if (!temp) {
        fclose(fp);
        return NULL;
    }

    // Lê as cidades
    for (int i = 0; i < N; i++) {
        if (fscanf(fp, "%d %255s", &temp[i].Posicao, temp[i].Nome) != 2) {
            printf("ERRO: leitura inválida na linha %d.\n", i + 3);
            free(temp);
            fclose(fp);
            return NULL;
        }

        // Verifica posição
        if (temp[i].Posicao <= 0 || temp[i].Posicao >= T) {
            printf("ERRO: posição inválida (%d) para cidade %s.\n", temp[i].Posicao, temp[i].Nome);
            free(temp);
            fclose(fp);
            return NULL;
        }

        // Verifica duplicata
        for (int j = 0; j < i; j++) {
            if (temp[i].Posicao == temp[j].Posicao) {
                printf("ERRO: posições duplicadas (%d) detectadas.\n", temp[i].Posicao);
                free(temp);
                fclose(fp);
                return NULL;
            }
        }
    }

    fclose(fp);

    // Ordena cidades pela posição
    qsort(temp, N, sizeof(TempCidade), compararCidades);

    Estrada *E = malloc(sizeof(Estrada));
    if (!E) {
        free(temp);
        return NULL;
    }

    E->T = T;
    E->N = N;
    E->Inicio = NULL;

    Cidade *anterior = NULL;
    for (int i = 0; i < N; i++) {
        Cidade *nova = criarCidade(temp[i].Posicao, temp[i].Nome);
        if (!nova) {
            free(temp);
            free(E);
            return NULL;
        }
        if (E->Inicio == NULL)
            E->Inicio = nova;
        else
            anterior->Proximo = nova;

        anterior = nova;
    }

    free(temp);
    return E;
}

// ==============================
// 2. calcularMenorVizinhanca()
// ==============================
double calcularMenorVizinhanca(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return -1.0;

    int T, N;
    if (fscanf(fp, "%d", &T) != 1 || fscanf(fp, "%d", &N) != 1) {
        fclose(fp);
        return -1.0;
    }

    TempCidade *temp = malloc(sizeof(TempCidade) * N);
    if (!temp) {
        fclose(fp);
        return -1.0;
    }

    for (int i = 0; i < N; i++)
        fscanf(fp, "%d %255s", &temp[i].Posicao, temp[i].Nome);
    fclose(fp);

    qsort(temp, N, sizeof(TempCidade), compararCidades);

    double menor = 1e9;
    for (int i = 0; i < N; i++) {
        double viz;
        if (i == 0)
            viz = (temp[i + 1].Posicao + temp[i].Posicao) / 2.0;
        else if (i == N - 1)
            viz = (double)T - (temp[i].Posicao + temp[i - 1].Posicao) / 2.0;
        else
            viz = (temp[i + 1].Posicao - temp[i - 1].Posicao) / 2.0;

        if (viz < menor)
            menor = viz;
    }

    free(temp);
    return menor;
}

// ==============================
// 3. cidadeMenorVizinhanca()
// ==============================
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return NULL;

    int T, N;
    if (fscanf(fp, "%d", &T) != 1 || fscanf(fp, "%d", &N) != 1) {
        fclose(fp);
        return NULL;
    }

    TempCidade *temp = malloc(sizeof(TempCidade) * N);
    if (!temp) {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < N; i++)
        fscanf(fp, "%d %255s", &temp[i].Posicao, temp[i].Nome);
    fclose(fp);

    qsort(temp, N, sizeof(TempCidade), compararCidades);

    double menor = 1e9;
    int indice = 0;
    for (int i = 0; i < N; i++) {
        double viz;
        if (i == 0)
            viz = (temp[i + 1].Posicao + temp[i].Posicao) / 2.0;
        else if (i == N - 1)
            viz = (double)T - (temp[i].Posicao + temp[i - 1].Posicao) / 2.0;
        else
            viz = (temp[i + 1].Posicao - temp[i - 1].Posicao) / 2.0;

        if (viz < menor) {
            menor = viz;
            indice = i;
        }
    }

    char *resultado = malloc(strlen(temp[indice].Nome) + 1);
    strcpy(resultado, temp[indice].Nome);

    free(temp);
    return resultado;
}
