#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"


void leerCadena(char *cadena, int n){
    int len;
    fgets(cadena,n,stdin);
    len = strlen(cadena) - 1;
    cadena[len]='\0';
}


float validarFloatRango(float a, float b){
    float n, aux;
    do
    {
        aux = scanf("%f",&n);
        while ((getchar())!='\n');
        if (aux != 1 || n < a || n > b){
            printf("Error: valor ingresado incorrecto\n");
            printf(">> ");
        }
    } while (aux != 1 || n < a || n > b);
    return n;
}

int menu(){
    printf("\n==================================================\n");
        printf("SISTEMA DE GESTION Y PREDICCION DE CONTAMINACION\n");
        printf("==================================================\n");
        printf("1. Ingresar datos actuales del clima y contaminacion\n");
        printf("2. Evaluar situacion actual de las zonas\n");
        printf("3. Predecir niveles (24h) y generar alertas\n");
        printf("4. Exportar reporte final\n");
        printf("5. Salir del sistema\n");
        printf("==================================================\n");
        printf(">> ");
    int opc = validarFloatRango(1,5);
    return opc;
}




// Inicializar En 0, para evitar errores
void inicializarZonas(ZonaUrbana *ciudad) {
    for (int i = 0; i < 5; i++) {
        ciudad[i].id_zona = i + 1;
        ciudad[i].co2_hist = 0.0;
        ciudad[i].so2_hist = 0.0;
        ciudad[i].no2_hist = 0.0;
        ciudad[i].pm25_hist = 0.0;
    }
}

// Carga de Datos y Análisis Histórico
void cargarPromediosHistoricos(ZonaUrbana *ciudad) {
    FILE *archivo = fopen("historial_zonas.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se encontro el archivo 'historial_zonas.txt'.\n");
        return; 
    }

    int id_zona, dia;
    float co2, so2, no2, pm25;
    int dias_registrados[5] = {0, 0, 0, 0, 0}; // Contador de días leídos por zona e inicializados.

    // Lectura del archivo línea por línea
    while (fscanf(archivo, "%d %d %f %f %f %f", &id_zona, &dia, &co2, &so2, &no2, &pm25) != EOF) {
        if (id_zona >= 1 && id_zona <= 5) {
            int indice = id_zona - 1; // La zona 1 corresponde al índice 0
            
            // Sumamos los registros
            ciudad[indice].co2_hist += co2;
            ciudad[indice].so2_hist += so2;
            ciudad[indice].no2_hist += no2;
            ciudad[indice].pm25_hist += pm25;
            
            dias_registrados[indice]++;
        }
    }
    fclose(archivo); // Cerramos el archivo de la memoria

    // Cálculo de promedios y comparación
    printf("\n--- RESULTADOS HISTORICOS RECIENTES ---\n");
    for (int i = 0; i < 5; i++) {
        if (dias_registrados[i] > 0) {
            // Se divide la suma total para los días registrados (idealmente 30)
            ciudad[i].co2_hist /= dias_registrados[i];
            ciudad[i].so2_hist /= dias_registrados[i];
            ciudad[i].no2_hist /= dias_registrados[i];
            ciudad[i].pm25_hist /= dias_registrados[i];

            printf("Zona %d | Promedios -> CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n",
                   ciudad[i].id_zona, ciudad[i].co2_hist, ciudad[i].so2_hist, ciudad[i].no2_hist, ciudad[i].pm25_hist);

            // Verificación con los límites de la OMS
            if(ciudad[i].pm25_hist > LIMITE_PM25) {
                printf("  -> [ALERTA HISTORICA] La Zona %d supera el limite de PM2.5 de la OMS.\n", ciudad[i].id_zona);
            }
        } else {
            printf("Zona %d | Sin datos historicos registrados.\n", i + 1);
        }
    }
    printf("---------------------------------------\n");
}