#ifndef FUNCIONES_H
#define FUNCIONES_H

// Límites de la OMS CAMBIAR POR VALORES REALES DE LA OMS
#define LIMITE_CO2 400.0
#define LIMITE_SO2 20.0
#define LIMITE_NO2 25.0
#define LIMITE_PM25 15.0


typedef struct {
    int id_zona;

    // Factores climáticos actuales
    float temperatura;
    float velocidad_viento;
    float humedad;

    // Niveles de contaminación actuales
    float co2;
    float so2;
    float no2;
    float pm2_5;

    //Promedios históricos
    float co2_hist;
    float so2_hist;
    float no2_hist;
    float pm25_hist;
} ZonaUrbana;

// Prototipos de funciones
void inicializarZonas(ZonaUrbana *ciudad);
void cargarPromediosHistoricos(ZonaUrbana *ciudad);
int menu();
float validarFloatRango(float a, float b);

#endif