/*
 * Gabriel Carrera 21216 y Luis Pedro Gonz�lez 21513
 * Proyecto #3 ED2 - Parqueo TIVAWARE
 */

// Inclusi�n de Librer�as necesarias para el proyecto
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h" // Biblioteca espec�fica para TIVA C series TM4C123G
#include "inc/hw_memmap.h" // Mapeo de memoria de hardware
#include "inc/hw_types.h" // Tipos de datos de hardware
#include "inc/hw_ints.h" // Manejo de interrupciones
#include "driverlib/sysctl.h" // Biblioteca del controlador del sistema
#include "driverlib/interrupt.h" // Biblioteca para manejo de interrupciones
#include "driverlib/gpio.h" // Biblioteca del controlador GPIO
#include "driverlib/timer.h" // Biblioteca del controlador de temporizadores
#include "driverlib/systick.h" // Biblioteca del controlador SysTick
#include "driverlib/uart.h" // Biblioteca del controlador UART
#include "driverlib/pin_map.h" // Biblioteca para mapeo de pines
#include "driverlib/debug.h" // Biblioteca de depuraci�n
#include "driverlib/adc.h" // Biblioteca del controlador ADC
#include "utils/ustdlib.h" // Biblioteca de utilidades est�ndar

#define XTAL 16000000 // Definici�n de la frecuencia del cristal externo (16 MHz)

// Declaraci�n de variables globales
int8_t spot1;   // Estado del parqueo 1
int8_t spot2;   // Estado del parqueo 2
int8_t spot3;   // Estado del parqueo 3
int8_t spot4;   // Estado del parqueo 4
char available; // Indicador de parqueos disponibles
uint32_t i;     // Variable para uso en delays

// Declaraci�n de funciones
void setup(void);                       // Funci�n de configuraci�n inicial
void setupUART1(void);                  // Funci�n de configuraci�n del UART1
void UART1_SendString(const char *str); // Funci�n para enviar cadenas de texto por UART1
void UART1_SendInteger(int8_t num);     // Funci�n para enviar enteros por UART1
void delay1ms(void);                    // Funci�n para generar un delay de 1ms
void delay(uint32_t msec);              // Funci�n para generar un delay en milisegundos

// Funci�n principal
int main(void){
    setup(); // Inicializar la configuraci�n del sistema
    setupUART1(); // Inicializar la configuraci�n del UART1

    // Bucle infinito para el monitoreo continuo de los parqueos
    while (1){
        // Monitoreo y control del PARQUEO 1
        if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) == 0){ // Verificar si el parqueo 1 est� ocupado
            spot1 = 1; // Marcar el parqueo 1 como ocupado
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0); // Apagar el LED verde del parqueo 1
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1); // Encender el LED rojo del parqueo 1
        }
        else {
            spot1 = 0; // Marcar el parqueo 1 como libre
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0); // Encender el LED verde del parqueo 1
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0); // Apagar el LED rojo del parqueo 1
        }

        // Monitoreo y control del PARQUEO 2
        if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6) == 0){ // Verificar si el parqueo 2 est� ocupado
            spot2 = 1; // Marcar el parqueo 2 como ocupado
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, 0); // Apagar el LED verde del parqueo 2
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3); // Encender el LED rojo del parqueo 2
        }
        else {
            spot2 = 0; // Marcar el parqueo 2 como libre
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2); // Encender el LED verde del parqueo 2
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0); // Apagar el LED rojo del parqueo 2
        }

        // Monitoreo y control del PARQUEO 3
        if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5) == 0){ // Verificar si el parqueo 3 est� ocupado
            spot3 = 1; // Marcar el parqueo 3 como ocupado
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0); // Apagar el LED verde del parqueo 3
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2); // Encender el LED rojo del parqueo 3
        }
        else {
            spot3 = 0; // Marcar el parqueo 3 como libre
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1); // Encender el LED verde del parqueo 3
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0); // Apagar el LED rojo del parqueo 3
        }

        // Monitoreo y control del PARQUEO 4
        if (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) == 0){ // Verificar si el parqueo 4 est� ocupado
            spot4 = 1; // Marcar el parqueo 4 como ocupado
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0); // Apagar el LED verde del parqueo 4
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1); // Encender el LED rojo del parqueo 4
        }
        else {
            spot4 = 0; // Marcar el parqueo 4 como libre
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3); // Encender el LED verde del parqueo 4
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0); // Apagar el LED rojo del parqueo 4
        }

        // Procesamiento de los estados de los parqueos para env�o por UART
        spot2 = spot2 << 1; // Desplazamiento de 1 bit para alinear los estados en una variable de 4 bits
        spot3 = spot3 << 2; // Desplazamiento de 2 bits para alinear los estados
        spot4 = spot4 << 3; // Desplazamiento de 3 bits para alinear los estados
        available = spot1 | spot2 | spot3 | spot4; // Combinaci�n de los estados para representar los parqueos disponibles en una sola variable, de manera p1p2p3p4
        UARTCharPutNonBlocking(UART1_BASE, available); // Env�o del estado de los parqueos a trav�s de UART1 al ESP32
    }
}

// Funci�n para configurar los par�metros iniciales del sistema
void setup(void){
    // Configurar el reloj del sistema a 40MHz utilizando un cristal externo de 16MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // Habilitar y esperar a que est�n listos los puertos GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        // Habilitar el puerto A
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)); // Esperar a que el puerto A est� listo
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        // Habilitar el puerto B
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)); // Esperar a que el puerto B est� listo
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);        // Habilitar el puerto D
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)); // Esperar a que el puerto D est� listo
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);        // Habilitar el puerto E
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)); // Esperar a que el puerto E est� listo
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        // Habilitar el puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Esperar a que el puerto F est� listo

    // Configurar pines como salidas o entradas
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);  // Configurar pines D0-D3 como salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);               // Configurar pines E1-E3 como salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);                                         // Configurar pin F1 como salida
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);   // Configurar pines A4-A7 como entradas

    IntMasterEnable(); // Habilitar las interrupciones globales
}

// Funci�n para configurar el m�dulo UART1
void setupUART1(void){
    // Habilitar los perif�ricos necesarios para UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); // Habilitar UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // Habilitar puerto B para UART1

    // Configurar los pines para RX y TX
    GPIOPinConfigure(GPIO_PB0_U1RX); // Configurar pin B0 como RX
    GPIOPinConfigure(GPIO_PB1_U1TX); // Configurar pin B1 como TX
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); // Habilitar pines B0 y B1 para UART1

    // Configurar los par�metros de UART1
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // Configurar UART1 a 115200 baudios, 8 bits de dato, 1 bit de parada, sin paridad

    UARTEnable(UART1_BASE); // Habilitar UART1
}

// Manejador de interrupciones para UART1 (no utilizado en este c�digo)
void UART1IntHandler(void){
    UARTIntClear(UART1_BASE, UART_INT_RX | UART_INT_RT); // Limpiar las interrupciones de recepci�n y tiempo de espera
    while(UARTCharsAvail(UART1_BASE)){ // Mientras haya caracteres disponibles
        UARTCharPutNonBlocking(UART1_BASE, available); // Enviar el car�cter disponible
    }
}

// Funci�n para enviar una cadena de caracteres a trav�s de UART1
void UART1_SendString(const char *str){
    while (*str){ // Recorrer cada car�cter de la cadena
        while (!UARTCharPutNonBlocking(UART1_BASE, *str)); // Esperar y enviar cada car�cter
        str++; // Avanzar al siguiente car�cter
    }
}

// Funci�n para enviar un n�mero entero a trav�s de UART1
void UART1_SendInteger(int8_t num){
    char buf[20]; // Buffer para almacenar la representaci�n de cadena del n�mero
    sprintf(buf, "%ld", num); // Convertir el n�mero a cadena
    UART1_SendString(buf); // Enviar la cadena convertida
}

// Funci�n para generar un retardo en milisegundos
void delay(uint32_t msec){
    for (i = 0; i < msec; i++){ // Repetir el ciclo por el n�mero especificado de milisegundos
        delay1ms(); // Llamar a la funci�n de retardo de 1ms
    }
}

// Funci�n para generar un retardo de aproximadamente 1 milisegundo
void delay1ms(void){
    SysTickDisable(); // Desactivar SysTick
    SysTickPeriodSet(40000-1); // Configurar el per�odo de SysTick para 1ms (ajustado al reloj del sistema)
    SysTickEnable(); // Activar SysTick

    while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) == 0); // Esperar hasta que el contador de SysTick llegue a 0
}
