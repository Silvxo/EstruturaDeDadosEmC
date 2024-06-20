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
FILE *arquivo;

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
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    if(volume >= 1000 || fumaca == 1){
        printf("Data: %02d/%02d/%d Hora: %02d:%02d - ", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
                (tm_info->tm_hour - 3), tm_info->tm_min);
        fprintf(arquivo, "Data: %02d/%02d/%d Hora: %02d:%02d - ", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
                (tm_info->tm_hour - 3), tm_info->tm_min);
        
        if(volume >= 1000){
            printf("Alarme ativo: Gases inflamáveis - ID do dispositivo: %s Presença de gases inflamáveis: %d PPM\n", id, volume);
            fprintf(arquivo, "Alarme ativo: Gases inflamáveis - ID do dispositivo: %s Presença de gases inflamáveis: %d PPM\n", id, volume);

        }
        if(fumaca == 1){
            printf( "Alarme ativo: Fumaça - ID do dispositivo: %s Presença de fumaça\n", id);
            fprintf(arquivo, "Alarme ativo: Fumaça - ID do dispositivo: %s Presença de fumaça\n", id);
        }
    } else {

    }
}

void identificandoSensores(struct Sensor *sensores){
    for(int i = 0; i < unidadesDeSensores; i++){
        gerarDadosDeIdentificacao(i, sensores[i].IP, sensores[i].ID, sensores[i].local);
    }
}

int main(void)
{
    srand(time(NULL));
    setbuf(stdout, NULL);
    printf("Testing monitor...\n");

    sensores = (struct Sensor *) malloc(unidadesDeSensores * sizeof(struct Sensor));
    if (sensores == NULL) {
        perror("Erro de alocação de memória");
        return 0;
    }

    identificandoSensores(sensores);

    while(1){
        arquivo = fopen("alarmes.txt", "a");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            return 0;
        }
        fflush(stdout);
        for(int i = 0; i < unidadesDeSensores; i++){
            gerarDadosDeLeitura(i, &sensores[i].volumeGasesInflamaveis, &sensores[i].presencaFumaca);
        }
        for(int i = 0; i < unidadesDeSensores; i++){
            analiseDosDados(sensores[i].presencaFumaca, sensores[i].volumeGasesInflamaveis, sensores[i].IP, sensores[i].ID, sensores[i].local, arquivo);
        }
        fclose(arquivo);
        sleep(10);
    }
    free(sensores);
    return 0;
}
