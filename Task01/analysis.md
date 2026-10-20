Task 1: Object-Oriented Analysis

Objects in the problem

Card
Data: card ID, balance in paise, passenger type (regular, student, senior)
Behaviour: deduct fare, check if travel allowed, recharge, get balance

Passenger
Data: type (regular, student, senior), concession percentage
Behaviour: get concession percentage

Journey
Data: entry zone, exit zone, entry time, passenger type, card reference
Behaviour: compute fare, execute (validate and deduct)

Zone
Data: name (inner, middle, outer), base fare in paise
Behaviour: get base fare

FareTable
Data: base fares for each zone, peak surcharge percent (20), min balance (5000 paise)
Behaviour: calculate fare for zone/time/passenger type

GateController
Data: list of journeys processed, daily collection total
Behaviour: process journey, generate report

Procedural vs Object-Oriented

Procedural style would use global variables for balance, fare arrays, and free functions like calculate_fare(), can_travel(), deduct_fare(). The balance would be a global variable that any function could modify directly. Adding a new rule like monthly pass would require changing multiple functions.

Object-oriented style encapsulates balance inside SmartCard class. Only deduct() and recharge() can modify it. Fare logic is in FareCalculator class. Adding monthly pass means creating a MonthlyPass class and adding a pointer to SmartCard. The existing code hardly changes.

OOP Principles

Encapsulation: SmartCard keeps balance private. Only deduct() and can_travel() access it. This prevents negative balance.

Abstraction: FareCalculator hides the fare computation details. The gate just calls calculate().

Inheritance: PassengerType could be a base class with Regular, Student, Senior subclasses each overriding get_concession(). MonthlyPass could inherit from a Pass base class.

Polymorphism: A Passenger pointer can point to Student or Senior object. Calling get_concession() gives the right value without if-else chains.

Rules Every Journey Must Satisfy

1. Balance after fare deduction must be at least 5000 paise (Rs 50)
2. Fare calculated correctly: base fare of farthest zone, plus 20% peak surcharge during 8-10am and 5-8pm, concession applied after surcharge
3. Peak hours: 8:00 to 9:59 and 17:00 to 19:59. 10:00 and 20:00 are off-peak
4. Zone must be valid (inner, middle, outer)
5. No negative balance ever
6. Concession: student 25%, senior 50%, applied after peak surcharge

Assumptions

- All money stored as integer paise (long long), no floating point
- Peak surcharge 20% on base fare before concession
- Journey is one-way, return trip is separate journey
- System provides hour and minute, no timezone issues
- Fourth zone (airport, Rs 60) reserved for future

Alternative Concept and Complexity Analysis

Procedural version: global balance variable, global fare array, free functions. Both are O(1) time per journey and O(1) memory per card. But when a fare rule changes, procedural needs changes in multiple functions. OOP only changes FareCalculator::calculate(). For 3 million journeys per day, both scale the same, but OOP is maintainable.