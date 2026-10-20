#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

enum class Zone { Inner, Middle, Outer };
enum class PassengerType { Regular, Student, Senior };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int CONCESSION[] = {0, 25, 50};

class SmartCard {
    std::string id_;
    long long balancePaise_;
    PassengerType type_;
public:
    SmartCard(std::string id, long long balance, PassengerType type)
        : id_(std::move(id)), balancePaise_(balance), type_(type) {}
    
    long long getBalance() const { return balancePaise_; }
    PassengerType getType() const { return type_; }
    const std::string& getId() const { return id_; }
    
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
    static long long calculate(Zone zone, int hour, PassengerType type) {
        bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
        long long fare = BASE_PAISE[static_cast<int>(zone)];
        if (peak) fare = fare * (100 + PEAK_SURCHARGE_PERCENT) / 100;
        int concession = CONCESSION[static_cast<int>(type)];
        fare = fare * (100 - concession) / 100;
        return fare;
    }
};

std::string zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
    }
    return "Unknown";
}

std::string typeToString(PassengerType t) {
    switch (t) {
        case PassengerType::Regular: return "Regular";
        case PassengerType::Student: return "Student";
        case PassengerType::Senior:  return "Senior";
    }
    return "Unknown";
}

int readInt(const char* prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal && val <= maxVal) return val;
            std::cout << "Value must be " << minVal << "-" << maxVal << "\n";
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number.\n";
        }
    }
}

Zone readZone() {
    return static_cast<Zone>(readInt("Zone (0=Inner,1=Middle,2=Outer): ", 0, 2));
}

PassengerType readPassengerType() {
    return static_cast<PassengerType>(readInt("Type (0=Regular,1=Student,2=Senior): ", 0, 2));
}

void printMoney(long long paise) {
    long long rs = paise / 100;
    long long p = paise % 100;
    if (p < 0) p = -p;
    std::cout << "Rs " << rs << '.' << std::setw(2) << std::setfill('0') << p << std::setfill(' ');
}

int main() {
    std::vector<SmartCard> cards;
    cards.emplace_back("CARD001", 10000, PassengerType::Regular);
    cards.emplace_back("CARD002", 5000, PassengerType::Student);
    cards.emplace_back("CARD003", 20000, PassengerType::Senior);
    
    std::cout << "=== Namma Metro Gate ===\n";
    std::cout << "Cards loaded:\n";
    for (auto& c : cards) {
        std::cout << "  " << c.getId() << " | " << typeToString(c.getType()) 
                  << " | "; printMoney(c.getBalance()); std::cout << "\n";
    }
    
    // Test with simulated inputs (for automated testing)
    std::cout << "\n--- Automated Test Journey ---\n";
    SmartCard& card = cards[0];
    Zone zone = Zone::Middle;
    int hour = 9;
    
    long long fare = FareCalculator::calculate(zone, hour, card.getType());
    bool success = card.deduct(fare);
    
    std::cout << "Journey Result:\n";
    std::cout << "  Card: " << card.getId() << " (" << typeToString(card.getType()) << ")\n";
    std::cout << "  Zone: " << zoneToString(zone) << " | Hour: " << hour 
              << " | Peak: " << (((hour>=8&&hour<10)||(hour>=17&&hour<20)) ? "Yes" : "No") << "\n";
    std::cout << "  Fare: "; printMoney(fare); std::cout << "\n";
    std::cout << "  Status: " << (success ? "APPROVED" : "DENIED (low balance)") << "\n";
    std::cout << "  New Balance: "; printMoney(card.getBalance()); std::cout << "\n";
}