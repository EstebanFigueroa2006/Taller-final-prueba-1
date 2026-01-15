#include <stdio.h>
#include <string.h>
#include <time.h>
#include "funciones.h"

// ============================================================================
// FUNCIONES DE ENTRADA Y VALIDACIÓN
// ============================================================================

void leerCadena(char *cadena, int n){
    fgets(cadena, n, stdin);
    int len = strlen(cadena) - 1;
    if(len >= 0 && cadena[len] == '\n') 
        cadena[len] = '\0';
}

int leerEnteroConRango(int min, int max){
    int num;
    int val;        
    do{
        val = scanf("%d", &num);
        if (val != 1 || num < min || num > max)
        {
            printf("El dato ingresado no es valido. Ingrese un valor entre %d y %d\n", min, max);
            while(getchar() != '\n');
        }
    }while(val != 1 || num < min || num > max);
    while(getchar() != '\n');
    return num;
}

float leerFlotanteConRango(float min, float max){ 
    float num;
    int val;
    do{
        val = scanf("%f", &num);
        if (val != 1 || num < min || num > max)
        {
            printf("El dato ingresado no es valido. Ingrese un valor entre %.2f y %.2f\n", min, max);
            while(getchar() != '\n');
        }
    }while(val != 1 || num < min || num > max);
    while(getchar() != '\n');
    return num;
}

// ============================================================================
// FUNCIONES DE UTILIDAD
// ============================================================================

/**
 * Calcula el Índice de Calidad del Aire (AQI) unificado
 * Basado en los niveles de contaminantes vs límites OMS
 * @param niveles Estructura con los niveles de contaminantes
 * @return Índice AQI (0-500+)
 */
int calcularAQI(Contaminantes niveles){
    // Cálculo de AQI para cada contaminante (0-100 = bueno, >100 = malo)
    float aqiPM25 = (niveles.pm25 / LIMITE_PM25) * 100;
    float aqiPM10 = (niveles.pm10 / LIMITE_PM10) * 100;
    float aqiCO2 = (niveles.co2 / LIMITE_CO2) * 100;
    float aqiSO2 = (niveles.so2 / LIMITE_SO2) * 100;
    float aqiNO2 = (niveles.no2 / LIMITE_NO2) * 100;
    
    // Retornar el peor índice (más crítico)
    float maxAQI = aqiPM25;
    if(aqiPM10 > maxAQI) maxAQI = aqiPM10;
    if(aqiCO2 > maxAQI) maxAQI = aqiCO2;
    if(aqiSO2 > maxAQI) maxAQI = aqiSO2;
    if(aqiNO2 > maxAQI) maxAQI = aqiNO2;
    
    return (int)maxAQI;
}

/**
 * Obtiene la categoría textual del AQI
 * @param aqi Índice de calidad del aire
 * @return Categoría como string
 */
char* obtenerCategoriaAQI(int aqi){
    if(aqi <= 50) return "Buena";
    if(aqi <= 100) return "Moderada";
    if(aqi <= 150) return "Daniina para grupos sensibles";
    if(aqi <= 200) return "Daniina";
    if(aqi <= 300) return "Muy daniina";
    return "Peligrosa";
}

/**
 * Valida si una fecha está dentro de los últimos N días
 * @param dia Día de la medición (1-31)
 * @param mes Mes de la medición (1-12)
 * @param anio Año de la medición
 * @param diasLimite Número de días hacia atrás a considerar
 * @return 1 si está dentro del rango, 0 si no
 */
int validarFechaReciente(int dia, int mes, int anio, int diasLimite){
    time_t ahora = time(NULL);
    struct tm tm_medicion = {0};
    tm_medicion.tm_year = anio - 1900;
    tm_medicion.tm_mon = mes - 1;
    tm_medicion.tm_mday = dia;
    tm_medicion.tm_hour = 12;  // Mediodía para evitar problemas de zona horaria
    
    time_t tiempo_medicion = mktime(&tm_medicion);
    if(tiempo_medicion == -1) return 0;  // Fecha inválida
    
    double dias_diferencia = difftime(ahora, tiempo_medicion) / (60 * 60 * 24);
    return (dias_diferencia >= 0 && dias_diferencia <= diasLimite);
}

/**
 * Calcula el factor de ajuste basado en condiciones climáticas
 * @param clima Estructura con datos climáticos
 * @return Factor multiplicador (>1 = más contaminación, <1 = menos)
 */
float calcularFactorClimatico(DatosClimaticos clima){
    float factor = 1.0;
    
    // Viento bajo retiene contaminantes
    if(clima.velocidadViento < 10) factor *= 1.15;
    else if(clima.velocidadViento < 20) factor *= 1.05;
    else if(clima.velocidadViento > 40) factor *= 0.85;  // Viento fuerte dispersa
    
    // Humedad alta retiene partículas
    if(clima.humedad > 70) factor *= 1.10;
    else if(clima.humedad < 30) factor *= 0.95;
    
    // Temperatura baja puede causar inversión térmica
    if(clima.temperatura < 10) factor *= 1.12;
    else if(clima.temperatura > 30) factor *= 1.08;  // Calor aumenta reacciones químicas
    
    return factor;
}

/**
 * Calcula el factor de ajuste basado en contexto social y cultural
 * @param factores Estructura con factores sociales
 * @param tipoZona Tipo de zona (Residencial, Industrial, etc.)
 * @return Factor multiplicador
 */
float calcularFactorSocial(FactoresSociales factores, char *tipoZona){
    float factor = 1.0;
    
    // Densidad de tráfico
    if(strcmp(factores.densidadTrafico, "Alta") == 0) factor *= 1.25;
    else if(strcmp(factores.densidadTrafico, "Media") == 0) factor *= 1.10;
    else if(strcmp(factores.densidadTrafico, "Baja") == 0) factor *= 0.85;
    
    // Días festivos tienen menos tráfico
    if(factores.esDiaFestivo) factor *= 0.70;
    
    // Hora del día (horas pico: 7-9, 17-19)
    if((factores.horaDelDia >= 7 && factores.horaDelDia <= 9) ||
       (factores.horaDelDia >= 17 && factores.horaDelDia <= 19)){
        factor *= 1.20;
    }
    
    // Tipo de zona
    if(strcmp(tipoZona, "Industrial") == 0) factor *= 1.30;
    else if(strcmp(tipoZona, "Comercial") == 0) factor *= 1.15;
    else if(strcmp(tipoZona, "Residencial") == 0) factor *= 0.90;
    
    return factor;
}

/**
 * Verifica que haya al menos 5 zonas registradas
 * Si no, fuerza el registro de las faltantes
 * @return Número de zonas registradas
 */
int verificarMinimoCincoZonas(){
    Zona zonas[MAX_ZONAS];
    int numZonas = obtenerZonas(zonas);
    
    if(numZonas < MIN_ZONAS_REQUERIDAS){
        printf("\n========================================================================\n");
        printf("   ATENCION: Se requieren al menos %d zonas para iniciar el sistema   \n", MIN_ZONAS_REQUERIDAS);
        printf("   Actualmente hay %d zona(s) registrada(s)                           \n", numZonas);
        printf("========================================================================\n");
        printf("\nDebe registrar %d zona(s) adicional(es)\n\n", MIN_ZONAS_REQUERIDAS - numZonas);
        
        for(int i = numZonas; i < MIN_ZONAS_REQUERIDAS; i++){
            printf("--- Registrando zona %d de %d ---\n", i + 1, MIN_ZONAS_REQUERIDAS);
            registrarZona();
        }
        
        numZonas = obtenerZonas(zonas);
        printf("\n[EXITO] Sistema listo con %d zonas registradas\n", numZonas);
    }
    
    return numZonas;
}

// ============================================================================
// MENÚ PRINCIPAL
// ============================================================================

int menuPrincipal(){
    printf("\n");
    printf("========================================================================\n");
    printf("   SISTEMA INTEGRAL DE GESTION Y PREDICCION DE CONTAMINACION DEL AIRE  \n");
    printf("========================================================================\n");
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1.  Registrar Zona de Monitoreo\n");
    printf("2.  Listar Zonas Registradas\n");
    printf("3.  Registrar Medicion de Contaminacion\n");
    printf("4.  Listar Todas las Mediciones\n");
    printf("5.  Listar Mediciones por Zona\n");
    printf("6.  Calcular Promedios Historicos (30 dias)\n");
    printf("7.  Comparar con Limites de la OMS\n");
    printf("8.  Predecir Niveles Futuros (24 horas)\n");
    printf("9.  Generar Alertas Preventivas\n");
    printf("10. Listar Alertas Emitidas\n");
    printf("11. Exportar Reporte General (TXT)\n");
    printf("12. Exportar Reporte a CSV\n");
    printf("13. Visualizar Reportes Guardados\n");
    printf("14. Salir\n");
    printf(">> Seleccione una opcion: ");
    int opc = leerEnteroConRango(1, 14);
    return opc;
}

// ============================================================================
// FUNCIONES DE ZONA
// ============================================================================

void registrarZona(){
    Zona zona;
    Zona zonas[MAX_ZONAS];
    int count = obtenerZonas(zonas);
    
    if(count >= MAX_ZONAS){
        printf("ERROR: Se ha alcanzado el limite maximo de zonas (%d)\n", MAX_ZONAS);
        return;
    }
    
    printf("\n--- REGISTRAR NUEVA ZONA DE MONITOREO ---\n");
    printf("Ingrese el ID de la zona (1-999): ");
    int idBuscado = leerEnteroConRango(1, 999);
    
    // Verificar que el ID no exista
    for(int i = 0; i < count; i++){
        if(zonas[i].id == idBuscado){
            printf("ERROR: Ya existe una zona con ese ID\n");
            return;
        }
    }
    
    zona.id = idBuscado;
    printf("Ingrese el nombre de la zona: ");
    leerCadena(zona.nombre, 50);
    printf("Ingrese la ubicacion (direccion o coordenadas): ");
    leerCadena(zona.ubicacion, 100);
    printf("Ingrese la poblacion aproximada (100-10000000): ");
    zona.poblacion = leerEnteroConRango(100, 10000000);
    printf("Ingrese el tipo de zona (Residencial/Industrial/Comercial/Mixta): ");
    leerCadena(zona.tipo, 30);
    
    guardarZona(&zona);
    printf("\n[EXITO] Zona registrada correctamente\n"); 
}

void listarZonas(Zona *zonas){
    int count = obtenerZonas(zonas);
    if(count == 0){ 
        printf("\nNo hay zonas registradas en el sistema\n");
        return;
    }
    
    printf("\n--- ZONAS DE MONITOREO REGISTRADAS ---\n");
    printf("%-5s %-5s %-25s %-30s %-15s %-20s\n", "#", "ID", "Nombre", "Ubicacion", "Poblacion", "Tipo");
    printf("====================================================================================================\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-5d %-25s %-30s %-15d %-20s\n",
               i,
               zonas[i].id,
               zonas[i].nombre,
               zonas[i].ubicacion,
               zonas[i].poblacion,
               zonas[i].tipo);
    }
    printf("====================================================================================================\n");
    printf("Total de zonas: %d\n", count);
}

void guardarZona(Zona *zona){
    Zona zonas[MAX_ZONAS];
    int count = obtenerZonas(zonas);
    
    zonas[count] = *zona;
    count++;
    
    FILE *f = fopen("zonas.dat", "wb");
    if (f == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de zonas\n");
        return;
    }
    fwrite(zonas, sizeof(Zona), count, f);
    fclose(f);
}

int obtenerZonas(Zona *zonas){
    FILE *f = fopen("zonas.dat", "rb");
    if (f == NULL)
    {
        return 0; 
    }
    int count = fread(zonas, sizeof(Zona), MAX_ZONAS, f);
    fclose(f);
    return count;
}

// ============================================================================
// FUNCIONES DE MEDICIÓN
// ============================================================================

void registrarMedicion(){
    Medicion medicion;
    Zona zonas[MAX_ZONAS];
    int numZonas = obtenerZonas(zonas);
    
    if(numZonas == 0){
        printf("\nERROR: No hay zonas registradas. Registre zonas primero\n");
        return;
    }
    
    printf("\n--- REGISTRAR NUEVA MEDICION DE CONTAMINACION ---\n");
    
    // Seleccionar zona
    listarZonas(zonas);
    printf("\nSeleccione el numero de zona (0-%d): ", numZonas - 1);
    int idxZona = leerEnteroConRango(0, numZonas - 1);
    medicion.idZona = zonas[idxZona].id;
    
    // Generar ID de medición automáticamente
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    int numMediciones = obtenerMediciones(mediciones);
    medicion.idMedicion = numMediciones + 1;
    
    // Fecha de la medición
    printf("\nIngrese la fecha de la medicion:\n");
    printf("Dia (1-30): ");
    medicion.dia = leerEnteroConRango(1, 30);
    printf("Mes (1-12): ");
    medicion.mes = leerEnteroConRango(1, 12);
    printf("Anio (1996-2026): ");
    medicion.anio = leerEnteroConRango(1996, 2026);
    
    // Datos climáticos
    printf("\n--- DATOS CLIMATICOS ---\n");
    printf("Temperatura (°C) (-20 a 50): ");
    medicion.clima.temperatura = leerFlotanteConRango(-20, 50);
    printf("Velocidad del viento (km/h) (0-150): ");
    medicion.clima.velocidadViento = leerFlotanteConRango(0, 150);
    printf("Humedad (%%) (0-100): ");
    medicion.clima.humedad = leerFlotanteConRango(0, 100);
    
    // Niveles de contaminantes
    printf("\n--- NIVELES DE CONTAMINANTES ---\n");
    printf("PM2.5 - Particulas finas (μg/m³) (0-500): ");
    medicion.niveles.pm25 = leerFlotanteConRango(0, 500);
    printf("PM10 - Particulas gruesas (μg/m³) (0-600): ");
    medicion.niveles.pm10 = leerFlotanteConRango(0, 600);
    printf("CO2 - Dioxido de carbono (ppm) (300-5000): ");
    medicion.niveles.co2 = leerFlotanteConRango(300, 5000);
    printf("SO2 - Dioxido de azufre (μg/m³) (0-500): ");
    medicion.niveles.so2 = leerFlotanteConRango(0, 500);
    printf("NO2 - Dioxido de nitrogeno (μg/m³) (0-400): ");
    medicion.niveles.no2 = leerFlotanteConRango(0, 400);
    
    // Factores sociales y culturales
    printf("\n--- FACTORES SOCIALES Y CONTEXTUALES ---\n");
    printf("Densidad de trafico (Alta/Media/Baja): ");
    leerCadena(medicion.factoresSociales.densidadTrafico, 20);
    printf("Es dia festivo? (0=No, 1=Si): ");
    medicion.factoresSociales.esDiaFestivo = leerEnteroConRango(0, 1);
    printf("Hora del dia (0-23): ");
    medicion.factoresSociales.horaDelDia = leerEnteroConRango(0, 23);
    printf("Estacion del anio (Verano/Invierno/Primavera/Otono): ");
    leerCadena(medicion.factoresSociales.estacion, 20);
    
    medicion.activa = 1; // Medición válida
    
    guardarMedicion(&medicion);
    printf("\n[EXITO] Medicion registrada correctamente (ID: %d)\n", medicion.idMedicion); 
}

void listarMediciones(Medicion *mediciones){
    int count = obtenerMediciones(mediciones);
    if(count == 0){ 
        printf("\nNo hay mediciones registradas en el sistema\n");
        return;
    }
    
    printf("\n========================================================================================================\n");
    printf("                                    HISTORIAL DE MEDICIONES                                           \n");
    printf("========================================================================================================\n");
    printf("Total de mediciones: %d\n\n", count);
    
    // Encabezado de la tabla
    printf("%-4s %-8s %-12s %-6s %-7s %-7s %-7s %-7s %-7s %-5s %-6s %-4s\n",
           "ID", "ZonaID", "Fecha", "PM2.5", "PM10", "CO2", "SO2", "NO2", "Temp", "Vien", "Hum", "AQI");
    printf("%-4s %-8s %-12s %-6s %-7s %-7s %-7s %-7s %-7s %-5s %-6s %-4s\n",
           "", "", "DD/MM/AAAA", "μg/m³", "μg/m³", "ppm", "μg/m³", "μg/m³", "°C", "km/h", "%", "");
    printf("========================================================================================================\n");
    
    for (int i = 0; i < count; i++)
    {
        if(mediciones[i].activa == 0) continue;
        
        // Calcular AQI para esta medición
        int aqi = calcularAQI(mediciones[i].niveles);
        
        // Formatear fecha
        char fecha[13];
        sprintf(fecha, "%02d/%02d/%d", mediciones[i].dia, mediciones[i].mes, mediciones[i].anio);
        
        // Imprimir fila de datos
        printf("%-4d %-8d %-12s %-6.1f %-7.1f %-7.0f %-7.1f %-7.1f %-7.1f %-5.0f %-6.0f %-4d\n",
               mediciones[i].idMedicion,
               mediciones[i].idZona,
               fecha,
               mediciones[i].niveles.pm25,
               mediciones[i].niveles.pm10,
               mediciones[i].niveles.co2,
               mediciones[i].niveles.so2,
               mediciones[i].niveles.no2,
               mediciones[i].clima.temperatura,
               mediciones[i].clima.velocidadViento,
               mediciones[i].clima.humedad,
               aqi);
    }
    printf("========================================================================================================\n");
    
    // Leyenda de AQI
    printf("\nLEYENDA AQI: 0-50=Buena | 51-100=Moderada | 101-150=Daniina sensibles | 151-200=Daniina | 201+=Peligrosa\n");
}

void listarMedicionesPorZona(int idZona){
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    int count = obtenerMediciones(mediciones);
    
    if(count == 0){ 
        printf("\nNo hay mediciones registradas\n");
        return;
    }
    
    int encontradas = 0;
    printf("\n--- MEDICIONES DE LA ZONA ID: %d ---\n", idZona);
    
    for (int i = 0; i < count; i++)
    {
        if(mediciones[i].idZona == idZona && mediciones[i].activa == 1){
            printf("\nMedicion #%d | Fecha: %02d/%02d/%d\n",
                   mediciones[i].idMedicion,
                   mediciones[i].dia,
                   mediciones[i].mes,
                   mediciones[i].anio);
            printf("  PM2.5=%.2f | PM10=%.2f | CO2=%.2f | SO2=%.2f | NO2=%.2f\n",
                   mediciones[i].niveles.pm25,
                   mediciones[i].niveles.pm10,
                   mediciones[i].niveles.co2,
                   mediciones[i].niveles.so2,
                   mediciones[i].niveles.no2);
            encontradas++;
        }
    }
    
    if(encontradas == 0){
        printf("No se encontraron mediciones para esta zona\n");
    } else {
        printf("\nTotal: %d mediciones\n", encontradas);
    }
}

void guardarMedicion(Medicion *medicion){
    FILE *f = fopen("mediciones.dat", "ab");
    if (f == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de mediciones\n");
        return;
    }
    fwrite(medicion, sizeof(Medicion), 1, f);
    fclose(f);
}

int obtenerMediciones(Medicion *mediciones){
    FILE *f = fopen("mediciones.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    int count = fread(mediciones, sizeof(Medicion), MAX_ZONAS * MAX_MEDICIONES_POR_ZONA, f);
    fclose(f);
    return count;
}

// ============================================================================
// FUNCIONES DE ANÁLISIS
// ============================================================================

void calcularPromediosHistoricos(){
    Zona zonas[MAX_ZONAS];
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    int numZonas = obtenerZonas(zonas);
    int numMediciones = obtenerMediciones(mediciones);
    
    if(numZonas == 0){
        printf("\nNo hay zonas registradas\n");
        return;
    }
    
    if(numMediciones == 0){
        printf("\nNo hay mediciones registradas\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("   PROMEDIOS HISTORICOS DE CONTAMINACION (ULTIMOS 30 DIAS)           \n");
    printf("========================================================================\n");
    
    for(int i = 0; i < numZonas; i++){
        printf("\n--- ZONA: %s (ID: %d) ---\n", zonas[i].nombre, zonas[i].id);
        
        float promPM25 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 1);
        float promPM10 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 2);
        float promCO2 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 3);
        float promSO2 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 4);
        float promNO2 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 5);
        
        if(promPM25 < 0){
            printf("  No hay mediciones suficientes para esta zona\n");
            continue;
        }
        
        printf("  Promedio PM2.5:  %.2f μg/m³ %s\n", promPM25, promPM25 > LIMITE_PM25 ? "[EXCEDE LIMITE OMS]" : "[DENTRO DE LIMITE]");
        printf("  Promedio PM10:   %.2f μg/m³ %s\n", promPM10, promPM10 > LIMITE_PM10 ? "[EXCEDE LIMITE OMS]" : "[DENTRO DE LIMITE]");
        printf("  Promedio CO2:    %.2f ppm %s\n", promCO2, promCO2 > LIMITE_CO2 ? "[EXCEDE LIMITE OMS]" : "[DENTRO DE LIMITE]");
        printf("  Promedio SO2:    %.2f μg/m³ %s\n", promSO2, promSO2 > LIMITE_SO2 ? "[EXCEDE LIMITE OMS]" : "[DENTRO DE LIMITE]");
        printf("  Promedio NO2:    %.2f μg/m³ %s\n", promNO2, promNO2 > LIMITE_NO2 ? "[EXCEDE LIMITE OMS]" : "[DENTRO DE LIMITE]");
        
        printf("------------------------------------------------------------------------\n");
    }
}

float calcularPromedioContaminante(Medicion *mediciones, int numMediciones, int idZona, int tipoContaminante){
    float suma = 0;
    int contador = 0;
    
    for(int i = 0; i < numMediciones; i++){
        if(mediciones[i].idZona == idZona && mediciones[i].activa == 1){
            // Validar que la medición sea de los últimos 30 días
            if(!validarFechaReciente(mediciones[i].dia, mediciones[i].mes, mediciones[i].anio, DIAS_HISTORICO)){
                continue;  // Saltar mediciones antiguas
            }
            
            switch(tipoContaminante){
                case 1: suma += mediciones[i].niveles.pm25; break;
                case 2: suma += mediciones[i].niveles.pm10; break;
                case 3: suma += mediciones[i].niveles.co2; break;
                case 4: suma += mediciones[i].niveles.so2; break;
                case 5: suma += mediciones[i].niveles.no2; break;
            }
            contador++;
        }
    }
    
    if(contador == 0) return -1;
    return suma / contador;
}

void compararConLimitesOMS(){
    printf("\n========================================================================\n");
    printf("   LIMITES DE CALIDAD DEL AIRE ESTABLECIDOS POR LA OMS               \n");
    printf("========================================================================\n");
    printf("  PM2.5 (Particulas finas):        %.2f μg/m³\n", LIMITE_PM25);
    printf("  PM10 (Particulas gruesas):       %.2f μg/m³\n", LIMITE_PM10);
    printf("  CO2 (Dioxido de carbono):        %.2f ppm\n", LIMITE_CO2);
    printf("  SO2 (Dioxido de azufre):         %.2f μg/m³\n", LIMITE_SO2);
    printf("  NO2 (Dioxido de nitrogeno):      %.2f μg/m³\n", LIMITE_NO2);
    printf("========================================================================\n");
    
    // Ahora calculamos y comparamos
    calcularPromediosHistoricos();
}

// ============================================================================
// FUNCIONES DE PREDICCIÓN
// ============================================================================

void predecirNivelesFuturos(){
    Zona zonas[MAX_ZONAS];
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    int numZonas = obtenerZonas(zonas);
    int numMediciones = obtenerMediciones(mediciones);
    
    if(numZonas == 0){
        printf("\nERROR: No hay zonas registradas\n");
        return;
    }
    
    if(numMediciones == 0){
        printf("\nERROR: No hay mediciones registradas para hacer predicciones\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("   PREDICCION DE NIVELES DE CONTAMINACION (PROXIMAS 24 HORAS)        \n");
    printf("========================================================================\n");
    printf("\nAlgoritmo: Promedio ponderado con mas peso a mediciones recientes\n");
    
    for(int i = 0; i < numZonas; i++){
        Prediccion pred = calcularPrediccion(mediciones, numMediciones, zonas[i].id, zonas[i]);
        
        if(pred.idZona == -1){
            printf("\n--- ZONA: %s (ID: %d) ---\n", zonas[i].nombre, zonas[i].id);
            printf("  No hay suficientes datos para realizar prediccion\n");
            continue;
        }
        
        printf("\n--- ZONA: %s (ID: %d) ---\n", pred.nombreZona, pred.idZona);
        printf("  Proyeccion: %d horas\n", pred.horasProyeccion);
        printf("  AQI (Indice de Calidad del Aire): %d - %s\n", pred.aqi, obtenerCategoriaAQI(pred.aqi));
        printf("  Niveles predichos:\n");
        printf("    PM2.5: %.2f μg/m³ %s\n", pred.nivelesPredichos.pm25, 
               pred.nivelesPredichos.pm25 > LIMITE_PM25 ? "[RIESGO - EXCEDE LIMITE]" : "[SEGURO]");
        printf("    PM10:  %.2f μg/m³ %s\n", pred.nivelesPredichos.pm10,
               pred.nivelesPredichos.pm10 > LIMITE_PM10 ? "[RIESGO - EXCEDE LIMITE]" : "[SEGURO]");
        printf("    CO2:   %.2f ppm %s\n", pred.nivelesPredichos.co2,
               pred.nivelesPredichos.co2 > LIMITE_CO2 ? "[RIESGO - EXCEDE LIMITE]" : "[SEGURO]");
        printf("    SO2:   %.2f μg/m³ %s\n", pred.nivelesPredichos.so2,
               pred.nivelesPredichos.so2 > LIMITE_SO2 ? "[RIESGO - EXCEDE LIMITE]" : "[SEGURO]");
        printf("    NO2:   %.2f μg/m³ %s\n", pred.nivelesPredichos.no2,
               pred.nivelesPredichos.no2 > LIMITE_NO2 ? "[RIESGO - EXCEDE LIMITE]" : "[SEGURO]");
        
        if(pred.riesgoAlto){
            printf("  [ALERTA] Esta zona presenta RIESGO ALTO de contaminacion\n");
        } else {
            printf("  [OK] Niveles dentro de parametros aceptables\n");
        }
        
        guardarPrediccion(&pred);
        printf("------------------------------------------------------------------------\n");
    }
    
    printf("\n[INFO] Predicciones guardadas en el sistema\n");
}

Prediccion calcularPrediccion(Medicion *mediciones, int numMediciones, int idZona, Zona zona){
    Prediccion pred;
    pred.idZona = -1; // Marca de error
    
    // Buscar las últimas 5 mediciones de la zona (más recientes)
    float sumaPM25 = 0, sumaPM10 = 0, sumaCO2 = 0, sumaSO2 = 0, sumaNO2 = 0;
    float pesoTotal = 0;
    int contador = 0;
    
    // Variables para calcular promedios de factores
    float sumaFactorClimatico = 0;
    float sumaFactorSocial = 0;
    
    // Recorrer desde el final (más reciente) hacia atrás
    for(int i = numMediciones - 1; i >= 0 && contador < 5; i--){
        if(mediciones[i].idZona == idZona && mediciones[i].activa == 1){
            // Peso: más reciente = más peso (5, 4, 3, 2, 1)
            float peso = 5.0 - contador;
            
            sumaPM25 += mediciones[i].niveles.pm25 * peso;
            sumaPM10 += mediciones[i].niveles.pm10 * peso;
            sumaCO2 += mediciones[i].niveles.co2 * peso;
            sumaSO2 += mediciones[i].niveles.so2 * peso;
            sumaNO2 += mediciones[i].niveles.no2 * peso;
            
            // Calcular factores para esta medición
            sumaFactorClimatico += calcularFactorClimatico(mediciones[i].clima) * peso;
            sumaFactorSocial += calcularFactorSocial(mediciones[i].factoresSociales, zona.tipo) * peso;
            
            pesoTotal += peso;
            contador++;
        }
    }
    
    if(contador < 2){
        return pred; // No hay suficientes datos
    }
    
    // Calcular promedios ponderados
    pred.idZona = idZona;
    strcpy(pred.nombreZona, zona.nombre);
    pred.horasProyeccion = 24;
    
    // Aplicar factores climáticos y sociales
    float factorClimaticoPromedio = sumaFactorClimatico / pesoTotal;
    float factorSocialPromedio = sumaFactorSocial / pesoTotal;
    float factorCombinado = (factorClimaticoPromedio + factorSocialPromedio) / 2.0;
    
    pred.nivelesPredichos.pm25 = (sumaPM25 / pesoTotal) * factorCombinado;
    pred.nivelesPredichos.pm10 = (sumaPM10 / pesoTotal) * factorCombinado;
    pred.nivelesPredichos.co2 = (sumaCO2 / pesoTotal) * factorCombinado;
    pred.nivelesPredichos.so2 = (sumaSO2 / pesoTotal) * factorCombinado;
    pred.nivelesPredichos.no2 = (sumaNO2 / pesoTotal) * factorCombinado;
    
    // Agregar factor de incremento del 3% por tendencia
    pred.nivelesPredichos.pm25 *= 1.03;
    pred.nivelesPredichos.pm10 *= 1.03;
    pred.nivelesPredichos.co2 *= 1.03;
    pred.nivelesPredichos.so2 *= 1.03;
    pred.nivelesPredichos.no2 *= 1.03;
    
    // Calcular AQI
    pred.aqi = calcularAQI(pred.nivelesPredichos);
    
    // Determinar si hay riesgo alto
    pred.riesgoAlto = (pred.nivelesPredichos.pm25 > LIMITE_PM25) ||
                      (pred.nivelesPredichos.pm10 > LIMITE_PM10) ||
                      (pred.nivelesPredichos.co2 > LIMITE_CO2) ||
                      (pred.nivelesPredichos.so2 > LIMITE_SO2) ||
                      (pred.nivelesPredichos.no2 > LIMITE_NO2);
    
    // Fecha actual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(pred.fecha, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    return pred;
}

void guardarPrediccion(Prediccion *prediccion){
    FILE *f = fopen("predicciones.dat", "ab");
    if (f == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de predicciones\n");
        return;
    }
    fwrite(prediccion, sizeof(Prediccion), 1, f);
    fclose(f);
}

int obtenerPredicciones(Prediccion *predicciones){
    FILE *f = fopen("predicciones.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    int count = fread(predicciones, sizeof(Prediccion), MAX_ZONAS * 100, f);
    fclose(f);
    return count;
}

// ============================================================================
// FUNCIONES DE ALERTAS
// ============================================================================

void generarAlertas(){
    Zona zonas[MAX_ZONAS];
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    int numZonas = obtenerZonas(zonas);
    int numMediciones = obtenerMediciones(mediciones);
    
    if(numZonas == 0 || numMediciones == 0){
        printf("\nERROR: No hay datos suficientes para generar alertas\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("   SISTEMA DE ALERTAS PREVENTIVAS DE CONTAMINACION                    \n");
    printf("========================================================================\n");
    
    int alertasGeneradas = 0;
    
    for(int i = 0; i < numZonas; i++){
        // Obtener la medición más reciente de cada zona
        Medicion ultimaMedicion;
        int encontrada = 0;
        
        for(int j = numMediciones - 1; j >= 0; j--){
            if(mediciones[j].idZona == zonas[i].id && mediciones[j].activa == 1){
                ultimaMedicion = mediciones[j];
                encontrada = 1;
                break;
            }
        }
        
        if(!encontrada) continue;
        
        printf("\n--- Analizando Zona: %s (ID: %d) ---\n", zonas[i].nombre, zonas[i].id);
        
        // Verificar cada contaminante
        int alertaEnZona = 0;
        
        if(ultimaMedicion.niveles.pm25 > LIMITE_PM25){
            verificarYEmitirAlerta(zonas[i].id, zonas[i].nombre, 
                                  ultimaMedicion.niveles.pm25, LIMITE_PM25, "PM2.5");
            alertaEnZona = 1;
        }
        
        if(ultimaMedicion.niveles.pm10 > LIMITE_PM10){
            verificarYEmitirAlerta(zonas[i].id, zonas[i].nombre,
                                  ultimaMedicion.niveles.pm10, LIMITE_PM10, "PM10");
            alertaEnZona = 1;
        }
        
        if(ultimaMedicion.niveles.co2 > LIMITE_CO2){
            verificarYEmitirAlerta(zonas[i].id, zonas[i].nombre,
                                  ultimaMedicion.niveles.co2, LIMITE_CO2, "CO2");
            alertaEnZona = 1;
        }
        
        if(ultimaMedicion.niveles.so2 > LIMITE_SO2){
            verificarYEmitirAlerta(zonas[i].id, zonas[i].nombre,
                                  ultimaMedicion.niveles.so2, LIMITE_SO2, "SO2");
            alertaEnZona = 1;
        }
        
        if(ultimaMedicion.niveles.no2 > LIMITE_NO2){
            verificarYEmitirAlerta(zonas[i].id, zonas[i].nombre,
                                  ultimaMedicion.niveles.no2, LIMITE_NO2, "NO2");
            alertaEnZona = 1;
        }
        
        if(alertaEnZona){
            alertasGeneradas++;
        } else {
            printf("  [OK] Niveles dentro de parametros normales\n");
        }
    }
    
    printf("\n========================================================================\n");
    printf("Total de alertas generadas: %d\n", alertasGeneradas);
    printf("========================================================================\n");
}

void verificarYEmitirAlerta(int idZona, char *nombreZona, float nivel, float limite, char *tipoContaminante){
    Alerta alerta;
    Alerta alertas[MAX_ALERTAS];
    int numAlertas = obtenerAlertas(alertas);
    
    alerta.id = numAlertas + 1;
    alerta.idZona = idZona;
    strcpy(alerta.nombreZona, nombreZona);
    strcpy(alerta.tipoContaminante, tipoContaminante);
    alerta.nivelActual = nivel;
    alerta.nivelLimite = limite;
    
    // Determinar gravedad
    float exceso = ((nivel - limite) / limite) * 100;
    
    if(exceso < 20){
        strcpy(alerta.gravedad, "Moderada");
    } else if(exceso < 50){
        strcpy(alerta.gravedad, "Alta");
    } else {
        strcpy(alerta.gravedad, "Critica");
    }
    
    // Generar recomendación
    generarRecomendaciones(nivel, limite, tipoContaminante, alerta.recomendacion);
    
    // Fecha
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(alerta.fecha, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    // Mostrar alerta
    printf("  [ALERTA %s] %s excede limite en %.2f%% (%.2f vs %.2f)\n",
           alerta.gravedad, tipoContaminante, exceso, nivel, limite);
    printf("  Recomendacion: %s\n", alerta.recomendacion);
    
    guardarAlerta(&alerta);
}

void generarRecomendaciones(float nivel, float limite, char *tipoContaminante, char *resultado){
    float exceso = ((nivel - limite) / limite) * 100;
    
    if(strcmp(tipoContaminante, "PM2.5") == 0 || strcmp(tipoContaminante, "PM10") == 0){
        if(exceso < 20){
            strcpy(resultado, "Reducir circulacion vehicular en horas pico");
        } else if(exceso < 50){
            strcpy(resultado, "Suspender actividades al aire libre, usar mascarillas");
        } else {
            strcpy(resultado, "ALERTA MAXIMA: Cerrar industrias, suspender transporte");
        }
    }
    else if(strcmp(tipoContaminante, "CO2") == 0){
        if(exceso < 20){
            strcpy(resultado, "Promover uso de transporte publico y bicicletas");
        } else if(exceso < 50){
            strcpy(resultado, "Restringir vehiculos particulares, aumentar areas verdes");
        } else {
            strcpy(resultado, "CRITICO: Cerrar fabricas, limitar trafico vehicular");
        }
    }
    else if(strcmp(tipoContaminante, "SO2") == 0){
        if(exceso < 20){
            strcpy(resultado, "Monitorear emisiones industriales");
        } else if(exceso < 50){
            strcpy(resultado, "Reducir produccion industrial al 50%");
        } else {
            strcpy(resultado, "URGENTE: Cierre temporal de plantas industriales");
        }
    }
    else if(strcmp(tipoContaminante, "NO2") == 0){
        if(exceso < 20){
            strcpy(resultado, "Control de emisiones vehiculares diesel");
        } else if(exceso < 50){
            strcpy(resultado, "Prohibir circulacion vehiculos diesel antiguos");
        } else {
            strcpy(resultado, "EMERGENCIA: Restriccion total vehiculos combustion");
        }
    }
}

void listarAlertas(Alerta *alertas){
    int count = obtenerAlertas(alertas);
    if(count == 0){ 
        printf("\nNo hay alertas registradas en el sistema\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("   HISTORIAL DE ALERTAS EMITIDAS                                       \n");
    printf("========================================================================\n");
    printf("Total de alertas: %d\n\n", count);
    
    for (int i = 0; i < count; i++)
    {
        printf("ALERTA #%d [%s] - Fecha: %s\n", 
               alertas[i].id, alertas[i].gravedad, alertas[i].fecha);
        printf("  Zona: %s (ID: %d)\n", alertas[i].nombreZona, alertas[i].idZona);
        printf("  Contaminante: %s\n", alertas[i].tipoContaminante);
        printf("  Nivel actual: %.2f | Limite: %.2f\n",
               alertas[i].nivelActual, alertas[i].nivelLimite);
        printf("  Recomendacion: %s\n", alertas[i].recomendacion);
        printf("------------------------------------------------------------------------\n");
    }
}

void guardarAlerta(Alerta *alerta){
    FILE *f = fopen("alertas.dat", "ab");
    if (f == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de alertas\n");
        return;
    }
    fwrite(alerta, sizeof(Alerta), 1, f);
    fclose(f);
}

int obtenerAlertas(Alerta *alertas){
    FILE *f = fopen("alertas.dat", "rb");
    if (f == NULL)
    {
        return 0;
    }
    int count = fread(alertas, sizeof(Alerta), MAX_ALERTAS, f);
    fclose(f);
    return count;
}

// ============================================================================
// FUNCIONES DE REPORTES
// ============================================================================

void exportarReporte(){
    Zona zonas[MAX_ZONAS];
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    Alerta alertas[MAX_ALERTAS];
    
    int numZonas = obtenerZonas(zonas);
    int numMediciones = obtenerMediciones(mediciones);
    int numAlertas = obtenerAlertas(alertas);
    
    Reporte reporte;
    Reporte reportes[MAX_REPORTES];
    int numReportes = 0;
    
    FILE *f = fopen("reportes.dat", "rb");
    if(f != NULL){
        numReportes = fread(reportes, sizeof(Reporte), MAX_REPORTES, f);
        fclose(f);
    }
    
    reporte.id = numReportes + 1;
    reporte.numeroZonasMonitoreadas = numZonas;
    reporte.numeroAlertasEmitidas = numAlertas;
    
    // Calcular promedio general de PM2.5 y CO2
    float sumaPM25 = 0, sumaCO2 = 0;
    int contadorMediciones = 0;
    
    for(int i = 0; i < numMediciones; i++){
        if(mediciones[i].activa == 1){
            sumaPM25 += mediciones[i].niveles.pm25;
            sumaCO2 += mediciones[i].niveles.co2;
            contadorMediciones++;
        }
    }
    
    if(contadorMediciones > 0){
        reporte.promedioGeneralPM25 = sumaPM25 / contadorMediciones;
        reporte.promedioGeneralCO2 = sumaCO2 / contadorMediciones;
    } else {
        reporte.promedioGeneralPM25 = 0;
        reporte.promedioGeneralCO2 = 0;
    }
    
    // Generar conclusiones
    if(numAlertas == 0){
        strcpy(reporte.conclusiones, "Calidad del aire en niveles aceptables en todas las zonas");
    } else if(numAlertas < 5){
        strcpy(reporte.conclusiones, "Algunas zonas presentan niveles elevados. Monitoreo continuo requerido");
    } else {
        strcpy(reporte.conclusiones, "ATENCION: Multiples alertas activas. Medidas urgentes necesarias");
    }
    
    // Fecha
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(reporte.fecha, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    
    // Guardar reporte
    guardarReporte(&reporte);
    
    // Generar archivo de texto
    FILE *txt = fopen("reporte_contaminacion.txt", "w");
    if(txt != NULL){
        fprintf(txt, "========================================================================\n");
        fprintf(txt, "   REPORTE GENERAL DE CONTAMINACION DEL AIRE\n");
        fprintf(txt, "========================================================================\n");
        fprintf(txt, "Reporte ID: %d\n", reporte.id);
        fprintf(txt, "Fecha: %s\n\n", reporte.fecha);
        fprintf(txt, "RESUMEN EJECUTIVO:\n");
        fprintf(txt, "  Zonas monitoreadas: %d\n", reporte.numeroZonasMonitoreadas);
        fprintf(txt, "  Total de mediciones: %d\n", contadorMediciones);
        fprintf(txt, "  Alertas emitidas: %d\n\n", reporte.numeroAlertasEmitidas);
        fprintf(txt, "PROMEDIOS GENERALES:\n");
        fprintf(txt, "  PM2.5: %.2f μg/m³ (Limite OMS: %.2f)\n", reporte.promedioGeneralPM25, LIMITE_PM25);
        fprintf(txt, "  CO2:   %.2f ppm (Limite OMS: %.2f)\n\n", reporte.promedioGeneralCO2, LIMITE_CO2);
        fprintf(txt, "CONCLUSIONES:\n");
        fprintf(txt, "  %s\n\n", reporte.conclusiones);
        fprintf(txt, "========================================================================\n");
        fprintf(txt, "DETALLE POR ZONAS:\n");
        fprintf(txt, "========================================================================\n\n");
        
        for(int i = 0; i < numZonas; i++){
            fprintf(txt, "ZONA: %s (ID: %d)\n", zonas[i].nombre, zonas[i].id);
            fprintf(txt, "  Ubicacion: %s\n", zonas[i].ubicacion);
            fprintf(txt, "  Tipo: %s | Poblacion: %d\n", zonas[i].tipo, zonas[i].poblacion);
            
            float promPM25 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 1);
            if(promPM25 >= 0){
                fprintf(txt, "  Promedio PM2.5: %.2f μg/m³\n", promPM25);
                fprintf(txt, "  Promedio CO2:   %.2f ppm\n", 
                       calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 3));
            }
            fprintf(txt, "\n");
        }
        
        fprintf(txt, "========================================================================\n");
        fprintf(txt, "ALERTAS ACTIVAS:\n");
        fprintf(txt, "========================================================================\n\n");
        
        for(int i = 0; i < numAlertas; i++){
            fprintf(txt, "Alerta #%d [%s]\n", alertas[i].id, alertas[i].gravedad);
            fprintf(txt, "  Zona: %s | Contaminante: %s\n", 
                   alertas[i].nombreZona, alertas[i].tipoContaminante);
            fprintf(txt, "  Nivel: %.2f | Limite: %.2f\n", 
                   alertas[i].nivelActual, alertas[i].nivelLimite);
            fprintf(txt, "  Accion recomendada: %s\n\n", alertas[i].recomendacion);
        }
        
        fprintf(txt, "========================================================================\n");
        fprintf(txt, "Fin del reporte\n");
        fclose(txt);
        
        printf("\n[EXITO] Reporte generado y exportado correctamente\n");
        printf("  - Archivo: reporte_contaminacion.txt\n");
        printf("  - ID Reporte: %d\n", reporte.id);
    } else {
        printf("\nERROR: No se pudo crear el archivo de reporte\n");
    }
}

void guardarReporte(Reporte *reporte){
    FILE *f = fopen("reportes.dat", "ab");
    if (f == NULL)
    {
        printf("ERROR: No se pudo abrir el archivo de reportes\n");
        return;
    }
    fwrite(reporte, sizeof(Reporte), 1, f);
    fclose(f);
}

void visualizarReportes(){
    Reporte reportes[MAX_REPORTES];
    FILE *f = fopen("reportes.dat", "rb");
    
    if(f == NULL){
        printf("\nNo hay reportes guardados en el sistema\n");
        return;
    }
    
    int count = fread(reportes, sizeof(Reporte), MAX_REPORTES, f);
    fclose(f);
    
    if(count == 0){
        printf("\nNo hay reportes guardados\n");
        return;
    }
    
    printf("\n========================================================================\n");
    printf("   REPORTES GUARDADOS EN EL SISTEMA                                    \n");
    printf("========================================================================\n");
    printf("Total de reportes: %d\n\n", count);
    
    for(int i = 0; i < count; i++){
        printf("REPORTE #%d - Fecha: %s\n", reportes[i].id, reportes[i].fecha);
        printf("  Zonas monitoreadas: %d\n", reportes[i].numeroZonasMonitoreadas);
        printf("  Alertas emitidas: %d\n", reportes[i].numeroAlertasEmitidas);
        printf("  Promedio PM2.5: %.2f μg/m³\n", reportes[i].promedioGeneralPM25);
        printf("  Promedio CO2: %.2f ppm\n", reportes[i].promedioGeneralCO2);
        printf("  Conclusiones: %s\n", reportes[i].conclusiones);
        printf("------------------------------------------------------------------------\n");
    }
}

/**
 * Exporta un reporte completo en formato CSV para análisis en Excel/Python
 * Incluye datos de zonas, mediciones, alertas y predicciones
 */
void exportarReporteCSV(){
    Zona zonas[MAX_ZONAS];
    Medicion mediciones[MAX_ZONAS * MAX_MEDICIONES_POR_ZONA];
    Alerta alertas[MAX_ALERTAS];
    
    int numZonas = obtenerZonas(zonas);
    int numMediciones = obtenerMediciones(mediciones);
    int numAlertas = obtenerAlertas(alertas);
    
    if(numZonas == 0){
        printf("\nNo hay datos para exportar\n");
        return;
    }
    
    // Generar archivo CSV de mediciones
    FILE *csvMediciones = fopen("mediciones_contaminacion.csv", "w");
    if(csvMediciones != NULL){
        fprintf(csvMediciones, "ID_Medicion,ID_Zona,Zona,Fecha,PM2.5,PM10,CO2,SO2,NO2,Temperatura,Viento,Humedad,Densidad_Trafico,AQI,Categoria\n");
        
        for(int i = 0; i < numMediciones; i++){
            if(mediciones[i].activa == 0) continue;
            
            // Buscar nombre de zona
            char nombreZona[50] = "Desconocida";
            for(int j = 0; j < numZonas; j++){
                if(zonas[j].id == mediciones[i].idZona){
                    strcpy(nombreZona, zonas[j].nombre);
                    break;
                }
            }
            
            int aqi = calcularAQI(mediciones[i].niveles);
            
            fprintf(csvMediciones, "%d,%d,%s,%02d/%02d/%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.1f,%.1f,%.1f,%s,%d,%s\n",
                   mediciones[i].idMedicion,
                   mediciones[i].idZona,
                   nombreZona,
                   mediciones[i].dia,
                   mediciones[i].mes,
                   mediciones[i].anio,
                   mediciones[i].niveles.pm25,
                   mediciones[i].niveles.pm10,
                   mediciones[i].niveles.co2,
                   mediciones[i].niveles.so2,
                   mediciones[i].niveles.no2,
                   mediciones[i].clima.temperatura,
                   mediciones[i].clima.velocidadViento,
                   mediciones[i].clima.humedad,
                   mediciones[i].factoresSociales.densidadTrafico,
                   aqi,
                   obtenerCategoriaAQI(aqi));
        }
        fclose(csvMediciones);
        printf("\n[EXITO] Archivo generado: mediciones_contaminacion.csv\n");
    }
    
    // Generar archivo CSV de zonas
    FILE *csvZonas = fopen("zonas_monitoreo.csv", "w");
    if(csvZonas != NULL){
        fprintf(csvZonas, "ID,Nombre,Ubicacion,Poblacion,Tipo,Promedio_PM2.5,Promedio_CO2\n");
        
        for(int i = 0; i < numZonas; i++){
            float promPM25 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 1);
            float promCO2 = calcularPromedioContaminante(mediciones, numMediciones, zonas[i].id, 3);
            
            fprintf(csvZonas, "%d,%s,%s,%d,%s,%.2f,%.2f\n",
                   zonas[i].id,
                   zonas[i].nombre,
                   zonas[i].ubicacion,
                   zonas[i].poblacion,
                   zonas[i].tipo,
                   promPM25 >= 0 ? promPM25 : 0,
                   promCO2 >= 0 ? promCO2 : 0);
        }
        fclose(csvZonas);
        printf("[EXITO] Archivo generado: zonas_monitoreo.csv\n");
    }
    
    // Generar archivo CSV de alertas
    FILE *csvAlertas = fopen("alertas_emitidas.csv", "w");
    if(csvAlertas != NULL){
        fprintf(csvAlertas, "ID,Zona,Contaminante,Nivel_Actual,Limite,Exceso_Porcentaje,Gravedad,Fecha,Recomendacion\n");
        
        for(int i = 0; i < numAlertas; i++){
            float exceso = ((alertas[i].nivelActual - alertas[i].nivelLimite) / alertas[i].nivelLimite) * 100;
            
            fprintf(csvAlertas, "%d,%s,%s,%.2f,%.2f,%.2f,%s,%s,%s\n",
                   alertas[i].id,
                   alertas[i].nombreZona,
                   alertas[i].tipoContaminante,
                   alertas[i].nivelActual,
                   alertas[i].nivelLimite,
                   exceso,
                   alertas[i].gravedad,
                   alertas[i].fecha,
                   alertas[i].recomendacion);
        }
        fclose(csvAlertas);
        printf("[EXITO] Archivo generado: alertas_emitidas.csv\n");
    }
    
    printf("\n========================================================================\n");
    printf("   EXPORTACION CSV COMPLETADA                                          \n");
    printf("========================================================================\n");
    printf("Archivos generados:\n");
    printf("  1. mediciones_contaminacion.csv (%d registros)\n", numMediciones);
    printf("  2. zonas_monitoreo.csv (%d zonas)\n", numZonas);
    printf("  3. alertas_emitidas.csv (%d alertas)\n", numAlertas);
    printf("\nEstos archivos pueden ser abiertos en Excel, Python o herramientas de analisis\n");
}   
