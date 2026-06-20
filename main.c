#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main() {
    ZonaUrbana ciudad[5]; // Se declaran las zonas requeridas
    int opc1=0;
    int opc2=0;

    // Preparamos la memoria con esta fucnion
    inicializarZonas(ciudad);

    // 2. Cargamos el historial al iniciar el programa
    printf("Iniciando Sistema de Monitoreo...\n");
    cargarPromediosHistoricos(ciudad);

    
    
    
    do {
        
        opc1=menu();
        

        switch(opc1) {
            case 1:
                printf("\n[Modulo en construccion: Ingresar Datos]\n");
                // ingresarDatosActuales(ciudad);
                break;
            case 2:
                printf("\n[Modulo en construccion: Evaluar Actual]\n");
                // evaluarContaminacionActual(ciudad);
                break;
            case 3:
                printf("\n[Modulo en construccion: Predecir y Alertas]\n");
                // predecirNivelesFuturos(ciudad);
                // generarAlertasYRecomendaciones(ciudad);
                break;
            case 4:
                printf("\n[Modulo en construccion: Exportar Reporte]\n");
                // exportarReporteFinal(ciudad);
                break;
            case 5:
                printf("\nCerrando el sistema...\n");
                break;
            default:
                printf("\nOpcion no valida. Por favor, intente de nuevo.\n");
        }
        printf("Desea ingresar otra opcion? 1.Si 2.No\n");
        opc2=validarFloatRango(1,2);
    } while(opc2 ==1);

    return 0;
}