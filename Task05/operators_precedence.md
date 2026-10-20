Task 5: Operators and Precedence

Key Expressions with Precedence & Associativity

Peak Hour Check
bool isPeak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);

Operator precedence (highest to lowest):
1. >=, < (relational) — left-to-right
2. && (logical AND) — left-to-right
3. || (logical OR) — left-to-right
4. = (assignment) — right-to-left

Evaluation order:
hour >= 8        → true/false
hour < 10        → true/false
(hour >= 8) && (hour < 10)  → true/false  (first &&)
hour >= 17       → true/false
hour < 20        → true/false
(hour >= 17) && (hour < 20) → true/false  (second &&)
(...) || (...)   → final result

Why parentheses help readability:
// With parentheses (explicit grouping)
(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)

// Without parentheses (same precedence, but harder to read)
hour >= 8 && hour < 10 || hour >= 17 && hour < 20

Both are equivalent because && binds tighter than ||. But the first shows intent: two distinct time windows.

Surcharge Calculation (Integer Arithmetic)
long long fare = baseFare;
if (peak) fare = fare * 120 / 100;  // 20% surcharge

Precedence: * and / same level, left-to-right → (fare * 120) / 100
Associativity: Left-to-right — critical for integer arithmetic to avoid truncation.

Why not fare * (120 / 100)? → 120/100 = 1 (integer division), loses surcharge.

Concession Calculation
fare = fare * (100 - concessionPct) / 100;
Same precedence logic — multiplication before division.

Compound Assignment
balance -= fare;  // Equivalent to: balance = balance - fare
Precedence: Lowest (assignment), right-to-left associativity.

x++ vs ++x and i = i++

Expression | Value of Expression | Side Effect
++x (pre) | New value (after increment) | x incremented
x++ (post) | Old value (before increment) | x incremented

int x = 5;
int y = ++x;  // y = 6, x = 6
int z = x++;  // z = 6, x = 7

i = i++ is Undefined Behaviour (C++17):
- Modifies i twice without sequencing
- Result unpredictable — compiler may warn: operation on 'i' may be undefined

/ and % with Negative Operands (C++17: Truncated toward zero)

Expression | Result | Rule
10 / 3 | 3 | Truncate toward zero
10 % 3 | 1 | a = (a/b)*b + a%b
-10 / 3 | -3 | Truncate toward zero
-10 % 3 | -1 | -10 = (-3)*3 + (-1)
10 / -3 | -3 | Truncate toward zero
10 % -3 | 1 | 10 = (-3)*(-3) + 1
-10 / -3 | 3 | Truncate toward zero
-10 % -3 | -1 | -10 = 3*(-3) + (-1)

Predict the Output (BEFORE RUNNING)

Program:
#include <iostream>

int main()
{
    int x = 5, y = 3;
    std::cout << x + y * 2 << ' ' << (x + y) * 2 << '\n';

    int z = x++ + ++y;
    std::cout << z << ' ' << x << ' ' << y << '\n';

    std::cout << (x > y ? x : y) << ' ' << x % 4 * 2 << '\n';
    std::cout << 10 / 3 << ' ' << 10 % 3 << ' ' << -10 / 3 << ' ' << -10 % 3 << '\n';

    int k = 0;
    for (int i = 0; i < 10; i += 3) k += i;
    std::cout << k << '\n';
}

My predicted output:
11 16
9 6 4
6 4
3 1 -3 -1
18

Actual Output & Explanation (AFTER RUNNING)

Compilation & Execution:
g++ -std=c++17 -Wall -Wextra -o task5_pred task5_pred.cpp
./task5_pred

Actual output:
11 16
9 6 4
6 4
3 1 -3 -1
18

Line-by-line operator analysis:

Line 1: x + y * 2 → 11. * (higher) → +: 5 + (3*2) = 11
Line 1: (x + y) * 2 → 16. () → +: (5+3)*2 = 16
Line 2: x++ + ++y → 9. ++y (pre) → y=4; x++ (post) → uses 5, then x=6; + → 5+4=9. z=9, x=6, y=4
Line 3: x > y ? x : y → 6. > → ?:: 6>4 true → 6
Line 3: x % 4 * 2 → 4. % and * same precedence, left-to-right: (6%4)*2 = 2*2 = 4
Line 4: 10 / 3 → 3. /: 3
Line 4: 10 % 3 → 1. %: 1
Line 4: -10 / 3 → -3. /: -3 (truncated toward zero)
Line 4: -10 % 3 → -1. %: -1 (sign follows dividend)
Line 5: Loop: i = 0, 3, 6, 9 → k += i: 0+3+6+9 = 18

Alternative Concept & Complexity Analysis

If peak-hour check used lookup table bool peak[24] instead of &&/||:

constexpr bool PEAK_HOURS[24] = {
    false, false, false, false, false, false, false, false,  // 0-7
    true,  true,  false, false, false, false, false, false,  // 8-15
    false, true,  true,  false, false, false, false, false   // 16-23
};
bool isPeak = PEAK_HOURS[hour];

Aspect | &&/|| Expression | Lookup Table
Time per check | 2-4 comparisons (branch prediction) | 1 array access + bounds check
Space | 0 extra | 24 bytes (compile-time constant)
Branch prediction | Good for predictable hours | Always 1 memory access
Maintainability | Change logic in code | Change table values
Extensibility | Harder (add weekend?) | Easy: peak[hour][isWeekend]

Analysis: Both O(1). Lookup table uses 24 bytes (negligible), avoids branch misprediction on boundary hours (8, 10, 17, 20), and is more maintainable for complex rules (e.g., weekend + peak). Recommended for production.

Evidence: Correct Expressions in Fare System

// Peak check - explicit grouping for readability
constexpr bool isPeakHour(int hour) {
    return (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
}

// Surcharge - multiplication before division, integer arithmetic
constexpr long long applySurcharge(long long baseFare, bool peak) {
    return peak ? (baseFare * 120 / 100) : baseFare;
}

// Concession - same pattern
constexpr long long applyConcession(long long fare, int concessionPct) {
    return fare * (100 - concessionPct) / 100;
}

// Complete fare
constexpr long long calculateFare(Zone zone, bool peak, int concessionPct) {
    long long fare = BASE_PAISE[static_cast<int>(zone)];
    fare = applySurcharge(fare, peak);
    fare = applyConcession(fare, concessionPct);
    return fare;
}

All expressions verified with test cases at boundaries (8:00, 9:59, 10:00, 17:00, 19:59, 20:00).