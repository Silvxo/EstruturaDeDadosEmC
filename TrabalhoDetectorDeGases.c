#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct Sensor 
{
    char IP[15]; // endereço IP
    char ID[30]; // string que identifica o dispositivo
    char local[30]; // string que indica o local onde o dispositivo está (ex.: sala 100n, prédio C08)
    int volumeGasesInflamaveis; // leitura do volume total de gases  inflamáveis em partes por milhão (PPM)
    int presencaFumaca; // leitura da presença de fumaça (0/1)
};

int unidadesDeSensores = 6;
struct Sensor *sensores; // Array dinâmico de sensores

void gerarDadosDeIdentificacao(int sensor, char *ip, char *id, char *lugar){
    sprintf(ip, "10.1.1.%d", (sensor + 48));
    sprintf(id, "%d", sensor);
    sprintf(lugar, "Laboratório %d", sensor);

    printf("Generated fake id %d\n", sensor);
}

void gerarDadosDeLeitura(int sensor, int *volumeDeGasesInflamaveis, int *presencaDeFumaca){
    *volumeDeGasesInflamaveis = (rand () % 1200) + 0;
    *presencaDeFumaca = (rand () % 2) + 0;
}

void analiseDosDados(int fumaca, int volume, char *ip, char *id, char *local, FILE *arquivo){
    if(volume >= 1000 || fumaca == 1){
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        
        fprintf(arquivo, "Data: %02d/%02d/%d Hora: %02d:%02d - ", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
                (tm_info->tm_hour - 3), tm_info->tm_min);
        
        if(volume >= 1000){
            fprintf(arquivo, "Alarme ativo: Gases inflamáveis - ID do dispositivo: %s Presença de gases inflamáveis: %d PPM\n", id, volume);
        }
        if(fumaca == 1){
            fprintf(arquivo, "Alarme ativo: Fumaça - ID do dispositivo: %s Presença de fumaça\n", id);
        }
    }
}

int main()
{
    srand(time(NULL));
    FILE *arquivo;
    arquivo = fopen("alarmes.txt", "a"); // Abre o arquivo para escrita no final (append)
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    sensores = (struct Sensor *) malloc(unidadesDeSensores * sizeof(struct Sensor));
    if (sensores == NULL) {
        perror("Erro de alocação de memória");
        return 1;
    }

    printf("Testing...\n");

    // Gerar dados de identificação para cada sensor
    for(int i = 0; i < unidadesDeSensores; i++){
        gerarDadosDeIdentificacao(i, sensores[i].IP, sensores[i].ID, sensores[i].local);
    }

    // Loop infinito para simular leituras contínuas
    while(1){
        // Gerar dados de leitura para cada sensor
        for(int i = 0; i < unidadesDeSensores; i++){
            gerarDadosDeLeitura(i, &sensores[i].volumeGasesInflamaveis, &sensores[i].presencaFumaca);
        }

        // Analisar os dados e registrar alarmes no arquivo
        for(int i = 0; i < unidadesDeSensores; i++){
            analiseDosDados(sensores[i].presencaFumaca, sensores[i].volumeGasesInflamaveis, sensores[i].IP, sensores[i].ID, sensores[i].local, arquivo);
        }

        // Dormir por 10 segundos entre cada iteração
        sleep(10);
    }

    // Fechar o arquivo ao final do programa (não será alcançado no loop infinito)
    fclose(arquivo);

    // Liberar memória alocada
    free(sensores);

    return 0;
}
