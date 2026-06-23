#include <iostream>
#include <cmath>

int main() {
    auto getUnwrapped = [&](double v, double v0) {
        while (v < v0 - 15.0) v += 30.0;
        while (v > v0 + 15.0) v -= 30.0;
        return v;
    };
    
    double v0 = 29.999;
    double targetValue = 31.0;
    
    double target = targetValue;
    while (target < v0 - 15.0) target += 30.0;
    while (target > v0 + 15.0) target -= 30.0;
    
    std::cout << "Target: " << target << std::endl;
    std::cout << "StartVal: " << getUnwrapped(29.999, v0) << std::endl;
    std::cout << "EndVal: " << getUnwrapped(1.9, v0) << std::endl;
    
    // Simulate bisection
    double low = 0.0;
    double high = 2.0;
    
    auto getValueFunc = [](double ah) {
        double v = 29.999 + ah * (31.9 - 29.999) / 2.0;
        if (v >= 30.0) v -= 30.0;
        return v;
    };
    
    for (int i=0; i<20; i++) {
        double mid = (low + high)/2.0;
        double unwrap = getUnwrapped(getValueFunc(mid), v0);
        if (unwrap < target) low = mid;
        else high = mid;
    }
    
    std::cout << "Found ah: " << high << std::endl;
    return 0;
}
