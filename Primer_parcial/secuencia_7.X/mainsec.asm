; PIC16F887 Configuration Bit Settings
; Assembly source line config statements
#include "p16f887.inc"
; CONFIG1
; __config 0x20D5
 __CONFIG _CONFIG1, _FOSC_INTRC_CLKOUT & _WDTE_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOREN_OFF & _IESO_OFF & _FCMEN_OFF & _LVP_OFF
; CONFIG2
; __config 0x3FFF
 __CONFIG _CONFIG2, _BOR4V_BOR40V & _WRT_OFF
 
    LIST P=16F887

N EQU 0xD0      ; constante (EQU palabra reservada para guardar valores en espacios)
cont1 EQU 0x20  ;RETARDO 
cont2 EQU 0x21  ;RETARDO
cont3 EQU 0x22  ;CONTADOR

V1 EQU 0x23
V2 EQU 0x24

    ORG 0x00
    GOTO INICIO  ; GOTO comando para ir hacia
    
INICIO
    
    BCF STATUS,RP1 
    BSF STATUS,RP0  
    MOVLW 0x71
    MOVWF OSCCON ;RELOJ 8M
  
   
    MOVLW 0x10
    MOVWF TRISA 
    CLRF TRISB 
    
    BSF STATUS,RP1  
    CLRF ANSEL 
    
    BCF STATUS,RP0  ;BANCO 0
    BCF STATUS,RP1
 
    
SECUENCIA7  
   
        MOVLW 0x01
        MOVWF V1 
    
        MOVLW 0x80
        MOVWF V2
   
        MOVLW 0x05
        MOVWF cont3
    
LOOP1

        MOVF V1,0 
        IORWF V2,0
        MOVWF PORTB
    
	call DELAY
    
	RRF V1 ;izquierda
	RLF V2 ;derecha 
    
	BSF V1,7
	BSF V2,0
    
	DECFSZ cont3,1
	    GOTO LOOP1
;------------------------
  
  MOVLW 0x10
  MOVWF V1
  
  MOVLW 0x08
  MOVWF V2
  
  MOVLW 0x05
  MOVWF cont3
     
;--------------------------
  
LOOP2

    
	MOVF V1,0 
	IORWF V2,0
	MOVWF PORTB
    
	call DELAY
    
	RLF V1 ;izquierda
	RRF V2 ;derecha 
    
	BSF V1,4
	BSF V2,3
    
	DECFSZ cont3,1
            GOTO LOOP2 
                   GOTO SECUENCIA7 
DELAY
  call RETARDO
  call RETARDO
  call RETARDO
  call RETARDO
  call RETARDO
  call RETARDO 
  return 

RETARDO
    MOVLW N
    MOVWF cont1
    
REP_1
    MOVLW N
    MOVWF cont2
    
REP_2
    DECFSZ cont2,1
    GOTO REP_2
    DECFSZ cont1,1
    GOTO REP_1
    RETURN
    
 End



