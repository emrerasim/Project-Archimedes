#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basit PID kontrolcü durumu (Arduino gibi gömülü sistemler için uygundur).
typedef struct {
    float kp;
    float ki;
    float kd;

    // Hedef değer (setpoint)
    float setpoint;

    // Çıkış sınırları (saturasyon)
    float output_min;
    float output_max;

    // İç durum değişkenleri
    float integral;
    float prev_measurement;
    float prev_output;

    // Türev filtresi için katsayı ve iç durum
    float derivative_filter_coeff;
    float derivative_state;

    bool initialized;
} PidController;

// PID yapılandırmasını başlatır.
void pid_init(PidController *pid,
              float kp,
              float ki,
              float kd,
              float output_min,
              float output_max,
              float derivative_filter_coeff);

// İç durumu sıfırlar (örn. yeni bir proses başlarken).
void pid_reset(PidController *pid);

// PID hesaplamasını günceller ve yeni çıkışı döndürür.
float pid_update(PidController *pid, float setpoint, float measurement, float dt_seconds);

#ifdef __cplusplus
}
#endif

#endif
