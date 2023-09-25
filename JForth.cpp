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
    bool in_string = false;

    while((*p) < input.size() && ((input[*p] != ' ' && input[*p] != '\t' && input[*p] != '\r' && input[*p] != '\n') || in_string)){
        if(input[*p] == '"'){
            in_string = !in_string;
        }
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

bool check_stack_size(size_t size){
    if(stack.size() < size){
        std::cout << "\033[31m" << "expected at least " << size << " number" << (size==1? "" : "s") << " on stack " << "\033[0m";
        return true;
    }

    return false;
}

bool evaluate(std::vector<Token> tokens){
    if(tokens[0].type == SYMBOL && tokens[0].get_string() == ":"){
        if(tokens[1].type == SYMBOL && tokens.back().type == SYMBOL && tokens.back().get_string() == ";"){
            function_map[tokens[1].get_string()] = std::vector<Token>(tokens.begin()+2, tokens.end()-1);
            std::cout << "\033[32m" << "Function definition for '" << tokens[1].get_string() << "' created. " << "\033[0m";
            return false;
        }
        std::cout << "\033[31m" << "Incomplete function definition ";
        return true;
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
        }else if(t.get_string()[0] == '.'){
            if(t.get_string().size() > 1){
                if(t.get_string()[1] != '"' && t.get_string()[t.get_string().size()-1] != '"'){
                    std::cout << "\033[31m" << "Incomplete string " << "\033[0m";
                    return true;
                }else{
                    for(size_t i = 2; i < t.get_string().size()-1; i++){
                        std::cout << t.get_string()[i];
                    }
                }
            }else{
                if(check_stack_size(1)) return true;
                std::cout << stack.top() << " ";
            }
            
        }else if(t.get_string() == "IF"){
            if(check_stack_size(1)) return true;

            int boolean = stack.top();
            stack.pop();

            while(!boolean && (t.type == NUMBER || (t.get_string() != "ELSE" && t.get_string() != "THEN"))){
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
        }else if(t.get_string() == "DUP"){
            if(check_stack_size(1)) return true;
            stack.push(stack.top());
        }else if(t.get_string() == "DROP"){
            if(check_stack_size(1)) return true;
            stack.pop();
        }else if(t.get_string() == "+"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b+a);
        }else if(t.get_string() == "-"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b-a);
        }else if(t.get_string() == "*"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b*a);
        }else if(t.get_string() == "/"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b/a);
        }else if(t.get_string() == "MOD"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b%a);
        }else if(t.get_string() == "SWAP"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(a);
            stack.push(b);
        }else if(t.get_string() == ">"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b > a ? 1 : 0);
        }else if(t.get_string() == "<"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b < a ? 1 : 0);
        }else if(t.get_string() == ">="){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b >= a ? 1 : 0);
        }else if(t.get_string() == "<="){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b <= a ? 1 : 0);
        }else if(t.get_string() == "="){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b == a ? 1 : 0);
        }else if(t.get_string() == "<>"){
            if(check_stack_size(2)) return true;
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(b != a ? 1 : 0);
        }else if(t.get_string() == "0>"){
            if(check_stack_size(1)) return true;
            int a = stack.top();
            stack.pop();
            stack.push(a < 0 ? 1 : 0);
        }else if(t.get_string() == "0<"){
            if(check_stack_size(1)) return true;
            int a = stack.top();
            stack.pop();
            stack.push(a > 0 ? 1 : 0);
        }else{
            if (function_map.find(t.get_string()) == function_map.end()) {
                std::cout << "Cannot find '" << t.get_string() << "' in function defintions.\n";
                return true;
            } else {
                evaluate(function_map[t.get_string()]);
            }
        }

        p++;
    }

    return false;
}

int main(){
    std::string input = "";

    while(true){
        std::cout << "\033[32m" << ">>> " << "\033[0m";
        std::getline(std::cin, input);

        if(input == "exit"){
            break;
        }

        std::vector<Token> tokens = lex_input(input);


        if(evaluate(tokens)){
            std::cout << "\033[1m\033[31m" << ":(";
        }else{
            std::cout << "\033[1m\033[32m" << ":)";
        }
        
        
        std::cout << "\033[0m" << "\n";
    }
}