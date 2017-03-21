#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>


void RemoveLeftRecurtion(std::map<char, std::set<std::string>> &left_recurtion_rules,
                         std::map<char, std::set<std::string>> *rules) {
    for (auto &non_terminal : left_recurtion_rules) {
        char new_non_terminal = non_terminal.first - 27;
        for (auto &rule : non_terminal.second) {
            (*rules)[new_non_terminal].
                insert(std::string(rule.begin() + 1, rule.end()) + new_non_terminal);
            (*rules)[new_non_terminal].insert(std::string(rule.begin() + 1, rule.end()));
        }
        auto normal_rules = (*rules)[non_terminal.first];
        for (auto rule : normal_rules) {
            (*rules)[non_terminal.first].insert(rule + new_non_terminal);
        }
    }
}

bool SummazeFirsts(std::set<char> &that, std::set<char> *to_this) {
    bool was_added = false;
    for (auto &element : that) {
         if (element != '$') {
             if ((*to_this).find(element) == (*to_this).end()) {
                 (*to_this).insert(element);
                 was_added = true;
             }
        }
    }
    return was_added;
}

int main() {
    size_t number_of_rules;
    std::cin >> number_of_rules;
    std::string line;
    getline(std::cin, line);
    std::map<char, std::set<std::string>> rules;
    std::map<char, std::set<std::string>> left_recurtion_rules;

    std::map<char, std::set<char>> first_elements;

    for (size_t iter = 0; iter < number_of_rules; ++iter) {
        getline(std::cin, line);
        if (line[0] != line[3]) {
            rules[line[0]].insert(std::string(line.begin() + 3, line.end()));
            if (line[3] == '$') {
                first_elements[line[0]].insert('$');
            }
        } else {
            if (line.size() != 4 && (line.size() != 5 || line[4] != '$')) {
                rules[line[0]].insert(std::string(line.begin() + 3, line.end()));
            }
        }
    }

    std::map<char, std::set<std::string>> good_rules;


    std::map<std::pair<char, std::string>, std::set<char>> NonTerminals;
    std::set<char> GeneratingNonTerminal;
    for (auto &element : rules) {
        for (auto &rule: element.second) {
            if (rule[0] == '$') {
                GeneratingNonTerminal.insert(element.first);
                good_rules[element.first].insert(rule);
                continue;
            }

            bool has_upper = false;
            for (auto &letter : rule) {
                if (!islower(letter)) {
                    NonTerminals[std::pair<char, std::string>(element.first, rule)].insert(letter);
                    has_upper = true;
                }
            }
            if (!has_upper) {
                GeneratingNonTerminal.insert(element.first);
                good_rules[element.first].insert(rule);
            }
        }
    }

    bool Continue;
    do {
        Continue = false;

        for (auto &element : NonTerminals) {
            size_t counter = 0;
            for (auto &non_term : element.second) {
                if (GeneratingNonTerminal.find(non_term) == GeneratingNonTerminal.end()) {
                    break;
                }
                ++counter;
            }

            if (counter == element.second.size()) {
                GeneratingNonTerminal.insert(element.first.first);
                if (good_rules[element.first.first].find(element.first.second) == 
                    good_rules[element.first.first].end()) {
                    good_rules[element.first.first].insert(element.first.second);
                    Continue = true;
                }
            }
        }
    } while (Continue);

    for (auto &element : good_rules) {
        auto rules = element.second;
        for (auto &rule : rules) {
            if (rule[0] == element.first) {
                left_recurtion_rules[element.first].insert(rule);
                good_rules[element.first].erase(rule);
            }
        }
    }

    RemoveLeftRecurtion(left_recurtion_rules, &good_rules);

    for (char iter = 'a'; iter <= 'z'; ++iter) {
        first_elements[iter].insert(iter);
    }

    first_elements['$'].insert('$');

    Continue = true;
    do {
        Continue = false;
        for (auto &element : good_rules) {
            for (auto &rule : element.second) {
                bool nonstop = true;
                for (size_t iter = 0; iter < rule.size() && nonstop;) {
                    if (SummazeFirsts(first_elements[rule[iter]], &first_elements[element.first])) {
                        Continue = true;
                    }
                    if (first_elements[rule[iter]].find('$') == first_elements[rule[iter]].end()) {
                        nonstop = false;
                    } else { 
                        ++iter; 
                    }
                }

                if (nonstop && first_elements[element.first].find('$') == 
                    first_elements[element.first].end()) {
                    first_elements[element.first].insert('$');
                    Continue = true;
                }
            }
        }
    } while (Continue);

    std::set<char> result;
    for (auto &element : first_elements) {
        if (element.first == 'S') {
            SummazeFirsts(element.second, &result);
        }
    }

    for (auto &res : result) {
        std::cout << res;
    }

    return 0;
}
