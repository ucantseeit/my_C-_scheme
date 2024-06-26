#include <iostream>
#include <exception>
#include "tokenize.hpp"

namespace tokens {

    std::unordered_set<char> delimeter = {' ', '\t'};

    vector<token> tokenize(const string & input) {
        vector<token> tokens;
        string word;
        for (auto track : input) {
            // when meeting ( or )
            if (track == '(' || track == ')') {
                if (!word.empty()) {
                    tokens.emplace_back(word);
                    word.clear();
                }
                tokens.emplace_back(string(1, track));
                continue;
            }

            // when meeting a delimeter
            if (delimeter.find(track) != delimeter.end()) {
                // push back a word when the word is not empty
                if (!word.empty()) {
                    tokens.emplace_back(word);
                    word.clear();
                }
                // continue when the word is empty
                continue;
            }

            // when meeting a normal char
            word += track;
        }

        if (!word.empty()) {
            tokens.emplace_back(word);
        }

        return tokens;
    }


}



// int main() {
//     using namespace tokens;
//     vector<token> tokens;
//     string str = "(123 5 6)";
//     tokens = tokenize(str);
//     for (auto& t : tokens) {
//         std::cout << t << " ";
//     }
//     std::cout << '\n';

//     string str1 = "(+ (w3e 3) ((i)))";
//     tokens = tokenize(str1);
//     for (auto& t : tokens) {
//         std::cout << t << " ";
//     }
//     std::cout << '\n';

//     string str2 = "(2.5 3.1.1 3.654a k 5e1)";
//     tokens = tokenize(str2);
//     for (auto & t : tokens) {
//         std::cout << t << " ";
//     }
//     std::cout << '\n';

//     return 0;
// }
