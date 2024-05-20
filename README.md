# Control de Relés mediante Comandos Serie

Este es un programa de Arduino que controla dos relés mediante comandos serie. Los comandos permiten activar, desactivar y reiniciar el sistema.

## Requisitos

- Placa de desarrollo Arduino compatible
- Dos relés
- Conexión serial con un dispositivo (por ejemplo, un ordenador)

## Instalación

1. Descarga el código fuente.
2. Abre el archivo en el IDE de Arduino.
3. Carga el programa en tu placa de desarrollo.

## Uso

El programa espera comandos serie para controlar los relés. Aquí están los comandos disponibles:

- `00S01B`: Consultar estado.
- `0011SA`: Activar Rele 1.
- `00S219`: Activar Rele 2.
- `00R11B`: Desactivar Rele 1.
- `00R21A`: Desactivar Rele 2.
- `00X016`: Reiniciar.

Puedes enviar estos comandos a través de la conexión serial (a 9600 baudios) para controlar los relés.

## Contribuciones

Las contribuciones son bienvenidas. Si encuentras algún error o tienes alguna mejora, no dudes en abrir un issue o enviar un pull request.

## Licencia

Este proyecto está bajo la [Licencia MIT](LICENSE).
