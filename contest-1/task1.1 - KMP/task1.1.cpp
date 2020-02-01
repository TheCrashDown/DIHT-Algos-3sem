/*  Task:
    Найдите все вхождения шаблона в строку.
     Длина шаблона – p, длина строки – n. 
     Время O(n + p), доп. память – O(p).
    p <= 30000, n <= 300000.

    Задача решена с использованием префикс-функции


    developed by Crashdown

*/
#include <iostream>
#include <vector>

class KMP
{
public:
    KMP(std::string& pattern, std::string& text) : _pattern(pattern), _text(text) {
        prefixBuild();
    }

    void prefixBuild();

    void find(std::vector< int >& result);

private:
    std::vector< int > _buffer;
    int _last = 0;

    std::string& _pattern;
    std::string& _text;

};

void KMP::prefixBuild(){
    _buffer.resize(_pattern.size());

    _buffer[0] = 0;
    for(int i = 1; i < _buffer.size(); ++i){
        int pos = _buffer[i - 1];
        while(pos > 0 && _pattern[pos] != _pattern[i]){
            pos = _buffer[pos - 1];
        }
        if(_pattern[pos] == _pattern[i]){
            ++pos;
        }
        _buffer[i] = pos;
    }
}

void KMP::find(std::vector< int >& result){

    for(int i = 0; i < _text.size(); ++i){
        int pos = _last;

        while(pos > 0 && _pattern[pos] != _text[i]){
            pos = _buffer[pos - 1];
        }

        if(_pattern[pos] == _text[i]){
            ++pos;
        }

        if(pos == _pattern.size()){
            result.push_back(i + 1 - _pattern.size());
        }

        _last = pos;
    }
}


int main() {

    std::string pattern;
    std::string text;

    std::cin >> pattern >> text;

    KMP kmp(pattern, text);
    
    std::vector< int > result;    
    kmp.find(result);

    for(int i = 0; i < result.size(); ++i){
        std::cout << result[i] << " ";
    }

    return 0;
}