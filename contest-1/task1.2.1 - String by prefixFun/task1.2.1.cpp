/*  Task:
    Найти лексикографически-минимальную строку, построенную по префикс-функции, в алфавите a-z.

    developed by Crashdown

 */

#include <iostream>
#include <vector>


class ProblemSolver{

public:
    ProblemSolver(std::vector< int >& prefixFun) : _prefixFun(prefixFun){}

    void findMinString(std::string& output);

private:
    std::vector< int >& _prefixFun;
};


void ProblemSolver::findMinString(std::string& output){

    output = "a";

    for (int i = 1; i < _prefixFun.size(); ++i){
        // маска для использованных символов
        std::vector< bool > used(26, 0);


        if(_prefixFun[i] > 0){
            // если значение префикс-функции > 0, то надо просто восстановить уже построенное значение
            output += output[_prefixFun[i] - 1];
        } else {
            // если значение префикс-функции = 0, то надо искать новый символ
            //
            // запоминаем уже использованные символы
            int current = _prefixFun[i - 1];
            while(current > 0){
                used[output[current] - 'a'] = 1;
                current = _prefixFun[current - 1];
            }
            // ищем минимальный походящий символ
            char current_char = 'b';
            while(used[current_char - 'a']){
                current_char++;
            }

            output += current_char;
        }
    }
}


int main(){

    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    std::vector< int > prefixFun;

    int a = 0;

    while(std::cin >> a){
        prefixFun.push_back(a);
    }

    ProblemSolver problemSolver(prefixFun);

    if(prefixFun.size() != 0){
        prefixFun[0] = 0;
    }
    std::string output;
    problemSolver.findMinString(output);

    std::cout << output << "\n";

    return 0;
}
