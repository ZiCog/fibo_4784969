
#include <iostream>
#include <cfenv>
#include <cassert>

int main() {

    // WARNING! This overflow checking only happens at -O0
/*
    feenableexcept(FE_INVALID   | 
                   FE_DIVBYZERO | 
                   FE_OVERFLOW  | 
                   FE_UNDERFLOW);
*/


    std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);

    double overflowing_var = 1000;
    double underflowing_var = 0.01;

    std::cout << "Overflow flag before: " << (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag before: " << (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;

    for(int i = 0; i < 40; ++i) {
        overflowing_var *= overflowing_var;
        underflowing_var *= underflowing_var;
    }
    assert (!(bool)std::fetestexcept(FE_OVERFLOW));
    assert (!(bool)std::fetestexcept(FE_UNDERFLOW));

    std::cout << "Overflow flag after: " << (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag after: " << (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;
}
