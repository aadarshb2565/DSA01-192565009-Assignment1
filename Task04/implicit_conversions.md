Task 4: Control Implicit Conversions

Implicit Conversion Points in Fare System

Location | Expression | Implicit Conversion
fare = base * 120 / 100 | int * int / int | Integer arithmetic throughout — no conversion
if (peak) | bool from condition | int → bool (0→false, non-zero→true)
balance >= fare | long long >= long long | None (same type)
hour >= 8 && hour < 10 | uint8_t >= int | uint8_t promoted to int
fare * (100 - concession) / 100 | long long * int / int | int promoted to long long
cout << balance / 100 | long long / int | int promoted to long long
setfill('0') | char to stream | char → char_type
Zone z = Zone::Inner | enum class | No implicit conversion to int (compile error if attempted)

Specific Conversion Issues

Integer Division & Integer-to-Double
int a = 7, b = 2;
double c = a / b;        // a/b = 3 (int division), then 3 → 3.0
double d = static_cast<double>(a) / b;  // 7.0 / 2 = 3.5

Signed vs Unsigned Comparison
unsigned int u = 1;
int s = -1;
s < u  // s converted to unsigned: -1 → 4294967295, result: false
// Warning: comparison of signed and unsigned

Narrowing Conversions & Brace Initialisation
int x = 3.7;        // Allowed: narrowing, truncates to 3
int y{3.7};       // ERROR: narrowing conversion forbidden in brace init
char c = 300;     // Allowed: implementation-defined (typically 44)
char d{300};      // ERROR: narrowing

static_cast When Needed
// enum class to underlying type
int zoneIndex = static_cast<int>(Zone::Outer);

// int to enum class (validated)
Zone z = static_cast<Zone>(index);  // Must ensure index is valid

// long long to int (narrowing, explicit)
int dailyJourneys = static_cast<int>(totalJourneys);  // Checked at runtime

Predict the Output (BEFORE RUNNING)

Program:
#include <iostream>

int main()
{
    int a = 7, b = 2;
    double c = a / b;
    double d = static_cast<double>(a) / b;
    std::cout << c << ' ' << d << '\n';

    char ch = 'A' + 2;
    std::cout << ch << ' ' << ch + 1 << '\n';

    bool flag = 5;
    std::cout << flag << ' ' << std::boolalpha << flag << '\n';

    unsigned int u = 1;
    int s = -1;
    std::cout << (s < u ? "s < u" : "s >= u") << '\n';
}

My predicted output:
3 3.5
C 68
1 true
s >= u

Actual Output & Explanation (AFTER RUNNING)

Compilation & Execution:
g++ -std=c++17 -Wall -Wextra -o task4_pred task4_pred.cpp
./task4_pred

Actual output:
3 3.5
C 68
1 true
s >= u

Line-by-line explanation:

Line 1: c = a / b → 3. Integer division: 7/2 = 3 (int), then int→double: 3 → 3.0
Line 1: d = static_cast<double>(a) / b → 3.5. Explicit cast: a → 7.0, then double/int division: 7.0/2 = 3.5
Line 2: ch = 'A' + 2 → C. Char promotion: 'A' (65) + 2 = 67 → 'C' (char)
Line 2: ch + 1 → 68. Integer promotion: ch promoted to int (67), +1 = 68, printed as int
Line 3: flag = 5 → 1. Int→bool: non-zero → true (prints as 1 without boolalpha)
Line 3: std::boolalpha << flag → true. Bool formatting: true printed as "true"
Line 4: s < u → s >= u. Signed→unsigned: s (-1) → 4294967295u, comparison: 4294967295 < 1 is false

Alternative Concept & Complexity Analysis

If every conversion were explicit with static_cast + brace init:

// Instead of: double c = a / b;
double c = static_cast<double>(a) / static_cast<double>(b);

// Instead of: char ch = 'A' + 2;
char ch{'A' + 2};  // Brace init - would error if narrowing

// Instead of: bool flag = 5;
bool flag{static_cast<bool>(5)};  // Explicit

// Instead of: s < u
static_cast<unsigned int>(s) < u  // Explicit, but still wrong logic
// Better: s < static_cast<int>(u)  // If u fits in int

Aspect | Implicit | Explicit (static_cast + braces)
Running time | Identical | Identical (casts compile away)
Memory | Identical | Identical
Errors caught at compile time | Few (only narrowing in braces) | Many: narrowing, signed/unsigned, enum→int
Errors caught at runtime | Silent bugs (wrong comparison) | Same, but intent documented
Code verbosity | Low | Higher

Verdict: Explicit casts + brace initialisation catch bugs at compile time (narrowing, unintended signed/unsigned) with zero runtime cost. The fare system uses static_cast for enum class → int and brace init for all variable definitions.