#include "Simulator.h"
#include <algorithm>

static Simulation makeSim(Concept c, const std::string& name,
                          const std::string& def,
                          std::vector<std::string> code,
                          std::vector<Step> steps) {
    return {c,name,def,code,steps};
}

Simulator::Simulator() {
    buildSimulations();
    select(Concept::ClassObject);
}

void Simulator::buildSimulations() {
    all_.push_back(makeSim(
        Concept::ClassObject, "Class & Object",
        "A class is a blueprint; an object is an instance created from that class.",
        {
            "class Student {",
            "public:",
            "    string name;",
            "    int rollNo;",
            "    void display();",
            "};",
            "Student s1;",
            "s1.name = \"Rahul\";",
            "s1.rollNo = 18;",
            "s1.display();"
        },
        {
            {"Declare class", "Student class defines data and behavior.", 1, ""},
            {"Create object", "s1 is created as an object of Student.", 7, ""},
            {"Assign data", "The object receives its own name and roll number.", 8, ""},
            {"Call method", "The object's display() member function is called.", 10, "Rahul - 18"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Encapsulation, "Encapsulation",
        "Encapsulation bundles data and methods and controls access to internal data.",
        {
            "class BankAccount {",
            "private:",
            "    int balance = 5000;",
            "public:",
            "    void deposit(int amount);",
            "    int getBalance();",
            "};",
            "account.balance = 10000;",
            "account.deposit(1000);",
            "account.getBalance();"
        },
        {
            {"Private data", "balance is hidden inside the class.", 3, ""},
            {"Blocked access", "Direct access to private balance is not allowed.", 8, "ACCESS DENIED"},
            {"Public method", "deposit() provides a controlled way to change the balance.", 9, ""},
            {"Read through method", "getBalance() exposes the permitted result.", 10, "Balance = 6000"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Inheritance, "Inheritance",
        "Inheritance allows a derived class to reuse accessible members of a base class.",
        {
            "class Vehicle {",
            "public:",
            "    void start();",
            "};",
            "",
            "class Car : public Vehicle {",
            "public:",
            "    void drive();",
            "};",
            "Car c;",
            "c.start();",
            "c.drive();"
        },
        {
            {"Base class", "Vehicle provides the base behavior.", 1, ""},
            {"Inheritance", "Car publicly inherits Vehicle.", 6, ""},
            {"Inherited method", "Car can use the accessible start() method.", 11, "Vehicle::start()"},
            {"Derived method", "Car adds its own drive() behavior.", 12, "Car::drive()"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Polymorphism, "Polymorphism",
        "Polymorphism allows one interface to represent different behaviors.",
        {
            "class Animal {",
            "public:",
            "    virtual void sound();",
            "};",
            "class Dog : public Animal {",
            "public:",
            "    void sound() override;",
            "};",
            "Animal* a = new Dog();",
            "a->sound();"
        },
        {
            {"Base interface", "Animal declares a virtual sound() function.", 3, ""},
            {"Derived override", "Dog supplies its own implementation.", 8, ""},
            {"Base pointer", "a points to a Dog object.", 10, ""},
            {"Dynamic dispatch", "The call resolves to Dog::sound().", 11, "BARK!"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Abstraction, "Abstraction",
        "Abstraction exposes an essential interface while hiding implementation details.",
        {
            "class Vehicle {",
            "public:",
            "    virtual void start() = 0;",
            "};",
            "",
            "class Car : public Vehicle {",
            "public:",
            "    void start() override;",
            "};",
            "Car c;",
            "c.start();"
        },
        {
            {"Abstract interface", "Vehicle exposes the required start() operation.", 3, ""},
            {"Hidden details", "Engine, ignition and sensors are implementation details.", 3, ""},
            {"Concrete class", "Car implements the abstract operation.", 8, ""},
            {"Use interface", "The user simply calls start().", 11, "Car started"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Constructor, "Constructor",
        "A constructor runs when an object is created and initializes its state.",
        {
            "class Student {",
            "public:",
            "    Student() {",
            "        name = \"Rahul\";",
            "    }",
            "    string name;",
            "};",
            "Student s1;"
        },
        {
            {"Create object", "Memory is reserved for s1.", 9, ""},
            {"Constructor call", "Student() executes automatically.", 4, "Constructor()"},
            {"Initialize state", "The constructor assigns the initial name.", 5, "name = Rahul"},
            {"Ready", "The object is now initialized and usable.", 9, "Object ready"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Destructor, "Destructor",
        "A destructor runs when an object is destroyed and can release owned resources.",
        {
            "class Student {",
            "public:",
            "    ~Student() {",
            "        cout << \"Destroyed\";",
            "    }",
            "};",
            "Student s1;",
            "// scope ends",
            "// destructor runs"
        },
        {
            {"Object exists", "s1 is alive inside its scope.", 7, "Object alive"},
            {"Scope ends", "The object's lifetime reaches its end.", 8, ""},
            {"Destructor call", "~Student() executes.", 4, "Destructor()"},
            {"Released", "The object's lifetime is complete.", 9, "Object destroyed"}
        }
    ));

    all_.push_back(makeSim(
        Concept::VirtualFunction, "Virtual Function",
        "A virtual function enables dynamic dispatch through a base-class interface.",
        {
            "class Animal {",
            "public:",
            "    virtual void sound();",
            "};",
            "class Dog : public Animal {",
            "public:",
            "    void sound() override;",
            "};",
            "Animal* p = new Dog();",
            "p->sound();"
        },
        {
            {"Virtual declaration", "Animal marks sound() as virtual.", 3, ""},
            {"Dog override", "Dog provides the specialized implementation.", 8, ""},
            {"Pointer", "p stores a reference to the Dog object.", 10, ""},
            {"Dispatch", "The virtual call reaches Dog::sound().", 11, "BARK!"}
        }
    ));

    all_.push_back(makeSim(
        Concept::Memory, "Stack / Heap",
        "This simplified model shows local variables and dynamically allocated objects.",
        {
            "Student s1;",
            "Student* p = new Student();",
            "p->name = \"Rahul\";",
            "delete p;"
        },
        {
            {"Stack object", "s1 is a local object shown in the stack region.", 1, ""},
            {"Heap allocation", "new Student() creates a dynamic object in the heap region.", 2, ""},
            {"Use heap object", "p refers to the dynamically allocated object.", 3, ""},
            {"Delete", "delete p releases the dynamic object's lifetime.", 4, "Memory released"}
        }
    ));
}

void Simulator::select(Concept c) {
    auto it = std::find_if(all_.begin(), all_.end(),
        [c](const Simulation& s){ return s.concept == c; });
    if (it != all_.end()) {
        current_ = *it;
        reset();
    }
}

void Simulator::reset() {
    stepIndex_ = 0;
    running_ = false;
}

void Simulator::step() {
    if (current_.steps.empty()) return;
    if (stepIndex_ < (int)current_.steps.size())
        ++stepIndex_;
    if (stepIndex_ >= (int)current_.steps.size())
        running_ = false;
}

void Simulator::pause() { running_ = false; }
void Simulator::runToggle() { running_ = !running_; }
bool Simulator::isRunning() const { return running_; }
int Simulator::currentStep() const { return stepIndex_; }
int Simulator::totalSteps() const { return (int)current_.steps.size(); }
const Simulation& Simulator::simulation() const { return current_; }
