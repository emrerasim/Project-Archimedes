#ifndef STUNE_H
#define STUNE_H

/****************************************************************************************
   sTune Library for Arduino - Version 2.4.0
   by dlloydev https://github.com/Dlloydev/sTune
   Licensed under the MIT License.

   Bu sürüm Project-Archimedes için Türkçe açıklamalarla uyarlanmıştır.
   sTune, açık çevrim PID autotune algoritması sağlar.
 ****************************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "sTan.h"

// PID ayar yöntemi seçenekleri.
enum TuningMethod : uint8_t {
    ZN_PID,
    DampedOsc_PID,
    NoOvershoot_PID,
    CohenCoon_PID,
    Mixed_PID,
    ZN_PI,
    DampedOsc_PI,
    NoOvershoot_PI,
    CohenCoon_PI,
    Mixed_PI
};

// Kontrol yönü seçenekleri.
enum Action : uint8_t {
    directIP,
    direct5T,
    reverseIP,
    reverse5T
};

// Seri çıktı seçenekleri.
enum SerialMode : uint8_t {
    printALL,
    printDEBUG,
    printSUMMARY,
    printNONE
};

// Tuner durum makinesi seçenekleri.
enum TunerStatus : uint8_t {
    test,
    sample,
    tunings,
    runPid,
    timerPid
};

class sTune {
public:
    sTune();
    sTune(float *input, float *output, TuningMethod tuningMethod, Action action, SerialMode serialMode);

    // Başlangıç durumuna döndürür.
    void Reset();

    // Test parametrelerini ayarlar.
    void Configure(float inputSpan, float outputSpan, float outputStart, float outputStep,
                   uint32_t testTimeSec, uint32_t settleTimeSec, uint16_t samples);

    // Tuning testini çalıştırır.
    uint8_t Run();

    // Acil durdurma eşiği ayarı.
    void SetEmergencyStop(float e_Stop);

    // Kontrol yönünü ayarlar.
    void SetControllerAction(Action Action);

    // Seri çıktı modunu ayarlar.
    void SetSerialMode(SerialMode SerialMode);

    // Tuning yöntemini ayarlar.
    void SetTuningMethod(TuningMethod TuningMethod);

    // Örnekleri plotter'a basar.
    void printPidTuner(uint8_t everyNth);

    // Arduino plotter için formatlı çıktı.
    void plotter(float input, float output, float setpoint, float outputScale, uint8_t everyNth);

    // Test sürecini yazdırır.
    void printTestRun();

    // Ayar sonuçlarını yazdırır.
    void printTunings();

    // Tüm sonuçları yazdırır.
    void printResults();

    // PID katsayılarını döndürür.
    void GetAutoTunings(float *kp, float *ki, float *kd);

    float GetKp();
    float GetKi();
    float GetKd();
    float GetTi();
    float GetTd();

    float GetProcessGain();
    float GetDeadTime();
    float GetTau();

    uint8_t GetControllerAction();
    uint8_t GetSerialMode();
    uint8_t GetTuningMethod();

    // Yazılımsal PWM (SSR için) üretir.
    float softPwm(uint8_t relayPin, float input, float output, float setpoint, uint32_t windowSize, uint8_t debounce);

private:
    float *_input = nullptr;
    float *_output = nullptr;

    TuningMethod _tuningMethod = ZN_PID;
    Action _action = directIP;
    SerialMode _serialMode = printALL;
    TunerStatus _tunerStatus = test;

    uint32_t usPrev = 0;
    uint32_t settlePrev = 0;
    uint32_t usStart = 0;
    uint32_t ipUs = 0;
    uint32_t us = 0;

    float _Ku = 0.0f;
    float _Tu = 0.0f;
    float _td = 0.0f;
    float _kp = 0.0f;
    float _ki = 0.0f;
    float _kd = 0.0f;

    float pvIp = 0.0f;
    float pvMax = 0.0f;
    float pvPk = 0.0f;
    float slopeIp = 0.0f;
    float pvTangent = 0.0f;
    float pvTangentPrev = 0.0f;
    float pvAvg = 0.0f;
    float pvStart = 0.0f;
    float pvInst = 0.0f;
    float pvInstRes = 0.0f;
    float pvAvgRes = 0.0f;

    uint16_t ipCount = 0;
    uint16_t plotCount = 0;
    uint16_t sampleCount = 0;
    uint16_t pvPkCount = 0;

    float _inputSpan = 0.0f;
    float _outputSpan = 0.0f;
    float _outputStart = 0.0f;
    float _outputStep = 0.0f;
    uint32_t _testTimeSec = 0;
    uint32_t _settleTimeSec = 0;
    uint16_t _samples = 0;
    uint16_t _bufferSize = 0;

    float _samplePeriodUs = 0.0f;
    float _tangentPeriodUs = 0.0f;
    float _settlePeriodUs = 0.0f;
    float _R = 0.0f;

    float eStop = 0.0f;
    bool eStopAbort = false;

    static constexpr float epsilon = 0.000001f;
    static constexpr float kexp = 2.7182818f;
};

#endif
