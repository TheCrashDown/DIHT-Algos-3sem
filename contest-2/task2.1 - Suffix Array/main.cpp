/*  Task:
    Дана строка длины n. Найти количество ее различных подстрок. 
    Используйте суффиксный массив.
    Построение суффиксного массива выполняйте за O(n log n). 
    Вычисление количества различных подстрок выполняйте за O(n).

    developed by Crashdown

 */
#include <iostream>
#include <string>
#include <vector>
#include <utility>

class ProblemSolver{

public:
    ProblemSolver(std::string& str);

    int substringsCount();


private:
    std::string& _str;

    std::vector< int > suffArray;
    std::vector< int > lcp;

    void buildSuffArray();
    void buildLCP();
};


ProblemSolver::ProblemSolver(std::string& str) : _str(str){
    _str += '\0';

    buildSuffArray();
    buildLCP();
}

void ProblemSolver::buildSuffArray(){

    int numEqClasses = 1;
    std::vector< int > perm(_str.size());
    std::vector< int > eqClasses(_str.size());

    //  выпонляем нулевыю фазу алгоритма сортировкой подсчетом
    std::vector< int > cnt(256, 0);

    for(int i = 0; i < _str.size(); ++i){
        ++cnt[_str[i]];
    }

    for(int i = 1; i < 256; ++i){
        cnt[i] += cnt[i - 1];
    }

    for(int i = 0; i < _str.size(); ++i){
        perm[--cnt[_str[i]]] = i;
    }

    eqClasses[perm[0]] = 0;
    for(int i = 1; i < _str.size(); ++i){
        if(_str[perm[i]] != _str[perm[i - 1]]){
            ++numEqClasses;
        }
        eqClasses[perm[i]] = numEqClasses - 1;
    }

    //  итеративно выполняем остальные фазы алгоритма, используя для
    //  временного хранения данных новой итерации массивы
    //  _perm_tmp и _eqClasses_tmp

    for(int i = 0; (1 << i) < _str.size(); ++i){
        std::vector< int > _perm_tmp(_str.size());
        std::vector< int > _eqClasses_tmp(_str.size());

        for(int j = 0; j < _str.size(); ++j){
            _perm_tmp[j] = perm[j] - (1 << i);

            if(_perm_tmp[j] < 0){
                _perm_tmp[j] += _str.size();
            }
        }

        for(int j = 0; j < numEqClasses; ++j){
            cnt[j] = 0;
        }

        for(int j = 0; j < _str.size(); ++j){
            ++cnt[eqClasses[_perm_tmp[j]]];
        }

        for(int j = 1; j < numEqClasses; ++j){
            cnt[j] += cnt[j - 1];
        }

        for(int j = _str.size() - 1; j >= 0; --j){
            perm[--cnt[eqClasses[_perm_tmp[j]]]] = _perm_tmp[j];
        }


        _eqClasses_tmp[perm[0]] = 0;
        numEqClasses = 1;

        for(int j = 1; j < _str.size(); ++j){
            int tmp1 = (perm[j] + (1 << i)) % _str.size();
            int tmp2 = (perm[j - 1] + (1 << i)) % _str.size();
            if(eqClasses[perm[j]] != eqClasses[perm[j - 1]] ||
               eqClasses[tmp1] != eqClasses[tmp2]){
                ++numEqClasses;
            }
            _eqClasses_tmp[perm[j]] = numEqClasses - 1;
        }

        eqClasses = std::move(_eqClasses_tmp);
    }

    suffArray = std::move(perm);
}


void ProblemSolver::buildLCP(){
    lcp.resize(_str.size(), 0);

    std::vector< int > reversed(suffArray.size());
    for(int i = 0; i < suffArray.size(); ++i){
        reversed[suffArray[i]] = i;
    }

    int pos = 0;

    for(int i = 0; i < _str.size(); ++i){
        if(pos > 0){
            --pos;
        }

        if(reversed[i] == _str.size() - 1){
            lcp[_str.size() - 1] = -1;
            pos = 0;
            continue;
        }

        int tmp = suffArray[reversed[i] + 1];
        while(i + pos < _str.size() && tmp + pos < _str.size() && _str[i + pos] == _str[tmp + pos]){
            pos++;
        }
        lcp[reversed[i]] = pos;
    }
}

int ProblemSolver::substringsCount(){
    int res = 0;
    for(int i = 1; i < _str.size(); ++i){
        res += _str.size() - suffArray[i] - 1;
    }
    for(int i = 0; i < _str.size() - 2; ++i){
        res -= lcp[i + 1];
    }
    return res;
}


int main()
{
    std::string str;
    std::cin >> str;

    ProblemSolver problemSolver(str);

    std::cout << problemSolver.substringsCount();

    return 0;
}
