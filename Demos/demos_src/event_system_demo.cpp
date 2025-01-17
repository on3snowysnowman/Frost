#include <iostream>

// #include "EventHandler.hpp"
#include "EventHandler.hpp"


// Example class with member functions
class ExampleClass {
public:
    int multiply(int a, int b) const {
        return a * b;
    }

    void print_message(const std::string& message) const {
        std::cout << "Message from ExampleClass: " << message << std::endl;
    }
};


int main() 
{
    
    ExampleClass instance;

    event_id sample_event = EventHandler::register_event<int, int, int>(
        "CALL", 
        std::function<int(int, int)>(
            [&instance](int a, int b) { return instance.multiply(a, b); }));

    std::cout << EventHandler::invoke_event<int, int, int>("CALL", 4, 5) << '\n';

    event_id sample_str_event = EventHandler::register_event<void, const std::string&>(
        "CALLSTR",
        std::function<void(const std::string&)>(
            [&instance](const std::string& message) { instance.print_message(message); }
        )
    );

    EventHandler::invoke_event<void, const std::string&>("CALLSTR", "This is a message.");

    EventHandler::unsubscribe("CALLSTR", sample_str_event);
    return 0;
}
