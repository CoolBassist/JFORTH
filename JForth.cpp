#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

enum TokenType {NUMBER, SYMBOL};



struct Token{
    TokenType type;
    std::string symbol;
    int number;

    Token(std::string c){
        this->symbol = c;
        this->type = SYMBOL;
    }

    Token(int number){
        this->number = number;
        this->type = NUMBER;
    }

    int get_int(){
        return this->number;
    }

    std::string get_string(){
        return this->symbol;
    }
};

int lex_number(size_t* p, std::string input){
    std::string t = "";

    while(std::isdigit(input[*p]) && (*p) < input.size()){
        t += input[*p];
        (*p)++;
    }

    return std::stoi(t);
}

std::string lex_symbol(size_t* p, std::string input){
    std::string t = "";

    while(input[*p] != ' ' && input[*p] != '\t' && input[*p] != '\r' && input[*p] != '\n' && (*p) < input.size()){
        t += input[*p];
        (*p)++;
    }

    return t;
}

std::vector<Token> lex_input(std::string input){
    size_t p = 0;
    std::vector<Token> tokens;

    while(p < input.size()){
        while(input[p] == ' ' || input[p] == '\t' || input[p] == '\r' || input[p] == '\n'){
            p++;
            continue;
        }

        if(std::isdigit(input[p])){
            tokens.emplace_back(lex_number(&p, input));
        }else{
            tokens.emplace_back(lex_symbol(&p, input));
        }  
    }

    return tokens;
}


std::stack<int> stack;
std::unordered_map<std::string, std::vector<Token>> function_map;

void evaluate(std::vector<Token> tokens){
    if(tokens[0].type == SYMBOL && tokens[0].get_string() == ":"){
        if(tokens[1].type == SYMBOL && tokens.back().type == SYMBOL && tokens.back().get_string() == ";"){
            function_map[tokens[1].get_string()] = std::vector<Token>(tokens.begin()+2, tokens.end()-1);
            std::cout << "Function definition for '" << tokens[1].get_string() << "' created.\n";
            return;
        }
        return;
    }

    size_t p = 0;

    while(p < tokens.size()){
        Token t = tokens[p];

        if(t.type == NUMBER){
            stack.push(t.get_int());
            p++;
            continue;
        }

        if(t.get_string() == "CR"){
            std::cout << std::endl;
        }else if(t.get_string() == "."){
            std::cout << stack.top() << " ";
        }else if(t.get_string() == "+"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b+a);
        }else if(t.get_string() == "-"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b-a);
        }else if(t.get_string() == "*"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b*a);
        }else if(t.get_string() == "/"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b/a);
        }else if(t.get_string() == "DUP"){
            stack.push(stack.top());
        }else if(t.get_string() == ">"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b > a ? 1 : 0);
        }else if(t.get_string() == "<"){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b < a ? 1 : 0);
        }else if(t.get_string() == ">="){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b >= a ? 1 : 0);
        }else if(t.get_string() == "<="){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b <= a ? 1 : 0);
        }else if(t.get_string() == "=="){
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b == a ? 1 : 0);
        }else if(t.get_string() == "IF"){
            int boolean = stack.top();

            while(!boolean && (t.type == NUMBER || t.get_string() != "ELSE")){
                p++;
                t = tokens[p];
            }
        }else if(t.get_string() == "ELSE"){
            while(t.type == NUMBER || t.get_string() != "THEN"){
                p++;
                t = tokens[p];
            }
        }else if(t.get_string() == "THEN"){
            // do nothing
        }else{
            if (function_map.find(t.get_string()) == function_map.end()) {
                std::cout << "Cannot find '" << t.get_string() << "' in function defintions.\n";
            } else {
                evaluate(function_map[t.get_string()]);
            }
        }

        p++;
    }
}

int main(){
    std::string input = "";

    while(input != "exit"){
        std::cout << "\033[32m" << ">>> " << "\033[0m";
        std::getline(std::cin, input);

        std::vector<Token> tokens = lex_input(input);

        evaluate(tokens);
        std::cout << "\033[1m\033[32m" << "ok" << "\033[0m" << "\n";
    }
}