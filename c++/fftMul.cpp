#include <vector>
#include <iostream>
#include <iomanip>
#include <cfenv>

// Uncomment to disable assert()
//#define NDEBUG
#include <cassert>

#include "FftComplex.hpp"

using std::complex;
using std::size_t;
using std::vector;

//constexpr double DIGITS = 4; // Decimal digits in each big integer array element.
//constexpr double BASE = pow(10, DIGITS);
//constexpr double invBASE = 1 / BASE;

#define BASE 10000.0l
#define invBASE 0.0001l
#define DIGITS 4

//#define BASE 10000
//#define invBASE 0.0001l
//constexpr double invBASE = 1.0 / double(BASE);
//#define DIGITS 4

//#include "bint.h"
//constexpr double invBASE = 1.0 / double(BASE);

vector<double> fftMul (const vector<complex<double>> a, const vector<complex<double>> b) {

	vector<complex<double> > conv(a.size());
	Fft::convolve(a, b, conv);

    std::cout << "Convolution = " << '\n';
    for (auto i =  conv.begin(); i != conv.end(); ++i) {
//        std::cout << i->real() << ", ";
    }
    std::cout << '\n';

    // Copy real parts to result vector
    vector<double> result(conv.size());
    for (size_t i = 0; i < conv.size() - 1; i++) {
        result[i] = conv[i].real();
    }

    std::cout << "Normalize: " << '\n';
    // Convert to normal form (0 <= result[i] < BASE)
    // Adapted from here: http://numbers.computation.free.fr/Constants/Programs/BigInt.c
    long i = 0;
    int carry = 0;
    long double x;

    for (size_t i = 0; i < result.size(); i++) {
        x = round(result[i] + carry);
        carry = floor(x * invBASE);
//        std::cout << "i = " << i << "; carry = " << carry << "; x = " << x << '\n';
        result[i] = round(x - carry * BASE);
    }
    while ((carry != 0.0)) {
        assert(false);
        x = carry;
        carry = floor(x * invBASE);
        std::cout << "i = " << i << "; carry = " << carry << "; x = " << x << '\n';
        result[i] = round(x - carry * BASE);
        i++;
    }

    return result;
} 

int main (int argc, char* argv[]) {     

	vector<complex<double> > op1;
	vector<complex<double> > op2;

    // Enable floating point exceptions. Note: Only works at -O0
    feenableexcept(FE_INVALID   | 
                   FE_DIVBYZERO | 
                   FE_OVERFLOW  | 
                   FE_UNDERFLOW);

    // Make operands with half a million 9's each
    int width = 1024 * 128 * 2;
    for (int i = 0; i < width; i++) {
        const complex<double> x = {8999, 0};
        op1.push_back(x);
        op2.push_back(x);
    }
    for (int i = 0; i < width; i++) {
        const complex<double> x = {     0, 0};
        op1.push_back(x);
        op2.push_back(x);
    }
/*
    std::cout << "Op1 = " << '\n';
    for (auto i =  op1.begin(); i != op1.end(); ++i) {
        std::cout << i->real() << ", ";
    }
    std::cout << '\n';

    std::cout << "Op2 = " << '\n';
    for (auto i =  op2.begin(); i != op2.end(); ++i) {
        std::cout << i->real() << ", ";
    }
    std::cout << '\n';
*/
    vector<double> result = fftMul(op1, op2); 

    if (result.size() == 0) {
        std::cout << "NULL";
    } else {
        std::cout << result[result.size() - 1];
//        for (int i = result.size() - 2; i >= 0; i--) {
//            std::cout << std::setfill('0') << std::setw(DIGITS) << result[i];
//        }
    }
    std::cout << '\n';
}