# Acondicionamiento de señales de alta impedancia — Laboratorio N.° 1

Instrumentación Biomédica III — Escuela Profesional de Ingeniería Biomédica, Universidad Nacional Mayor de San Marcos (UNMSM).
Docente: María Elisia Armas Alvarado.

## Descripción

Este repositorio contiene el código, el esquemático y los datos del Laboratorio N.° 1, cuyo objetivo es observar el efecto de carga (*loading error*) al medir una fuente de alta impedancia y corregirlo mediante un buffer seguidor de voltaje. Se compara un amplificador con entrada JFET (TL084) frente a uno con entrada bipolar (LM324).

Un ESP32 simula la señal de un electrodo de pH en tres puntos (pH 4, 7 y 10) mediante la ecuación de Nernst, y una resistencia de 1 MΩ en serie representa la impedancia interna del electrodo.

## Contenido

| Carpeta / archivo | Descripción |
|---|---|
| `codigo/generador_ph/generador_ph.ino` | Programa del ESP32 que genera la señal simulada de pH en GPIO25 |
| `esquematico/` | Esquemático del montaje |
| `datos/` | Datos medidos en cada etapa, en formato CSV |
| `fotos/` | Fotografías del montaje y de las mediciones |

## Materiales

TL084N, LM324N, resistencia de 1 MΩ, placa ESP32 con cable USB, protoboard, cables jumper, fuente dual ±9 V, dos capacitores cerámicos de 0,1 µF, osciloscopio y multímetro digital.

## Cómo reproducir el experimento

1. Abrir `codigo/generador_ph/generador_ph.ino` en el Arduino IDE, seleccionar una placa ESP32 en *Herramientas > Placa* y cargar el programa.
2. Abrir el Monitor Serial a 115200 baudios y escribir un valor de pH (por ejemplo, 7). El programa muestra el voltaje entregado en GPIO25.
3. **Etapa 1:** medir GPIO25 directamente con el osciloscopio para pH 4, 7 y 10.
4. **Etapa 2:** intercalar R1 = 1 MΩ entre GPIO25 y el punto de medición (Nodo A) y medir con multímetro y con osciloscopio.
5. **Etapa 3:** armar el buffer seguidor con el TL084 (pin 3, entrada no inversora, conectado al Nodo A; pin 1, salida, realimentado al pin 2; pin 4 a +9 V; pin 11 a −9 V; tierras de la fuente, el ESP32 y los instrumentos unidas) y medir en la salida (Nodo B). Repetir reemplazando el TL084 por un canal del LM324.

## Ecuación utilizada

El programa calcula el voltaje como:

E = 1,65 V − 5 × 0,05916 V/pH × (pH − 7)

El punto de pH 7 se desplaza a 1,65 V porque el DAC del ESP32 solo entrega entre 0 y 3,3 V, y la pendiente se multiplica por 5 para que la variación sea visible en el osciloscopio. Los voltajes teóricos resultantes son 2,537 V (pH 4), 1,650 V (pH 7) y 0,763 V (pH 10). Como el DAC es de 8 bits, el voltaje real entregado difiere ligeramente (ver `datos/tabla1_voltaje_teorico.csv`).

## Integrantes

- [Cava Leon, Elmer Alexis — 23190437]
- [Diaz Melendez, Sofia Marleny — 23190439]
- Enriquez Villalobos, Cristhian Anghelo — 23190440
- [Huarcaya Vasquez, Alice — 23190121]
