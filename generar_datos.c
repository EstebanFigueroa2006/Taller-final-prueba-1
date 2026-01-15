#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main() {
    printf("================================================================================\n");
    printf("   GENERADOR AUTOMATICO DE DATOS DE PRUEBA\n");
    printf("================================================================================\n");
    printf("Este programa generara automaticamente:\n");
    printf("  - 5 zonas de monitoreo\n");
    printf("  - 50 mediciones (10 por zona)\n");
    printf("  - Todas dentro de los ultimos 30 dias\n");
    printf("================================================================================\n\n");
    
    // ============================================================================
    // GENERAR 5 ZONAS
    // ============================================================================
    
    Zona zonas[5];
    
    // ZONA 1: Centro Historico
    zonas[0].id = 1;
    strcpy(zonas[0].nombre, "Centro Historico");
    strcpy(zonas[0].ubicacion, "Plaza Mayor y alrededores");
    zonas[0].poblacion = 45000;
    strcpy(zonas[0].tipo, "Comercial");
    
    // ZONA 2: Parque Industrial Norte
    zonas[1].id = 2;
    strcpy(zonas[1].nombre, "Parque Industrial Norte");
    strcpy(zonas[1].ubicacion, "Carretera Norte Km 15");
    zonas[1].poblacion = 12000;
    strcpy(zonas[1].tipo, "Industrial");
    
    // ZONA 3: Barrio Los Pinos
    zonas[2].id = 3;
    strcpy(zonas[2].nombre, "Barrio Los Pinos");
    strcpy(zonas[2].ubicacion, "Sector Sur - Colonia Residencial");
    zonas[2].poblacion = 65000;
    strcpy(zonas[2].tipo, "Residencial");
    
    // ZONA 4: Avenida Principal
    zonas[3].id = 4;
    strcpy(zonas[3].nombre, "Avenida Principal");
    strcpy(zonas[3].ubicacion, "Corredor vial Este-Oeste");
    zonas[3].poblacion = 38000;
    strcpy(zonas[3].tipo, "Mixta");
    
    // ZONA 5: Complejo Industrial Sur
    zonas[4].id = 5;
    strcpy(zonas[4].nombre, "Complejo Industrial Sur");
    strcpy(zonas[4].ubicacion, "Zona Franca Industrial Sur Km 8");
    zonas[4].poblacion = 8500;
    strcpy(zonas[4].tipo, "Industrial");
    
    // Guardar zonas
    FILE *fzonas = fopen("zonas.dat", "wb");
    if(fzonas == NULL){
        printf("ERROR: No se pudo crear archivo zonas.dat\n");
        return 1;
    }
    fwrite(zonas, sizeof(Zona), 5, fzonas);
    fclose(fzonas);
    printf("[OK] 5 zonas generadas correctamente\n");
    
    // ============================================================================
    // GENERAR 50 MEDICIONES (10 POR ZONA)
    // ============================================================================
    
    Medicion mediciones[50];
    int idx = 0;
    
    // -------- ZONA 1: CENTRO HISTORICO (10 mediciones) --------
    // Medicion 1
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 15; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 28; mediciones[idx].clima.velocidadViento = 8; mediciones[idx].clima.humedad = 75;
    mediciones[idx].niveles.pm25 = 32; mediciones[idx].niveles.pm10 = 55; mediciones[idx].niveles.co2 = 1150;
    mediciones[idx].niveles.so2 = 38; mediciones[idx].niveles.no2 = 28;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Alta"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 18; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 2
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 18; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 30; mediciones[idx].clima.velocidadViento = 6; mediciones[idx].clima.humedad = 78;
    mediciones[idx].niveles.pm25 = 35; mediciones[idx].niveles.pm10 = 60; mediciones[idx].niveles.co2 = 1200;
    mediciones[idx].niveles.so2 = 42; mediciones[idx].niveles.no2 = 30;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Alta"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 8; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 3
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 21; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 27; mediciones[idx].clima.velocidadViento = 12; mediciones[idx].clima.humedad = 65;
    mediciones[idx].niveles.pm25 = 28; mediciones[idx].niveles.pm10 = 48; mediciones[idx].niveles.co2 = 1050;
    mediciones[idx].niveles.so2 = 35; mediciones[idx].niveles.no2 = 25;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Media"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 14; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 4
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 24; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 26; mediciones[idx].clima.velocidadViento = 15; mediciones[idx].clima.humedad = 60;
    mediciones[idx].niveles.pm25 = 22; mediciones[idx].niveles.pm10 = 42; mediciones[idx].niveles.co2 = 950;
    mediciones[idx].niveles.so2 = 30; mediciones[idx].niveles.no2 = 22;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Baja"); mediciones[idx].factoresSociales.esDiaFestivo = 1;
    mediciones[idx].factoresSociales.horaDelDia = 11; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 5
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 27; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 29; mediciones[idx].clima.velocidadViento = 7; mediciones[idx].clima.humedad = 72;
    mediciones[idx].niveles.pm25 = 33; mediciones[idx].niveles.pm10 = 58; mediciones[idx].niveles.co2 = 1180;
    mediciones[idx].niveles.so2 = 40; mediciones[idx].niveles.no2 = 29;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Alta"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 17; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 6
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 30; mediciones[idx].mes = 12; mediciones[idx].anio = 2025;
    mediciones[idx].clima.temperatura = 31; mediciones[idx].clima.velocidadViento = 5; mediciones[idx].clima.humedad = 80;
    mediciones[idx].niveles.pm25 = 38; mediciones[idx].niveles.pm10 = 65; mediciones[idx].niveles.co2 = 1250;
    mediciones[idx].niveles.so2 = 45; mediciones[idx].niveles.no2 = 32;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Alta"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 19; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 7
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 2; mediciones[idx].mes = 1; mediciones[idx].anio = 2026;
    mediciones[idx].clima.temperatura = 28; mediciones[idx].clima.velocidadViento = 10; mediciones[idx].clima.humedad = 68;
    mediciones[idx].niveles.pm25 = 30; mediciones[idx].niveles.pm10 = 52; mediciones[idx].niveles.co2 = 1100;
    mediciones[idx].niveles.so2 = 37; mediciones[idx].niveles.no2 = 27;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Media"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 12; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 8
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 5; mediciones[idx].mes = 1; mediciones[idx].anio = 2026;
    mediciones[idx].clima.temperatura = 29; mediciones[idx].clima.velocidadViento = 9; mediciones[idx].clima.humedad = 70;
    mediciones[idx].niveles.pm25 = 31; mediciones[idx].niveles.pm10 = 54; mediciones[idx].niveles.co2 = 1120;
    mediciones[idx].niveles.so2 = 39; mediciones[idx].niveles.no2 = 28;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Media"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 15; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 9
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 8; mediciones[idx].mes = 1; mediciones[idx].anio = 2026;
    mediciones[idx].clima.temperatura = 27; mediciones[idx].clima.velocidadViento = 14; mediciones[idx].clima.humedad = 62;
    mediciones[idx].niveles.pm25 = 25; mediciones[idx].niveles.pm10 = 45; mediciones[idx].niveles.co2 = 1000;
    mediciones[idx].niveles.so2 = 32; mediciones[idx].niveles.no2 = 23;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Baja"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 10; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // Medicion 10
    mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 1;
    mediciones[idx].dia = 11; mediciones[idx].mes = 1; mediciones[idx].anio = 2026;
    mediciones[idx].clima.temperatura = 30; mediciones[idx].clima.velocidadViento = 8; mediciones[idx].clima.humedad = 74;
    mediciones[idx].niveles.pm25 = 34; mediciones[idx].niveles.pm10 = 59; mediciones[idx].niveles.co2 = 1170;
    mediciones[idx].niveles.so2 = 41; mediciones[idx].niveles.no2 = 29;
    strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Alta"); mediciones[idx].factoresSociales.esDiaFestivo = 0;
    mediciones[idx].factoresSociales.horaDelDia = 18; strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
    mediciones[idx].activa = 1; idx++;
    
    // -------- ZONA 2: PARQUE INDUSTRIAL NORTE (10 mediciones) --------
    // Mediciones 11-20
    int medicionesIndustrialNorte[][13] = {
        {14, 12, 2025, 32, 4, 82, 48, 88, 1650, 68, 42, 9},
        {17, 12, 2025, 33, 3, 85, 52, 95, 1750, 72, 45, 14},
        {20, 12, 2025, 31, 6, 78, 45, 82, 1550, 65, 40, 7},
        {23, 12, 2025, 30, 8, 75, 42, 78, 1480, 62, 38, 11},
        {26, 12, 2025, 34, 2, 88, 55, 98, 1800, 75, 48, 15},
        {29, 12, 2025, 32, 5, 80, 49, 90, 1680, 70, 43, 13},
        {1, 1, 2026, 31, 7, 76, 43, 80, 1520, 64, 39, 10},
        {4, 1, 2026, 33, 4, 83, 50, 92, 1720, 71, 44, 14},
        {7, 1, 2026, 32, 5, 79, 47, 86, 1620, 67, 41, 16},
        {10, 1, 2026, 34, 3, 86, 53, 96, 1770, 73, 46, 9}
    };
    
    for(int i = 0; i < 10; i++){
        mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 2;
        mediciones[idx].dia = medicionesIndustrialNorte[i][0];
        mediciones[idx].mes = medicionesIndustrialNorte[i][1];
        mediciones[idx].anio = medicionesIndustrialNorte[i][2];
        mediciones[idx].clima.temperatura = medicionesIndustrialNorte[i][3];
        mediciones[idx].clima.velocidadViento = medicionesIndustrialNorte[i][4];
        mediciones[idx].clima.humedad = medicionesIndustrialNorte[i][5];
        mediciones[idx].niveles.pm25 = medicionesIndustrialNorte[i][6];
        mediciones[idx].niveles.pm10 = medicionesIndustrialNorte[i][7];
        mediciones[idx].niveles.co2 = medicionesIndustrialNorte[i][8];
        mediciones[idx].niveles.so2 = medicionesIndustrialNorte[i][9];
        mediciones[idx].niveles.no2 = medicionesIndustrialNorte[i][10];
        strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Media");
        mediciones[idx].factoresSociales.esDiaFestivo = (mediciones[idx].dia == 1 && mediciones[idx].mes == 1) ? 1 : 0;
        mediciones[idx].factoresSociales.horaDelDia = medicionesIndustrialNorte[i][11];
        strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
        mediciones[idx].activa = 1; idx++;
    }
    
    // -------- ZONA 3: BARRIO LOS PINOS (10 mediciones) --------
    int medicionesResidencial[][13] = {
        {16, 12, 2025, 24, 22, 55, 16, 32, 780, 22, 16, 9},
        {19, 12, 2025, 25, 20, 58, 18, 35, 820, 24, 17, 11},
        {22, 12, 2025, 23, 25, 52, 14, 28, 750, 20, 15, 13},
        {25, 12, 2025, 22, 28, 50, 12, 25, 720, 18, 14, 10},
        {28, 12, 2025, 24, 23, 54, 15, 30, 770, 21, 15, 8},
        {31, 12, 2025, 25, 21, 56, 17, 33, 800, 23, 16, 12},
        {3, 1, 2026, 23, 26, 51, 13, 27, 740, 19, 14, 10},
        {6, 1, 2026, 24, 24, 53, 15, 31, 780, 22, 16, 14},
        {9, 1, 2026, 25, 22, 55, 16, 32, 790, 23, 17, 16},
        {12, 1, 2026, 24, 25, 52, 14, 29, 760, 21, 15, 11}
    };
    
    for(int i = 0; i < 10; i++){
        mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 3;
        mediciones[idx].dia = medicionesResidencial[i][0];
        mediciones[idx].mes = medicionesResidencial[i][1];
        mediciones[idx].anio = medicionesResidencial[i][2];
        mediciones[idx].clima.temperatura = medicionesResidencial[i][3];
        mediciones[idx].clima.velocidadViento = medicionesResidencial[i][4];
        mediciones[idx].clima.humedad = medicionesResidencial[i][5];
        mediciones[idx].niveles.pm25 = medicionesResidencial[i][6];
        mediciones[idx].niveles.pm10 = medicionesResidencial[i][7];
        mediciones[idx].niveles.co2 = medicionesResidencial[i][8];
        mediciones[idx].niveles.so2 = medicionesResidencial[i][9];
        mediciones[idx].niveles.no2 = medicionesResidencial[i][10];
        strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Baja");
        mediciones[idx].factoresSociales.esDiaFestivo = (mediciones[idx].dia == 25 && mediciones[idx].mes == 12) ? 1 : 0;
        mediciones[idx].factoresSociales.horaDelDia = medicionesResidencial[i][11];
        strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
        mediciones[idx].activa = 1; idx++;
    }
    
    // -------- ZONA 4: AVENIDA PRINCIPAL (10 mediciones) --------
    int medicionesMixta[][13] = {
        {15, 12, 2025, 27, 12, 65, 24, 46, 980, 32, 24, 8},
        {18, 12, 2025, 28, 10, 68, 26, 49, 1020, 34, 25, 18},
        {21, 12, 2025, 26, 15, 62, 21, 42, 920, 29, 22, 13},
        {24, 12, 2025, 25, 18, 58, 19, 38, 880, 27, 20, 11},
        {27, 12, 2025, 27, 11, 66, 25, 48, 1000, 33, 24, 17},
        {30, 12, 2025, 29, 9, 70, 28, 52, 1050, 36, 26, 19},
        {2, 1, 2026, 26, 14, 63, 22, 44, 940, 30, 22, 12},
        {5, 1, 2026, 27, 13, 64, 23, 45, 960, 31, 23, 15},
        {8, 1, 2026, 28, 11, 67, 25, 47, 990, 33, 24, 9},
        {11, 1, 2026, 29, 10, 69, 27, 50, 1030, 35, 25, 18}
    };
    
    for(int i = 0; i < 10; i++){
        mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 4;
        mediciones[idx].dia = medicionesMixta[i][0];
        mediciones[idx].mes = medicionesMixta[i][1];
        mediciones[idx].anio = medicionesMixta[i][2];
        mediciones[idx].clima.temperatura = medicionesMixta[i][3];
        mediciones[idx].clima.velocidadViento = medicionesMixta[i][4];
        mediciones[idx].clima.humedad = medicionesMixta[i][5];
        mediciones[idx].niveles.pm25 = medicionesMixta[i][6];
        mediciones[idx].niveles.pm10 = medicionesMixta[i][7];
        mediciones[idx].niveles.co2 = medicionesMixta[i][8];
        mediciones[idx].niveles.so2 = medicionesMixta[i][9];
        mediciones[idx].niveles.no2 = medicionesMixta[i][10];
        char *trafico = (medicionesMixta[i][11] == 18 || medicionesMixta[i][11] == 19 || medicionesMixta[i][11] == 8) ? "Alta" : "Media";
        strcpy(mediciones[idx].factoresSociales.densidadTrafico, trafico);
        mediciones[idx].factoresSociales.esDiaFestivo = (mediciones[idx].dia == 24 && mediciones[idx].mes == 12) ? 1 : 0;
        mediciones[idx].factoresSociales.horaDelDia = medicionesMixta[i][11];
        strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
        mediciones[idx].activa = 1; idx++;
    }
    
    // -------- ZONA 5: COMPLEJO INDUSTRIAL SUR (10 mediciones) --------
    int medicionesIndustrialSur[][13] = {
        {14, 12, 2025, 31, 5, 77, 44, 84, 1580, 66, 40, 10},
        {17, 12, 2025, 32, 4, 81, 47, 89, 1640, 69, 42, 14},
        {20, 12, 2025, 30, 7, 74, 41, 78, 1490, 63, 38, 8},
        {23, 12, 2025, 29, 9, 71, 38, 72, 1420, 60, 36, 11},
        {26, 12, 2025, 33, 3, 84, 51, 94, 1720, 72, 45, 15},
        {29, 12, 2025, 31, 6, 76, 45, 86, 1600, 67, 41, 13},
        {1, 1, 2026, 30, 8, 72, 39, 74, 1450, 61, 37, 10},
        {4, 1, 2026, 32, 5, 79, 46, 88, 1630, 68, 42, 14},
        {7, 1, 2026, 31, 6, 75, 43, 82, 1560, 65, 39, 16},
        {10, 1, 2026, 33, 4, 82, 49, 91, 1680, 70, 43, 9}
    };
    
    for(int i = 0; i < 10; i++){
        mediciones[idx].idMedicion = idx + 1; mediciones[idx].idZona = 5;
        mediciones[idx].dia = medicionesIndustrialSur[i][0];
        mediciones[idx].mes = medicionesIndustrialSur[i][1];
        mediciones[idx].anio = medicionesIndustrialSur[i][2];
        mediciones[idx].clima.temperatura = medicionesIndustrialSur[i][3];
        mediciones[idx].clima.velocidadViento = medicionesIndustrialSur[i][4];
        mediciones[idx].clima.humedad = medicionesIndustrialSur[i][5];
        mediciones[idx].niveles.pm25 = medicionesIndustrialSur[i][6];
        mediciones[idx].niveles.pm10 = medicionesIndustrialSur[i][7];
        mediciones[idx].niveles.co2 = medicionesIndustrialSur[i][8];
        mediciones[idx].niveles.so2 = medicionesIndustrialSur[i][9];
        mediciones[idx].niveles.no2 = medicionesIndustrialSur[i][10];
        strcpy(mediciones[idx].factoresSociales.densidadTrafico, "Media");
        mediciones[idx].factoresSociales.esDiaFestivo = (mediciones[idx].dia == 1 && mediciones[idx].mes == 1) ? 1 : 0;
        mediciones[idx].factoresSociales.horaDelDia = medicionesIndustrialSur[i][11];
        strcpy(mediciones[idx].factoresSociales.estacion, "Verano");
        mediciones[idx].activa = 1; idx++;
    }
    
    // Guardar mediciones
    FILE *fmediciones = fopen("mediciones.dat", "wb");
    if(fmediciones == NULL){
        printf("ERROR: No se pudo crear archivo mediciones.dat\n");
        return 1;
    }
    fwrite(mediciones, sizeof(Medicion), 50, fmediciones);
    fclose(fmediciones);
    printf("[OK] 50 mediciones generadas correctamente (10 por zona)\n");
    
    printf("\n================================================================================\n");
    printf("   DATOS GENERADOS EXITOSAMENTE\n");
    printf("================================================================================\n");
    printf("Archivos creados:\n");
    printf("  - zonas.dat (5 zonas)\n");
    printf("  - mediciones.dat (50 mediciones)\n");
    printf("\nAhora puedes ejecutar 'programa.exe' y los datos estaran disponibles!\n");
    printf("================================================================================\n");
    
    return 0;
}
