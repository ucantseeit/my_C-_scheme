#include <exception>
#include <unordered_map>
#include <numeric>
#include "eval.hpp"
#include "special_forms.hpp"

using namespace SyntaxTree_;

SyntaxTree eval_expr(const SyntaxTree & expr, Frame & env) {
    using std::cout, std::endl, std::vector;

    if (expr.isLeaf()) {
        if (expr.isSymbol() && isVariable(get<Symbol>(expr.value))) {
            auto result = env.lookup(get<Symbol>(expr.value));
            if (result == global_env.endOfFrame()) {
                throw std::invalid_argument(
                    "the symbol " + get<Symbol>(expr.value) + "is undefined.");
            }
            return eval_expr(result->second, env);
        } else {
            return expr;
        }
    }
    
    Symbol sym;   SyntaxTree findSymResult;  
    Proc op = nullptr;
    if (expr.items.front().isSymbol()) {
        sym = get<Symbol>(expr.items.front().value);
    } else if (expr.items.front().isSubexpr()) {
        auto item = eval_expr(expr.items.front(), env);
        if (item.isSymbol()) {
            sym = get<Symbol>(item.value);
        }  else if (item.isProc()) {
            op = get<Proc>(item.value);
        } else {
            throw std::invalid_argument(
                "the first item in the parentheses is not a procedure");
        }
    } else {
        throw std::invalid_argument(
            "the first item in the parentheses is not a procedure");
    }
    



    // handle special forms
    if (!sym.empty() && specialForms.find(sym) != specialForms.end()) {
        return handleSpecialForms(expr.items, env);
    }

    if (!sym.empty() && env.lookup(sym) == env.endOfFrame()) {
        throw std::invalid_argument(
            sym + " is undefined.");
    } else if (!sym.empty()) {
        findSymResult = env.lookup(sym)->second;
    }

    // handle the built-in procedures
    if (findSymResult.isProc()) {
        op = get<Proc>(findSymResult.value);
    }
    if (op != nullptr) {
        list<SyntaxTree> arguments;
        auto track = std::next(expr.items.begin());
        for (; track != expr.items.end();
            track = next(track)) {
            if (track->isSubexpr() || track->isSymbol()) {
                arguments.emplace_back(eval_expr(*track, env));
            } else {
                arguments.emplace_back(*track);
            }       
        }

        return op(arguments);
    } 
    
    // handle the user-defined functions
    if (findSymResult.isLambda()) {
        // create a new frame to evaluate the call
        Lambda lamb = get<Lambda>(findSymResult.value);
        Frame newFrame(lamb.pLocalFrame);

        // get the arguments of the call
        list<SyntaxTree> arguments;
        auto track = std::next(expr.items.begin());
        for (; track != expr.items.end();
            track = next(track)) {
            if (track->isSubexpr() || track->isSymbol()) {
                arguments.emplace_back(eval_expr(*track, env));
            } else {
                arguments.emplace_back(*track);
            }       
        }

        if (lamb.params.size() != arguments.size()) {
            throw std::invalid_argument(
                "arity is not matched");
        }

        // bind the arguments to the formal parameters
        auto parg = arguments.begin();
        for (auto & para : lamb.params) {
            newFrame.insert(para, *parg);
            parg = next(parg);
        }

        // evaluate all the expressions in the body of user defined function
        return eval_sequence(lamb.body, newFrame);
    }

    return nil;
}

SyntaxTree eval_sequence(const list<SyntaxTree> & exprs, Frame & env) {
    for (auto pexpr = exprs.begin(); next(pexpr) != exprs.end();
         pexpr++) {
        eval_expr(*pexpr, env);
    }
    return eval_expr(exprs.back(), env);
}
