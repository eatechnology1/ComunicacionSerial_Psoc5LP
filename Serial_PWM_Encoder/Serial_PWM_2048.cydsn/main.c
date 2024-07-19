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
 * Estudiante de Ingeniería Mecatrónica
 * Proyecto en desarrollo para proyecto de grado
 * del programa de Ingeniería Mecatrónica
 * 
 * Realizado en julio del 2024, Semestre 2024-1
 * 
 * Departamento de Ingenierías y Arquitectura
 *
 * ========================================
 *
 * Resumen del código:
 * Este código implementa un sistema de control y monitoreo utilizando un microcontrolador.
 * El sistema lee datos de un sensor de ángulo AS5600 a través de I2C y muestra los resultados
 * en una pantalla OLED. El ángulo crudo se convierte a grados y se calcula el delta del ángulo 
 * para obtener la velocidad en RPM. Los datos se envían por UART para su visualización.
 * Además, se utiliza un temporizador para actualizar el sistema a intervalos regulares y controlar
 * un PWM que ajusta un actuador. El código también implementa un filtro de media móvil para suavizar 
 * los cambios en los ángulos.
 *
 * Funcionalidades clave:
 * - Lectura del ángulo crudo del sensor AS5600.
 * - Conversión del ángulo crudo a grados.
 * - Cálculo del delta del ángulo y RPM.
 * - Envío de datos a través de UART.
 * - Visualización de datos en una pantalla OLED.
 * - Control de un PWM para ajustar un actuador.
 * - Implementación de un filtro de media móvil.
 *
 * ========================================
*/
#include "project.h"
#include "stdlib.h"
#include "stdio.h"
#include <ssd1306.h>


#define Timer_INTR_MASK_TC           (0x01u)
#define Timer_INTR_MASK_CC_MATCH     (0x02u)

// Definiciones de pines y direcciones I2C
#define AS5600_ADDR                   0x36
#define ANGLE_REG                     0x0E
#define AS5600_CONF_MSB               0x07
#define AS5600_CONF_LSB               0x08


// Definiciones de pines y direcciones I2C Oled
#define DISPLAY_ADDRESS 0x3C 

// Definiciones de timer
#define UPDATE_PERIOD_MS 15
#define MINUTE_INTERVAL_MS 60000 // 1 minuto en milisegundos
#define DELTA_TIME_CONSTANT 1.7e-5
double long currentTime = 0;
float delta_tiempo;

// Media Movil Delta del angulo
#define MOVING_AVERAGE_WINDOW 5
float deltaAngleHistory[MOVING_AVERAGE_WINDOW]; // Array para almacenar el historial de deltaAngle
int deltaAngleIndex; // Índice para actualizar el array circularmente

///// sumador de pwm
double  long previousMillis = 0; // Variable para almacenar el tiempo del último cambio de pwm
const long interval = 600; // Intervalo de tiempo en milisegundos (1 minuto)
double long prevTime;
double long milliseconds = 0; // Variable para almacenar los milisegundos transcurridos

// Variables Globales
uint16_t rawAngle;
uint16_t dutyCycle = 0; // Variable para almacenar el ciclo de trabajo del PWM // valores 0 - 4096
uint16_t pwm = 500; // valores 0 - 4096
float rpm=0;
float pwmup=250;

float anguloGrados;
float angAnterior = 0;
float deltaAngle = 0;

uint32_t elapsedMillis = 0; // Variable para almacenar el tiempo transcurrido

char buffer[500]; // Buffer para convertir los valores a cadena de caracteres

 

// Función para leer el ángulo crudo del sensor AS5600
uint16_t readRawAngle() {
    // Iniciar transmisión I2C al AS5600
    I2C_MasterSendStart(AS5600_ADDR, I2C_WRITE_XFER_MODE);
    I2C_MasterWriteByte(ANGLE_REG);
    I2C_MasterSendStop();  
    // Solicitar 2 bytes del ángulo
    I2C_MasterSendStart(AS5600_ADDR, I2C_READ_XFER_MODE);
    uint8_t msb = I2C_MasterReadByte(I2C_ACK_DATA);
    uint8_t lsb = I2C_MasterReadByte(I2C_NAK_DATA);
    I2C_MasterSendStop();
    rawAngle = (msb << 8) | lsb;
    rawAngle &= 0x0FFF; // Asegurarse de que es un valor de 12 bits
    return rawAngle;
}



CY_ISR(Timer_ISR_Handler) {
     // Incrementa la variable de milisegundos
    currentTime+=UPDATE_PERIOD_MS;
    rawAngle = readRawAngle();// Leer el ángulo crudo y convertir a grados
    delta_tiempo = currentTime - prevTime ; //segundos
    prevTime = currentTime;
    display_clear();  
    anguloGrados =  (rawAngle * 360) / 4096.0; // convertir a grados
    // Calcular la diferencia de ángulo
    if (anguloGrados > angAnterior) {
        deltaAngle = anguloGrados - angAnterior;
    } else if (anguloGrados < angAnterior) {
        deltaAngle = (360 - angAnterior) + anguloGrados;
    }
    angAnterior = anguloGrados;
    ///Media Movil
    deltaAngleHistory[deltaAngleIndex] = deltaAngle;
    deltaAngleIndex = (deltaAngleIndex + 1) % MOVING_AVERAGE_WINDOW;

    // Calcular la media móvil de deltaAngle
    float moving_deltaAngle = 0.0;
    for (int i = 0; i < MOVING_AVERAGE_WINDOW; i++) {
     moving_deltaAngle += deltaAngleHistory[i];
    }
    moving_deltaAngle /= MOVING_AVERAGE_WINDOW;

    // // Almacenar la media móvil en deltaAngle
    deltaAngle = moving_deltaAngle;
//    if (deltaAngle > 5.0){
//            deltaAngle = 0;
//        }
//    ///
    // Calcular RPM    
    double deltaTime = (double)delta_tiempo * DELTA_TIME_CONSTANT; // Convertir a minutos    
     if (deltaTime > 0) {
        float omega = deltaAngle / deltaTime;
        rpm =  omega * (1.0 / 360.0); // pulsos por revolución        
    }

    milliseconds++;
    
    //La condición milliseconds > 2000 se cumple aproximadamente cada 2000 * 15 ms, que es 30000 ms (aproximadamente 30 segundos).
    if  (milliseconds>(5000/15)){    
        pwm=1000;
        milliseconds = 0;
    }
    sprintf(buffer, "PWM, %d ,    Time, %.2Lf  ,     Crudo, %u   ,   Grados,   %.5f  , deltaAngle,  %.3f    ,  deltaTime,   %.2f   ,   RPM  ,  %.6f,\n",
                     dutyCycle, currentTime, rawAngle, anguloGrados, deltaAngle, delta_tiempo, rpm);
    UART_PutString(buffer);
    dutyCycle = pwm;
    LPWM_WriteCompare(dutyCycle); 
}
void writeRegister16(uint8_t reg, uint16_t value) {
    uint8_t data[3];
    data[0] = reg;
    data[1] = (uint8_t)(value >> 8); // MSB
    data[2] = (uint8_t)(value & 0xFF); // LSB
    
    // Iniciar transmisión I2C
    I2C_MasterSendStart(AS5600_ADDR, I2C_WRITE_XFER_MODE);
    I2C_MasterWriteByte(data[0]);
    I2C_MasterWriteByte(data[1]);
    I2C_MasterWriteByte(data[2]);
    I2C_MasterSendStop();
}

int main(void)
{
    
    /// valores para timer
    CyGlobalIntEnable; /* Habilita las interrupciones globales. */
    // Iniciar la comunicación serial
    UART_Start();
    // Iniciar los PWM
    RPWM_Start();
    LPWM_Start();
    //
    Clock_1_Start();
 
    // Inicializar el puerto I2C y Oled
    I2C_Start();
    display_init(DISPLAY_ADDRESS);
    // Configuración para alta frecuencia de actualización
  uint16_t configValue = 0;
  configValue |= (0b00 << 12); // PM: Nominal (para la mayor tasa de actualización)
  configValue |= (0b00 << 10); // Hysteresis: OFF
  configValue |= (0b00 << 8);  // Output stage: Analog (opcional, cambiar si se usa salida PWM)
  configValue |= (0b00 << 6);  // PWM Frequency: 115 Hz (opcional si se usa salida PWM)
  configValue |= (0b00 << 4);  // Slow filter: 16x (mínimo filtro)
  configValue |= 0b0000;       // Fast filter threshold: No fast filter

  writeRegister16(AS5600_CONF_MSB, configValue);
  UART_PutString("Configuration written for high frequency update!");

    RPWM_WriteCompare(0);
    LPWM_WriteCompare(0);
    // clearing display
    display_clear();    
    display_update();
    gfx_setTextSize(2);
    gfx_setTextColor(WHITE);
    gfx_setCursor(35,22);
    gfx_println("READ");
    display_update();    
    CyDelay(10000);
    // Escribir encabezado solo una vez
    UART_PutString("\r\nPWM, , Crudo, , Grados,  delta, deltaAngle\r\n");
    display_clear();    
    gfx_setCursor(40, 0);
    gfx_setTextSize(1);
    gfx_print("READING");
    gfx_setCursor(0,20);
    gfx_print("pwm:   ");
    sprintf(buffer, "%d",pwm);
    gfx_print(buffer);
    display_update();
       // Configurar el temporizador
    Timer1_Start();
    isr_StartEx(Timer_ISR_Handler); // Configurar ISR para el temporizador
 
    while (1) { 
            
    }
}

/* [] END OF FILE */
