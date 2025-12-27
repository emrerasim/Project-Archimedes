#include "pid_controller.h"

// Çıkışı belirtilen aralıkta sınırlar.
static float clamp_float(float value, float min_value, float max_value) {
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

// PID parametrelerini ve iç durumları başlatır.
void pid_init(PidController *pid,
              float kp,
              float ki,
              float kd,
              float output_min,
              float output_max,
              float derivative_filter_coeff) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = 0.0f;
    pid->output_min = output_min;
    pid->output_max = output_max;
    pid->integral = 0.0f;
    pid->prev_measurement = 0.0f;
    pid->prev_output = 0.0f;
    pid->derivative_filter_coeff = derivative_filter_coeff;
    pid->derivative_state = 0.0f;
    pid->initialized = false;
}

// PID iç durumunu sıfırlar.
void pid_reset(PidController *pid) {
    pid->integral = 0.0f;
    pid->prev_measurement = 0.0f;
    pid->prev_output = 0.0f;
    pid->derivative_state = 0.0f;
    pid->initialized = false;
}

// PID döngüsünü günceller. dt_seconds <= 0 ise önceki çıkışı döndürür.
float pid_update(PidController *pid, float setpoint, float measurement, float dt_seconds) {
    if (dt_seconds <= 0.0f) {
        return pid->prev_output;
    }

    if (!pid->initialized) {
        pid->prev_measurement = measurement;
        pid->initialized = true;
    }

    pid->setpoint = setpoint;
    float error = setpoint - measurement;

    // İntegral terimi (toplam hata)
    pid->integral += error * dt_seconds;

    // Türev terimi (ölçüm üzerinden) ve düşük geçiren filtre
    float raw_derivative = (measurement - pid->prev_measurement) / dt_seconds;
    pid->derivative_state = pid->derivative_filter_coeff * pid->derivative_state
        + (1.0f - pid->derivative_filter_coeff) * raw_derivative;

    // P + I - D (türev ölçüm üzerinden alındığı için eksi)
    float output = pid->kp * error
        + pid->ki * pid->integral
        - pid->kd * pid->derivative_state;

    // Çıkış sınırlaması (saturasyon)
    float clamped_output = clamp_float(output, pid->output_min, pid->output_max);

    // Anti-windup: saturasyon varsa integrali geri al
    if (output != clamped_output) {
        float excess = output - clamped_output;
        if (pid->ki != 0.0f) {
            pid->integral -= excess / pid->ki;
        }
    }

    pid->prev_measurement = measurement;
    pid->prev_output = clamped_output;

    return clamped_output;
}
