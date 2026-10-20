#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

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

void printMoney(long long paise) {
    long long rs = paise / 100;
    long long p = paise % 100;
    if (p < 0) p = -p;
    std::cout << "Rs " << rs << '.' << std::setw(2) << std::setfill('0') << p << std::setfill(' ');
}

struct TestCase {
    std::string name;
    Zone zone;
    int hour;
    PassengerType type;
    long long expectedFare;
    bool shouldPass;
};

int main() {
    std::vector<TestCase> tests = {
        {"Inner off-peak Regular", Zone::Inner, 11, PassengerType::Regular, 1000, true},
        {"Inner peak Regular", Zone::Inner, 9, PassengerType::Regular, 1200, true},
        {"Middle off-peak Regular", Zone::Middle, 14, PassengerType::Regular, 2000, true},
        {"Middle peak Regular", Zone::Middle, 18, PassengerType::Regular, 2400, true},
        {"Outer off-peak Regular", Zone::Outer, 12, PassengerType::Regular, 3000, true},
        {"Outer peak Regular", Zone::Outer, 19, PassengerType::Regular, 3600, true},
        {"Inner off-peak Student", Zone::Inner, 11, PassengerType::Student, 750, true},
        {"Inner peak Student", Zone::Inner, 9, PassengerType::Student, 900, true},
        {"Outer peak Student", Zone::Outer, 19, PassengerType::Student, 2700, true},
        {"Inner off-peak Senior", Zone::Inner, 11, PassengerType::Senior, 500, true},
        {"Outer peak Senior", Zone::Outer, 19, PassengerType::Senior, 1800, true},
        {"08:00 peak", Zone::Inner, 8, PassengerType::Regular, 1200, true},
        {"09:59 peak", Zone::Inner, 9, PassengerType::Regular, 1200, true},
        {"10:00 off-peak", Zone::Inner, 10, PassengerType::Regular, 1000, true},
        {"17:00 peak", Zone::Inner, 17, PassengerType::Regular, 1200, true},
        {"19:59 peak", Zone::Inner, 19, PassengerType::Regular, 1200, true},
        {"20:00 off-peak", Zone::Inner, 20, PassengerType::Regular, 1000, true},
        {"Exact min balance", Zone::Inner, 11, PassengerType::Regular, 1000, true},
    };
    
    std::cout << "=== FARE LOGIC TEST SUITE ===\n\n";
    
    int passed = 0, failed = 0;
    
    for (const auto& tc : tests) {
        long long initialBalance = (tc.name == "Exact min balance") ? 6000 : 10000;
        SmartCard card("TEST", initialBalance, tc.type);
        
        long long fare = FareCalculator::calculate(tc.zone, tc.hour, tc.type);
        bool success = card.deduct(fare);
        
        bool fareMatch = (fare == tc.expectedFare);
        bool travelMatch = (success == tc.shouldPass);
        bool testPassed = fareMatch && travelMatch;
        
        std::cout << (testPassed ? "[PASS]" : "[FAIL]") << " " << tc.name << "\n";
        std::cout << "  Hour: " << tc.hour << " | Zone: " << zoneToString(tc.zone) 
                  << " | Type: " << typeToString(tc.type) << "\n";
        std::cout << "  Expected fare: "; printMoney(tc.expectedFare); std::cout << "\n";
        std::cout << "  Actual fare:   "; printMoney(fare); std::cout << "\n";
        std::cout << "  Expected travel: " << (tc.shouldPass ? "YES" : "NO") 
                  << " | Actual: " << (success ? "YES" : "NO") << "\n";
        if (!testPassed) {
            std::cout << "  *** MISMATCH ***\n";
            failed++;
        } else {
            passed++;
        }
        std::cout << "\n";
    }
    
    std::cout << "[TEST] Invalid zone handling:\n";
    Zone invalid = static_cast<Zone>(99);
    SmartCard card("TEST", 10000, PassengerType::Regular);
    long long fare = FareCalculator::calculate(invalid, 10, PassengerType::Regular);
    std::cout << "  Fare for invalid zone: "; printMoney(fare); std::cout << " (UB - out of bounds)\n";
    
    std::cout << "\n[TEST] Negative recharge:\n";
    SmartCard card2("TEST", 10000, PassengerType::Regular);
    card2.recharge(-500);
    std::cout << "  Balance after -500 recharge: "; printMoney(card2.getBalance()); std::cout << " (unchanged)\n";
    
    std::cout << "\n[TEST] Invalid input handling: covered by Task 8 input validation\n";
    
    std::cout << "\n=== HAND-CALCULATED 5-JOURNEY TEST ===\n";
    SmartCard handCard("HAND", 10000, PassengerType::Regular);
    struct HandTrip { Zone z; int h; long long expected; };
    HandTrip handTrips[] = {
        {Zone::Inner, 9, 1200},
        {Zone::Outer, 14, 3000},
        {Zone::Middle, 18, 2400},
        {Zone::Inner, 10, 1000},
        {Zone::Outer, 19, 3600},
    };
    long long handTotal = 0;
    for (size_t i = 0; i < 5; ++i) {
        long long f = FareCalculator::calculate(handTrips[i].z, handTrips[i].h, PassengerType::Regular);
        bool ok = handCard.deduct(f);
        handTotal += f;
        std::cout << "Trip " << i+1 << ": " << zoneToString(handTrips[i].z) << " " << handTrips[i].h 
                  << ":00 -> "; printMoney(f); std::cout << " (" << (ok?"OK":"DENIED") << ")\n";
    }
    std::cout << "Total fares: "; printMoney(handTotal); std::cout << " (expected 11200 paise = Rs 112.00)\n";
    std::cout << "Final balance: "; printMoney(handCard.getBalance()); std::cout << "\n";
    
    std::cout << "\n=== SUMMARY ===\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";
    std::cout << "Total:  " << passed + failed << "\n";
}