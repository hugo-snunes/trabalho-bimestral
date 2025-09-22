#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "processo.h"

/*
v 1. o número de processos presentes na base de dados;

2. o id_ultimo_oj a partir de id_processo;
v 3. o id_processo do processo com dt_recebimento mais antigo;

v 4. quantos processos estão relacionadas a causas de violência doméstica;
v 5. quantos processos estão relacionadas a causas de feminicídio;
v 6. quantos processos estão relacionadas a causas ambientais;
v 7. quantos processos estão relacionadas a causas de quilombolas;
v 8. quantos processos estão relacionadas a causas indígenas;
v 9. quantos processos envolve causas relacionadas a infância e juventude; 

10. o número de dias entre dt_recebimento e dt_resolvido;

v 11. o percentual de cumprimento da meta 1; e
v 12. gerar um arquivo CSV com todos os processos julgados (mérito) na Meta 1
*/


//função para contar o numero de processos de uma flag específica

int contarPorFlag(Processo * processos, int tamanho, int tipo_flag){
    int contador = 0;
    for (int i = 0; i < tamanho; i++){
        switch (tipo_flag){
            case 1:
                if (processos[i].flag_violencia_domestica == 1) contador++;
                break;

            case 2:
                if (processos[i].flag_feminicidio == 1) contador++;
                break;

            case 3:
                if(processos[i].flag_ambiental == 1) contador++;
                break;

            case 4:
                if(processos[i].flag_indigenas == 1) contador ++;
                break;

            case 5:
                if(processos[i].flag_quilombolas == 1) contador++;
                break;

            case 6:
                if(processos[i].flag_infancia == 1) contador++;
                break;
            }
        }
    return contador;
}

//função para calcular o percentual de cumprimento Meta1

double calcularMeta1(Processo * processos, int tamanho){
    long long cnm1_total = 0;
    long long julgadom1_total = 0;
    long long desm1_total = 0;
    long long susm1_total = 0;

    for (int i = 0; i < tamanho; i++){
        cnm1_total += processos[i].cnm1;
        julgadom1_total += processos[i].julgadom1;
        desm1_total += processos[i].desm1;
        susm1_total += processos[i].susm1;
    }

    double denominador = (double)(cnm1_total + desm1_total - susm1_total);

    if (denominador == 0){
        printf("Aviso: Denominador 0. Nao foi possivel calcular\n");
        return 0.0;
    }
    return ((double)julgadom1_total / denominador) * 100.0;

}


int ContarLinha(char * arquivo){

    FILE *fp = fopen(arquivo, "r"); 
    if (fp == NULL){
        printf("ERRO: arquivo nao pode ser aberto!\n");
        exit(1); 
    }    
    
    // --- 1ª PASSAGEM: contar linhas ---
    int count = 0;
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp); // pula o cabeçalho
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        count++;
    }    
    rewind(fp); // volta ao início
    fclose(fp);
    
    return count; 
}    


Processo * lerDados(char * arquivo){
    
    printf("Arquivo a ser aberto: %s\n", arquivo); 
    FILE *fp = fopen(arquivo, "r"); 
    if (fp == NULL){
        printf("ERRO: arquivo nao pode ser aberto!\n");
        exit(1); 
    }
    
    int tamanho = ContarLinha(arquivo);
    printf("Encontradas %d linhas de dados\n", tamanho);
    
    Processo * x = malloc(tamanho * sizeof(Processo)); 
    memset(x, 0, tamanho * sizeof(Processo));
    
    char cabecalho[1024]; 
    fgets(cabecalho, sizeof(cabecalho), fp);
    
    int i = 0;
    char linha[1024];
    
    // Lê linha por linha e depois usa sscanf
    while (fgets(linha, sizeof(linha), fp) != NULL && i < tamanho) {
        // Remove quebras de linha
        linha[strcspn(linha, "\r\n")] = '\0';
        
        // Substitui campos vazios por "0"
        char temp_linha[1024];
        strcpy(temp_linha, linha);
        char *ptr = temp_linha;
        while (*ptr) {
            if (*ptr == ';' && (*(ptr+1) == ';' || *(ptr+1) == '\0')) {
                memmove(ptr + 2, ptr + 1, strlen(ptr + 1) + 1);
                *(ptr + 1) = '0';
            }
            ptr++;
        }
        
    // Usa sscanf para ler
    int result = sscanf(temp_linha, "%d;%[^;];%[^;];%[^;];%[^;];%[^;];%d;%d;%d;%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%[^;];%d;%d;%d;%d;%d;%d;%d;%d",
        &x[i].id_processo, 
        x[i].numero_sigilo, x[i].sigla_grau, x[i].procedimento, 
        x[i].ramo_justica, x[i].sigla_tribunal, &x[i].id_tribunal, 
        &x[i].recurso, &x[i].id_ultimo_oj, x[i].dt_recebimento, 
        &x[i].id_ultima_classe, &x[i].flag_violencia_domestica, 
        &x[i].flag_feminicidio, &x[i].flag_ambiental, &x[i].flag_quilombolas, 
        &x[i].flag_indigenas, &x[i].flag_infancia, &x[i].decisao, 
        x[i].dt_resolvido, &x[i].cnm1, &x[i].primeirasentm1, &x[i].baixm1, 
        &x[i].decm1, &x[i].mpum1, &x[i].julgadom1, &x[i].desm1, &x[i].susm1);
        
        if (result >= 20) {
            i++;
        }
    }
        
        fclose(fp);
        printf("Registros lidos com sucesso: %d\n", i);
        return x;
}

// Função para converter data no formato "YYYY-MM-DD" para time_t
time_t converterData(const char *data_str) {
    if (data_str == NULL || strlen(data_str) == 0) {
        return 0;
    }
    
    struct tm tm = {0};
    sscanf(data_str, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900; // Ano desde 1900
    tm.tm_mon -= 1;     // Mês de 0-11
    
    return mktime(&tm);
}


int antigo(Processo * processos, int tamanho) {
    if (tamanho == 0 || processos == NULL) {
        printf("Nenhum processo encontrado.\n");
        return -1;
    }

    int id_mais_antigo = processos[0].id_processo;
    time_t data_mais_antiga = converterData(processos[0].dt_recebimento);

    for (int i = 1; i < tamanho; i++) {
        time_t data_atual = converterData(processos[i].dt_recebimento);

        if (data_atual < data_mais_antiga && data_atual != 0) {
            data_mais_antiga = data_atual;
            id_mais_antigo = processos[i].id_processo;
        }
    }

    return id_mais_antigo;
}


// Função para calcular diferença em dias entre recebimento e resolvido
int diasEntreDatas(Processo p) {
    time_t data_receb = converterData(p.dt_recebimento);
    time_t data_res = converterData(p.dt_resolvido);

    if (data_receb == 0 || data_res == 0) {
        return -1; // retorno especial se alguma das datas for inválida
    }

    double diff = difftime(data_res, data_receb) / (60 * 60 * 24);
    return (int) diff;
}

// Função para Buscar id_ultimo_oj a partir do id_processo
int buscarUltimoOJ(Processo *processos, int tamanho, int id_processo) {
    for (int i = 0; i < tamanho; i++) {
        if (processos[i].id_processo == id_processo) {
            return processos[i].id_ultimo_oj;
        }
    }
    return -1; // não encontrado
}

// 12. Gerar CSV com processos julgados (mérito)
void gerarCSVJulgados(Processo *processos, int tamanho, char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "w");
    if (!fp) {
        printf("ERRO: não foi possível criar o arquivo %s\n", nomeArquivo);
        return;
    }

    // Cabeçalho
    fprintf(fp, "id_processo;numero_sigilo;sigla_grau;procedimento;ramo_justica;sigla_tribunal;id_tribunal;recurso;id_ultimo_oj;dt_recebimento;id_ultima_classe;flag_violencia_domestica;flag_feminicidio;flag_ambiental;flag_quilombolas;flag_indigenas;flag_infancia;decisao;dt_resolvido;cnm1;primeirasentm1;baixm1;decm1;mpum1;julgadom1;desm1;susm1\n");

    for (int i = 0; i < tamanho; i++) {
        if (processos[i].julgadom1 > 0) {
            fprintf(fp, "%d;%s;%s;%s;%s;%s;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d\n",
                processos[i].id_processo,
                processos[i].numero_sigilo,
                processos[i].sigla_grau,
                processos[i].procedimento,
                processos[i].ramo_justica,
                processos[i].sigla_tribunal,
                processos[i].id_tribunal,
                processos[i].recurso,
                processos[i].id_ultimo_oj,
                processos[i].dt_recebimento,
                processos[i].id_ultima_classe,
                processos[i].flag_violencia_domestica,
                processos[i].flag_feminicidio,
                processos[i].flag_ambiental,
                processos[i].flag_quilombolas,
                processos[i].flag_indigenas,
                processos[i].flag_infancia,
                processos[i].decisao,
                processos[i].dt_resolvido,
                processos[i].cnm1,
                processos[i].primeirasentm1,
                processos[i].baixm1,
                processos[i].decm1,
                processos[i].mpum1,
                processos[i].julgadom1,
                processos[i].desm1,
                processos[i].susm1
            );
        }
    }

    fclose(fp);
    printf("Arquivo %s gerado com sucesso!\n", nomeArquivo);
}
