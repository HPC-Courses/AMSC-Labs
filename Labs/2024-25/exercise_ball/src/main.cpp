#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include "utils.h"

int main(int argc, char** argv)
{

    static const int max_iter = 1e3;
    static const double rtol = 1e-5;
    static const double stol = 1e-5;
    static const double tolerance_reaction_term = 1e-7;

    static const double step_size = 1e-6;
    static const double v_0 = 10;
    static const double theta_0 = 0;
    static const double gravity = 9.81;
    static const double R = 30;
                 double dt = 1e-3;

    if (std::abs(v_0)>=std::sqrt(gravity*R)) 
    {
        std::cout << "The initial condition is such that the ball already detaches at the beginning of the simulation" << std::endl;
        exit(1);
    }


    double vtheta_n = v_0;
    double theta_n = theta_0;
    double current_reaction;

    type_fun func = [&theta_n, &vtheta_n, &dt](double const& vtheta_np1){return (-vtheta_np1 + vtheta_n + gravity*dt*sin(theta_n + dt/R*vtheta_np1));};

    type_fun2 vincular_reaction = [&theta_n, &vtheta_n](){return (gravity*cos(theta_n) - vtheta_n*vtheta_n/R);};
    
    
    Newton root_method(vtheta_n, func, max_iter, rtol, stol, nullopt, step_size);

    double time = 0.;
    while (true)
    {
        vtheta_n = root_method.solve();
        theta_n += dt/R*vtheta_n;

        current_reaction = vincular_reaction();
        time += dt; // update time

        cout << "Angle at which the ball detaches: " << theta_n*180/M_PI << ", Current reaction term value: " << current_reaction << ", Time at which the ball detaches: " << time << endl;

        if (std::abs(current_reaction)<tolerance_reaction_term) break;
        if (current_reaction<0) dt = -std::abs(dt)/2.; else dt = std::abs(dt); // heuristic algorithm based on bisection
    }

    return 0;
}
