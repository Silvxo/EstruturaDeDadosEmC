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

struct Sensor sensor[6];
int unidadesDeSensores = 6;
int numeroDeLeituras = 0;

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

void analiseDosDados(int fumaca, int volume, char *ip, char *id, char *local){
    if(volume >= 1000 || fumaca == 1){
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);

        printf("Data: %02d/%02d/%d ", tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
        printf("Hora: %02d:%02d\n", (tm_info->tm_hour - 3), tm_info->tm_min);
        if(volume >= 1000){
            printf("Gases em níveis perigosos!\n");
        }
        if(fumaca == 1){
            printf("Fumaça detectada!\n");
        }
        printf("IP: %s, ID: %s, Local: %s, Volume de gases inflamáveis: %d PPM\n", ip, id, local, volume);
        printf("IP: %s, ID: %s, Local: %s, Presença de fumaça: %d\n", ip, id, local, fumaca);
        printf("\n");
    }
}

int main()
{
    srand(time(NULL));
    setbuf(stdout, NULL);
    printf("Testing...\n");
    
    for(numeroDeLeituras = 0; numeroDeLeituras < unidadesDeSensores; numeroDeLeituras++){
        gerarDadosDeIdentificacao(numeroDeLeituras, sensor[numeroDeLeituras].IP, sensor[numeroDeLeituras].ID, sensor[numeroDeLeituras].local);
        fflush(stdout);
    }
    
    while(1){
        fflush(stdout);
        numeroDeLeituras = 0;
        for(numeroDeLeituras = 0; numeroDeLeituras < unidadesDeSensores; numeroDeLeituras++){
            gerarDadosDeLeitura(numeroDeLeituras, &sensor[numeroDeLeituras].volumeGasesInflamaveis, &sensor[numeroDeLeituras].presencaFumaca);
        }
        numeroDeLeituras = 0;
        for(numeroDeLeituras = 0; numeroDeLeituras < unidadesDeSensores; numeroDeLeituras++){
            analiseDosDados(sensor[numeroDeLeituras].presencaFumaca, sensor[numeroDeLeituras].volumeGasesInflamaveis, sensor[numeroDeLeituras].IP, sensor[numeroDeLeituras].ID, sensor[numeroDeLeituras].local);            
        }
        sleep(10);
    }
    
    return 0;
}
