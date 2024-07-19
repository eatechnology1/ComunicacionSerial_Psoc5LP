# Comunicación Serial en PSoC5LP

La comunicación serial es una técnica fundamental en la programación de microcontroladores, permitiendo la transferencia de datos entre dispositivos. En esta guía, exploraremos cómo configurar y utilizar la comunicación serial en el PSoC5LP, un microcontrolador potente y versátil. Aprenderás los pasos esenciales para establecer una comunicación eficiente y realizar transferencias de datos confiables.

## Contenido
- [¿Qué es la Comunicación Serial?](#qué-es-la-comunicación-serial)
- [UART: Universal Asynchronous Receiver-Transmitter](#uart-universal-asynchronous-receiver-transmitter)
- [Características de la Comunicación Serial](#características-de-la-comunicación-serial)
- [Ejemplo de Configuración UART](#ejemplo-de-configuración-uart)
- [Configuración del Entorno de Desarrollo](#configuración-del-entorno-de-desarrollo)
- [Configuración del Componente UART](#configuración-del-componente-uart)
- [Código para la Comunicación Serial](#código-para-la-comunicación-serial)
- [Enviar Datos](#enviar-datos)
- [Recibir Datos](#recibir-datos)
- [Ventajas de la Comunicación Serial](#ventajas-de-la-comunicación-serial)
- [Proyectos De Robótica](#proyectos-de-robótica)
- [Licencia](#licencia)

## ¿Qué es la Comunicación Serial?

La comunicación serial es un método de transmisión de datos en el que la información se envía bit a bit a través de un solo canal o línea de comunicación. Este método es ampliamente utilizado debido a su simplicidad y eficiencia en la transferencia de datos a largas distancias y entre diferentes dispositivos electrónicos.

## UART: Universal Asynchronous Receiver-Transmitter

UART (Transmisor-Receptor Asíncrono Universal) es un componente clave en la comunicación serial. Actúa como un intermediario entre el microcontrolador y el dispositivo receptor, convirtiendo los datos paralelos en datos seriales y viceversa. La comunicación a través de UART es asíncrona, lo que significa que no requiere una señal de reloj compartida entre los dispositivos.

## Características de la Comunicación Serial

- **Velocidad de Transmisión (Baud Rate):** El baud rate es la velocidad a la que se transmiten los datos a través del canal de comunicación. Se mide en bits por segundo (bps). Algunos baud rates comunes son 9600, 115200, etc. Un baud rate más alto permite una transferencia de datos más rápida, pero también puede aumentar la probabilidad de errores si la calidad de la señal no es óptima.
- **Bits de Datos:** La cantidad de bits que componen una unidad de datos (byte) en la comunicación serial puede variar. El valor típico es de 8 bits, pero también se pueden usar 5, 6 o 7 bits en algunos casos.
- **Bits de Paridad:** El bit de paridad es un mecanismo de detección de errores. Puede ser par (even), impar (odd) o ninguno (none). Un bit de paridad se añade a los datos para asegurarse de que la cantidad total de bits 1 en la transmisión sea par o impar, lo que ayuda a detectar errores en los datos transmitidos.
- **Bits de Stop:** Los bits de stop indican el final de una unidad de datos. Comúnmente se utilizan 1 o 2 bits de stop. Estos bits permiten que el receptor se sincronice y se prepare para recibir el siguiente byte de datos.

## Ejemplo de Configuración UART

- **Baud Rate:** 115200 bps
- **Bits de Datos:** 8
- **Paridad:** Ninguna (None)
- **Bits de Stop:** 1

## Configuración del Entorno de Desarrollo

Para comenzar, asegúrate de tener los siguientes elementos:

- PSoC5LP Development Kit
- Cypress PSoC Creator (software de desarrollo)
- Cable USB

## Creación de un Nuevo Proyecto

1. **Abrir PSoC Creator:** Inicia el software y selecciona «File» > «New» > «Project…».
2. **Configurar el Proyecto:** Selecciona «PSoC 5LP» como la familia de dispositivos y elige un diseño en blanco.
3. **Nombrar el Proyecto:** Asigna un nombre a tu proyecto y selecciona una ubicación para guardarlo.

## Configuración del Componente UART

1. **Agregar el Componente UART:** En el «Component Catalog», busca y arrastra el componente UART a tu diseño.
2. **Configurar UART:**
   - Baud Rate: Configura la velocidad de transmisión (por ejemplo, 9600).
   - Data Bits, Parity, Stop Bits: Ajusta según tus necesidades (por defecto: 8N1).
3. **Asignar Pines:** Asigna los pines RX y TX a los pines físicos del PSoC5LP.

## Código para la Comunicación Serial

### Iniciar UART

En tu archivo de código principal (`main.c`), incluye las siguientes líneas para iniciar el UART:

```c
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start(); /* Iniciar UART */

    for (;;)
    {
        /* Código principal */
    }
}
```

### Enviar Datos

#### Enviar Cadenas de Texto

Para enviar datos a través de UART, puedes utilizar la función `UART_PutString`. Esta función envía una cadena de texto terminada en null (`\0`).

```c
UART_PutString("Hola, mundo!\r\n");
```

#### Enviar Variables Numéricas

##### Enviar Enteros

Para enviar un número entero, primero necesitas convertirlo a una cadena de texto. Puedes hacerlo usando la función `sprintf`.

```c
int number = 123;
char buffer[20];
sprintf(buffer, "%d", number);
UART_PutString(buffer);
UART_PutString("\r\n"); // Añadir nueva línea
```

##### Enviar Flotantes

De manera similar, puedes convertir un número flotante a una cadena de texto antes de enviarlo.

```c
float number = 123.45;
char buffer[20];
sprintf(buffer, "%.2f", number);
UART_PutString(buffer);
UART_PutString("\r\n"); // Añadir nueva línea
```

##### Enviar Long

Para enviar números largos, se usa `sprintf` con el especificador de formato `%ld`.

```c
long number = 1234567890;
char buffer[20];
sprintf(buffer, "%ld", number);
UART_PutString(buffer);
UART_PutString("\r\n");
```

##### Enviar Double

Para enviar números de punto flotante de doble precisión, se usa `sprintf` con el especificador de formato `%lf`.

```c
double number = 12345.6789;
char buffer[30];
sprintf(buffer, "%.4lf", number);
UART_PutString(buffer);
UART_PutString("\r\n");
```

##### Enviar Múltiples Variables usando un Buffer

Un buffer es un área de memoria que almacena temporalmente los datos antes de enviarlos. Utilizar un buffer es útil cuando deseas enviar múltiples variables juntas.

```c
int integerVar = 123;
float floatVar = 456.78;
char buffer[50];

sprintf(buffer, "Int: %d, Float: %.2f\r\n", integerVar, floatVar);
UART_PutString(buffer);
```

### Recibir Datos

Para recibir datos, utiliza `UART_GetChar` y almacénalos en una variable:

```c
uint8_t received_data = UART_GetChar();
if(received_data)
{
    /* Procesar datos recibidos */
}
```

Sin embargo, la recepción de datos mediante comunicación serial en el PSoC5LP requiere la configuración de la UART y la habilitación de interrupciones para manejar los datos entrantes de manera eficiente. A continuación, se detalla cómo configurar y utilizar las interrupciones para recibir datos.

#### Configurar las interrupciones del UART

1. **En las propiedades del componente UART, habilita la interrupción RX (Recepción).**
2. **Asigna un nombre a la interrupción, por ejemplo, `isrRX`.**

### Declarar la interrupción en el código:

En tu archivo principal (por ejemplo, `main.c`), declara la función de interrupción y su prototipo.

```c
CY_ISR_PROTO(UART_RX_ISR);
```

### Definir la función de interrupción:

Define la función de interrupción para manejar los datos recibidos.

```c
CY_ISR(UART_RX_ISR) {
    // Verificar si hay datos disponibles
    if (UART_GetRxBufferSize() > 0) {
        // Leer el dato recibido
        uint8 receivedData = UART_ReadRxData();
        // Procesar el dato recibido (puedes almacenar o utilizarlo directamente)
        // Ejemplo: Guardar el dato en un buffer
        receivedBuffer[bufferIndex++] = receivedData;
    }
}
```

### Asignar la interrupción al vector de interrupciones:

En la función `main()`, asigna la función de interrupción al vector de interrupciones y habilítala.

```c
int main(void) {
    CyGlobalIntEnable; // Habilitar interrupciones globales

    // Inicializar UART
    UART_Start();

    // Asignar la interrupción RX del UART al vector de interrupciones
    isrRX_StartEx(UART_RX_ISR);

    for (;;) {
        // Loop principal
    }
}
```

## Código de ejemplo de lectura y escritura Serial

Un código completo para el

 manejo de la lectura y escritura de datos mediante comunicación serial en el PSoC5LP puede verse así:

```c
#include "project.h"

// Buffer para almacenar datos recibidos
#define BUFFER_SIZE 100
char receivedBuffer[BUFFER_SIZE];
uint8 bufferIndex = 0;

CY_ISR(UART_RX_ISR) {
    // Verificar si hay datos disponibles
    if (UART_GetRxBufferSize() > 0) {
        // Leer el dato recibido
        uint8 receivedData = UART_ReadRxData();
        // Guardar el dato en el buffer si no está lleno
        if (bufferIndex < BUFFER_SIZE) {
            receivedBuffer[bufferIndex++] = receivedData;
        }
        // Procesar el dato recibido
        // Ejemplo: Echo de vuelta el dato recibido
        UART_PutChar(receivedData);
    }
}

int main(void) {
    CyGlobalIntEnable; // Habilitar interrupciones globales

    // Inicializar UART
    UART_Start();

    // Asignar la interrupción RX del UART al vector de interrupciones
    isrRX_StartEx(UART_RX_ISR);

    // Mensaje de bienvenida
    UART_PutString("Iniciando Comunicación Serial\r\n");

    for (;;) {
        // Loop principal
        // Aquí puedes procesar los datos recibidos en el buffer
        // Ejemplo: Mostrar el buffer recibido cada vez que se presiona Enter
        if (bufferIndex > 0 && receivedBuffer[bufferIndex - 1] == '\r') {
            // Añadir un null terminador al buffer recibido para imprimirlo como string
            receivedBuffer[bufferIndex - 1] = '\0';
            UART_PutString("Recibido: ");
            UART_PutString(receivedBuffer);
            UART_PutString("\r\n");

            // Resetear el índice del buffer
            bufferIndex = 0;
        }
    }
}
```

Este código configura la UART, envía y recibe datos a través de la comunicación serial, y maneja las interrupciones de recepción. El buffer recibido se imprime en la UART cada vez que se presiona Enter.

## Ventajas de la Comunicación Serial

1. **Simplicidad:** La comunicación serial es más sencilla de implementar en comparación con otros métodos de comunicación.
2. **Larga Distancia:** Permite la transmisión de datos a largas distancias sin pérdida significativa de señal.
3. **Uso Eficiente de Pines:** Requiere menos pines de entrada/salida en comparación con la comunicación paralela.
4. **Flexibilidad:** Es compatible con una amplia variedad de dispositivos y protocolos de comunicación.

## Proyectos De Robótica

Este proyecto puede integrarse fácilmente en proyectos de robótica donde se requiera comunicación eficiente entre microcontroladores y otros dispositivos electrónicos, como sensores, actuadores y módulos de comunicación inalámbrica. Al dominar la comunicación serial, abrirás la puerta a un mundo de posibilidades en la interacción y control de sistemas robóticos.

## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.

