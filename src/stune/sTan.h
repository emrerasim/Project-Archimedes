#ifndef STAN_H
#define STAN_H

#include <stdint.h>

// sTan: kayan pencere ile ortalama ve eğim hesabı yapan yardımcı sınıf.
class sTan {
public:
    sTan();

    // Tampon boyutunu ayarlar ve başlangıç değerini yükler.
    void begin(uint16_t bufferSize);

    // Tamponu belirli bir okuma ile başlatır.
    void init(float reading);

    // Yeni okuma ekler ve ortalamayı döndürür.
    float avgVal(float reading);

    // Tampondaki en eski değeri döndürür.
    float startVal();

    // Başlangıç değerine göre eğim (delta) hesaplar.
    float slope(float reading);

    // Tampon uzunluğunu döndürür.
    uint16_t length();

private:
    float *inputArray = nullptr;
    uint16_t bufSize = 0;
    uint16_t index = 0;
    float sum = 0.0f;
};

#endif
