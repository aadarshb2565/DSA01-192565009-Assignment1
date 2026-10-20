#include <iostream>
#include <iomanip>
#include <cmath>
#include <climits>

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr double BASE_RUPEES[] = {10.0, 20.0, 30.0};
constexpr int PEAK_SURCHARGE = 20;
constexpr int JOURNEYS_PER_DAY[] = {20000, 500000, 3000000};

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "=== SCALE TEST: Double Rupees vs Integer Paise ===\n\n";
    
    for (int journeys : JOURNEYS_PER_DAY) {
        long long intTotal = 0;
        double doubleTotal = 0.0;
        
        for (int i = 0; i < journeys; ++i) {
            int zone = i % 3;
            bool peak = (i % 2 == 0);
            
            long long fare = BASE_PAISE[zone];
            if (peak) fare = fare * 120 / 100;
            intTotal += fare;
            
            double dfare = BASE_RUPEES[zone];
            if (peak) dfare *= 1.2;
            doubleTotal += dfare;
        }
        
        double intRupees = intTotal / 100.0;
        double diff = std::abs(doubleTotal - intRupees);
        
        std::cout << "Journeys: " << journeys << "\n";
        std::cout << "  Integer paise total: " << intTotal << " paise = Rs " << intRupees << "\n";
        std::cout << "  Double rupees total: " << doubleTotal << " rupees\n";
        std::cout << "  Difference: " << diff << " rupees = " << (diff * 100) << " paise\n";
        std::cout << "\n";
    }
    
    std::cout << "=== INT OVERFLOW CHECK ===\n";
    long long maxDailyPaise = 3000000LL * 3600;
    std::cout << "Max daily collection (paise): " << maxDailyPaise << "\n";
    std::cout << "INT_MAX: " << INT_MAX << "\n";
    std::cout << "LLONG_MAX: " << LLONG_MAX << "\n";
    std::cout << "Fits in int? " << (maxDailyPaise <= INT_MAX ? "YES" : "NO") << "\n";
    std::cout << "Fits in long long? " << (maxDailyPaise <= LLONG_MAX ? "YES" : "NO") << "\n";
    
    // Real-world double error test
    std::cout << "\n=== REAL-WORLD DOUBLE ERROR TEST ===\n";
    const double base = 10.10;
    const double peakMult = 1.20;
    const double concessionMult = 0.75;
    
    double total = 0.0;
    for (int i = 0; i < 3000000; ++i) {
        double fare = base * peakMult * concessionMult;
        total += fare;
    }
    
    long long intTotal = 3000000LL * 909;  // 1010 * 120 / 100 * 75 / 100 = 909
    
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Double total: " << total << " rupees\n";
    std::cout << "Integer total: " << intTotal/100.0 << " rupees\n";
    std::cout << "Difference: " << std::abs(total - intTotal/100.0) << " rupees\n";
    std::cout << "Difference in paise: " << std::abs(total*100 - intTotal) << "\n";
}