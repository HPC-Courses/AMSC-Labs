#ifndef __UTILS_H__
#define __UTILS_H__

#include <iterator>
#include <algorithm>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>

template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]"; // use two ANSI backspace characters '\b' to overwrite final ", "
    }
    return out;
}

auto timeit(const std::function<void()>& f) {
    using namespace std::chrono;
    const auto t0 = high_resolution_clock::now();
    f();
    const auto t1 = high_resolution_clock::now();
    return duration_cast<milliseconds>(t1 - t0).count();
}

#endif