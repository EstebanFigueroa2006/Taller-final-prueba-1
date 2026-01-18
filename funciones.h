// Sistema Integral de Gestión y Predicción de Contaminación del Aire
// Definir límites máximos
#define MAX_ZONAS 10
#define MIN_ZONAS_REQUERIDAS 5  // Mínimo requerido por la consigna
#define MAX_MEDICIONES_POR_ZONA 30  // Últimos 30 días
#define MAX_ALERTAS 50
#define MAX_REPORTES 100
#define DIAS_HISTORICO 30  // Días para cálculo de promedios históricos

// Límites de la OMS para contaminantes (μg/m³)
#define LIMITE_PM25 15.0      // PM2.5 - Partículas finas
#define LIMITE_PM10 45.0      // PM10 - Partículas gruesas
#define LIMITE_CO2 4000.0     // CO2 en ppm (partes por millón)
#define LIMITE_SO2 40.0       // SO2 - Dióxido de azufre
#define LIMITE_NO2 25.0       // NO2 - Dióxido de nitrógeno

// Estructura para datos climáticos
typedef struct{
    float temperatura;        // En grados Celsius
    float velocidadViento;    // En km/h
    float humedad;            // En porcentaje (0-100)
}DatosClimaticos;

// Estructura para factores sociales y culturales
typedef struct{
    char densidadTrafico[20];  // Alta, Media, Baja
    int esDiaFestivo;          // 0 = día normal, 1 = festivo
    int horaDelDia;            // 0-23 horas
    char estacion[20];         // Verano, Invierno, Primavera, Otono
}FactoresSociales;

// Estructura para niveles de contaminantes
typedef struct{
    float pm25;    // Partículas finas (μg/m³)
    float pm10;    // Partículas gruesas (μg/m³)
    float co2;     // Dióxido de carbono (ppm)
    float so2;     // Dióxido de azufre (μg/m³)
    float no2;     // Dióxido de nitrógeno (μg/m³)
}Contaminantes;

// Estructura para una zona de monitoreo
typedef struct{
    int id;
    char nombre[50];
    char ubicacion[100];
    int poblacion;
    char tipo[30];  // Residencial, Industrial, Comercial, etc.
}Zona;

// Estructura para una medición de contaminación
typedef struct{
    int idMedicion;
    int idZona;
    int dia;              // Día del mes (1-30)
    int mes;              // Mes (1-12)
    int anio;             // Año
    Contaminantes niveles;
    DatosClimaticos clima;
    FactoresSociales factoresSociales;  // Factores contextuales
    int activa;           // 1 = medición válida, 0 = descartada
}Medicion;

// Estructura para predicción
typedef struct{
    int idZona;
    char nombreZona[50];
    Contaminantes nivelesPredichos;
    int horasProyeccion;  // 24 horas
    int riesgoAlto;       // 1 = sí, 0 = no
    int aqi;              // Índice de Calidad del Aire (0-500)
    char fecha[20];       // Fecha de la predicción
}Prediccion;

// Estructura para alertas
typedef struct{
    int id;
    int idZona;
    char nombreZona[50];
    char tipoContaminante[20];  // PM2.5, PM10, CO2, SO2, NO2
    float nivelActual;
    float nivelLimite;
    char gravedad[20];          // Moderada, Alta, Crítica
    char recomendacion[200];
    char fecha[20];
}Alerta;

// Estructura para reporte
typedef struct{
    int id;
    char fecha[20];
    int numeroZonasMonitoreadas;
    int numeroAlertasEmitidas;
    float promedioGeneralPM25;
    float promedioGeneralCO2;
    char conclusiones[300];
}Reporte;

// Funciones de entrada y validación
void leerCadena(char *cadena, int n);
int leerEnteroConRango(int min, int max);
float leerFlotanteConRango(float min, float max);

// Menú principal
int menuPrincipal();

// Funciones de Zona
void registrarZona();
void listarZonas(Zona *zonas);
void guardarZona(Zona *zona);
int obtenerZonas(Zona *zonas);
void editarZona();
void eliminarZona();

// Funciones de Medición
void registrarMedicion();
void listarMediciones(Medicion *mediciones);
void listarMedicionesPorZona(int idZona);
void guardarMedicion(Medicion *medicion);
int obtenerMediciones(Medicion *mediciones);
void editarMedicionDeZona();
void eliminarMedicionDeZona();

// Funciones de Análisis
void calcularPromediosHistoricos();
float calcularPromedioContaminante(Medicion *mediciones, int numMediciones, int idZona, int tipoContaminante);
void compararConLimitesOMS();

// Funciones de Predicción
void predecirNivelesFuturos();
Prediccion calcularPrediccion(Medicion *mediciones, int numMediciones, int idZona, Zona zona);
void guardarPrediccion(Prediccion *prediccion);
int obtenerPredicciones(Prediccion *predicciones);

// Funciones de Alertas
void generarAlertas();
void verificarYEmitirAlerta(int idZona, char *nombreZona, float nivel, float limite, char *tipoContaminante);
void listarAlertas(Alerta *alertas);
void guardarAlerta(Alerta *alerta);
int obtenerAlertas(Alerta *alertas);

// Funciones de Recomendaciones
void generarRecomendaciones(float nivel, float limite, char *tipoContaminante, char *resultado);

// Funciones de Reportes
void exportarReporte();
void exportarReporteCSV();
void guardarReporte(Reporte *reporte);
void visualizarReportes();

// Funciones de utilidad
int calcularAQI(Contaminantes niveles);
char* obtenerCategoriaAQI(int aqi);
int validarFechaReciente(int dia, int mes, int anio, int diasLimite);
float calcularFactorClimatico(DatosClimaticos clima);
float calcularFactorSocial(FactoresSociales factores, char *tipoZona);
int verificarMinimoCincoZonas();

