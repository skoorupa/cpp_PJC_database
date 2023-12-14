class Interpreter {
    bool running;

public:
    Interpreter();
    auto runAST();

    bool isRunning() const;
    void quit();
};
