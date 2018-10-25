#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>
#include <stack>
#include <queue>
#include <string.h>
#include <deque>
#include <set>
#include <map>

#define PI_NUM "3.141592653589793238462643383279"
#define E_NUM "2.7182818284590452353602874713527"



using namespace std;

enum States
{
    State_A = 0, // begin
    State_B,
    State_C,
    State_D,
    State_E,
    State_F,
    State_G,
    State_H
};

void error (int i, int num_error)
{
    string ans;
    ans += "error in column " + to_string(i) + ": ";
    if (num_error == 0) ans += "unknown character";
    if (num_error == 1) ans += "incorrect brackets";
    if (num_error == 2) ans += "incorrect ','";
    if (num_error == 3) ans += "unknown function";
    ans += '\n';

    throw runtime_error(ans);
    //exit(1);
}
/**   0 - unknown character
 *   1 - incorrect brackets
 *   2 - incorrect ','
 *   3 - unknown function
 */

int priority (string input)
{
    if (input == "^") return 3;
    if (input == "~") return 2;
    if (input == "*" || input == "/") return 1;
    if (input == "+" || input == "-") return 0;
    return -1;
}

void avtomat (string input)
{
    int brackets_balance = 0;
    for(char token: input){
        if(token=='(')
            brackets_balance++;
        if(token==')')
            brackets_balance--;
        if(brackets_balance<0)
            throw runtime_error("The balance of brackets is broken.");
    }
    if(brackets_balance>0)
        throw runtime_error("The balance of brackets is broken.");

    input += ")";
    States state = State_A;
    stack <int> brack;
    brack.push(0);

    string number;

    stack <string> stack_;
    // унарный минус ~

    for (unsigned int i=0; i<input.size(); i++){
        const char current = input[i];
        switch(state)
        {
            case State_A:
                if (current == '+'){
                    state = State_B;
                }
                else if (current == '-'){
                    state = State_B;
                }
                else if (current >= '0' && current <= '9'){
                    state = State_C;
                    number.clear();
                    number += current;
                }
                else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                    state = State_H;
                    number.clear();
                    number += current;
                }
                else if (current == '('){
                    brack.push(0);
                }
                else {
                    error(i, 0);
                }

                break;
            case State_B:
                if (current == '('){
                    state = State_A;
                    brack.push(0);
                }
                else if (current >= '0' && current <= '9'){
                    state = State_C;
                    number.clear();
                    number += current;
                }
                else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                    state = State_H;
                    number.clear();
                    number += current;
                }
                else {
                    error(i, 0);
                }

                break;
            case State_C:
                if (current >= '0' && current <= '9'){
                    number += current;
                }
                else if (current == '.'){
                    state = State_D;
                    number += current;
                }
                else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                    state = State_G;
                    number.clear();
                }
                else if (current == ')'){
                    state = State_F;
                    number.clear();
                    if (brack.empty()) error(i, 1);
                    else if (brack.top() == 0){
                        brack.pop();
                    }
                    else {
                        error(i, 1);
                    }
                }
                else if (current == ','){
                    state = State_A;
                    number.clear();
                    if (brack.top() > 0){
                        brack.top()--;
                    }
                    else {
                        error(i, 2);
                    }
                }
                else {
                    error(i, 0);
                }

                break;
            case State_D:
                if (current >= '0' && current <= '9'){
                    state = State_E;
                    number += current;
                }
                else {
                    error(i, 0);
                }

                break;
            case State_E:
                if (current >= '0' && current <= '9'){
                    number += current;
                }
                else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                    state = State_G;
                    number.clear();
                }
                else if (current == ')'){
                    state = State_F;
                    number.clear();
                    if (brack.empty()) error(i, 1);
                    else if (brack.top() == 0){
                        brack.pop();
                    }
                    else {
                        error(i, 1);
                    }
                }
                else if (current == ','){
                    state = State_A;
                    number.clear();
                    if (brack.top() > 0){
                        brack.top()--;
                    }
                    else {
                        error(i, 2);
                    }
                }
                else {
                    error(i, 0);
                }

                break;
            case State_F:
                if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                    state = State_B;
                }
                else if (current == ')'){
                    if (brack.empty()) error(i, 1);
                    else if (brack.top() == 0){
                        brack.pop();
                    }
                    else {
                        error(i, 1);
                    }
                }
                else if (current == ','){
                    state = State_A;
                    if (brack.top() > 0){
                        brack.top()--;
                    }
                    else {
                        error(i, 2);
                    }
                }
                else {
                    error(i, 0);
                }

                break;
            case State_G:
                if (current == '('){
                    state = State_A;
                    brack.push(0);
                }
                else if (current >= '0' && current <= '9'){
                    state = State_C;
                    number.clear();
                    number += current;
                }
                else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                    state = State_H;
                    number.clear();
                    number += current;
                }
                else {
                    error(i, 0);
                }

                break;
            case State_H:
                if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9')){
                    number += current;
                }
                else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                    state = State_B;
                    number.insert(number.begin(), '&');
                    number.clear();
                }
                else if (current == ','){
                    state = State_A;
                    number.insert(number.begin(), '&');
                    number.clear();
                    if (brack.top() > 0){
                        brack.top()--;
                    }
                    else {
                        error(i, 2);
                    }
                }
                else if (current == '('){
                    state = State_A;
                    if (number == "sin" || number == "cos" || number == "ln" || number == "exp" || number == "sqrt"){
                        number.clear();
                        brack.push(0);
                    }
                    else if (number == "log"){
                        number.clear();
                        brack.push(1);
                    }
                    else {
                        error(i, 3);
                    }
                }
                else if (current == ')'){
                    state = State_F;
                    number.insert(number.begin(), '&');
                    number.clear();
                    if (brack.empty()) error(i, 1);
                    else if (brack.top() == 0){
                        brack.pop();
                    }
                    else {
                        error(i, 1);
                    }
                }
                else {
                    error(i, 0);
                }

                break;
        }
    }
    if (!stack_.empty()) error(input.size()-1, 1);
    return;
}


class BigNumber{
private:
    vector<int> number = {};
    int point = 0;
    int percision = 45;
    int output_percision = 6;


public:
    bool sign = true;

    void read_string(string input_number){
        reverse(input_number.begin(), input_number.end());
        int p = 0;
        for(char i: input_number){
            if(isdigit(i)){
                number.push_back(i-'0');
                p++;
            }
            if(i == '-')
                sign = false;
            if(i == '.')
                point = p;

        }
    }

    void read_int(int input_number){
        if(input_number<0){
            sign = false;
            input_number *= -1;
        }

        while(input_number){
            number.push_back(input_number%10);
            input_number = input_number/10;
        }
    }

    int get_int(){
        this->normalize();
        int result = 0, k = 1;
        for(int i: number){
            result += k*i;
            k *= 10;
        }

        return result;
    }

    string get_string(){
        string output_number;

        int start = max(0, point-output_percision);

        for(int i=start; i<number.size();i++){
            if(i == point && point != 0)
                output_number += '.';
            output_number += '0' + number[i];
        }
        if(output_number.length()==0)
            output_number = "0";
        else if(!sign)
            output_number += '-';

        reverse(output_number.begin(), output_number.end());

        return output_number;
    }

    void mantissa_like(BigNumber argument){
        int n = argument.point-point;
        for(int i=0;i<n;i++){
            point++;
            number.insert(number.begin(), 0);
        }

    }

    void floor(){
        vector<int> new_number;
        for(int i=point;i<number.size();i++)
            new_number.push_back(number[i]);

        point = 0;
        number = new_number;
    }

    void normalize(){
        vector<int> new_number;
        int number_end = number.size()-1, number_start=0;

        if(point>percision)
            number_start = point-percision;

        if(number.size()-point>1)
            while(number[number_end]==0 && number_end>point)
                number_end--;
        if(point)
            while(number[number_start]==0 && number_start<point)
                number_start++;

        for(int i=0;i<=number_end;i++)
            if(i>=number_start && i<=number_end)
                new_number.push_back(number[i]);

        point -= number_start;
        number = new_number;
    }

    BigNumber add(BigNumber argument1, BigNumber argument2){
        vector<int> a = argument1.number, b = argument2.number;
        int p1 = argument1.point, p2 = argument2.point, dp, l;
        if(p1<p2){
            swap(a, b);
            swap(p1, p2);
        }

        dp = p1-p2;
        int l1=a.size(), l2=b.size()+dp;
        l = max(l1, l2);

        int prob = 0, temp = 0;
        for(int i=0;i<l;i++){
            if(i<a.size()){
                if(i>=dp && i<b.size()+dp)
                    a[i] += b[i-dp];
                a[i] += prob;
                prob = a[i]/10;
                a[i] = a[i]%10;
            }
            else if(i-dp<b.size()){
                temp = b[i-dp] + prob;
                prob = temp/10;
                temp = temp%10;
                a.push_back(temp);
            }
        }
        if(prob)
            a.push_back(prob);

        BigNumber result;
        result.number = a;
        result.point = p1;
        result.normalize();

        return result;
    }

    BigNumber sub(BigNumber argument1,  BigNumber argument2){
        argument1.normalize();
        argument2.normalize();
        bool s = true;

        if(argument1.point>argument2.point)
            argument2.mantissa_like(argument1);
        else
            argument1.mantissa_like(argument2);

        vector<int> a = argument1.number, b = argument2.number;
        int p = argument1.point;

        if(argument1<argument2){
            swap(a, b);
            s = false;
        }

        for(int i=0; i<b.size(); i++){
            a[i] -= b[i];
            if(a[i]<0){
                a[i+1]--;
                a[i]+=10;
            }
        }

        for(int i=0; i<a.size(); i++){
            if(a[i]<0){
                a[i+1]--;
                a[i]+=10;
            }
        }

        BigNumber result;
        result.number = a;
        result.sign = s;
        result.point = p;

        result.normalize();
        return result;
    }

    BigNumber mul(BigNumber argument1, BigNumber argument2){
        if(argument1.point>argument2.point)
            argument2.mantissa_like(argument1);
        else
            argument1.mantissa_like(argument2);

        if(argument1.number.size() < argument1.number.size())
            swap(argument1, argument2);

        BigNumber result, temp;
        int prop, t;

        for(int i=0;i<argument2.number.size();i++){
            temp.reset();
            for(int j=0; j<i; j++)
                temp.number.push_back(0);

            prop = 0;
            for(int j=0; j<argument1.number.size();j++){
                t = prop;
                t += argument2.number[i] * argument1.number[j];
                prop = t/10;
                t = t%10;
                temp.number.push_back(t);
            }
            if(prop)
                temp.number.push_back(prop);

            result = result.add(result, temp);
        }

        result.point = argument1.point + argument2.point;
        result.normalize();
        return result;
    }

    BigNumber div(BigNumber argument1, BigNumber argument2){
        BigNumber result, current;
        int c, k = 0;

        if(argument1.point>argument2.point)
            argument2.mantissa_like(argument1);
        else
            argument1.mantissa_like(argument2);


        argument1.point = 0;
        argument2.point = 0;

        argument1.sign = true;
        argument2.sign = true;

        while (argument1.number.size() != 0) {
            if(current>=argument2){
                k++;
                current = current-argument2;
            }
            else{
                result.number.insert(result.number.begin(), k);
                k = 0;

                c = argument1.number[argument1.number.size()-1];
                argument1.number.pop_back();
                current.number.insert(current.number.begin(), c);
            }
        }

        while(current>=argument2){
            k++;
            current = current-argument2;
        }

        result.number.insert(result.number.begin(), k);
        k = 0;

        int p = 0;
        current.number.insert(current.number.begin(), 0);
        while(p<percision){
            if(current>=argument2){
                k++;
                current = current-argument2;
            }
            else{
                p++;
                result.number.insert(result.number.begin(), k);
                k = 0;
                result.point++;
                current.number.insert(current.number.begin(), 0);
            }
        }

        result.normalize();
        return result;
    }

    BigNumber mod(BigNumber argument1, BigNumber argument2){
        BigNumber result, curent;
        bool s;

        s = argument1.sign;
        argument1.sign = true;
        argument2.sign = true;

        curent = argument1/argument2;
        curent.floor();
        result = argument1-curent*argument2;
        result.sign = s;

        result.normalize();
        return result;
    }

    BigNumber sin(BigNumber argument){
        BigNumber result, pi, c, one, two, k, a;
        bool s;

        k.read_string("1");
        one.read_string("1");
        two.read_string("2");
        pi.read_string(PI_NUM);


        c.read_string("1");

        argument = argument%(pi*two);
        s = argument.sign;
        argument.sign = true;

        result = argument;
        a = argument;

        for(int i=0;i<100;i++){
            a = a*argument*argument;
            c = (c * (k+one) * (k+two));
            k = k+two;


            if(i%2 == 0)
                result = result-a/c;
            else
                result = result+a/c;
        }

        if(result.sign == s)
            result.sign = true;
        else
            result.sign = false;

        return result;
    }

    BigNumber cos(BigNumber argument){
        BigNumber result, pi, c, one, two, k, a;
        argument.sign = true;

        k.read_string("0");
        c.read_string("1");

        one.read_string("1");
        two.read_string("2");
        pi.read_string(PI_NUM);

        argument = argument%(pi*two);

        result = one;
        a = one;

        for(int i=0;i<100;i++){
            a = a*argument*argument;
            c = (c * (k+one) * (k+two));
            k = k+two;

            if(i%2 == 0)
                result = result-a/c;
            else
                result = result+a/c;
        }

        return result;
    }

    BigNumber exp(BigNumber argument){
        BigNumber result, one, a, b, k;

        one.read_string("1");
        result = one;

        a.read_string("1");
        b.read_string("1");
        k.read_string("0");


        for(int i=0;i<200;i++){
            a = a*argument;
            k = k+one;
            b = b*k;

            result = result + a/b;
        }

        return result;
    }

    BigNumber ln(BigNumber argument){
        BigNumber result, one, a, b, k, log10, bd;
        int d;

        if(!argument.sign)
            throw runtime_error("ln from negative number");

        argument.normalize();
        log10.read_string("2.302585092994046");

        if(argument.number.size()-argument.point == 1 && argument.number[0] == 0){
            d = 0;
        }
        else{
            d = argument.number.size()-argument.point;
            argument.point = argument.number.size();
            argument.number.push_back(0);
        }

        bd.read_int(d);

        one.read_string("1");
        argument = argument - one;
        result = argument;

        a = argument;
        k.read_string("1");



        for(int i=0;i<200;i++){
            a = a*argument;
            k = k+one;


            if(i%2 == 0)
                result = result - a/k;
            else
                result = result + a/k;
        }

        result = result + bd*log10;

        return result;
    }

    BigNumber log(BigNumber argument, BigNumber base){
        BigNumber a, b, result, one;

        one.read_string("1");

        if(!argument.sign)
            throw runtime_error("log from negative number");

        if(!base.sign)
            throw runtime_error("log with negative base");

        if(base == one)
            throw runtime_error("log with base equal one");

        a = ln(argument);
        b = ln(base);

        result = a/b;
        return result;
    }

    void reset(){
        number = {};
        point = 0;
        sign = true;
    }

    BigNumber pow(BigNumber argument, BigNumber base){
        BigNumber a, result;
        int int_base;

        argument.normalize();
        base.normalize();

        if(base.point == 0){
            result.read_string("1");
            int_base = base.get_int();
            for(int i=0; i<int_base; i++)
                result = result * argument;
        }
        else{
            if(!argument.sign)
                throw runtime_error("power from negative number");

            a = ln(argument) * base;
            result = exp(a);
        }
        return result;
    }

    bool is_bigger(BigNumber argument1, BigNumber argument2){
        argument1.normalize();
        argument2.normalize();

        if(!argument1.sign && argument2.sign)
            return true;

        if(argument1.sign && !argument2.sign)
            return false;

        if(argument1.number.size()-argument1.point<argument2.number.size()-argument2.point)
            return true;
        if(argument1.number.size()-argument1.point>argument2.number.size()-argument2.point)
            return false;

        if(argument1.number.size()>argument2.number.size())
            argument2.mantissa_like(argument1);
        else
            argument1.mantissa_like(argument2);

        for(int i=argument1.number.size()-1;i>=0;i--){
            if(argument1.number[i]<argument2.number[i])
                return true;
            if(argument1.number[i]>argument2.number[i])
                return false;
        }
        return false;
    }

    BigNumber operator+(BigNumber b){
        BigNumber result;
        if(sign==b.sign){
            result = this->add(*this, b);
            result.sign = sign;
        }
        else{
            if(sign)
                result = this->sub(*this, b);
            else
                result = this->sub(b, *this);
        }

        result.normalize();
        return result;
    }

    BigNumber operator-(BigNumber b){
        BigNumber result;
        if(sign==b.sign){
            result = this->sub(*this, b);
        }
        else{
            result = this->add(*this, b);
        }

        if(!sign)
            result.sign = !result.sign;

        result.normalize();
        return result;
    }

    BigNumber operator*(BigNumber b){
        BigNumber result;
        if(sign==b.sign){
            result = this->mul(*this, b);
            result.sign = true;
        }
        else{
            result = this->mul(*this, b);
            result.sign = false;
        }

        result.normalize();
        return result;
    }

    BigNumber operator/(BigNumber b){
        BigNumber result, zero;

        zero.read_string("0");

        if(b == zero)
            throw runtime_error("devision by zero");

        if(sign==b.sign){
            result = this->div(*this, b);
            result.sign = true;
        }
        else{
            result = this->div(*this, b);
            result.sign = false;
        }

        result.normalize();
        return result;
    }

    BigNumber operator%(BigNumber b){
        BigNumber result;
        result = this->mod(*this, b);

        return result;
    }

    bool operator>(BigNumber b){
        return is_bigger(b, *this);
    }

    bool operator<(BigNumber b){
        return is_bigger(*this, b);
    }

    bool operator==(BigNumber b){
        return !is_bigger(b, *this) && !is_bigger(*this, b);
    }

    bool operator>=(BigNumber b){
        return  !(*this<b);
    }

    bool operator<=(BigNumber b){
        return !(*this>b);
    }

};


map<string,int> OPERATION_PRIORITY = {
    {"(", 0},
    {")", 1},
    {"+", 2}, {"-", 2},
    {"*",3}, {"/", 3},
    {"^", 4},
    {"sin",5}, {"cos", 5}, {"lg", 5}, {"log2", 5},
    {"exp", 5}, {"sqrt", 5}, {"log", 5}, {"ln", 5}
};

bool is_number(const std::string& s)
{
    try
    {
        std::stod(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

void error_unexpected_symbol(char symbol){
    string message = "Unexpected symbol: '";
    message += symbol;
    message += "'";
    throw runtime_error(message);
}

void expression_validation(string expression){
    int brackets_balance = 0;
    for(char token: expression){
        if(token=='(')
            brackets_balance++;
        if(token==')')
            brackets_balance--;
        if(brackets_balance<0)
            throw runtime_error("The balance of brackets is broken.");
    }
    if(brackets_balance>0)
        throw runtime_error("The balance of brackets is broken.");


    set<char> allowed_c = {
        '(', ')', '_', '*', '/', ',',
        '-', '+', '*', '^', '.'
    };

    set<char> operations = {
        '*', '/', ',', '-', '+', '*', '^'
    };


    for(char token: expression){
        if(!(isdigit(token) || isalpha(token) || allowed_c.count(token))){
            error_unexpected_symbol(token);
        }
    }

    int state = 0;
    // 0 - start
    // 1 - digit
    // 2 - digit2
    // 3 - alpha
    // 4 - operations
    // 5 - digit3

    for(char token: expression){
        if(state == 0){
            if(operations.count(token) && token != '-' && token != ',')
                error_unexpected_symbol(token);
            else if(isdigit(token))
                state = 1;
            else if(isalpha(token))
                state = 3;
        }
        else if(state == 1){
            if(token == 'e')
                state = 2;
            else if(isalpha(token))
                error_unexpected_symbol(token);
            else if(operations.count(token))
                state = 4;
            else if(token == '(')
                error_unexpected_symbol(token);
            else if(token == ')')
                state = 0;
        }
        else if(state == 2){
            if(isdigit(token))
                state = 5;
            else if(token == ')')
                state = 0;
            else
                error_unexpected_symbol(token);

        }
        else if(state == 3){
            if(operations.count(token))
                state = 4;
//            else if(token == '(')
//                error_unexpected_symbol(token);
            else if(token == ')')
                state = 0;
        }
        else if(state == 4){
            if(operations.count(token))
                error_unexpected_symbol(token);
            else if(token == ')')
                error_unexpected_symbol(token);
            else if(token == '(')
                state = 0;
            else if(isalpha(token))
                state = 3;
            else if(isdigit(token))
                state = 1;
        }
        else if(state == 5){
            if(isalpha(token))
                error_unexpected_symbol(token);
            else if(operations.count(token))
                state = 4;
            else if(token == '(')
                error_unexpected_symbol(token);
            else if(token == ')')
                state = 0;
        }
    }

}

vector<string> tokenize_expression(string expression){
    vector<string> tokens;
    string token = "";
    int token_type = -1;

    for(char c: expression){
        if(c == ' ')
            continue;

        if(isalpha(c) or c == '_'){
            if(token_type == 0){
                token += c;
            }
            else if(token_type == 1){
                token += c;
            }
            else{
                if (!token.empty())
                    tokens.push_back(token);
                token = "";
                token += c;
                token_type = 0;
            }
        }
        else if (isdigit(c) || c == '.'){
            if(token_type == 1){
                token += c;
            }
            else if(token_type == 0){
                token += c;
            }
            else{
                if (!token.empty())
                    tokens.push_back(token);
                token = "";
                token += c;
                token_type = 1;
            }
        }
        else if(c == ','){
            if (!token.empty())
                tokens.push_back(token);
            tokens.push_back(")");
            token = "(";
            token_type = -1;
        }
        else if(c == '-' && token_type == -1){
            if (!token.empty())
                tokens.push_back(token);
            token = "";
            token += c;
            token_type = 1;
        }
        else if(c == ')'){
            if (!token.empty())
                tokens.push_back(token);
            token = "";
            token += c;
            token_type = 2;
        }
        else{
            if (!token.empty())
                tokens.push_back(token);
            token = "";
            token += c;
            token_type = -1;
        }
    }

    tokens.push_back(token);
    return tokens;
}

vector<string> infix2postfix(vector<string>& expression){
    stack<string> temp_stack;
    vector<string> postfix_expression;

    for(string token: expression){
        if(token == "("){
            temp_stack.push(token);
            continue;
        }

        if(token == ")"){
            while(temp_stack.top() != "("){
                postfix_expression.push_back(temp_stack.top());
                temp_stack.pop();
            }
            temp_stack.pop();
            continue;
        }

        if(!OPERATION_PRIORITY.count(token)){
            postfix_expression.push_back(token);
            continue;
        }

        int cur_priority = OPERATION_PRIORITY[token];
        if(token == "^"){
            while(!temp_stack.empty() && OPERATION_PRIORITY[temp_stack.top()] > cur_priority){
                postfix_expression.push_back(temp_stack.top());
                temp_stack.pop();
            }
        }
        else{
            while(!temp_stack.empty() && OPERATION_PRIORITY[temp_stack.top()] >= cur_priority){
                postfix_expression.push_back(temp_stack.top());
                temp_stack.pop();
            }
        }
        temp_stack.push(token);

    }

    while(!temp_stack.empty()){
        postfix_expression.push_back(temp_stack.top());
        temp_stack.pop();
    }

    return postfix_expression;
}

map<string, BigNumber> get_varibles(vector<string> expression){
    BigNumber pi, e;
    pi.read_string(PI_NUM);
    e.read_string(E_NUM);

    map<string, BigNumber> varibles = {
        {"pi", pi}, {"e", e}
    };
    string value;

    for(string token: expression){
        if(OPERATION_PRIORITY.count(token) || is_number(token) || varibles.count(token))
            continue;
        cout<<token<<" = ";
        cin>>value;
        BigNumber num;
        num.read_string(value);

        varibles.insert(pair<string, BigNumber>(token, num));
    }
    return varibles;
}

vector<string> get_varibles_names(vector<string> expression){
    vector<string> varibles_names;
    set<string> used = {"pi", "e"};

    for(string token: expression){
        if(OPERATION_PRIORITY.count(token) || is_number(token) || used.count(token))
            continue;
        varibles_names.push_back(token);
        used.insert(token);
    }

    return varibles_names;
}

BigNumber calculation(vector<string> expression, map<string,BigNumber> varibles){
    stack<BigNumber> calculation_stack;
    BigNumber a, b, c, zero,half, two, ten;

    half.read_string("0.5");
    two.read_string("2");
    zero.read_string("0");
    ten.read_string("10");

//    calculation_stack.push(zero);

    for(string token: expression){
        if(token == "-"){
            if(calculation_stack.size()<2)
                throw runtime_error("substractionq calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            b = calculation_stack.top();
            calculation_stack.pop();

            c = b-a;
            calculation_stack.push(c);
        }

        else if(token == "+"){
            if(calculation_stack.size()<2)
                throw runtime_error("sum calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            b = calculation_stack.top();
            calculation_stack.pop();

            c = a+b;
            calculation_stack.push(c);
        }

        else if(token == "*"){
            if(calculation_stack.size()<2)
                throw runtime_error("multiply calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            b = calculation_stack.top();
            calculation_stack.pop();

            c = a*b;
            calculation_stack.push(c);
        }

        else if(token == "/"){
            if(calculation_stack.size()<2)
                throw runtime_error("division calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            b = calculation_stack.top();
            calculation_stack.pop();

            c = b/a;
            calculation_stack.push(c);
        }

        else if(token == "^"){
            if(calculation_stack.size()<2)
                throw runtime_error("pow calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            b = calculation_stack.top();
            calculation_stack.pop();

            c = a.pow(b, a);
            calculation_stack.push(c);
        }
        else if(token == "cos"){
            if(calculation_stack.size()<1)
                throw runtime_error("cos calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            c = a.cos(a);
            calculation_stack.push(c);
        }
        else if(token == "sin"){
            if(calculation_stack.size()<1)
                throw runtime_error("sin calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            c = a.sin(a);
            calculation_stack.push(c);
        }
        else if(token == "lg"){
            if(calculation_stack.size()<1)
                throw runtime_error("lg calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            c = a.log(a, ten);
            calculation_stack.push(c);
        }
        else if(token == "ln"){
            if(calculation_stack.size()<1)
                throw runtime_error("ln calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();
            c = a.ln(a);
            calculation_stack.push(c);
        }
        else if(token == "log2"){
            if(calculation_stack.size()<1)
                throw runtime_error("log2 calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            c = a.log(a, two);
            calculation_stack.push(c);
        }
        else if(token == "exp"){
            if(calculation_stack.size()<2)
                throw runtime_error("exp calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();

            c = a.exp(a);
            calculation_stack.push(c);
        }
        else if(token == "sqrt"){
            if(calculation_stack.size()<1)
                throw runtime_error("sqrt calculation error");

            a = calculation_stack.top();

            if(!a.sign)
                throw runtime_error("sqrt from negative number");

            calculation_stack.pop();

            c = a.pow(a, half);
            calculation_stack.push(c);
        }
        else if(token == "log"){
            if(calculation_stack.size()<2)
                throw runtime_error("log calculation error");

            a = calculation_stack.top();
            calculation_stack.pop();
            b = calculation_stack.top();
            calculation_stack.pop();

            c = b.log(a, b);
            calculation_stack.push(c);
        }
        else if (is_number(token)){
            BigNumber num;
            num.read_string(token);
            calculation_stack.push(num);
        }
        else{
            calculation_stack.push(varibles[token]);
        }
    }

    return calculation_stack.top();
}
