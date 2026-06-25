#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include <string.h>

int main() {
    ZonaUrbana ciudad[5]; // Se declaran las zonas requeridas
    int opc1 = 0;
    int opc2 = 0;
    
    // NUEVA BANDERA LÓGICA (0 = No ingresados, 1 = Ingresados)
    int datos_ingresados = 0; 

    // Preparamos la memoria con esta funcion
    inicializarZonas(ciudad);

    // Cargamos el historial al iniciar el programa
    printf("Iniciando Sistema de Monitoreo...\n");
    cargarPromediosHistoricos(ciudad);

    do {
        opc1 = menu();

        switch(opc1) {
            case 1:
                ingresarDatosActuales(ciudad);
                // Activamos la bandera porque el usuario ya hizo su trabajo
                datos_ingresados = 1; 
                break;
            case 2:
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Primero debe ingresar los datos actuales (Opcion 1).\n");
                } else {
                    evaluarContaminacionActual(ciudad);
                }
                break;
            case 3:
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Primero debe ingresar los datos actuales (Opcion 1).\n");
                } else {
                    predecirNivelesFuturos(ciudad);
                    generarAlertasYRecomendaciones(ciudad);
                }
                break;
            case 4:
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Primero debe ingresar los datos actuales (Opcion 1).\n");
                } else {
                    exportarReporteFinal(ciudad);
                }
                break;
            case 5:
                printf("\nCerrando el sistema...\n");
                break;
            default:
                printf("\nError: Opcion no valida.\n");
        }
        
        // Solo pregunta si desea continuar si el usuario no eligió Salir (5)
        if (opc1 != 5) {
            printf("\n¿Desea ingresar otra opcion? 1.Si 2.No\n");
            printf(">> ");
            opc2 = (int)validarFloatRango(1, 2);
            
            if (opc2 == 2) {
                printf("\nCerrando el sistema...\n");
                opc1 = 5; // Forzamos la salida del bucle principal
            }
        }

    } while(opc1 != 5);

    return 0;
}