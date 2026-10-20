#include <iomanip>
#include <iostream>
#include <array>

enum class Zone { Inner, Middle, Outer, Airport };
enum class PassengerType { Regular, Student, Senior };

constexpr std::array<long long, 4> BASE_PAISE = {1000, 2000, 3000, 6000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr std::array<int, 3> CONCESSION = {0, 25, 50};

class MonthlyPass {
    std::array<Zone, 2> zones_;
    int validityDays_;
    int daysUsed_ = 0;
public:
    MonthlyPass(std::array<Zone, 2> zones, int days) : zones_(zones), validityDays_(days) {}
    
    bool covers(Zone z) const {
        return zones_[0] == z || zones_[1] == z;
    }
    
    bool isValid() const { return daysUsed_ < validityDays_; }
    void useDay() { if (isValid()) ++daysUsed_; }
    int daysRemaining() const { return validityDays_ - daysUsed_; }
};

class SmartCard {
    std::string id_;
    long long balancePaise_;
    PassengerType type_;
    MonthlyPass* pass_ = nullptr;
public:
    SmartCard(std::string id, long long balance, PassengerType type)
        : id_(std::move(id)), balancePaise_(balance), type_(type) {}
    
    long long getBalance() const { return balancePaise_; }
    PassengerType getType() const { return type_; }
    const std::string& getId() const { return id_; }
    bool hasPass() const { return pass_ != nullptr; }
    
    void setPass(MonthlyPass* pass) { pass_ = pass; }
    
    bool canTravel(long long fare) const {
        return (balancePaise_ - fare) >= MIN_BALANCE_PAISE;
    }
    
    bool deduct(long long fare, Zone journeyZone) {
        if (pass_ && pass_->covers(journeyZone) && pass_->isValid()) {
            pass_->useDay();
            return true;
        }
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
    static bool isPeakHour(int hour) {
        return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    }
    
    static long long calculate(Zone zone, int hour, bool isWeekend, PassengerType type) {
        long long fare = BASE_PAISE[static_cast<int>(zone)];
        
        bool peak = isPeakHour(hour);
        
        if (peak && !isWeekend) {
            fare = fare * (100 + PEAK_SURCHARGE_PERCENT) / 100;
        } else if (isWeekend && !peak) {
            fare = fare * (100 - WEEKEND_DISCOUNT_PERCENT) / 100;
        }
        
        int concession = CONCESSION[static_cast<int>(type)];
        fare = fare * (100 - concession) / 100;
        
        return fare;
    }
};

std::string zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner: return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer: return "Outer";
        case Zone::Airport: return "Airport";
    }
    return "Unknown";
}

void printMoney(long long paise) {
    long long rs = paise / 100;
    long long p = paise % 100;
    if (p < 0) p = -p;
    std::cout << "Rs " << rs << '.' << std::setw(2) << std::setfill('0') << p << std::setfill(' ');
}

int main() {
    std::cout << "=== REDESIGNED FARE SYSTEM TEST ===\n\n";
    
    std::cout << "--- Test 1: Airport Zone ---\n";
    for (bool peak : {false, true}) {
        long long fare = FareCalculator::calculate(Zone::Airport, peak ? 9 : 11, false, PassengerType::Regular);
        std::cout << "Airport " << (peak ? "peak" : "off-peak") << ": "; printMoney(fare); std::cout << "\n";
    }
    
    std::cout << "\n--- Test 2: Weekend Discount (off-peak) ---\n";
    long long fare = FareCalculator::calculate(Zone::Outer, 14, true, PassengerType::Regular);
    std::cout << "Outer weekend off-peak: "; printMoney(fare); std::cout << " (expected 3000*0.9=2700)\n";
    
    std::cout << "\n--- Test 3: Peak + Weekend (mutually exclusive) ---\n";
    fare = FareCalculator::calculate(Zone::Outer, 9, true, PassengerType::Regular);
    std::cout << "Outer peak+weekend: "; printMoney(fare); std::cout << " (expected base 3000)\n";
    
    std::cout << "\n--- Test 4: Student + Weekend ---\n";
    fare = FareCalculator::calculate(Zone::Middle, 14, true, PassengerType::Student);
    std::cout << "Middle weekend Student: "; printMoney(fare); std::cout << " (2000*0.9*0.75=1350)\n";
    
    std::cout << "\n--- Test 5: Monthly Pass ---\n";
    MonthlyPass pass({Zone::Inner, Zone::Middle}, 30);
    SmartCard card("PASS001", 0, PassengerType::Regular);
    card.setPass(&pass);
    
    std::cout << "Card balance: "; printMoney(card.getBalance()); std::cout << "\n";
    
    bool ok = card.deduct(0, Zone::Inner);
    std::cout << "Inner (covered): " << (ok ? "FREE" : "DENIED") << " | Pass days left: " << pass.daysRemaining() << "\n";
    
    ok = card.deduct(0, Zone::Middle);
    std::cout << "Middle (covered): " << (ok ? "FREE" : "DENIED") << " | Pass days left: " << pass.daysRemaining() << "\n";
    
    ok = card.deduct(3000, Zone::Outer);
    std::cout << "Outer (not covered): " << (ok ? "PAID" : "DENIED") << "\n";
    
    card.recharge(10000);
    ok = card.deduct(3000, Zone::Outer);
    std::cout << "Outer after recharge: " << (ok ? "PAID" : "DENIED") << " | Balance: "; printMoney(card.getBalance()); std::cout << "\n";
    
    std::cout << "\n--- Test 6: Pass + Concession (free journey) ---\n";
    SmartCard studentCard("PASS002", 0, PassengerType::Student);
    studentCard.setPass(&pass);
    ok = studentCard.deduct(0, Zone::Inner);
    std::cout << "Student Inner (covered): " << (ok ? "FREE" : "DENIED") << "\n";
}