#include <iomanip>
#include <iostream>

enum Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

long long balance = 10000;

long long calculateFare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base = base * 120 / 100;
    return base;
}

bool canTravel(long long fare) {
    return (balance - fare) >= MIN_BALANCE_PAISE;
}

void deductFare(long long fare) {
    balance -= fare;
}

void processTrip(Zone zone, bool peak) {
    long long f = calculateFare(zone, peak);
    if (canTravel(f)) {
        deductFare(f);
        std::cout << "Fare Rs " << f/100 << '.' << std::setw(2) << std::setfill('0') << f%100;
    } else {
        std::cout << "Denied (low balance)";
    }
    std::cout << "  balance Rs " << balance/100 << '\n';
}

int main() {
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Inner, false}, {Outer, true}, {Middle, false}};
    for (const auto& t : trips) processTrip(t.zone, t.peak);
    
    balance = -1000;
    std::cout << "Corrupted balance: Rs " << balance/100 << '\n';
}