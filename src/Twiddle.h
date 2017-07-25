#ifndef PID_TWIDDLE_H
#define PID_TWIDDLE_H

#include "PID.h"
#include <iostream>
#include <cmath>

class Twiddle {
public:
    double *p;
    double *dp;
    double tol;
    double best_err;

    double running_sum;
    int iteration;
    int current_parameter;
    int phase;

    PID pid;

    Twiddle(double tolerance);

    ~Twiddle();

    void Init(double init_p[], double init_dp[]);

    double ProcessCTE(double cte);

    double GetAvgCte();

    bool IsReinitRequired();

    void Reinit();

private:
    void _ResetPid();
};


#endif //PID_TWIDDLE_H
