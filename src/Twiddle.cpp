#include "Twiddle.h"

Twiddle::Twiddle(double tolerance) {
    tol = tolerance;
    best_err = 9999.;
    current_parameter = 0;
    phase = -1; // -1 -- baseline / 0 -- increase / 1 -- decrease
}

Twiddle::~Twiddle() {}

void Twiddle::Init(double *init_p, double *init_dp) {
    p = init_p;
    dp = init_dp;

    pid = PID();
    _ResetPid();
}


double Twiddle::ProcessCTE(double cte) {
    running_sum += std::abs(cte);
    iteration++;

    pid.UpdateError(cte);
    return pid.TotalError();
}

double Twiddle::GetAvgCte() {
    return running_sum / iteration;
}

bool Twiddle::IsReinitRequired() {
    return iteration >= 500;
}

void Twiddle::Reinit() {
    std::cout << "REINIT" << std::endl;
    if (dp[0] + dp[1] + dp[2] < tol) {
        std::cout << "Solution found: " << dp[0] << " " << dp[1] << " " << dp[2] << std::endl;
        return;
    }

    double err = GetAvgCte();
    std::cout << "Err: " << err << " Best: " << best_err << std::endl;

    if(phase == -1) {
        std::cout << "Evaluation finished" << std::endl;
        best_err = err;
        current_parameter = 0;

        phase = 0;
        p[current_parameter] += dp[current_parameter];
        _ResetPid();
        return;
    }

    if(phase == 0 && err < best_err) {
        std::cout << "Nice phase 0! Next parameter now..." << std::endl;
        best_err = err;
        dp[current_parameter] *= 1.1;
        current_parameter = ++current_parameter % 3;

        phase = 0;
        p[current_parameter] += dp[current_parameter];
        _ResetPid();
        return;
    }

    if(phase == 0 && err >= best_err) {
        std::cout << "Phase 0 failed, going to phase 1" << std::endl;
        phase = 1;
        p[current_parameter] -= 2 * dp[current_parameter];
        _ResetPid();
        return;
    }

    if(phase == 1 && err < best_err) {
        std::cout << "Nice phase 1! Next parameter now..." << std::endl;
        best_err = err;
        dp[current_parameter] *= 1.1;
        current_parameter = ++current_parameter % 3;

        phase = 0;
        p[current_parameter] += dp[current_parameter];
        _ResetPid();
        return;
    }

    if(phase == 1 && err >= best_err) {
        std::cout << "Phase 1 failed! Restore param, decrease dp and go next..." << std::endl;
        p[current_parameter] += dp[current_parameter];
        dp[current_parameter] *= 0.9;
        current_parameter = ++current_parameter % 3;

        phase = 0;
        p[current_parameter] += dp[current_parameter];
        _ResetPid();
        return;
    }
}

void Twiddle::_ResetPid() {
    pid.Init(p[0], p[1], p[2]);
    iteration = 0;
    running_sum = 0;
}
