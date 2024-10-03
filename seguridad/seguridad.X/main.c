/*!
\file   seguridad.c
\date   2022-06-28
\author Johan Paladinez <johandiaz@unicauca.edu.co>
\brief  Security House.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#include <xc.h>
#include <pic16f887.h>  // Header file PIC16f887 definitions
#include <stdio.h>
#include <string.h>
#include "CONFIG.h"
#include "LCD.h"
#include "KEYPAD.h"

// Definición de los pines de los LEDs
#define LED_VERDE PORTDbits.RD0
#define LED_AZUL PORTDbits.RD1
#define LED_ROJO PORTDbits.RD2


const char password[6] = {'2','0','2','1','1',0}; // Contraseña predefinida
char pass_user[6];
unsigned char idx = 0;
int intentos = 0;  // Contador de intentos fallidos


void main() {
    char key = '0';
    // Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71;  // Configura oscilador interno (FOSC = 8Mhz)
    
    TRISD = 0x00;   
    PORTD = 0x00;
    TRISB = 0x00; 
    ANSELH = 0x00;
    
    LCD_Init();     // Inicializa el LCD
    LCD_Load_Custom_Chars();  // Cargar caracteres especiales al LCD
    keypad_init();  // Inicializa el keypad

    while (1) {
        if (intentos < 3) {
            LCD_Clear();
            LCD_String_xy(0, 0, "Ingrese key:");
            LCD_Command(0xC0);  // Mover cursor a la segunda línea

            // Capturar los caracteres ingresados en el teclado
            idx = 0;
           
            do {
                key = keypad_getkey();
                if (key != 0) {
                    LCD_Char('*');  // Mostrar '*' en vez del carácter real
                    pass_user[idx++] = key;
                }
                __delay_ms(200);
            } while (idx < 8 && key != '#');  // Fin de ingreso con '#' o máximo 8 caracteres

            pass_user[idx] = 0;  // Terminar la cadena ingresada
            
            
            // Verificar si la clave es correcta
            if (strncmp(pass_user, password, 5) == 0) {
                // Clave correcta
                LCD_Clear();
                LCD_String_xy(0, 0, "Clave Correcta");
                LCD_Char(1);  // Mostrar el símbolo de check
                for (int i = 0; i < 6; i++) {  // Blink LED Verde por 3 segundos
                    LED_VERDE = 1;
                    __delay_ms(500);
                    LED_VERDE = 0;
                    __delay_ms(500);
                }
                intentos = 0;  // Reiniciar contador de intentos
            } else {
                // Clave incorrecta
                intentos++;
                LCD_Clear();
                LCD_String_xy(0, 0, "Clave Incorrecta");
                LCD_Char(2);  // Mostrar símbolo de X
                for (int i = 0; i < 3; i++) {  // Blink LED Azul por 2 segundos
                    LED_AZUL = 1;
                    __delay_ms(300);
                    LED_AZUL = 0;
                    __delay_ms(700);
                }
                __delay_ms(2000);
            }

            // Limpiar pantalla para nueva entrada
            LCD_Clear();
        } else {
            // Después de 3 intentos fallidos, bloquear el sistema
            LCD_Clear();
            LCD_String_xy(0, 0, "Sistema Bloqueado");
            LCD_Char(0);  // Mostrar el símbolo de candado
            for (int i = 0; i < 10; i++) {  // Blink LED Rojo por 8 segundos
                LED_ROJO = 1;
                __delay_ms(300);
                LED_ROJO = 0;
                __delay_ms(500);
            }
            __delay_ms(8000);  // Espera de 8 segundos
            intentos = 0;  // Reiniciar los intentos
            LCD_Clear();
        }
    }
}