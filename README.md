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



--- 

## Algoritmos implementados y la documentación de sus funciones
**Operaciones a nivel de bits**

***1 función:***

La primera función desarrollada para resolver el desafío fue la de XOR. Se consideraron dos formas de implementarla, pero principalmente se decidió por una versión con memoria dinámica, que creaba un puntero para reescribir todo el buffer original y luego aplicaba el XOR. Sin embargo, esta versión resultó menos eficiente, comparada con la que no usa punteros; se implementaron ambas y se compararon:
```cpp
unsigned char* aplicarXORDinamico(const unsigned char* pixelData, const unsigned char* imData, int totalBytes)
{
    unsigned char* resultado = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; ++i) {
        resultado[i] = pixelData[i] ^ imData[i];
    }
    return resultado;
}
```

En cambio, la versión que se empleó para el desafío no es muy diferente en cuanto a funcionamiento, pero resuelve el mismo problema sin necesidad de reservar memoria adicional ni de reescribir todo el buffer original:

```cpp
void aplicarXOR(unsigned char* pixelData, unsigned char* imData, int totalBytes) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = pixelData[i] ^ imData[i];
    }
}
```

Lo que hace esta función es tomar el arreglo de la imagen distorsionada, el arreglo de la imagen máscara y la cantidad de valores en total (bytes). Recorre cada posición desde 0 hasta totalBytes - 1 (ya que se usa < y no <=). En cada posición toma el valor de pixelData[i] y el de imData[i], les aplica la lógica del XOR, que en C++ se representa con ^, y guarda este resultado en pixelData[i].

***2 y 3 función:***

Estas fueron las siguientes funciones desarrolladas, son las funciones de rotaciones. Usamos la misma lógica que con la funcion XOR solo que aplicando la operacion de rotar bits a la derecha y a la izquierda, implementamos estas dos ya que no sabemos cual de las dos rotaciones es la que nos servirá, las funciones que creamos fueron las siguientes: 
```cpp
void rotarDerecha(unsigned char* pixelData, int totalBytes, int n) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = ((pixelData[i] >> n) | (pixelData[i] << (8 - n))) & 0xFF;
    }
}

void rotarIzquierda(unsigned char* pixelData, int totalBytes, int n) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = ((pixelData[i] << n) | (pixelData[i] >> (8 - n))) & 0xFF;
    }
}
```
& 0xFF garantiza que solo operamos con un byte (8 bits), ignorando cualquier extensión de signo o bits superiores. Evita resultados incorrectos debido a bits no deseados.

***4 y 5 función***

Estas fueron las siguientes funciones desarrolladas, son las funciones de desplazamiento. Usamos la misma lógica que con la funcion XOR y función 2 y 3, solo que aplicando la operacion de desplazar bits a la derecha y a la izquierda, implementamos estas dos ya que no sabemos cual de las dos rotaciones es la que nos servirá, las funciones que creamos fueron las siguientes: 

```cpp
void desplazarDerecha(unsigned char* pixelData, int totalBytes, int n) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = pixelData[i] >> n; // pierde bits
    }
}

void desplazarIzquierda(unsigned char* pixelData, int totalBytes, int n) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = (pixelData[i] << n) & 0xFF;
    }
}
```

Esta funcion hace que cuando se desplaze hacia la derecha se pierdan los bits menos significativos que es la derecha y entran ceros por los bits mas significativos que es la izquierda, y con la función de desplazar hacia la izquierda es la inversa pero se usa &0xFF para matener el resulta de los 8 bits como en las funciones 2 y 3

**Aplicar mascara**

***6 función:***

La siguiente función que implementamos fue una para aplicar la mascara, esto debido a que ideamos las funciones según el orden en el que las ibamos a necesitar, el bloque de codigo implementado fue:

```cpp
unsigned int* aplicarMascara(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, int seed, int maskWidth, int maskHeight) {
    int maskSize = maskWidth * maskHeight * 3;  // total de componentes RGB
    int offset   = seed * 3;                   // desplazamiento en bytes

    unsigned int* resultado = new unsigned int[maskSize];
    for (int k = 0; k < maskSize; ++k) {
        resultado[k] = static_cast<unsigned int>(pixelDataTransformada[offset + k])
        + static_cast<unsigned int>(pixelDataMask[k]);
    }
    return resultado;
}
```
La función aplicarMascara recibe el buffer de la imagen ya transformada (pixelDataTransformada), el buffer de la máscara RGB (pixelDataMask), la semilla de desplazamiento en píxeles (seed) y las dimensiones de la máscara (maskWidth, maskHeight). Primero convierte ese desplazamiento en bytes multiplicando seed por 3 y calcula el tamaño total de la máscara como maskWidth * maskHeight * 3. Luego reserva dinámicamente un arreglo de enteros de ese tamaño y recorre cada componente del canal, sumando el valor correspondiente de la imagen transformada (a partir del offset) con el de la máscara. El resultado de cada suma se almacena en el nuevo arreglo que ya creamos y así al terminar éste contiene las sumas para cada píxel de la máscara.

En esta función nosotros usamos un static_cast el cual habíamos investigado que deja explícito que queremos trabajar con un entero de 32 bits sin signo para almacenar la suma. Sin embargo, al ser algo que no vimos en clase podía generar algún error o confusión, así que lo quitamos ya que de igual forma debería funcionar, quedando así la función:

```cp
unsigned int* aplicarMascara(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, int seed, int maskWidth, int maskHeight) {
    int maskSize    = maskWidth * maskHeight * 3;
    int offsetBytes = seed * 3;

    unsigned int* resultado = new unsigned int[maskSize];
    for (int k = 0; k < maskSize; ++k) {
        resultado[k] = pixelDataTransformada[offsetBytes + k]
                     + pixelDataMask[k];
    }
    return resultado;
}
```

**funcion de verificación**

***funcion 7***

Esta función recibe la imagen ya transformada (pixelDataTransformada), la máscara (pixelDataMask), los datos leídos del archivo TXT (maskingData), la semilla (seed) y el tamaño total de los canales de la máscara (maskSize). Primero convierte la semilla en un desplazamiento en bytes multiplicando seed por tres. Luego itera desde cada componente k de 0 a maskSize - 1, computa la suma esperada imagen transformada más la máscara (esperado), y la compara con el valor presente en maskingData[k]. Si alguna suma no coincide, devuelve false inmediatamente; si todas coinciden, devuelve true. Así es cómo sabes si el enmascaramiento aplicado efectivamente es un enmascaramiento o no, ya que coincide con los datos del archivo.

```cp
bool verificarEnmascaramiento(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, const unsigned int*   maskingData, int seed, int maskSize) {
    int offsetBytes = seed * 3;  // convertir píxeles a componentes RGB
    for (int k = 0; k < maskSize; ++k) {
        unsigned int esperado = pixelDataTransformada[offsetBytes + k]+ pixelDataMask[k];
        if (esperado != maskingData[k]) {
            return false;
        }
    }
    return true;
}

```

--- 

## Problemas de desarrollo que se afrontó 

Principalmente los problemas de desarrollo que afrontamos fueron en el main, donde ya teníamos que implementar las funciones que habiamos creado:

- Al inicio no sabíamos como funcionaba la operación de rotación a nivel de bits así que tuvimos que repasar para poder realizar las funciones 

- Uno de los problemas fue el como mantener la imagen distorsionada en caso de que la transformación aplicada no fuera correcta.
  
- Otro problema fue el de cual función de transformación ibamos a probar primero.
  
- En una de las partes donde mas se nos dificultó fue comparar nuestro resultado con el txt ya que en todos los intentos nos salió que no concordaba nuestro resultado con el txt.

- Al no poder pasar la primera verificacion con el txt, no pudimos ver las otras verificaciones con los otros archivos de texto.
      
