#include "interpreter.hpp"

Interpreter::Interpreter() : running(true) {}

bool Interpreter::isRunning() const {
    return running;
}

void Interpreter::quit() {
    running = false;
}
