/* ========================================
 *
 * U N I V E R S I D A D   D E   P A M P L O N A
 *
 * Copyright Edwin Alexander Calderón Leal, 2024
 * Todos los Derechos Reservados
 * NO PUBLICADO, SOFTWARE LICENCIADO.
 *
 * INFORMACIÓN CONFIDENCIAL Y PROPIETARIA
 * PROPIEDAD DE Edwin Alexander Calderón Leal.
 *
 * ========================================
 *
 * Nombre: 
 * Edwin Alexander Calderón Leal
 * Kevin Andres Rios Trigos
 * Estudiantes de Ingeniería Mecatrónica
 * Proyecto para la materia de Sistemas Embebidos 1 
 * del programa de Ingeniería Electrónica
 * 
 * Docente de Aula: Ingeniero Julio Cesar Ospino Arias
 * 
 * Realizado en julio del 2024, Semestre 2024-1
 * 
 * Departamento de Ingenierías y Arquitectura
 *
 * ========================================
*/

#include "project.h"
#include "stdio.h"


// Definición de variable
    char buffer[600];
    int dutyCycle = 75;                // Valor de ejemplo para el ciclo de trabajo del PWM
    long double currentTime = 12345678; // Tiempo actual en segundos (valor de ejemplo)
    unsigned int rawAngle = 1023;      // Valor crudo del ángulo (valor de ejemplo)
    float anguloGrados = 45.67890;     // Ángulo en grados (valor de ejemplo)
    float deltaAngle = 0.123;          // Cambio en el ángulo (valor de ejemplo)
    float delta_tiempo = 0.01;         // Cambio en el tiempo (valor de ejemplo)
    float rpm = 1500.123456;           // Revoluciones por minuto (valor de ejemplo)

int main(void)
{
    CyGlobalIntEnable; /* Habilita las interrupciones globales. */
    UART_Start(); /* Iniciar UART */ // Iniciar la comunicación serial
    UART_PutString("!Manejo de variables! \r\n");
for(;;)
{
        /* Código principal */
    
    sprintf(buffer, "PWM, %d ,    Time, %.2Lf  ,     Crudo, %u   ,   Grados,   %.5f  , deltaAngle,  %.3f    ,  deltaTime,   %.2f   ,   RPM  ,  %.6f,\n",
                     dutyCycle, currentTime, rawAngle, anguloGrados, deltaAngle, delta_tiempo, rpm);
    UART_PutString(buffer);
}
}

/* [] FIN DEL ARCHIVO */
