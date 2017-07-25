#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
    p_coef = Kp;
    i_coef = Ki;
    d_coef = Kd;

    previous_cte = 0;
    cte_sum = 0;
}

void PID::UpdateError(double cte) {
    p_error = - p_coef * cte;
    i_error = - i_coef * cte_sum;
    d_error = - d_coef * (cte - previous_cte);
    cte_sum += cte;
    previous_cte = cte;
}

double PID::TotalError() {
    return p_error + i_error + d_error;
}

