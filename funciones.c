#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        
        // Limpiamos el historial
        ciudad[i].co2_hist = 0.0;
        ciudad[i].so2_hist = 0.0;
        ciudad[i].no2_hist = 0.0;
        ciudad[i].pm25_hist = 0.0;

        // Limpiamos los datos actuales
        ciudad[i].temperatura = 0.0;
        ciudad[i].velocidad_viento = 0.0;
        ciudad[i].humedad = 0.0;
        
        ciudad[i].co2 = 0.0;
        ciudad[i].so2 = 0.0;
        ciudad[i].no2 = 0.0;
        ciudad[i].pm2_5 = 0.0;

        // Limpiamos las predicciones
        ciudad[i].pred_co2 = 0.0;
        ciudad[i].pred_so2 = 0.0;
        ciudad[i].pred_no2 = 0.0;
        ciudad[i].pred_pm25 = 0.0;
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

void predecirNivelesFuturos(ZonaUrbana *ciudad) {
    printf("\n--- CALCULANDO PREDICCIONES PARA LAS PROXIMAS 24 HORAS ---\n");

    for (int i = 0; i < 5; i++) {
        // 1. Cálculo Base: Promedio Ponderado
        // Le damos 60% de importancia a la contaminación de hoy, y 40% al historial de 30 días.
        float peso_actual = 0.60;
        float peso_hist = 0.40;

        // Validación de seguridad por si no hay historial previo
        if (ciudad[i].pm25_hist == 0) { 
            peso_actual = 1.0;
            peso_hist = 0.0;
        }

        // Aplicamos la fórmula matemática para los 4 contaminantes
        ciudad[i].pred_co2 = (ciudad[i].co2 * peso_actual) + (ciudad[i].co2_hist * peso_hist);
        ciudad[i].pred_so2 = (ciudad[i].so2 * peso_actual) + (ciudad[i].so2_hist * peso_hist);
        ciudad[i].pred_no2 = (ciudad[i].no2 * peso_actual) + (ciudad[i].no2_hist * peso_hist);
        ciudad[i].pred_pm25 = (ciudad[i].pm2_5 * peso_actual) + (ciudad[i].pm25_hist * peso_hist);

        // 2. Modificadores Climáticos (El motor de predicción ambiental)
        
        // Factor Viento: Si el viento supera los 15 km/h, actúa como limpiador natural.
        if (ciudad[i].velocidad_viento > 15.0) {
            ciudad[i].pred_co2 *= 0.90;  // Disminuye la predicción un 10%
            ciudad[i].pred_so2 *= 0.90;
            ciudad[i].pred_no2 *= 0.85;  // NO2 y PM2.5 son más ligeros, se dispersan un 15%
            ciudad[i].pred_pm25 *= 0.85; 
        } 
        // Factor Estancamiento: Viento menor a 5 km/h concentra los gases
        else if (ciudad[i].velocidad_viento < 5.0) {
            ciudad[i].pred_co2 *= 1.05;  // Aumenta un 5%
            ciudad[i].pred_pm25 *= 1.15; // Las partículas sólidas aumentan un 15%
        }

        // Factor Temperatura: Más de 28 grados favorece la formación de ozono y retiene gases pesados
        if (ciudad[i].temperatura > 28.0) {
            ciudad[i].pred_so2 *= 1.10; // Aumenta un 10%
            ciudad[i].pred_no2 *= 1.10;
        }

        // Factor Humedad/Lluvia: Humedad mayor al 80% indica alta probabilidad de lluvia, la cual "lava" el PM2.5
        if (ciudad[i].humedad > 80.0) {
            ciudad[i].pred_pm25 *= 0.80; // Reduce un 20%
        }

        printf("Zona %d: Prediccion calculada exitosamente.\n", ciudad[i].id_zona);
    }
    printf("----------------------------------------------------------\n");
}

void generarAlertasYRecomendaciones(ZonaUrbana *ciudad) {
    printf("\n--- ALERTAS Y RECOMENDACIONES PREVENTIVAS (24 HORAS) ---\n");
    int alertas_generadas = 0; // Un contador para saber si la ciudad está a salvo

    for (int i = 0; i < 5; i++) {
        int zona_con_alerta = 0; // Bandera para imprimir el encabezado de la zona solo una vez

        // Evaluamos PM2.5 (Afecta vías respiratorias - Riesgo Social/Salud)
        if (ciudad[i].pred_pm25 > LIMITE_PM25) {
            if (zona_con_alerta == 0) {
                printf("\n[!] ALERTA ROJA EN ZONA %d:\n", ciudad[i].id_zona);
                zona_con_alerta = 1;
            }
            printf("  - PM2.5 critico (%.2f). RECOMENDACION: Suspender actividades fisicas al aire libre y obligar el uso de mascarillas en poblacion vulnerable.\n", ciudad[i].pred_pm25);
        }

        // Evaluamos NO2 y SO2 (Gases de combustión - Riesgo Vehicular/Industrial)
        if (ciudad[i].pred_no2 > LIMITE_NO2 || ciudad[i].pred_so2 > LIMITE_SO2) {
            if (zona_con_alerta == 0) {
                printf("\n[!] ALERTA ROJA EN ZONA %d:\n", ciudad[i].id_zona);
                zona_con_alerta = 1;
            }
            printf("  - Gases toxicos elevados (NO2/SO2). RECOMENDACION: Implementar restriccion vehicular severa (Pico y Placa ambiental) y pausar operaciones industriales locales.\n");
        }

        // Evaluamos CO2 (Efecto invernadero - Riesgo Global/Ambiental)
        if (ciudad[i].pred_co2 > LIMITE_CO2) {
            if (zona_con_alerta == 0) {
                printf("\n[!] ALERTA ROJA EN ZONA %d:\n", ciudad[i].id_zona);
                zona_con_alerta = 1;
            }
            printf("  - CO2 excede la norma (%.2f). RECOMENDACION: Promover teletrabajo masivo para reducir la huella de carbono de la zona en las proximas 24h.\n", ciudad[i].pred_co2);
        }

        // Si la bandera cambió a 1, significa que hubo al menos un problema en esta zona
        if (zona_con_alerta == 1) {
            alertas_generadas++;
        }
    }

    // Si el ciclo termina y el contador sigue en 0, damos buenas noticias
    if (alertas_generadas == 0) {
        printf("\n✅ Todo en orden. No se preve superar los limites de la OMS en ninguna zona.\n");
    }
    printf("--------------------------------------------------------\n");
}

void exportarReporteFinal(ZonaUrbana *ciudad) {
    // Abrimos un archivo nuevo en modo escritura ("w" de write)
    FILE *archivo = fopen("reporte_contaminacion.txt", "w");
    
    // Validación de seguridad por si el disco duro está lleno o sin permisos
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo crear el archivo de reporte.\n");
        return;
    }

    // Comenzamos a escribir en el archivo usando fprintf
    fprintf(archivo, "========================================================\n");
    fprintf(archivo, "      REPORTE OFICIAL DE CONTAMINACION URBANA\n");
    fprintf(archivo, "========================================================\n\n");

    for (int i = 0; i < 5; i++) {
        fprintf(archivo, "--- ZONA %d ---\n", ciudad[i].id_zona);
        
        fprintf(archivo, "1. PROMEDIOS HISTORICOS (Ultimos 30 dias):\n");
        fprintf(archivo, "   CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n\n", 
                ciudad[i].co2_hist, ciudad[i].so2_hist, ciudad[i].no2_hist, ciudad[i].pm25_hist);
        
        fprintf(archivo, "2. CONDICIONES ACTUALES INGRESADAS:\n");
        fprintf(archivo, "   Clima -> Temp: %.1f C | Viento: %.1f km/h | Humedad: %.1f%%\n", 
                ciudad[i].temperatura, ciudad[i].velocidad_viento, ciudad[i].humedad);
        fprintf(archivo, "   Gases -> CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n\n", 
                ciudad[i].co2, ciudad[i].so2, ciudad[i].no2, ciudad[i].pm2_5);
        
        fprintf(archivo, "3. PREDICCION CALCULADA (24 Horas):\n");
        fprintf(archivo, "   CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n", 
                ciudad[i].pred_co2, ciudad[i].pred_so2, ciudad[i].pred_no2, ciudad[i].pred_pm25);
        
        fprintf(archivo, "--------------------------------------------------------\n");
    }

    fprintf(archivo, "Fin del reporte generado por el Sistema de Gestion.\n");
    
    // Es obligatorio cerrar el archivo para que los cambios se guarden en el disco
    fclose(archivo);
    
    // Mensaje en consola para que el usuario sepa que funcionó
    printf("\n✅ Reporte exportado exitosamente en 'reporte_contaminacion.txt'.\n");
}

void ingresarDatosActuales(ZonaUrbana *ciudad) {
    printf("\n--- INGRESO DE DATOS ACTUALES ---\n");
    for (int i = 0; i < 5; i++) {
        printf("\n--- ZONA %d ---\n", ciudad[i].id_zona);
        
        printf("Temperatura (C): ");
        ciudad[i].temperatura = validarFloatRango(-10.0, 55.0);
        
        printf("Velocidad del viento (km/h): ");
        ciudad[i].velocidad_viento = validarFloatRango(0.0, 150.0);
        
        printf("Humedad (%%): ");
        ciudad[i].humedad = validarFloatRango(0.0, 100.0);

        printf("Nivel de CO2: ");
        ciudad[i].co2 = validarFloatRango(0.0, 5000.0);
        
        printf("Nivel de SO2: ");
        ciudad[i].so2 = validarFloatRango(0.0, 1000.0);
        
        printf("Nivel de NO2: ");
        ciudad[i].no2 = validarFloatRango(0.0, 1000.0);
        
        printf("Nivel de PM2.5: ");
        ciudad[i].pm2_5 = validarFloatRango(0.0, 1000.0);
    }
    printf("\n✅ Datos ingresados correctamente para todas las zonas.\n");
}

void evaluarContaminacionActual(ZonaUrbana *ciudad) {
    printf("\n--- EVALUACION DE CALIDAD DEL AIRE ACTUAL ---\n");
    for (int i = 0; i < 5; i++) {
        printf("Zona %d: ", ciudad[i].id_zona);
        
        if (ciudad[i].co2 > LIMITE_CO2 || ciudad[i].so2 > LIMITE_SO2 || 
            ciudad[i].no2 > LIMITE_NO2 || ciudad[i].pm2_5 > LIMITE_PM25) {
            printf("[!] ADVERTENCIA: Se superan los limites de la OMS.\n");
        } else {
            printf("[OK] Niveles dentro de los rangos aceptables.\n");
        }
    }
    printf("-----------------------------------------------\n");
}