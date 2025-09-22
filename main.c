#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "processo.h"

int main(int argc, char const *argv[]){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Processo * pro = lerDados("TJDFT_amostra.csv");
    int tamanho = ContarLinha("TJDFT_amostra.csv"); 
    int pro_antigo = antigo(pro, tamanho);


    printf("----------------------------\n\n");

    //exibir informações básicas dos processos
    for (int i = 0; i < tamanho; i++) {
        printf("%d\t%s\t%s\n", pro[i].id_processo, pro[i].sigla_grau, pro[i].procedimento);
    }
    printf("\n----------------------------\n\n");
    
    //exibir cumprimento Meta1
    double cumprimento_meta1 = calcularMeta1(pro, tamanho);
    printf("\n\tRelatório de Metas\n");
    printf("Cumprimento da Meta 1: %.2f%%\n", cumprimento_meta1);
    printf("\n--------------------------\n\n");
    
    //exibir contagens de processos por tipo de causa
    printf("\tContagem por Causa\n\n");
    printf("Violência Doméstica: %d\n", contarPorFlag(pro, tamanho, 1));
    printf("Feminicídio: %d\n", contarPorFlag(pro, tamanho, 2));
    printf("Ambiental: %d\n", contarPorFlag(pro, tamanho, 3));
    printf("Indígena: %d\n", contarPorFlag(pro, tamanho, 4));
    printf("Quilombolas: %d\n", contarPorFlag(pro, tamanho, 5));
    printf("Infância: %d\n", contarPorFlag(pro, tamanho, 6));
    
    //exibir o id do processo com a data de recebimento mais antiga
    printf("\n----------------------------\n\n");
    printf("\tProcesso mais antigo\n\n");
    printf("Id do processo com a data mais antiga: %d", pro_antigo); 
    
    
    
    //exibir a diferença de dias entre a data de recebimento e data resolvido
    printf("\n----------------------------\n\n");
    printf("\tDiferença em dias entre recebimento e resolução\n\n");

    for (int i = 0; i < tamanho; i++) {
        int dias = diasEntreDatas(pro[i]);
        
        if (dias >= 0) {
            printf("Processo %d: %d dias\n", pro[i].id_processo, dias);
        } else {
            printf("Processo %d: datas inválidas\n", pro[i].id_processo);
        }
    }

       printf("\n----------------------------\n\n");
    printf("\tTeste de busca por id_ultimo_oj\n\n");
    int exemplo = pro[0].id_processo;
    int ultimoOJ = buscarUltimoOJ(pro, tamanho, exemplo);
    if (ultimoOJ != -1)
        printf("Processo %d tem id_ultimo_oj = %d\n", exemplo, ultimoOJ);
    else
        printf("Processo %d não encontrado\n", exemplo);

    printf("\n----------------------------\n\n");
    gerarCSVJulgados(pro, tamanho, "processos_julgados.csv");

    free(pro);
    return 0;
}