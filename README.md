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

- Dos conjuntos de datos de prueba (Caso 1 y Caso 2) que incluyen:

   -Imagen distorsionada final (Iᴅ)

   -Imagen aleatoria (IM)
  
   -Máscara (M)
  
   -Archivos .txt generados en cada etapa

**Características clave:**

1. Las transformaciones se aplicaron en orden desconocido (variable entre casos)

2. Cada enmascaramiento usó una posición aleatoria diferente (s)

3. El último paso nunca incluyó enmascaramiento (según especificaciones)


Despues de investigar en como se podia solucionar el problema y repasar los temas vistos en clase y la explicacion de los profesores en las clases para este desafio llegamos a una porsible solucion la cual se basa en la revesibilidad, cada accion que debe tener la capacidad de ser revertida y comparar con lo texto que se nos fueron dados y verificar si si coinciden


### Planteamiento de funciones 

Cargar archivos:
── ID (imagen final)
── M (máscara de enmascaramiento)
── Archivos M1.txt, M2.txt, ..., MN.txt

Implementar funciones de transformación:
   ─ XOR entre imágenes
    ─ Rotación de bits izquierda/derecha
   ─ Desplazamiento de bits izquierda/derecha

Implementar funciones inversas de las anteriores

 Implementar verificación de enmascaramiento:
 ── Extraer segmento desde semilla 's'
── Sumar con la máscara
  ── Comparar con datos del archivo .txt

 Algoritmo de reconstrucción:
   ── Probar combinaciones de transformaciones
 ─ Validar paso a paso con archivos .txt
── Al validar todos los pasos → se obtiene imagen original IO



