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
            while (v < v0 - halfWrap) v += wrapValue;
            while (v > v0 + halfWrap) v -= wrapValue;
            return v;
        };

        double target = targetValue;
        while (target < v0 - halfWrap) target += wrapValue;
        while (target > v0 + halfWrap) target -= wrapValue;

        double low = searchStartAhar;
        double high = searchStartAhar + maxDays;
        double startVal = getUnwrapped(low);
        double endVal = getUnwrapped(high);

        if (startVal <= target && target <= endVal) {
        } else if (target < startVal) {
            low = searchStartAhar - maxDays;
            high = searchStartAhar;
            startVal = getUnwrapped(low);
            endVal = getUnwrapped(high);
            if (!(startVal <= target && target <= endVal)) return -1.0;
        } else {
            return -1.0;
        }

        for (int i = 0; i < 20; i++) {
            double mid = (low + high) / 2.0;
            if (getUnwrapped(mid) < target) low = mid;
            else high = mid;
        }
        return high;
    };

    auto getTithi = [](double ah) {
        // Mock getTithi
        // ah goes from 0.0 to 2.0
        // Tithi advances by 1.0 / 0.94 days
        double val = std::fmod(ah * (1.0 / 0.94) + 29.2, 30.0);
        if (val < 0) val += 30.0;
        return val;
    };

    double startAhar = 0.0;
    double valAtStart = getTithi(startAhar);
    int currentIndex = static_cast<int>(std::floor(valAtStart / 1.0));
    double targetEndValue = (currentIndex + 1) * 1.0;

    double startTimeAhar = findTransit(startAhar, getTithi, currentIndex * 1.0, 2.0, 30.0);
    double endTimeAhar = findTransit(startTimeAhar != -1.0 ? startTimeAhar : startAhar, getTithi, targetEndValue, 2.0, 30.0);

    std::cout << "startAhar: " << startAhar << " val: " << valAtStart << " currentIdx: " << currentIndex << std::endl;
    std::cout << "startTimeAhar: " << startTimeAhar << " endTimeAhar: " << endTimeAhar << std::endl;
    
    double currentStartTimeAhar = endTimeAhar;
    currentIndex++;
    targetEndValue = (currentIndex + 1) * 1.0;
    double nextEndTimeAhar = findTransit(currentStartTimeAhar, getTithi, targetEndValue, 2.0, 30.0);

    std::cout << "Next: currentStartTimeAhar: " << currentStartTimeAhar << " currentIdx: " << currentIndex << " targetEndValue: " << targetEndValue << std::endl;
    std::cout << "nextEndTimeAhar: " << nextEndTimeAhar << std::endl;
    return 0;
}
