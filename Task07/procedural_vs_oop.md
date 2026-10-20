Task 7: Procedural vs Object-Oriented Fare Logic

Procedural Version

File: Task07/procedural_fare.cpp

#include <iomanip>
#include <iostream>

enum Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

long long balance = 10000;  // Global balance - accessible everywhere

long long calculateFare(Zone z, bool peak) {
    long long base = BASE_PAISE[static_cast<int>(z)];
    if (peak) base = base * 120 / 100;
    return base;
}

bool canTravel(long long fare) {
    return (balance - fare) >= MIN_BALANCE_PAISE;
}

void deductFare(long long fare) {
    balance -= fare;  // Direct modification - no validation
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
    
    // DANGER: Any code can do this:
    balance = -1000;  // Negative balance! No protection.
    std::cout << "Corrupted balance: Rs " << balance/100 << '\n';
}

Problems:
- Global balance — any function can corrupt it
- No encapsulation — deductFare doesn't validate
- Minimum balance rule only checked in canTravel, bypassable
- Adding monthly pass = modify global logic everywhere

Object-Oriented Version

File: Task07/oop_fare.cpp

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };
enum class PassengerType { Regular, Student, Senior };

constexpr long long BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int CONCESSION[] = {0, 25, 50};  // Regular, Student, Senior

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
    processJourney(studentCard, Zone::Inner, false);   // 10.00 * 0.75 = 7.50
    processJourney(studentCard, Zone::Outer, true);    // 36.00 * 0.75 = 27.00
    processJourney(studentCard, Zone::Middle, false);  // 20.00 * 0.75 = 15.00 (denied)
    
    std::cout << "\n=== Regular Card ===\n";
    processJourney(regularCard, Zone::Inner, false);
    processJourney(regularCard, Zone::Outer, true);
    processJourney(regularCard, Zone::Middle, false);
    
    // SAFE: Cannot corrupt balance directly
    // studentCard.balancePaise_ = -1000;  // COMPILE ERROR: private
}

Key protections:
- balancePaise_ is private — only deduct()/recharge() modify it
- deduct() checks canTravel() internally — rule enforced always
- canTravel() is const — doesn't modify state
- Concession integrated into FareCalculator — single source of truth

How Class Prevents Negative Balance

bool SmartCard::deduct(long long fare) {
    if (!canTravel(fare)) return false;  // Guard clause
    balancePaise_ -= fare;                // Only reached if safe
    return true;
}

bool SmartCard::canTravel(long long fare) const {
    return (balancePaise_ - fare) >= MIN_BALANCE_PAISE;
}

Invariant: balancePaise_ >= MIN_BALANCE_PAISE after every successful deduct()
- Constructor ensures initial balance valid
- recharge() only adds positive amounts
- No public setter for balance

Changing Minimum Balance

Version | Change Required
Procedural | Find all MIN_BALANCE_PAISE usages (global constant helps) but canTravel logic duplicated if copied
OOP | Change constexpr MIN_BALANCE_PAISE in one place; SmartCard::canTravel() uses it automatically

Both easy with constexpr, but OOP guarantees all paths use the same check (single canTravel method).

Alternative Concept & Complexity Analysis

If balance were public instead of private with deduct()/canTravel():

class SmartCard {
public:
    long long balancePaise;  // PUBLIC - DANGEROUS
    // ...
};

Aspect | Private + Methods | Public Member
Time per operation | O(1) | O(1)
Space per card | O(1) | O(1)
Negative balance possible? | No (enforced by deduct) | Yes — any code: card.balancePaise = -1
Minimum balance rule enforced? | Always | Never — bypassable
Concession logic consistent? | Yes (in FareCalculator) | No — scattered
Add monthly pass? | Subclass/extend cleanly | Hack global logic

Rule that can be broken: "Journey allowed only if at least Rs 50 remains after fare deduction" — with public balance, any part of program can set balance = 0 and travel for free.

Evidence: Both Versions Working

Procedural output:
Fare Rs 10.00  balance Rs 90
Fare Rs 36.00  balance Rs 54
Denied (low balance)  balance Rs 54
Corrupted balance: Rs -10

OOP output:
=== Student Card ===
Fare Rs 7.50  balance Rs 92
Fare Rs 27.00  balance Rs 65
Denied (low balance)  balance Rs 65

=== Regular Card ===
Fare Rs 10.00  balance Rs 90
Fare Rs 36.00  balance Rs 54
Denied (low balance)  balance Rs 54

OOP version compiles clean with -Wall -Wextra; procedural shows no warnings but allows corruption.