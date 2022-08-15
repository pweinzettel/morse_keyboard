# morse_keyboard

Proyecto para usar un manipulador morse como teclado en una PC o celular, o usar solo con la pantalla LCD que se le puede conectar

Incluye un pequeño entrenador web <https://www.lu9abm.com/morse/game> La idea es ir agregandole funcionalidades

Este proyecto se encuentra en desarrollo

Dentro del archivo morse_keyboard.hpp se configuran los pines donde van conectados los componentes al arduino, los pines del manipular DEBEN ser interrupt

Esta pensado para utilizarse en placas basadas en el armega 32u4, que que posee la posibilidad de ser usado como dispositivo HID

Estoy en proceso de portar el programa a una libreria que maneje todo y abstraerla del ino principal, para poder hacer una segunda version que pueda usar cualquier otra placa arduino y se comunique por bluetooth para simular el teclado, muy util sobre todo para poder usarlo con celulares sin necesidad que soporte USB-OTG y/o contar con el cable.

En esta segunda version, seria muy interesante agragarle pack de baterias

A medida que vaya avanzando el proyecto, lo voy a ir documentando para que sea facil de implementar
