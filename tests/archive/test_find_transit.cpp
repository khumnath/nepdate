#include <iostream>
#include <cmath>
#include <functional>

int main() {
    auto findTransit = [](double searchStartAhar,
                          std::function<double(double)> getValueFunc,
                          double targetValue, double maxDays, double wrapValue) -> double {
        double v0 = getValueFunc(searchStartAhar);
        double halfWrap = wrapValue / 2.0;
        auto getUnwrapped = [&](double ah) {
            double v = getValueFunc(ah);
            while (v < v0 - halfWrap)
            v += wrapValue;
            while (v > v0 + halfWrap)
            v -= wrapValue;
            return v;
        };

        double target = targetValue;
        while (target < v0 - halfWrap)
            target += wrapValue;
        while (target > v0 + halfWrap)
            target -= wrapValue;

        double low = searchStartAhar;
        double high = searchStartAhar + maxDays;
        double startVal = getUnwrapped(low);
        double endVal = getUnwrapped(high);

        std::cout << "v0=" << v0 << " targetValue=" << targetValue << " target=" << target 
                  << " startVal=" << startVal << " endVal=" << endVal << std::endl;

        if (startVal <= target && target <= endVal) {
            // Current window ok
            std::cout << "Window OK" << std::endl;
        } else if (target < startVal) {
            // Try searching backward
            std::cout << "Backward search" << std::endl;
            low = searchStartAhar - maxDays;
            high = searchStartAhar;
            startVal = getUnwrapped(low);
            endVal = getUnwrapped(high);

            if (!(startVal <= target && target <= endVal)) {
            return -1.0;
            }
        } else {
            return -1.0;
        }

        for (int i = 0; i < 20; i++) {
            double mid = (low + high) / 2.0;
            if (getUnwrapped(mid) < target) {
            low = mid;
            } else {
            high = mid;
            }
        }
        return high;
    };

    auto getTithi = [](double ah) {
        // ah goes from 0.0 to 10.0
        // Tithi advances by 1.0 every 1.0 ahar
        // Let's simulate a wrap at 30.0
        double val = std::fmod(ah * 1.05 + 29.5, 30.0);
        return val;
    };

    double startAhar = 0.0;
    double valAtStart = getTithi(startAhar);
    int currentIndex = static_cast<int>(std::floor(valAtStart / 1.0));
    double targetEndValue = (currentIndex + 1) * 1.0;

    std::cout << "Initial: val=" << valAtStart << " idx=" << currentIndex << " target=" << targetEndValue << std::endl;

    double startTimeAhar = findTransit(startAhar, getTithi, currentIndex * 1.0, 2.0, 30.0);
    double endTimeAhar = findTransit(startTimeAhar != -1.0 ? startTimeAhar : startAhar, getTithi, targetEndValue, 2.0, 30.0);

    std::cout << "start=" << startTimeAhar << " end=" << endTimeAhar << std::endl;
    
    // LOOP
    double currentStartTimeAhar = endTimeAhar;
    currentIndex++;
    targetEndValue = (currentIndex + 1) * 1.0;
    
    std::cout << "Next target=" << targetEndValue << std::endl;
    double nextEndTimeAhar = findTransit(currentStartTimeAhar, getTithi, targetEndValue, 2.0, 30.0);
    std::cout << "Next end=" << nextEndTimeAhar << std::endl;

    return 0;
}
