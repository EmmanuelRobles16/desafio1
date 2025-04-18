# 🧾 Informe del Desafío 1

**Materia:** Informática II  
**Universidad:** Universidad de Antioquia  
**Estudiantes:** Diego Steven Ramírez y Emmanuel Robles  
**Profesores:** Aníbal Guerra y Augusto Salazar  
**Semestre:** 2025-1  

---

## 📘 Desafío 1

---

## 📝 1. Introducción

Este informe presenta el análisis y diseño de la solución para el Desafío 1 de Informática II. El reto consiste en recuperar una imagen BMP original que ha sido sometida a una serie de transformaciones a nivel de bits, tales como operaciones XOR, rotaciones y desplazamientos, y posteriormente enmascarada con una máscara de color. Para resolverlo, se debe implementar un sistema capaz de aplicar estas transformaciones en orden inverso y validar los resultados con archivos de rastreo proporcionados.

## 2. Metodología

### 2.1 Análisis del Problema

La imagen distorsionada (Iᴅ) que recibimos como entrada es el producto final de aplicar una secuencia desconocida de transformaciones a nivel de bits a la imagen original. Según las especificaciones del problema , estas transformaciones incluyen:

**1. Operaciones básicas a nivel de bits:**

 ***Operaciones XOR:*** Se aplicaron entre la imagen original/intermedia y una imagen aleatoria (IM) proporcionada. Esta operación es reversible aplicando el mismo XOR nuevamente.

***Rotaciones de bits:*** Se realizaron desplazamientos circulares de 1 a 8 bits (ej: rotación de 3 bits a la derecha). Su inversa sería rotar en dirección contraria la misma cantidad de bits.

***Desplazamientos de bits:*** A diferencia de las rotaciones, estos pierden los bits que salen del rango, por lo que su reversión requiere otras estrategias.

**2. Enmascaramiento:**

Después de cada transformación principal, se aplicó un enmascaramiento que consistió en: Seleccionar aleatoriamente una posición de inicio (s) en la imagen, sumar pixel a pixel una porción de la imagen con la máscara M proporcionada y luego guardar estos resultados en los archivos .txt (M1.txt, M2.txt, etc.)



**Datos de entrada proporcionados:**

Se nos da código base para:

- Abrir y manipular imágenes BMP (acceso a valores de píxeles mediante arreglos dinámicos)

- Procesar los archivos .txt con los resultados de enmascaramiento

- Exportar arreglos dinámicos como imágenes

- Dos ejemplos prácticos completos (Caso 1 y Caso 2) que incluyen: Imagen distorsionada final (Iᴅ), imagen aleatoria (IM), máscara (M), archivos .txt generados en cada etapa

**Características clave:**

1. Las transformaciones se aplicaron en orden desconocido 

2. Cada enmascaramiento usó una posición aleatoria diferente (s)

3. El último paso nunca incluyó enmascaramiento (según las especificaciones del pdf)

### 2.2 Alternativa de solución

Después de analizar cómo se podía solucionar el problema, repasar los temas vistos en clase y la explicación de los profesores en las clases, para este desafío llegamos a una posible solución, la cual se basa en la reversibilidad: cada acción debe tener la capacidad de ser revertida y comparada con los archivos de texto que se nos dieron, para verificar si coinciden.

Así que, para resolver el problema, se propone el siguiente enfoque:

***Aplicar transformaciones inversas:*** Se probarán diferentes combinaciones de operaciones inversas (como XOR y rotaciones en dirección opuesta) sobre la imagen distorsionada Io. Dado que el XOR es reversible (aplicarlo dos veces cancela el efecto), y las rotaciones pueden invertirse rotando en la dirección contraria, estas operaciones permiten recuperar estados anteriores de la imagen.

***Validación con archivos .txt:*** Después de cada transformación inversa, se aplicará el enmascaramiento a una porción de la imagen y se comparará con los datos del archivo .txt correspondiente. Si coinciden, significa que la transformación inversa fue correcta y se procederá a la siguiente etapa; de lo contrario, se probará otra combinación.

***Reconstrucción final:*** Una vez revertidas todas las transformaciones en el orden correcto, se obtendrá la imagen original.

Para manejar las imágenes de forma eficiente, utilizaremos memoria dinámica mediante arreglos bidimensionales y punteros, lo que exige especial cuidado en: (1) la correcta asignación y liberación de memoria, (2) el acceso seguro a los píxeles mediante aritmética de punteros, y (3) la prevención de fugas de memoria.




### 2.3 Planteamiento de funciones 

Cargar archivos:
- Iᴅ (imagen final)
- M (máscara de enmascaramiento)
- Archivos M1.txt, M2.txt, ..., MN.txt

Implementar funciones de transformación:
- XOR entre imágenes
- Rotación de bits izquierda/derecha
- Desplazamiento de bits izquierda/derecha

Implementar funciones inversas de las anteriores

Implementar verificación de enmascaramiento:
- Extraer segmento desde semilla 's'
- Sumar con la máscara
- Comparar con datos del archivo .txt

Algoritmo de reconstrucción:
- Probar combinaciones de transformaciones
- Validar paso a paso con archivos .txt
- Al validar todos los pasos → se obtiene imagen original IO

## 3. Conclusiones
  
-    Las transformaciones a nivel de bits (XOR, rotaciones) son reversibles matemáticamente, permiten reconstruir la imagen original al aplicar las operaciones inversas en el orden correcto.
  
-    Los archivos `.txt` nos ayudan a que no estemos en un proceso ciego, confirmando mediante comparación numérica si una transformación inversa aplicada es la correcta.
  
-    El uso de arreglos bidimensionales dinámicos mediante punteros requiere un buen manejo de memoria para evitar que no hayan fugas y accesos inválidos.

