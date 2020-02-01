/*  Task:
    Заданы две строки s, t и целое число k. Рассмотрим множество всех таких непустых строк, 
    которые встречаются как подстроки в s и t одновременно. 
    Найдите k-ую в лексикографическом порядке строку из этого множества. 

    developed by Crashdown
    
*/
#include <iostream>
#include <string>
#include <vector>
#include <utility>

class ProblemSolver{

public:
    ProblemSolver(std::string& str1, std::string& str2);

    void kCommonSubstring(long long k, std::string& result);

private:
    std::string _str;

    int len = 0;

    std::vector< int > suffArray;
    std::vector< int > lcp;

    void buildSuffArray();
    void buildLCP();
};


ProblemSolver::ProblemSolver(std::string& str1, std::string& str2) : _str(str1){
    _str += "$" + str2 + "#";

    len = str1.size();

    buildSuffArray();
    buildLCP();
}

void ProblemSolver::buildSuffArray(){

    int numEqClasses = 1;
    std::vector< int > perm(_str.size());
    std::vector< int > eqClasses(_str.size());

    //  выпонляем нулевыю фазу алгоритма сортировкой подсчетом
    std::vector< int > cnt(1e6, 0);

    for(int i = 0; i < _str.size(); ++i){
        ++cnt[_str[i]];
    }

    for(int i = 1; i < 1e6; ++i){
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

void ProblemSolver::kCommonSubstring(long long k, std::string& result){
    //  счетчик количества общих подстрок
    long long cnt = 0ll;

    int min = _str.size();
    //  поддерживаем номер строки из конкатенации к которой относятся
    //  текущий и предыдущий суффиксы
    int current = 0;
    int last = (suffArray[0] > len) ? 2 : 1;

    for(int i = 1; i < suffArray.size(); ++i){
        current = (suffArray[i] > len) ? 2 : 1;

        //  суммируем общие подстроки
        if(current != last){
            cnt += std::max(0, lcp[i - 1] - min);
            min = lcp[i - 1];
        } else {
            min = std::min(min, lcp[i - 1]);
        }

        last = current;

        //  убираем то что прибавили лишнее
        if(cnt >= k){
            for(int j = 0; j < lcp[i - 1] + k - cnt; ++j){
                result += _str[suffArray[i] + j];
            }
            return;
        }
    }

    result = "-1";
    return;
}

int main()
{
    std::string str1, str2;
    std::cin >> str1 >> str2;

    ProblemSolver problemSolver(str1, str2);

    long long k;
    std::cin >> k;

    std::string result;
    problemSolver.kCommonSubstring(k, result);

    std::cout << result;

    return 0;
}
