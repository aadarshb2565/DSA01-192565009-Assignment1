#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
enum class PassengerType { Regular, Student, Senior };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int CONCESSION[] = {0, 25, 50};

class SmartCard {
    long long balancePaise_;
    PassengerType type_;
    
public:
    SmartCard(long long initialBalance, PassengerType type = PassengerType::Regular)
        : balancePaise_(initialBalance), type_(type) {}
    
    long long getBalance() const { return balancePaise_; }
    PassengerType getType() const { return type_; }
    
    bool canTravel(long long fare) const {
        return (balancePaise_ - fare) >= MIN_BALANCE_PAISE;
    }
    
    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balancePaise_ -= fare;
        return true;
    }
    
    void recharge(long long amount) {
        if (amount > 0) balancePaise_ += amount;
    }
};

class FareCalculator {
public:
    static long long calculate(Zone zone, bool peak, PassengerType type) {
        long long fare = BASE_PAISE[static_cast<int>(zone)];
        if (peak) fare = fare * (100 + PEAK_SURCHARGE_PERCENT) / 100;
        int concession = CONCESSION[static_cast<int>(type)];
        fare = fare * (100 - concession) / 100;
        return fare;
    }
};

void processJourney(SmartCard& card, Zone zone, bool peak) {
    long long fare = FareCalculator::calculate(zone, peak, card.getType());
    if (card.deduct(fare)) {
        std::cout << "Fare Rs " << fare/100 << '.' << std::setw(2) << std::setfill('0') << fare%100;
    } else {
        std::cout << "Denied (low balance)";
    }
    std::cout << "  balance Rs " << card.getBalance()/100 << '\n';
}

int main() {
    SmartCard studentCard(10000, PassengerType::Student);
    SmartCard regularCard(10000, PassengerType::Regular);
    
    std::cout << "=== Student Card ===\n";
    processJourney(studentCard, Zone::Inner, false);
    processJourney(studentCard, Zone::Outer, true);
    processJourney(studentCard, Zone::Middle, false);
    
    std::cout << "\n=== Regular Card ===\n";
    processJourney(regularCard, Zone::Inner, false);
    processJourney(regularCard, Zone::Outer, true);
    processJourney(regularCard, Zone::Middle, false);
}