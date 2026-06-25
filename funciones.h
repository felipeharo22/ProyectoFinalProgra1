#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#ifndef FUNCIONES_H
#define FUNCIONES_H

// Límites de la OMS CAMBIAR POR VALORES REALES DE LA OMS
// Limites de salud reales (Exposicion 24h)
#define LIMITE_CO2 1000.0 // Estándar de calidad (ASHRAE)
#define LIMITE_SO2 40.0   // OMS 2021: 40 ug/m3
#define LIMITE_NO2 25.0   // OMS 2021: 25 ug/m3
#define LIMITE_PM25 15.0  // OMS 2021: 15 ug/m3


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

    // Predicciones a 24 horas
    float pred_co2;
    float pred_so2;
    float pred_no2;
    float pred_pm25;
} ZonaUrbana;

// Prototipos de funciones
void inicializarZonas(ZonaUrbana *ciudad);
void cargarPromediosHistoricos(ZonaUrbana *ciudad);
int menu();
float validarFloatRango(float a, float b);
void predecirNivelesFuturos(ZonaUrbana *ciudad);
void generarAlertasYRecomendaciones(ZonaUrbana *ciudad);
void exportarReporteFinal(ZonaUrbana *ciudad);
void ingresarDatosActuales(ZonaUrbana *ciudad);
void evaluarContaminacionActual(ZonaUrbana *ciudad);

#endif