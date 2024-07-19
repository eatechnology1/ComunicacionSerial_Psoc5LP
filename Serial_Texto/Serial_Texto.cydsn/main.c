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

int main(void)
{
    CyGlobalIntEnable; /* Habilita las interrupciones globales. */
    UART_Start(); /* Iniciar UART */ // Iniciar la comunicación serial

for(;;)
{
        /* Código principal */
     UART_PutString("!Hola Mundo! \r\n");
}
}

/* [] FIN DEL ARCHIVO */