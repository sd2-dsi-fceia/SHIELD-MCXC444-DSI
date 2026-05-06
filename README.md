# SHIELD-MCXC444-DSI
Repositorio de ejemplos y drivers para el shield MCXC-444 DSI  
DSI - FCEIA - UNR

## Descripción general

Este repositorio contiene implementaciones, drivers y ejemplos prácticos para el uso didáctico del shield MCXC-444 
Pensado para aplicaciones docentes en Sistemas Digitales 2 - Ingeniería Electrónica, abarca periféricos soldadaos en el shield y su manejo desde firmware propio.

## Estructura

Cada carpeta contiene un proyecto autocontenible con todo lo necesario para compilar, flashear y probar el uso de un recurso en particular del shield:

- **SD2_1_MCXC444_TestShield_GPIO**  
  Demostración y prueba de manejo de LEDs, pulsadores, relé y salidas digitales del shield.

- **SD2_2_MCXC444_TestShield_OLED**  
  Uso del display OLED gráfico del shield, mostrando gráficos, texto y funciones de inicialización.

- **SD2_3_MCXC444_TestShield_RS485**  
  Comunicación RS485 entre el shield y una PC (usando conversor USB→RS485 externo), con intercambio de datos a través de botones/pantalla.

- **SD2_4_MCXC444_TestShield_CAN-BUS**  
  Comunicación CAN BUS entre la placa Shield y otro dispositivo que tenga esa comunicación.
... 

## Cómo compilar y probar

1. Abrir la carpeta del ejemplo deseado.
2. Importar el proyecto en tu IDE (MCUXpresso, Keil, etc) o compilar según instrucciones de cada carpeta.
3. Consultá el README propio de cada ejemplo para instrucciones específicas sobre conexión, pruebas y terminal serie si aplica.

## Contribución

Todos los proyectos fueron desarrollados para la cátedra Sistemas Digitales 2 - DSI - FCEIA - UNR.  
Colaboraciones, mejoras y ejemplos de nuevos periféricos son bienvenidos mediante pull request o issues.

---
