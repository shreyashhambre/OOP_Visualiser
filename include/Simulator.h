#pragma once
#include <string>
#include <vector>

enum class Concept {
    ClassObject,
    Encapsulation,
    Inheritance,
    Polymorphism,
    Abstraction,
    Constructor,
    Destructor,
    VirtualFunction,
    Memory
};

struct Step {
    std::string title;
    std::string description;
    int codeLine = -1;
    std::string output;
};

struct Simulation {
    Concept concept;
    std::string name;
    std::string definition;
    std::vector<std::string> code;
    std::vector<Step> steps;
};

class Simulator {
public:
    Simulator();

    void select(Concept c);
    void reset();
    void step();
    void pause();
    void runToggle();

    bool isRunning() const;
    int currentStep() const;
    int totalSteps() const;
    const Simulation& simulation() const;

private:
    Simulation current_;
    int stepIndex_ = 0;
    bool running_ = false;
    std::vector<Simulation> all_;

    void buildSimulations();
};
