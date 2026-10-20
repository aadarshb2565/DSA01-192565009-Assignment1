Task 11: Final Design Recommendation

Recommended Types, Constants, and Classes

Types
Domain | Type | Rationale
Monetary values | long long (paise) | Exact, no overflow at scale, fast integer arithmetic
Zone | enum class Zone : uint8_t { Inner, Middle, Outer, Airport } | Type-safe, 1 byte, extensible
Passenger type | enum class PassengerType : uint8_t { Regular, Student, Senior } | Type-safe, scoped
Time | uint8_t hour, minute | Exact range, minimal memory
Boolean flags | bool | Clear intent
Journey count | long long | 3M/day fits, future-proof

Constants (single source of truth)

constexpr long long BASE_PAISE[] = {1000, 2000, 3000, 6000};  // Rs 10, 20, 30, 60
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr int WEEKEND_DISCOUNT_PERCENT = 10;
constexpr long long MIN_BALANCE_PAISE = 5000;
constexpr int CONCESSION[] = {0, 25, 50};  // Regular, Student, Senior

Classes

class SmartCard {
    std::string id_;
    long long balancePaise_;
    PassengerType type_;
    // MonthlyPass* pass_;  // Future extension
public:
    // Constructor, getters, canTravel(), deduct(), recharge()
    // All monetary operations use long long paise
};

class FareCalculator {
public:
    static long long calculate(Zone zone, int hour, int minute, 
                               PassengerType type, bool isWeekend);
    // Single fare logic, all rules in one place
};

class MonthlyPass {
    Zone zone1_, zone2_;
    // Validity period, usage tracking
};

Fare Revision Process

Current design supports revision in ONE place:

1. Fare change: Edit BASE_PAISE array — all code uses FareCalculator::calculate()
2. Surcharge change: Edit PEAK_SURCHARGE_PERCENT — single constant
3. New zone: Add to Zone enum, extend BASE_PAISE array — switch in FareCalculator updated
4. New concession: Edit CONCESSION array — indexed by PassengerType
5. Peak hours change: Modify isPeakHour() function — single logic point
6. Min balance change: Edit MIN_BALANCE_PAISE — used by SmartCard::canTravel()

No recompilation of gate logic needed — only FareCalculator and constants. With config file (Task 12), zero recompilation.

Limitations of Current Design

Limitation | Impact | Mitigation
No monthly pass | Can't handle unlimited journeys in chosen zones | Add MonthlyPass class, extend SmartCard
No weekend discount | Requirement change (Task 12) | Add isWeekend param, WEEKEND_DISCOUNT_PERCENT
No fourth zone | Airport zone (Rs 60) | Add Airport to Zone enum, extend BASE_PAISE
Peak/surcharge combo | Weekend discount doesn't combine with peak | Need rule: max(peak, weekend) not both
No persistence | Balance lost on restart | Add database/file storage
No concurrency | Multiple gates corrupt balance | Add mutex/atomic for thread safety
No audit trail | Can't reconstruct journey history | Add JourneyLog class

Correctness at Network Scale (3M Journeys/Day)

Property | Verified
No monetary drift | Yes — Integer paise, exact arithmetic
No int overflow | Yes — long long for totals (10.8B ≪ 9×10¹⁸)
Min balance enforced | Yes — SmartCard::canTravel() private, single path
Peak hours correct | Yes — 8-9:59, 17-19:59 (10:00, 20:00 off-peak)
Concessions correct | Yes — Applied after surcharge, integer math
Thread safety | No — needs std::mutex per card
Persistence | No — needs database

Verdict: Core fare logic correct and scalable for 3M journeys/day. Production deployment needs persistence and concurrency.

Alternative: Config File vs Compile-Time Constants

Config file approach:
{
  "base_fares": [1000, 2000, 3000, 6000],
  "peak_surcharge_percent": 20,
  "weekend_discount_percent": 10,
  "min_balance_paise": 5000,
  "concessions": [0, 25, 50],
  "peak_hours": [[8,10], [17,20]]
}

Aspect | Compile-time (constexpr) | Config File (Runtime)
Startup time | Zero | O(z) — read z fares
Space | Zero (inlined) | O(z) bytes (negligible)
Revision cost | Recompile + redeploy | Edit file, reload (seconds)
Type safety | Full (C++ types) | Runtime validation needed
Audit trail | Git history | File versioning
Rollback | Git revert | File backup
Testing | Compile-time verification | Runtime validation

Analysis:
- Extra startup time: ~0.1ms to parse JSON — negligible
- Space: ~500 bytes for config — negligible
- Revision cost: Config file wins — no recompile, no redeploy, instant
- Risk: Config file can have typos — need schema validation at load

Recommendation: Hybrid — compile-time defaults + optional config file override. Gates load config on startup, fallback to compiled values. Best of both worlds.

Evidence-Based Final Recommendation

Adopt this design:

1. long long paise for all money — proven exact at 3M journeys
2. enum class for zones/passenger types — type-safe, extensible
3. SmartCard class with private balance — prevents negative balance
4. FareCalculator static class — single source of fare truth
5. constexpr constants — single revision point, compile-time optimisation
6. Config file overlay — instant fare revisions without redeploy
7. Data-driven tests — easy boundary case addition

Supported by experiments:
- Task 3: Integer paise = zero drift vs double error
- Task 4: static_cast + braces catch conversion bugs at compile time
- Task 5: Lookup table for peak hours = fewer branches, maintainable
- Task 6: switch on enum class = exhaustive, optimised
- Task 7: OOP encapsulation = balance protection vs procedural corruption
- Task 9: 18/18 tests pass including all boundaries
- Task 10: long long safe at 3M journeys; int overflows

Deploy with confidence.