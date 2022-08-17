# morse_keyboard

Proyecto para usar un manipulador morse (iambico) como teclado en una PC o celular, o usar solo con la pantalla LCD que se le puede conectar

Incluye un pequeño entrenador web <https://www.lu9abm.com/morse/game> La idea es ir agregandole funcionalidades

Este proyecto se encuentra en desarrollo


## Composicion

En el proyecto actual, la placa debe ser un Arduino Micro, pero entiendo que cualquier otro con el Atmega32U4 deberia funcionar


## Hardware

Sacando el Arduino Micro, cuenta con 3 leds y 4 botones, un display LCD (opcional) y un parlante para los tonos


### LEDS

1- Indica cuando se termina el tiempo de caracter

2- Indica cuando se termina el tiempo de palabra y agrega espacio automaticamente

3- Marca los pulsos, podria conectarse un opto y conectarlo a un equipo externo (incluso raido)


### Botones fisicos

Cuenta con 4 botones fisicos, los mismos sirven para:

1- Eliminear linea completa (simula teclas en secuencia: INI > INI > SHIFT > END > DEL)

2- Backspace

3- Enter

4- CTRL + Enter (enviar en algunas apps)


### LCD

Es opcional, pero util


## Set WPM

Para setear los WPM se debe mantener apretado el boton 2 y con 3 y 4 se modifica, con saltos de a 5 WPM


## NOTAS

Dentro del archivo morse_keyboard.hpp se configuran los pines donde van conectados los componentes al arduino, los pines del manipular DEBEN ser interrupt

Estoy en proceso de portar el programa a una libreria que maneje todo y abstraerla del ino principal, para poder hacer una segunda version que pueda usar cualquier otra placa arduino y se comunique por bluetooth para simular el teclado, muy util sobre todo para poder usarlo con celulares sin necesidad que soporte USB-OTG y/o contar con el cable.

Por lo anterior comentado, el codigo puede estar desordenado, pero estoy en eso... y siempre que subo, queda funcional y compatible con versiones anteriores SALVO INDIQUE LO CONTRARIO en algun futuro

En esta segunda version, seria muy interesante agragarle pack de baterias

A medida que vaya avanzando el proyecto, lo voy a ir documentando para que sea facil de implementar
