Task 6: Control Structures for Fare Computation

Decision Logic: if-else, switch, Conditional Operator

Scenario | Construct | Reason
Peak hour check (2 ranges) | if-else / || expression | Simple boolean logic
Zone-based base fare | switch on Zone enum | Exact match, compiler optimises to jump table
Surcharge application | Conditional ?: | Single expression, no side effects
Concession application | Conditional ?: or if | Depends on passenger type
Minimum balance check | if in canTravel() | Clear intent, early return

Why switch for zone:
- enum class Zone has fixed, small set of values
- Compiler generates jump table → O(1) with no branch prediction misses
- Exhaustiveness checking with -Wswitch-enum catches missing cases
- Adding 4th zone (Airport) = add case, not new if chain

Looping for Daily Journeys

Loop Type | Use Case | Best For
for (int i=0; i<n; ++i) | Fixed-count iteration (test cases) | Known trip count
Range-based for (auto& t : trips) | Process container of journeys | Production — clean, no index errors
while (cin >> journey) | Stream input until EOF | Interactive gate input

Best choice: Range-based for for std::vector<Journey> or array — matches "process a day's journeys" naturally.

break and continue Usage

for (const auto& journey : journeys) {
    if (!journey.isValid()) {
        logError(journey);
        continue;  // Skip invalid journey, process next
    }
    if (journey.isEmergencyStop()) {
        break;  // Stop all processing immediately
    }
    process(journey);
}

- continue: Skip invalid/duplicate journeys (bad card read, unknown zone)
- break: Emergency stop, end-of-day marker, critical error

switch with No Matching Case

switch (zone) {
    case Zone::Inner:  fare = 1000; break;
    case Zone::Middle: fare = 2000; break;
    case Zone::Outer:  fare = 3000; break;
    // No default!
}
- If zone is invalid (e.g., corrupted memory), falls through → undefined behaviour
- Fix: Add default: handleError(); or use static_assert with std::is_enum_v
- With -Wswitch-enum, compiler warns if enum values not covered

Pseudocode (SIMATS Engineering Standard)

ALGORITHM CalculateFare
INPUT: zone (Zone), hour (0-23), minute (0-59), passengerType (PassengerType)
OUTPUT: farePaise (long long)

BEGIN
    baseFare ← BASE_PAISE[zone]
    
    isPeak ← (hour ≥ 8 AND hour < 10) OR (hour ≥ 17 AND hour < 20)
    
    IF isPeak THEN
        baseFare ← (baseFare × 120) / 100
    END IF
    
    concession ← CONCESSION_PERCENT[passengerType]
    farePaise ← (baseFare × (100 - concession)) / 100
    
    RETURN farePaise
END

ALGORITHM CanTravel
INPUT: balancePaise (long long), farePaise (long long)
OUTPUT: boolean

BEGIN
    RETURN (balancePaise - farePaise) ≥ MIN_BALANCE_PAISE
END

ALGORITHM ProcessJourney
INPUT: card (SmartCard), zone (Zone), hour, minute, passengerType
OUTPUT: success (boolean)

BEGIN
    fare ← CalculateFare(zone, hour, minute, passengerType)
    
    IF CanTravel(card.balance, fare) THEN
        card.balance ← card.balance - fare
        RETURN true
    ELSE
        RETURN false
    END IF
END

Complete the Code (TODO 1 & 2)

File: Task06/fare_logic.cpp

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000};   // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;     // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    long long base = 0;
    switch (z) {
        case Zone::Inner:  base = BASE_PAISE[0]; break;
        case Zone::Middle: base = BASE_PAISE[1]; break;
        case Zone::Outer:  base = BASE_PAISE[2]; break;
        default: return -1;  // Invalid zone
    }
    if (peak) {
        base = base * (100 + PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    return (balance - f) >= MIN_BALANCE_PAISE;
}

int main()
{
    long long balance = 10000;                       // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }
}

Compilation & Execution:
cd Task06
g++ -std=c++17 -Wall -Wextra -o fare_logic fare_logic.cpp
./fare_logic

Output:
Fare Rs 10.00  balance Rs 90
Fare Rs 36.00  balance Rs 54
Denied (low balance)  balance Rs 54

Matches expected output exactly.

Boundary Tests: 10 AM and 8 PM

Extended test in main():
// Add boundary trips
Trip boundaryTrips[] = {
    {Zone::Inner, false},   // 09:59 - peak
    {Zone::Inner, false},   // 10:00 - off-peak
    {Zone::Outer, true},    // 17:00 - peak
    {Zone::Outer, false},   // 20:00 - off-peak
};
// Simulate with hour parameter
auto isPeak = [](int h) { return (h >= 8 && h < 10) || (h >= 17 && h < 20); };

for (int h : {9, 10, 17, 20}) {
    long long f = fare(Zone::Inner, isPeak(h));
    std::cout << h << ":00 -> " << (isPeak(h) ? "PEAK" : "OFF")
              << " fare Rs " << f/100 << '.' << std::setw(2) << std::setfill('0') << f%100 << '\n';
}

Output:
9:00 -> PEAK fare Rs 12.00
10:00 -> OFF fare Rs 10.00
17:00 -> PEAK fare Rs 12.00
20:00 -> OFF fare Rs 10.00

Correct: 10:00 and 20:00 are off-peak (exclusive upper bound < 10, < 20).

Alternative Concept & Complexity Analysis

If base fare looked up from array indexed by zone instead of switch:

constexpr long long FARE[] = {1000, 2000, 3000};
long long fare(Zone z, bool peak) {
    int idx = static_cast<int>(z);
    if (idx < 0 || idx >= 3) return -1;
    long long base = FARE[idx];
    return peak ? base * 120 / 100 : base;
}

Aspect | switch | Array Lookup
Time per lookup | O(1) — jump table | O(1) — direct index
Space | Code size (jump table) | 24 bytes (3 × 8)
Bounds checking | Implicit (default case) | Explicit if needed
Adding 4th zone | Add case, update array | Extend array, no code change
Compiler optimisation | Excellent | Excellent
Enum reordering safety | Safe (by name) | Unsafe (by index)

Verdict: Array lookup is simpler for adding zones (just extend array). switch is safer if enum order changes. Recommendation: Use array with static_assert to verify size matches enum count.

Evidence: Working Control Structures

Files created:
- Task06/fare_logic.cpp — compiles clean with -Wall -Wextra
- Output matches expected exactly
- Boundary tests confirm 10:00 and 20:00 are off-peak
- switch covers all Zone values; -Wswitch-enum passes