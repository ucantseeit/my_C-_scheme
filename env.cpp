#include "env.hpp"
#include "built_in.hpp"

void Frame::insert(const string & symbol, const SyntaxTree & content) {
    if (symbolTable.find(symbol) != symbolTable.end()) {
        std::cout << symbol << " has been defined." << std::endl;
        return;
    }
    symbolTable[symbol] = content;
}

SyntaxTree Frame::lookup(const string & symbol) const {
    if (symbolTable.find(symbol) != symbolTable.end()) {
        return symbolTable.at(symbol);
    } else if (symbolTable.find(symbol) == symbolTable.end() && parentFrame != nullptr) {
        return parentFrame->lookup(symbol);
    } else {
        std::cout << "the variable " << symbol << " is not defined" << std::endl;
        return SyntaxTree(-1, INT);
    }
}

Frame setGlobalFrame() {
    Frame gFrame;
    gFrame.insert("+", SyntaxTree(&add, PROC));
    gFrame.insert("-", SyntaxTree(&minus, PROC));
    gFrame.insert("*", SyntaxTree(&multiple, PROC));
    gFrame.insert("/", SyntaxTree(&divide, PROC));
    
    gFrame.insert("=", SyntaxTree(&equalNum, PROC));
    gFrame.insert("!=", SyntaxTree(&notEqual, PROC));
    gFrame.insert(">", SyntaxTree(&greaterThan, PROC));
    gFrame.insert("<", SyntaxTree(&lessThan, PROC));
    gFrame.insert(">=", SyntaxTree(&greaterEqual, PROC));
    gFrame.insert("<=", SyntaxTree(&lessEqual, PROC));

    gFrame.insert("#t", SyntaxTree(true, BOOL));
    gFrame.insert("#f", SyntaxTree(false, BOOL));

    gFrame.insert("load", SyntaxTree(&load, PROC));


    return gFrame;
}
Frame global_env = setGlobalFrame();



