#include <cstdio>
#include <cstdlib>
#include <vector>
#include <list>

enum {
    PLUS = '+',
    MINUS = '-',
    MULTIPLY = '*',
    CONNECT = ' '
};

long result_number;
std::list <char> opers;

long perebor(const std::vector<long>& numbers, int cur_it, long cur_eval);

int main(int argc, char** argv) {
    result_number = strtoll(argv[1], nullptr, 10);

    std::vector <long> numbers;
    for (int i = 2; i < argc; i++) {
        numbers.push_back(strtoll(argv[i], nullptr, 10));
    }

    long result = perebor(numbers, 1, numbers[0]);
    printf("%ld\n", result);

    for (auto elem : opers) {
        printf("%c ", elem);
    }

    printf("\n");
}

long perebor(const std::vector<long>& numbers, int cur_it, long cur_eval) {
    if (cur_it == numbers.size()) {
        return cur_eval;
    }

    opers.push_back('+');
    long res = perebor(numbers, cur_it + 1, cur_eval + numbers[cur_it]);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    opers.push_back('-');
    res = perebor(numbers, cur_it + 1, cur_eval - numbers[cur_it]);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    opers.push_back('*');
    res = perebor(numbers, cur_it + 1, cur_eval * numbers[cur_it]);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    opers.push_back('&');
    res = perebor(numbers, cur_it + 1, cur_eval*10 + numbers[cur_it]);
    if (res == result_number) {
        return res;
    }
    opers.pop_back();

    return -1;
}