#include "sTan.h"

// sTan sınıfı: kayan ortalama ve eğim hesaplamak için kullanılır.

sTan::sTan() {}

void sTan::begin(uint16_t bufferSize) {
    // Tamponu oluştur ve varsayılan değer ile doldur.
    inputArray = new float[bufferSize];
    bufSize = bufferSize;
    sTan::init(0);
}

void sTan::init(float reading) {
    // Tüm tamponu aynı başlangıç değeri ile doldur.
    index = 0;
    sum = reading * bufSize;
    for (uint16_t i = 0; i < bufSize; i++) {
        inputArray[i] = reading;
    }
}

float sTan::avgVal(float reading) {
    // Yeni değeri ekle, en eski değeri çıkar, ortalamayı döndür.
    index++;
    if (index >= bufSize) {
        index = 0;
    }
    sum += reading - inputArray[index];
    inputArray[index] = reading;
    return float(sum / bufSize);
}

float sTan::startVal() {
    // En eski değeri döndür (tamponun kuyruğu).
    uint16_t tailIndex = index + 1;
    if (tailIndex >= bufSize) {
        tailIndex = 0;
    }
    return inputArray[tailIndex];
}

float sTan::slope(float reading) {
    // Başlangıç değerine göre eğimi hesapla.
    return reading - sTan::startVal();
}

uint16_t sTan::length() {
    return bufSize;
}
