/*
 * Programa demostrativo de manipulaciónprocesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 * Asistencia de ChatGPT para mejorar la forma y presentación del código fuente
 */

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);
void aplicarXOR(unsigned char* pixelData, unsigned char* imData, int totalBytes);
void rotarDerecha(unsigned char* pixelData, int totalBytes, int n);
void rotarIzquierda(unsigned char* pixelData, int totalBytes, int n);
void desplazarDerecha(unsigned char* pixelData, int totalBytes, int n);
void desplazarIzquierda(unsigned char* pixelData, int totalBytes, int n);
unsigned int* aplicarMascara(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, int seed, int maskWidth, int maskHeight);
bool verificarEnmascaramiento(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, const unsigned int*   maskingData, int seed, int maskSize);


int main() {
    // Definir nombres de los archivos de entrada y salida
    QString archivoEntrada = "I_D.bmp";    // Imagen distorsionada (P3.bmp para Caso 1)
    QString archivoMascara = "M.bmp";      // Máscara de suma
    QString archivoMascaraXOR = "I_M.bmp"; // Imagen de distorsión para XOR
    QString archivoSalida = "I_D_resultado.bmp"; // Imagen reconstruida

    // Variables para dimensiones de las imágenes
    int width = 0, height = 0;
    int maskWidth = 0, maskHeight = 0;

    // Cargar la imagen distorsionada (I_D)
    unsigned char* pixelData = loadPixels(archivoEntrada, width, height);
    if (pixelData == nullptr) {
        cout << "Error: No se pudo cargar I_D.bmp" << endl;
        return -1;
    }
    int totalBytes = width * height * 3; // Tamaño en bytes (3 por RGB)

    // Cargar la máscara de suma (M)
    unsigned char* pixelDataMask = loadPixels(archivoMascara, maskWidth, maskHeight);
    if (pixelDataMask == nullptr) {
        cout << "Error: No se pudo cargar M.bmp" << endl;
        delete[] pixelData;
        return -1;
    }

    // Cargar la imagen de distorsión (I_M)
    unsigned char* pixelDataMaskXOR = loadPixels(archivoMascaraXOR, width, height);
    if (pixelDataMaskXOR == nullptr) {
        cout << "Error: No se pudo cargar I_M.bmp" << endl;
        delete[] pixelData;
        delete[] pixelDataMask;
        return -1;
    }

    // Archivos de rastreo en orden inverso (M2.txt, M1.txt, M0.txt), ya que estamos realizando el proceso inverso
    const char* archivosMasking[] = {"M2.txt", "M1.txt", "M0.txt"};
    const int totalTransformaciones = 3; // Número de transformaciones para el Caso 1

    // Procesar cada etapa en orden inverso
    for (int etapa = 0; etapa < totalTransformaciones; etapa++) {
        cout << "\nProcesando etapa " << etapa + 1 << " con archivo " << archivosMasking[etapa] << endl;

        // Cargar el archivo de rastreo
        int seed = 0, n_pixels = 0;
        unsigned int* maskingData = loadSeedMasking(archivosMasking[etapa], seed, n_pixels);
        if (maskingData == nullptr) {
            cout << "Error: No se pudo cargar " << archivosMasking[etapa] << endl;
            delete[] pixelData;
            delete[] pixelDataMask;
            delete[] pixelDataMaskXOR;
            return -1;
        }
        int maskSize = n_pixels * 3; // Tamaño en bytes (3 por RGB)

        // Crear una copia de la imagen actual para probar transformaciones
        unsigned char* copia = new unsigned char[totalBytes];
        bool transformacionEncontrada = false;

        // Probar XOR
        memcpy(copia, pixelData, totalBytes);
        aplicarXOR(copia, pixelDataMaskXOR, totalBytes);
        if (verificarEnmascaramiento(copia, pixelDataMask, maskingData, seed, maskSize)) {
            cout << "Transformacion encontrada: XOR" << endl;
            memcpy(pixelData, copia, totalBytes); // Actualizar imagen
            transformacionEncontrada = true;
        }

        // Probar rotación izquierda (1 a 7 bits)
        for (int n = 1; n <= 7 && !transformacionEncontrada; n++) {
            memcpy(copia, pixelData, totalBytes);
            rotarIzquierda(copia, totalBytes, n);
            if (verificarEnmascaramiento(copia, pixelDataMask, maskingData, seed, maskSize)) {
                cout << "Transformacion encontrada: Rotacion izquierda de " << n << " bits" << endl;
                memcpy(pixelData, copia, totalBytes);
                transformacionEncontrada = true;
            }
        }

        // Probar rotación derecha (1 a 7 bits)
        for (int n = 1; n <= 7 && !transformacionEncontrada; n++) {
            memcpy(copia, pixelData, totalBytes);
            rotarDerecha(copia, totalBytes, n);
            if (verificarEnmascaramiento(copia, pixelDataMask, maskingData, seed, maskSize)) {
                cout << "Transformacion encontrada: Rotacion derecha de " << n << " bits" << endl;
                memcpy(pixelData, copia, totalBytes);
                transformacionEncontrada = true;
            }
        }

        // Probar desplazamiento izquierda (1 a 7 bits)
        for (int n = 1; n <= 7 && !transformacionEncontrada; n++) {
            memcpy(copia, pixelData, totalBytes);
            desplazarIzquierda(copia, totalBytes, n);
            if (verificarEnmascaramiento(copia, pixelDataMask, maskingData, seed, maskSize)) {
                cout << "Transformacion encontrada: Desplazamiento izquierda de " << n << " bits" << endl;
                memcpy(pixelData, copia, totalBytes);
                transformacionEncontrada = true;
            }
        }

        // Probar desplazamiento derecha (1 a 7 bits)
        for (int n = 1; n <= 7 && !transformacionEncontrada; n++) {
            memcpy(copia, pixelData, totalBytes);
            desplazarDerecha(copia, totalBytes, n);
            if (verificarEnmascaramiento(copia, pixelDataMask, maskingData, seed, maskSize)) {
                cout << "Transformacion encontrada: Desplazamiento derecha de " << n << " bits" << endl;
                memcpy(pixelData, copia, totalBytes);
                transformacionEncontrada = true;
            }
        }

        // Verificar si se encontró una transformación válida
        if (!transformacionEncontrada) {
            cout << "Error: No se encontro ninguna transformacion valida para la etapa " << etapa + 1 << endl;
            delete[] copia;
            delete[] maskingData;
            delete[] pixelData;
            delete[] pixelDataMask;
            delete[] pixelDataMaskXOR;
            return -1;
        }

        // Liberar memoria de la copia y el archivo de rastreo
        delete[] copia;
        delete[] maskingData;
    }

    // Exportar la imagen reconstruida
    if (exportImage(pixelData, width, height, archivoSalida)) {
        cout << "Imagen reconstruida guardada como " << archivoSalida.toStdString() << endl;
    } else {
        cout << "Error: No se pudo guardar la imagen reconstruida" << endl;
    }

    // Liberar memoria de las imágenes
    delete[] pixelData;
    delete[] pixelDataMask;
    delete[] pixelDataMaskXOR;

    return 0;
}


unsigned char* loadPixels(QString input, int &width, int &height){
/*
 * @brief Carga una imagen BMP desde un archivo y extrae los datos de píxeles en formato RGB.
 *
 * Esta función utiliza la clase QImage de Qt para abrir una imagen en formato BMP, convertirla al
 * formato RGB888 (24 bits: 8 bits por canal), y copiar sus datos de píxeles a un arreglo dinámico
 * de tipo unsigned char. El arreglo contendrá los valores de los canales Rojo, Verde y Azul (R, G, B)
 * de cada píxel de la imagen, sin rellenos (padding).
 *
 * @param input Ruta del archivo de imagen BMP a cargar (tipo QString).
 * @param width Parámetro de salida que contendrá el ancho de la imagen cargada (en píxeles).
 * @param height Parámetro de salida que contendrá la altura de la imagen cargada (en píxeles).
 * @return Puntero a un arreglo dinámico que contiene los datos de los píxeles en formato RGB.
 *         Devuelve nullptr si la imagen no pudo cargarse.
 *
 * @note Es responsabilidad del usuario liberar la memoria asignada al arreglo devuelto usando `delete[]`.
 */

    // Cargar la imagen BMP desde el archivo especificado (usando Qt)
    QImage imagen(input);

    // Verifica si la imagen fue cargada correctamente
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << std::endl;
        return nullptr; // Retorna un puntero nulo si la carga falló
    }

    // Convierte la imagen al formato RGB888 (3 canales de 8 bits sin transparencia)
    imagen = imagen.convertToFormat(QImage::Format_RGB888);

    // Obtiene el ancho y el alto de la imagen cargada
    width = imagen.width();
    height = imagen.height();

    // Calcula el tamaño total de datos (3 bytes por píxel: R, G, B)
    int dataSize = width * height * 3;

    // Reserva memoria dinámica para almacenar los valores RGB de cada píxel
    unsigned char* pixelData = new unsigned char[dataSize];

    // Copia cada línea de píxeles de la imagen Qt a nuestro arreglo lineal
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);              // Línea original de la imagen con posible padding
        unsigned char* dstLine = pixelData + y * width * 3;     // Línea destino en el arreglo lineal sin padding
        memcpy(dstLine, srcLine, width * 3);                    // Copia los píxeles RGB de esa línea (sin padding)
    }

    // Retorna el puntero al arreglo de datos de píxeles cargado en memoria
    return pixelData;
}

bool exportImage(unsigned char* pixelData, int width,int height, QString archivoSalida){
/*
 * @brief Exporta una imagen en formato BMP a partir de un arreglo de píxeles en formato RGB.
 *
 * Esta función crea una imagen de tipo QImage utilizando los datos contenidos en el arreglo dinámico
 * `pixelData`, que debe representar una imagen en formato RGB888 (3 bytes por píxel, sin padding).
 * A continuación, copia los datos línea por línea a la imagen de salida y guarda el archivo resultante
 * en formato BMP en la ruta especificada.
 *
 * @param pixelData Puntero a un arreglo de bytes que contiene los datos RGB de la imagen a exportar.
 *                  El tamaño debe ser igual a width * height * 3 bytes.
 * @param width Ancho de la imagen en píxeles.
 * @param height Alto de la imagen en píxeles.
 * @param archivoSalida Ruta y nombre del archivo de salida en el que se guardará la imagen BMP (QString).
 *
 * @return true si la imagen se guardó exitosamente; false si ocurrió un error durante el proceso.
 *
 * @note La función no libera la memoria del arreglo pixelData; esta responsabilidad recae en el usuario.
 */

    // Crear una nueva imagen de salida con el mismo tamaño que la original
    // usando el formato RGB888 (3 bytes por píxel, sin canal alfa)
    QImage outputImage(width, height, QImage::Format_RGB888);

    // Copiar los datos de píxeles desde el buffer al objeto QImage
    for (int y = 0; y < height; ++y) {
        // outputImage.scanLine(y) devuelve un puntero a la línea y-ésima de píxeles en la imagen
        // pixelData + y * width * 3 apunta al inicio de la línea y-ésima en el buffer (sin padding)
        // width * 3 son los bytes a copiar (3 por píxel)
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }

    // Guardar la imagen en disco como archivo BMP
    if (!outputImage.save(archivoSalida, "BMP")) {
        // Si hubo un error al guardar, mostrar mensaje de error
        cout << "Error: No se pudo guardar la imagen BMP modificada.";
        return false; // Indica que la operación falló
    } else {
        // Si la imagen fue guardada correctamente, mostrar mensaje de éxito
        cout << "Imagen BMP modificada guardada como " << archivoSalida.toStdString() << endl;
        return true; // Indica éxito
    }

}

unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels){
/*
 * @brief Carga la semilla y los resultados del enmascaramiento desde un archivo de texto.
 *
 * Esta función abre un archivo de texto que contiene una semilla en la primera línea y,
 * a continuación, una lista de valores RGB resultantes del proceso de enmascaramiento.
 * Primero cuenta cuántos tripletes de píxeles hay, luego reserva memoria dinámica
 * y finalmente carga los valores en un arreglo de enteros.
 *
 * @param nombreArchivo Ruta del archivo de texto que contiene la semilla y los valores RGB.
 * @param seed Variable de referencia donde se almacenará el valor entero de la semilla.
 * @param n_pixels Variable de referencia donde se almacenará la cantidad de píxeles leídos
 *                 (equivalente al número de líneas después de la semilla).
 *
 * @return Puntero a un arreglo dinámico de enteros que contiene los valores RGB
 *         en orden secuencial (R, G, B, R, G, B, ...). Devuelve nullptr si ocurre un error al abrir el archivo.
 *
 * @note Es responsabilidad del usuario liberar la memoria reservada con delete[].
 */

    // Abrir el archivo que contiene la semilla y los valores RGB
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        // Verificar si el archivo pudo abrirse correctamente
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    // Leer la semilla desde la primera línea del archivo
    archivo >> seed;

    int r, g, b;

    // Contar cuántos grupos de valores RGB hay en el archivo
    // Se asume que cada línea después de la semilla tiene tres valores (r, g, b)
    while (archivo >> r >> g >> b) {
        n_pixels++;  // Contamos la cantidad de píxeles
    }

    // Cerrar el archivo para volver a abrirlo desde el inicio
    archivo.close();
    archivo.open(nombreArchivo);

    // Verificar que se pudo reabrir el archivo correctamente
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    // Reservar memoria dinámica para guardar todos los valores RGB
    // Cada píxel tiene 3 componentes: R, G y B
    unsigned int* RGB = new unsigned int[n_pixels * 3];

    // Leer nuevamente la semilla desde el archivo (se descarta su valor porque ya se cargó antes)
    archivo >> seed;

    // Leer y almacenar los valores RGB uno por uno en el arreglo dinámico
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    // Cerrar el archivo después de terminar la lectura
    archivo.close();

    // Mostrar información de control en consola
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de píxeles leídos: " << n_pixels << endl;

    // Retornar el puntero al arreglo con los datos RGB
    return RGB;
}



void aplicarXOR(unsigned char* pixelData, unsigned char* imData, int totalBytes) {
    for (int i = 0; i < totalBytes; i++) {
        pixelData[i] = pixelData[i] ^ imData[i];
    }
}

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

unsigned int* aplicarMascara(const unsigned char* pixelDataTransformada, const unsigned char* pixelDataMask, int seed, int maskWidth, int maskHeight) {
    int maskSize = maskWidth * maskHeight * 3;  // total de componentes RGB
    int offset   = seed * 3;                   // desplazamiento en bytes

    unsigned int* resultado = new unsigned int[maskSize];
    for (int k = 0; k < maskSize; ++k) {
        resultado[k] = pixelDataTransformada[offset + k] + pixelDataMask[k];
    }
    return resultado;
}

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









