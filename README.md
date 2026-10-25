GANGSTERS: ORGANIZED CRIME - COMPATIBILIDAD WINDOWS
Version experimental 0.1 - 25 octubre 2026

OBJETIVO
Adaptacion local del parche comunitario goc_compatability v0.2.0:
https://github.com/xsixbulletsx/goc_compatability
Creditos adicionales: cnc-ddraw y miniaudio. Se conservan sus avisos adjuntos.
Requiere una copia propia de Gangsters: Organized Crime, edicion GOG.

CORRECCIONES HEREDADAS DEL PARCHE COMUNITARIO
- Compatibilidad DirectDraw mediante cnc-ddraw.
- Proteccion ante dimensiones de sprite no positivas que causaban cierres en New Game.
- Reproduccion musical alternativa y configuracion por usuario.

CAMBIOS DE ESTA ADAPTACION
- Textos de botones tambien cuando el tamano de ventana coincide con el nativo.
- Coordenadas del dibujo de botones relativas a la superficie, no al escritorio.
- Resolucion de funciones reales de Windows para evitar conflictos entre wrappers.
- Cursor del juego alineado con la posicion real dentro de la ventana.
- Recuperacion de pulsaciones cortas mediante lectura de botones de Windows.
- Perfil de ventana con bordes; no fuerza pantalla completa.

VALIDACION REAL Y LIMITES
Probado en una PC con Windows 11, copia GOG cuyo EXE tiene SHA256:
78C66B2EEA09A4945FE29CA0F7EE19B936E3D9E9CB32D2EC4349ABE39DD4DB71
Se observo entrada a New Game, carga de partida, periodico y mapa.
El usuario confirmo cursor alineado y clics funcionando.
NO se certificaron guardar/cargar, partidas prolongadas, multijugador, todas las
resoluciones, otras GPU o equipos. No es una version estable universal.
No modifica reglas de economia, reclutamiento, combate ni diplomacia.
El EXE del juego permanece intacto; las correcciones se aplican en memoria.

INSTALACION
1. Cerrar el juego y conservar una copia de su carpeta/configuracion actual.
2. No combinar esta descarga con otras instalaciones de DxWrapper o DxWnd.
   Conservar y retirar primero sus DLL/configuraciones si estan presentes.
3. Copiar ddraw.dll, cnc-ddraw.dll, ddraw.ini, gangsters_patch.ini y Shaders
   junto a gangsters.exe. Conservar tambien GangstersPatch_info.
4. Ejecutar normalmente, sin exigir modo administrador ni compatibilidad antigua.
5. Esta descarga contiene el perfil probado de ventana. No incluye el watchdog
   usado durante el desarrollo ni altera las preferencias globales de Windows.

DESINSTALACION
Cerrar el juego y restaurar la copia previa de los archivos reemplazados.
Si antes no existian, retirar los archivos/carpetas que se copiaron con este paquete.
No borrar gangsters.exe, datos ni partidas del juego.

FUENTE
source contiene los dos archivos modificados y changes.patch contra el proyecto
comunitario. La compilacion usa su build.ps1 y Zig. El codigo original del juego,
sus graficos, musica y partidas NO forman parte de esta descarga.
La DLL experimental no esta firmada digitalmente.
