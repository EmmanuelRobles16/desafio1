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


## 2 Planteamiento de funciones

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



