#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"
#include <string.h>

int main() {
    ZonaUrbana ciudad[5]; // Se declaran las zonas requeridas
    int opc1 = 0;
       
    // NUEVA BANDERA LÓGICA (0 = No ingresados, 1 = Ingresados)
    int datos_ingresados = 0;
    int predicciones_actualizadas = 0;

    // Preparamos la memoria con esta funcion
    inicializarZonas(ciudad);

    // Cargamos el historial al iniciar el programa
    printf("Iniciando Sistema de Monitoreo...\n");
    cargarPromediosHistoricos(ciudad);
    
    // CAMBIO AQUÍ: Intentar cargar persistencia al iniciar
    if (cargarEstadoSistema(ciudad)) {
        printf("\n[i] Se han restaurado los datos actuales y predicciones del ultimo estado guardado.\n");
        datos_ingresados = 1;
        predicciones_actualizadas = 1;// Si se restaura con éxito, ambos están al día
    } else {
        printf("\n[i] No se encontraron respaldos previos. Inicie ingresando datos (Opcion 1).\n");
    }

    do {
        opc1 = menu();
        switch(opc1) {
            case 1:
                ingresarDatosActuales(ciudad);
                guardarEstadoSistema(ciudad);
                datos_ingresados = 1;
                predicciones_actualizadas = 0; 
                break;
            case 2: // <-- NUEVO CASE DE EDICION
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Primero debe ingresar los datos iniciales (Opcion 1).\n");
                } else {
                    editarNivelesZona(ciudad);
                    guardarEstadoSistema(ciudad); // Guardamos tras editar
                    predicciones_actualizadas = 0; // Se invalidan las predicciones viejas
                }
                break;
            case 3: // Antiguo case 2
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Faltan datos (Opcion 1).\n");
                } else {
                    evaluarContaminacionActual(ciudad);
                }
                break;
            case 4: // Antiguo case 3
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Faltan datos (Opcion 1).\n");
                } else {
                    predecirNivelesFuturos(ciudad);
                    generarAlertasYRecomendaciones(ciudad);
                    guardarEstadoSistema(ciudad);
                    predicciones_actualizadas = 1;
                }
                break;
            case 5: // Antiguo case 4
                if (datos_ingresados == 0) {
                    printf("\n[!] ACCESO DENEGADO: Faltan datos.\n");
                } else if (predicciones_actualizadas == 0) {
                    printf("\n[!] ACCESO DENEGADO: Debe calcular nuevas predicciones (Opcion 4) antes de exportar.\n");
                } else {
                    exportarReporteFinal(ciudad);
                }
                break;
            case 6: // Salida
                printf("\nCerrando el sistema...\n");
                break;
            default:
                printf("\nError: Opcion no valida.\n");
        }
        
        /* Solo pregunta si desea continuar si el usuario no eligió Salir (5)
        if (opc1 != 5) {
            printf("\n¿Desea ingresar otra opcion? 1.Si 2.No\n");
            printf(">> ");
            opc2 = (int)validarFloatRango(1, 2);
            
            if (opc2 == 2) {
                printf("\nCerrando el sistema...\n");
                opc1 = 5; // Forzamos la salida del bucle principal
            }
        }*/

    } while(opc1 != 5);

    return 0;
}