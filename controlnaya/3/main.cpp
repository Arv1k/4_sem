#include <cstdio>
#include <cstdlib>
#include <vector>


enum {
    PLUS = '+',
    MINUS = '-',
    MULTIPLY = '*',
    CONNECT = ' '
};

long result_number;


long perebor(const std::vector<long> &numbers,
             std::vector <char> &opers,
             int cur_it, long cur_eval);

void printing(const std::vector<long> &numbers, std::vector<char> &opers);


int main(int argc, char** argv) {
    result_number = strtoll(argv[1], nullptr, 10);

    std::vector <long> numbers;
    for (int i = 2; i < argc; i++) {
        numbers.push_back(strtoll(argv[i], nullptr, 10));
    }

    std::vector <char> opers;

    perebor(numbers, opers, 1, numbers[0]);
}

long perebor(const std::vector<long> &numbers,
             std::vector <char> &opers,
             int cur_it, long cur_eval) {
    if (cur_it == numbers.size()) {
        return cur_eval;
    }

    opers.push_back(PLUS);
    long res = perebor(numbers, opers, cur_it + 1, cur_eval + numbers[cur_it]);
    if (res == result_number) {
        if (cur_it + 1 == numbers.size()) {
            printing(numbers, opers);
        }
    }
    opers.pop_back();

    opers.push_back(MINUS);
    res = perebor(numbers, opers, cur_it + 1, cur_eval - numbers[cur_it]);
    if (res == result_number) {
        if (cur_it + 1 == numbers.size()) {
            printing(numbers, opers);
        }
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
            action = cur_eval * numbers[cur_it] - cur_eval;
            break;
    }
    opers.push_back(MULTIPLY);
    res = perebor(numbers, opers, cur_it + 1, cur_eval + action);
    if (res == result_number) {
        if (cur_it + 1 == numbers.size()) {
            printing(numbers, opers);
        }
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
    res = perebor(numbers, opers, cur_it + 1, cur_eval + action);
    if (res == result_number) {
        if (cur_it + 1 == numbers.size()) {
            printing(numbers, opers);
        }
    }
    opers.pop_back();
}

void printing(const std::vector<long> &numbers, std::vector<char> &opers) {
    for (int i = 0; i < opers.size(); i++) {
        if (opers[i] == ' ') {
            printf("%ld", numbers[i]);
            continue;
        }
        printf("%ld%c", numbers[i], opers[i]);
    }
    printf("%ld = %ld\n", numbers.back(), result_number);
}