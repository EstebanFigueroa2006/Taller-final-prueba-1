#include <stdio.h>
#include "funciones.h"

int main(int argc, char *argv[]) {
    
    int opc1, opc2;
    
    printf("========================================================================\n");
    printf("   SISTEMA INTEGRAL DE GESTION Y PREDICCION                           \n");
    printf("   DE CONTAMINACION DEL AIRE EN ZONAS URBANAS                         \n");
    printf("========================================================================\n");
    printf("   Desarrollado para monitoreo ambiental y salud publica              \n");
    printf("   Basado en limites de calidad del aire de la OMS                    \n");
    printf("========================================================================\n");
    
    // Verificar que haya al menos 5 zonas registradas (requisito de la consigna)
    verificarMinimoCincoZonas();
    
    do{
        opc1 = menuPrincipal();
        
        switch (opc1)
        {
        case 1:
            registrarZona();
            break;
            
        case 2:{
            Zona zonas[MAX_ZONAS]; 
            listarZonas(zonas);
            break;
        }
        
        case 3:
            editarZona();
            break;
            
        case 4:
            eliminarZona();
            break;
        
        case 5:
            registrarMedicion();
            break;
            
        case 6:{
            Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];  
            listarMediciones(mediciones);
            break;
        }
        
        case 7:{
            Zona zonas[MAX_ZONAS];
            int numZonas = obtenerZonas(zonas);
            
            if(numZonas == 0){
                printf("\nNo hay zonas registradas\n");
                break;
            }
            
            listarZonas(zonas);
            printf("\nSeleccione el numero de zona (0-%d): ", numZonas - 1);
            int idx = leerEnteroConRango(0, numZonas - 1);
            listarMedicionesPorZona(zonas[idx].id);
            break;
        }
        
        case 8:
            editarMedicionDeZona();
            break;
            
        case 9:
            eliminarMedicionDeZona();
            break;
        
        case 10:
            calcularPromediosHistoricos();
            break;
            
        case 11:
            compararConLimitesOMS();
            break;
            
        case 12:
            predecirNivelesFuturos();
            break;
            
        case 13:
            generarAlertas();
            break;
            
        case 14:{
            Alerta alertas[MAX_ALERTAS];
            listarAlertas(alertas);
            break;
        }
        
        case 15:
            exportarReporte();
            break;
            
        case 16:
            exportarReporteCSV();
            break;
            
        case 17:
            visualizarReportes();
            break;
            
        case 18:
            printf("\n========================================================================\n");
            printf("   Gracias por usar el Sistema de Gestion de Contaminacion del Aire    \n");
            printf("   Cuidemos nuestro medio ambiente y salud publica                      \n");
            printf("========================================================================\n");
            return 0;
            
        default:
            printf("\nOpcion no valida\n"); 
            break;
        }
        
        printf("\n¿Desea realizar otra operacion? (1=Si / 2=No): ");
        opc2 = leerEnteroConRango(1, 2);
        
    }while(opc2 == 1);
    
    printf("\n========================================================================\n");
    printf("   Gracias por usar el Sistema de Gestion de Contaminacion del Aire    \n");
    printf("   Cuidemos nuestro medio ambiente y salud publica                      \n");
    printf("========================================================================\n");

    return 0;
}
