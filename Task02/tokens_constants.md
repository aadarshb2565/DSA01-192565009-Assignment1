Task 2: Tokens, Keywords, Identifiers and Constants

Five Kinds of Tokens in the Program

Keywords: class, enum, constexpr, switch, case, return, if, else, for, private, public, static, long long, bool, true, false

Identifiers: SmartCard, Zone, BASE_PAISE, calculateFare, balancePaise, canTravel, PEAK_SURCHARGE_PERCENT, MIN_BALANCE_PAISE, fare, journey

Constants: 1000, 2000, 3000 (integer literals), 20 (surcharge percent), 5000 (min balance), 8, 10, 17, 20 (peak hours), 'A' (char)

Operators: +, -, *, /, %, =, ==, <, >, <=, >=, &&, ||, ?:, ++, --, +=, -=, static_cast

Punctuators: {, }, (, ), [, ], ;, :, ::, ., ->, <<, >>

#define vs const vs constexpr

#define: no type safety, global scope, invisible to debugger, no compile-time evaluation, no storage

const: type safety yes, block scope, visible in debugger, compile-time not guaranteed, may allocate storage

constexpr: type safety yes, block scope, visible in debugger, guaranteed compile-time evaluation, usually no storage (inlined)

Why constexpr for fare table:

constexpr int BASE_PAISE[] = {1000, 2000, 3000};
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

Guarantees compile-time evaluation, usable in static_assert, array bounds, template parameters. Type-safe: BASE_PAISE[Zone::Inner] is int, not text. Scoped to class/namespace. Optimizer can inline and constant-fold.

enum vs enum class for Zones

C-style enum (unscoped) - problematic:
enum Zone { Inner, Middle, Outer };
int z = Inner;  // implicit conversion to int, dangerous

enum class (scoped) - preferred:
enum class Zone { Inner, Middle, Outer };
Zone z = Zone::Inner;  // explicit scope required
// int x = z;  // compile error - no implicit conversion
int x = static_cast<int>(z);  // explicit cast required

Why enum class is safer:
1. No namespace pollution - Inner not visible globally
2. No implicit conversion to int - prevents zone + 1 bugs
3. Type safety - Zone not interchangeable with int or other enums
4. Forward declaration possible - enum class Zone;
5. Underlying type controllable - enum class Zone : uint8_t

Naming Rules and Conventions

Classes/Types: PascalCase (SmartCard, FareTable, PassengerType)
Constants (constexpr): UPPER_SNAKE_CASE (BASE_PAISE, PEAK_SURCHARGE_PERCENT)
Variables/Functions: camelCase (balancePaise, calculateFare, canTravel)
Private members: camelCase with trailing underscore (balancePaise_)
Enum values: PascalCase (Zone::Inner, PassengerType::Student)
Macros (avoid): UPPER_SNAKE_CASE (MAX_ZONES)

Rules:
- Start with letter or underscore, then letters/digits/underscore
- No keywords as identifiers
- Case-sensitive: balance not equal to Balance
- Descriptive: minBalancePaise not mb or x

Alternative Concept and Complexity Analysis

If #define macros used instead of constexpr + enum class:

#define BASE_INNER 1000
#define BASE_MIDDLE 2000
#define BASE_OUTER 3000
#define PEAK_SURCHARGE 20
#define MIN_BALANCE 5000
#define INNER 0
#define MIDDLE 1
#define OUTER 2

Aspect | #define | constexpr + enum class
Type checking: none - BASE_INNER + "string" compiles | full - type mismatch caught
Scope: global - leaks everywhere | scoped to namespace/class
Debugging: no symbol info | full symbol info in debugger
Memory: no storage | usually no storage (inlined)
Running time: identical | identical
Array size usage: works | works
Template arg usage: no | yes
Namespace pollution: high | none

Conclusion: Zero runtime difference. constexpr + enum class catches errors at compile time, enables better tooling, and costs nothing at runtime. The only advantage of macros is textual substitution for non-type things like __FILE__, irrelevant here.