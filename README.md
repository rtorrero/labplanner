# Descripción del Problema y Objetivo del Software

## 1. Introducción
En el Basque Culinary Center (BCC), los doctorandos deben diseñar y ejecutar experimentos para optimizar el crecimiento de microorganismos en medios de cultivo específicos. Para ello, utilizan diseños experimentales generados con *StatGraphics*, los cuales producen matrices de diseño como la de *Box-Behnken*. Estas matrices definen combinaciones de concentraciones de solutos y tiempos de fermentación que deben ser preparadas y procesadas en laboratorio.

El principal problema es que el proceso de preparación de estas muestras es manual y repetitivo, lo que genera errores y consume mucho tiempo. La preparación de los medios de cultivo requiere la conversión de unidades, la agrupación de solutos con concentraciones similares y la optimización de las cantidades para minimizar el error de medición. Actualmente, estos cálculos se realizan manualmente, lo que introduce imprecisiones y aumenta la carga de trabajo.

## 2. Objetivo del Software
El objetivo de este proyecto es desarrollar una aplicación en C++ con interfaz Qt que automatice el proceso de preparación de muestras experimentales. La aplicación permitirá:

1. **Importar archivos CSV** generados desde *StatGraphics* con el diseño experimental.
2. **Agrupar muestras** con valores similares de concentración para optimizar la preparación.
3. **Realizar conversiones de unidades automáticamente** (por ejemplo, de g/L a g/mL).
4. **Generar instrucciones detalladas** para la preparación de soluciones de manera eficiente.
5. **Minimizar errores de medición** agrupando grandes volúmenes de soluciones madre en lugar de hacer múltiples mediciones pequeñas.
6. **Exportar los resultados en un formato claro** para su uso en laboratorio.

---

## 3. Ejemplo de Formato de Archivo CSV Importado

A continuación, se muestra un ejemplo de un archivo CSV que el software deberá importar y procesar:

\`\`\`csv
BLOQUE;CS;DAP;Salt;Wet;Dry
1;70;2,5;10;36,5;13,82
1;40;0;10;41,11;12,88
1;100;0;10;71,97;15,14
1;40;5;10;41,31;13,48
1;100;5;10;41,9;14,57
1;40;2,5;5;53,4;14,15
1;100;2,5;5;55,98;8,67
1;70;2,5;5;32,61;12,72
1;40;2,5;15;27,61;14,4
1;100;2,5;15;62,14;16,77
1;70;0;5;37,28;5,73
1;70;5;5;51,95;10,64
1;70;0;15;63,04;11,85
1;70;5;15;45,91;16,02
1;70;2,5;10;57,94;9,68
\`\`\`

Importante: Al importar un fichero como este, el usuario deberá seleccionar qué columnas pertenecen a elementos que formarán parte de los compuestos a preparar, ya que es común la existencia de campos como Wet, Dry o Time que no hacen referencia a solutos.
---

## 4. Descripción de las Columnas del Archivo CSV

| **Columna** | **Descripción** |
|------------|----------------------------------------------|
| `BLOQUE`   | Grupo de experimentos que comparten ciertas condiciones. |
| `CS`       | Concentración de cebada malteada (g/L). |
| `DAP`      | Concentración de fosfato de amonio dibásico (g/L). |
| `Salt`     | Concentración de sal en la solución (g/L). |
| `Wet`      | Peso de la muestra en estado húmedo (g). |
| `Dry`      | Peso de la muestra en estado seco (g). |

---

## 5. Funcionalidades del Software
1. **Carga de Datos**
   - Importación de archivos CSV con las columnas especificadas.
   - Visualización previa de los datos antes del procesamiento.

2. **Agrupación y Optimización**
   - Detección de valores repetidos en las columnas de concentración.
   - Cálculo de las cantidades necesarias de cada soluto basado en frecuencias.
   - Optimización de volúmenes para reducir errores en la medición.

3. **Conversión de Unidades**
   - Transformación automática de g/L a g/mL según las necesidades del experimento.
   - Ajuste de volúmenes para evitar pérdidas por evaporación o trasvase.

4. **Generación de Instrucciones de Preparación**
   - Cálculo de mezclas madre y distribución en muestras individuales.
   - Exportación de las instrucciones en formato CSV o PDF para su uso en laboratorio.

5. **Interfaz de Usuario**
   - Desarrollo con *Qt* para una experiencia gráfica intuitiva.
   - Botón para cargar archivos y obtener instrucciones detalladas.

---

## 6. Beneficios Esperados
- **Ahorro de tiempo:** Automatización de cálculos que actualmente se hacen manualmente.
- **Reducción de errores:** Minimización de imprecisiones en la medición de pequeñas cantidades.
- **Mayor reproducibilidad:** Generación de instrucciones estandarizadas para la preparación de medios de cultivo.
- **Facilidad de uso:** Interfaz gráfica amigable que simplifica la carga y visualización de datos.

---
