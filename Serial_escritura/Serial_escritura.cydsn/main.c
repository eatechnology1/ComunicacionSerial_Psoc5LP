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
#include "stdlib.h"
#include "stdio.h"

#define BUFFER_SIZE 254
 
CY_ISR_PROTO(UART_RX_ISR);

uint8 receivedBuffer[BUFFER_SIZE];
uint8 bufferIndex = 0;

CY_ISR(UART_RX_ISR) {
// Verificar si hay datos disponibles en el buffer de recepción
while (UART_GetRxBufferSize() > 0) {
// Leer el dato recibido
uint8 receivedData = UART_ReadRxData();

// Enviar el dato recibido de vuelta por UART
UART_PutChar(receivedData);

// Almacenar el dato en el buffer si hay espacio
if (bufferIndex < BUFFER_SIZE) {
receivedBuffer[bufferIndex++] = receivedData;
} else {
// Buffer lleno, reiniciar índice (o manejar el overflow de otra forma)
bufferIndex = 0;
}
}
}

int main(void)
{
    CyGlobalIntEnable; /* Habilita las interrupciones globales. */
    UART_Start(); /* Iniciar UART */ // Iniciar la comunicación serial
    
    // Asignar la interrupción RX del UART al vector de interrupciones
    isrRX_StartEx(UART_RX_ISR);
for(;;)
{
        /* Código principal */
     UART_PutString("!Hola Mundo! \r\n");
}
}

/* [] FIN DEL ARCHIVO */