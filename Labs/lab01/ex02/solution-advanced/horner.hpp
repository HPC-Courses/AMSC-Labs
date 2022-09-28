#ifndef HORNER_H
#define HORNER_H

#include <unordered_map>
#include <chrono>
#include <regex>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>

#include <execution>

using eval_method_t = std::function<double(const std::vector<double>&, double)>;

// https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const std::string& filename) {
    std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

std::unordered_map<std::string, double> parse_parameters(const std::string& s) {
    // https://stackoverflow.com/questions/638565/parsing-scientific-notation-sensibly
    // https://en.cppreference.com/w/cpp/regex
    const std::regex parameter_pattern(R"((\w+)=(-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+\-]?\d+)?)(\n|\r\n)?)");
    std::cmatch base_match;
    std::unordered_map<std::string, double> parameters;
    auto c = s.c_str();
    while (std::regex_search(c, base_match, parameter_pattern)) {
        c += base_match.length();
        // conversion int to double is safe till 2^53-1
        parameters.emplace(base_match[1], std::stod(base_match[2]));
    }
    return parameters;
}

std::vector<double> evaluate_poly(const std::vector<double>& points,
    const std::vector<double>& a,
    eval_method_t method,
    bool parallel) {
    std::vector<double> result(points.size());
    // lambda with capture by reference, what happens if you capture by value?
    const auto compute = [&a, &method](double x) { return method(a, x); };
    if (parallel) {
        std::transform(std::execution::par, points.begin(), points.end(), result.begin(), compute);
    }
    else {
        std::transform(points.begin(), points.end(), result.begin(), compute);
    }
    return result;
}

inline double pow_integer(double x, unsigned int n) {
    double result{ 1.0 };
    while (n--) {
        result *= x;
    }
    return result;
}

inline double pow_squaring(double x, unsigned int n) {
    double res = 1.0;
    while (n > 0) {
        if (n & 1)
            res *= x;
        x *= x;
        n >>= 1;
    }
    return res;
}

inline double pow_branchless(double x, unsigned int n) {
    double r = 1.0;
    while (n > 0) {
        r *= 1 + (n & 1) * (x - 1);
        x *= x;
        n >>= 1;
    }
    return r;
}

double eval_std(const std::vector<double>& a, double x) {
    double result{ a[0] };
    for (std::vector<double>::size_type k = 1; k < a.size(); ++k) {
        result += a[k] * std::pow(x, k);
    }
    return result;
}

double eval_pow_integer(const std::vector<double>& a, double x) {
    double result{ a[0] };
    for (std::vector<double>::size_type k = 1; k < a.size(); ++k) {
        result += a[k] * pow_integer(x, k);
    }
    return result;
}

double eval_squaring(const std::vector<double>& a, double x) {
    double result{ a[0] };
    for (std::vector<double>::size_type k = 1; k < a.size(); ++k) {
        result += a[k] * pow_squaring(x, k);
    }
    return result;
}

double eval_branchless(const std::vector<double>& a, double x) {
    double result{ a[0] };
    for (std::vector<double>::size_type k = 1; k < a.size(); ++k) {
        result += a[k] * pow_branchless(x, k);
    }
    return result;
}

double eval_horner(const std::vector<double>& a, double x) {
    double result{ a.back() };
    for (auto i = a.crbegin() + 1; i != a.crend(); ++i)
        result = result * x + (*i);

    return result;
}

auto timeit(const std::function<void()>& f) {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    f();
    const auto t1 = high_resolution_clock::now();
    return duration_cast<milliseconds>(t1 - t0).count();
}

#endif /* HORNER_H */