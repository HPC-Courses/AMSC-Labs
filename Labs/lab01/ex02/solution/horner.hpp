#ifndef HORNER_H
#define HORNER_H

using eval_method_t = double (*)(const std::vector<double>&, const double&);

std::vector<double> evaluate_poly(const std::vector<double>& points,
    const std::vector<double>& a,
    eval_method_t method) {
    std::vector<double> result;
    result.reserve(points.size());
    for (std::size_t i = 0; i < points.size(); ++i)
        result.push_back(method(a, points[i]));
    return result;
}

inline double pow_integer(const double& x, const unsigned int& n) {
    double result{ x };

    for (unsigned int i = 2; i != n + 1; ++i)
        result *= x;

    return result;
}

double eval(const std::vector<double>& a, const double& x) {
    double result{ a[0] };
    for (std::vector<double>::size_type k = 1; k < a.size(); ++k) {
        result += a[k] * std::pow(x, k);
    }
    return result;
}

double eval_horner(const std::vector<double>& a, const double& x) {
    double result{ a.back() };
    for (auto i = a.crbegin() + 1; i != a.crend(); ++i)
        result = result * x + (*i);

    return result;
}

#endif /* HORNER_H */