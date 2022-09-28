#ifndef AUTODIFF_H
#define AUTODIFF_H

#include <functional>
#include <cmath>
#include <ranges>
#include <algorithm>



namespace autodiff {
  using func_t = std::function<double(double)>;

  class Var {
  public:
    Var(
      const func_t& val,
      const func_t& der
    ) :
      m_val(val), m_der(der) {}

    double eval(double x) const {
      return m_val(x);
    }
    double eval_der(double x) const {
      return m_der(x);
    }

    //======================================================================================
    Var& operator+=(const Var& rhs) // compound assignment (does not need to be a member,
    {                           // but often is, to modify the private members)
        /* addition of rhs to *this takes place here */
      m_val = [m_val = m_val, eval = rhs.m_val](double x) { return m_val(x) + eval(x);};
      m_der = [m_der = m_der, evald = rhs.m_der](double x) { return m_der(x) + evald(x);};
      return *this; // return the result by reference
    }

    // friends defined inside class body are inline and are hidden from non-ADL lookup
    friend Var operator+(Var lhs,        // passing lhs by value helps optimize chained a+b+c
      const Var& rhs) // otherwise, both parameters may be const references
    {
      lhs += rhs; // reuse compound assignment
      return lhs; // return the result by value (uses move constructor)
    }
    //======================================================================================
    Var& operator+=(double rhs) {
      m_val = [m_val = m_val, rhs = rhs](double x) { return m_val(x) + rhs;};
      m_der = [m_der = m_der](double x) { return m_der(x);};
      return *this;
    }
    friend Var operator+(Var lhs, double rhs) {
      lhs += rhs;
      return lhs;
    }
    friend Var operator+(double lhs, Var rhs) {
      rhs += lhs;
      return rhs;
    }
    //======================================================================================
    Var& operator*=(const Var& rhs) {
      // care, order matters, you need to copy m_val before re-assigning its value
      m_der = [m_val = m_val, eval = rhs.m_val, m_der = m_der, evald = rhs.m_der](double x) { 
        return m_der(x) * eval(x) + m_val(x) * evald(x);
      };
      m_val = [m_val = m_val, eval = rhs.m_val](double x) { return m_val(x) * eval(x);};
      return *this;
    }
    friend Var operator*(Var lhs, const Var& rhs) {
      lhs *= rhs;
      return lhs;
    }
    //======================================================================================
    Var& operator*=(double rhs) {
      m_val = [m_val = m_val, rhs = rhs](double x) { return m_val(x) * rhs;};
      m_der = [m_der = m_der, rhs = rhs](double x) { return m_der(x) * rhs;};
      return *this;
    }
    friend Var operator*(Var lhs, double rhs) {
      lhs *= rhs;
      return lhs;
    }
    friend Var operator*(double lhs, Var rhs) {
      rhs *= lhs;
      return rhs;
    }
    //======================================================================================
    Var& operator^=(double rhs) {
      m_der = [m_val = m_val, m_der = m_der, rhs = rhs](double x) { return rhs * std::pow(m_val(x), rhs - 1.0) * m_der(x);};
      m_val = [m_val = m_val, rhs = rhs](double x) { return std::pow(m_val(x), rhs);};
      return *this;
    }
    friend Var operator^(Var lhs, double rhs) {
      lhs ^= rhs;
      return lhs;
    }
    //======================================================================================
    Var operator()(const Var& v) const {
      return {[m_val = m_val, eval = v.m_val](double x) { return m_val(eval(x));}, 
      [m_val = m_val, eval = v.m_val, m_der = m_der, evald = v.m_der](double x) {
        return m_der(eval(x)) * evald(x);
      }};
    }
  private:
    func_t m_val, m_der;
  };
  
  const Var sin([](double x) {return std::sin(x);}, [](double x) {return std::cos(x);});
  const Var cos([](double x) {return std::cos(x);}, [](double x) {return -std::sin(x);});
  const Var exp([](double x) {return std::exp(x);}, [](double x) {return std::exp(x);});
  const Var x([](double x) {return x;}, [](double x) {return 1.0;});

  using test_t = std::tuple<Var, func_t, func_t>;
  std::pair<double, double> test(const Var& v,
    const func_t& val,
    const func_t& der,
    double min, double max, unsigned n) {
    const auto h = (max - min) / n;
    const auto x_range = std::ranges::views::iota(0u, n) |
      std::ranges::views::transform([=](auto i) { return min + i * h; });
    const std::vector<double> points(x_range.begin(), x_range.end());

    std::vector<double> err_val(n), err_der(n);

    std::transform(points.begin(), points.end(), err_val.begin(), [&](double t) {return std::abs(v.eval(t) - val(t));});
    std::transform(points.begin(), points.end(), err_der.begin(), [&](double t) {return std::abs(v.eval_der(t) - der(t));});

    return { *std::max_element(err_val.cbegin(), err_val.cend()),
             *std::max_element(err_der.cbegin(), err_der.cend()) };
  }
}



#endif /* AUTODIFF_H */