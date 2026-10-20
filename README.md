DSA0102 Assignment 1 - Metro Smart-Card Fare System

Student: Aadarsh. B
Register Number: 192565009
Compiler: g++ 13.2.1 (Void Linux)
Standard: C++17

Compilation Command
g++ -std=c++17 -Wall -Wextra -o <output> <source.cpp>

Repository Structure
Task01/  - Object-Oriented Analysis (analysis.md)
Task02/  - Tokens and Constants (tokens_constants.md)
Task03/  - Data Types and Compatibility (data_types.md)
Task04/  - Implicit Conversions (implicit_conversions.md)
Task05/  - Operators and Precedence (operators_precedence.md)
Task06/  - Control Structures (control_structures.md, fare_logic.cpp)
Task07/  - Procedural vs OOP (procedural_vs_oop.md, procedural_fare.cpp, oop_fare.cpp)
Task08/  - Input and Output (input_output.md, gate_program.cpp)
Task09/  - Implementation & Testing (implementation_testing.md, fare_system.cpp)
Task10/  - Expectation vs Execution (expectation_execution.md, scale_test.cpp)
Task11/  - Final Recommendation (final_recommendation.md)
Task12/  - Design Challenge (design_challenge.md, redesigned_fare.cpp)

How to Run

Task 6 - Fare Logic
cd Task06
g++ -std=c++17 -Wall -Wextra -o fare_logic fare_logic.cpp
./fare_logic

Task 7 - Procedural vs OOP
cd Task07
g++ -std=c++17 -Wall -Wextra -o procedural_fare procedural_fare.cpp
./procedural_fare
g++ -std=c++17 -Wall -Wextra -o oop_fare oop_fare.cpp
./oop_fare

Task 8 - Gate Program
cd Task08
g++ -std=c++17 -Wall -Wextra -o gate_program gate_program.cpp
./gate_program

Task 9 - Full Test Suite
cd Task09
g++ -std=c++17 -Wall -Wextra -o fare_system fare_system.cpp
./fare_system

Task 10 - Scale Test
cd Task10
g++ -std=c++17 -Wall -Wextra -o scale_test scale_test.cpp
./scale_test

Task 12 - Design Challenge
cd Task12
g++ -std=c++17 -Wall -Wextra -o redesigned_fare redesigned_fare.cpp
./redesigned_fare

Key Results Summary

Task 1: 7 objects identified: Zone, FareTable, Time, Passenger, SmartCard, Journey, GateController
Task 2: constexpr + enum class preferred over #define for type safety
Task 3: long long paise for money — exact, no overflow at 3M journeys/day
Task 4: Implicit conversions identified; static_cast + braces catch bugs at compile time
Task 5: Correct precedence; lookup table for peak hours is maintainable
Task 6: Switch + integer arithmetic; boundary tests pass (10:00, 20:00 off-peak)
Task 7: OOP prevents negative balance; procedural allows corruption
Task 8: Safe cin with clear()/ignore(); formatted money output
Task 9: 18/18 tests pass including all boundaries, concessions, min balance
Task 10: Integer exact; double has ~0.04 paise error at 3M journeys; int overflows
Task 11: Recommended: long long paise, enum class, SmartCard, FareCalculator, config file overlay
Task 12: MODIFY design: array fares, MonthlyPass class, mutually exclusive peak/weekend

Design Decisions

1. Money as integer paise (long long) — eliminates floating-point drift entirely
2. enum class for zones/passenger types — type-safe, no implicit conversions
3. constexpr constants — single source of truth, compile-time evaluation
4. SmartCard with private balance — encapsulation prevents invalid state
5. FareCalculator static class — all fare rules in one place
6. Array-based fare lookup — extensible for new zones without code changes
7. Config file overlay (recommended) — instant fare revisions without recompilation
