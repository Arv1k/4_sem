#include <cstdio>
#include <cstdlib>
#include <vector>
#include <list>
#include <iostream>

enum {
    PLUS = '+',
    MINUS = '-',
    MULTIPLY = '*',
    CONNECT = ' '
};

long result_number;
std::vector <char> opers;

long perebor(const std::vector<long>& numbers, int cur_it, long cur_eval);

int main(int argc, char** argv) {
    result_number = strtoll(argv[1], nullptr, 10);

    std::vector <long> numbers;
    for (int i = 2; i < argc; i++) {
        numbers.push_back(strtoll(argv[i], nullptr, 10));
    }

    long result = perebor(numbers, 1, numbers[0]);

    for (int i = 0; i < opers.size(); i++) {
        if (opers[i] == ' ') {
            printf("%ld", numbers[i]);
            continue;
        }
        printf("%ld%c", numbers[i], opers[i]);
    }
    printf("%ld = %ld", numbers.back(), result_number);
}

long perebor(const std::vector<long>& numbers, int cur_it, long cur_eval) {
    if (cur_it == numbers.size()) {
        return cur_eval;
    }

    opers.push_back(PLUS);
    long res = perebor(numbers, cur_it + 1, cur_eval + numbers[cur_it]);
    if (res == result_number) {

        return res;
    }
    opers.pop_back();

    opers.push_back(MINUS);
    res = perebor(numbers, cur_it + 1, cur_eval - numbers[cur_it]);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    char last_ch = opers.back();
    long action = 0;
    switch (last_ch) {
        case PLUS:
            action -= numbers[cur_it - 1];
            action += numbers[cur_it - 1]*numbers[cur_it];
            break;

        case MINUS:
            action += numbers[cur_it - 1];
            action -= numbers[cur_it - 1]*numbers[cur_it];
            break;

        case MULTIPLY:
        case CONNECT:
        default:
            action *= numbers[cur_it] - 1;
            break;

    }
    opers.push_back(MULTIPLY);
    res = perebor(numbers, cur_it + 1, cur_eval + action);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    last_ch = opers.back();
    switch (last_ch) {
        case PLUS:
            action -= numbers[cur_it - 1];
            action += numbers[cur_it - 1]*10 + numbers[cur_it];
            break;

        case MINUS:
            action += numbers[cur_it - 1];
            action -= numbers[cur_it - 1]*10 + numbers[cur_it];
            break;

        case MULTIPLY:
            action = cur_eval / numbers[cur_it - 1];
            action *= numbers[cur_it - 1]*10 + numbers[cur_it];
            action -= cur_eval;
            break;

        case CONNECT:
        default:
            action = cur_eval*10 + numbers[cur_it];
            action -= cur_eval;
            break;
    }
    opers.push_back(CONNECT);
    res = perebor(numbers, cur_it + 1, cur_eval + action);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    return -1;
}