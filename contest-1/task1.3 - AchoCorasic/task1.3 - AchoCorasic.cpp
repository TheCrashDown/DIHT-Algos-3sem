/*  Task:
    Шаблон поиска задан строкой длины m, в которой кроме обычных символов могут встречаться 
    символы “?”. Найти позиции всех вхождений шаблона в тексте длины n. 
    Каждое вхождение шаблона предполагает, что все обычные символы совпадают с 
    соответствующими из текста, а вместо символа “?” в тексте встречается произвольный символ.

    Время работы - O(n + m + Z), где Z - общее -число вхождений подстрок шаблона 
    “между вопросиками” в исходном тексте. m ≤ 5000, n ≤ 2000000.

    developed by Crashdown

*/
#pragma GCC optimize("Ofast,unroll-all-loops")

#include <iostream>
#include <vector>


class AchoKorasic{
public:

    AchoKorasic(std::string& mask);

    // функция для поиска ответа на задачу
    void find(std::string& text, std::vector< int >& ans);

private:

    // функции для построения бора
    void subMasksInit();
    void addSubMask(int start, int end, int pattern);

    // парные рекурсивные функции для построения суфф ссфлок и терминальных переходов
    int buildSuffLink(int vertex);
    int buildTerminateLink(int vertex, char chr);

    // структура с данными вершины
    struct Point;

    std::string& _mask;
    // бор
    std::vector< Point > _buffer;
    // позиции подмасок без ?
    std::vector< std::pair<int, int > > _subMasks;
};

struct AchoKorasic::Point{
    char chr;
    bool isTerminal;

    int parent;
    int suffLink;

    std::vector< int > children;
    std::vector< int > jumps;
    std::vector< int > patterns;

    Point(int parent_, char chr_) : chr(chr_), parent(parent_){
        suffLink = -1;
        init();
    }

    Point(int parent_, char chr_, int suffLink_) : chr(chr_), parent(parent_){
        suffLink = suffLink_;
        init();
    }

    void init(){
        isTerminal = false;
        children.resize(26, -1);
        jumps.resize(26, -1);
    }
};

void AchoKorasic::subMasksInit(){
    // функция режет исходную маску на подмаски без символов ?
    int current_start = 0;

    while(_mask[current_start] == '?' && current_start < _mask.size()){
        current_start++;
    }

    int i = current_start + 1;
    while(i < _mask.size()){
        if(_mask[i] == '?' && _mask[i - 1] != '?'){
            _subMasks.push_back(std::make_pair(current_start, i - 1));

            while(_mask[i] == '?' && i < _mask.size()){
                i++;
            }
            current_start = i;
        }
        i++;
    }

    if(_mask[_mask.size() - 1] != '?'){
        _subMasks.push_back(std::make_pair(current_start, _mask.size() - 1));
    }
}

void AchoKorasic::addSubMask(int start, int end, int pattern){
    // функция добавляет подмаски в бор
    int current = 0;
    for(int i = start; i <= end; ++i){
        char chr = _mask[i] - 'a';
        if(_buffer[current].children[chr] == -1){
            _buffer.push_back(Point(current, chr));
            _buffer[current].children[chr] = _buffer.size() - 1;
        }
        current = _buffer[current].children[chr];
    }
    _buffer[current].isTerminal = true;
    _buffer[current].patterns.push_back(pattern);
}

AchoKorasic::AchoKorasic(std::string& mask) : _mask(mask) {
    _buffer.push_back(Point(0, -1, 0));

    subMasksInit();

    for(int i = 0; i < _subMasks.size(); ++i){
        addSubMask(_subMasks[i].first, _subMasks[i].second, i);
    }
}

int AchoKorasic::buildSuffLink(int vertex){
    // строим суффиксную ссылку, если надо, и возвращаем ее
    if(_buffer[vertex].suffLink == -1){

        if(_buffer[vertex].parent == 0){
            _buffer[vertex].suffLink = 0;
        } else {
            _buffer[vertex].suffLink = buildTerminateLink(buildSuffLink(_buffer[vertex].parent),
                                                             _buffer[vertex].chr);
        }
    }
    return _buffer[vertex].suffLink;
}

int AchoKorasic::buildTerminateLink(int vertex, char chr){
    // строим переход, если надо, и возвращаем его
    if(_buffer[vertex].jumps[chr] == -1){

        if(_buffer[vertex].children[chr] != -1){
            _buffer[vertex].jumps[chr] = _buffer[vertex].children[chr];
        } else if (vertex == 0){
            _buffer[vertex].jumps[chr] = 0;
        } else {
            _buffer[vertex].jumps[chr] = buildTerminateLink(buildSuffLink(vertex), chr);
        }
    }
    return _buffer[vertex].jumps[chr];
}

void AchoKorasic::find(std::string& text, std::vector< int >& ans){
    // функция, которая ищет вхождения маски в текст и записывает их в ans
    std::vector< int > entry(text.size());
    int vertex = 0;

    for(int i = 0; i < text.size(); ++i){
        vertex = buildTerminateLink(vertex, text[i] - 'a');
        int next = vertex;

        while(next != 0){
            if(!_buffer[next].isTerminal){
                next = buildSuffLink(next);
                continue;
            }

            for(int j = 0; j < _buffer[next].patterns.size(); ++j){

                int tmp = i - _subMasks[_buffer[next].patterns[j]].second;

                if(tmp >= 0 && tmp + _mask.size() <= text.size()){
                    entry[tmp]++;
                }
            }
            next = buildSuffLink(next);
        }
    }

    for(int i = 0; i < entry.size(); ++i){
        if(entry[i] == _subMasks.size()){
            ans.push_back(i);
        }
    }
}


int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pattern = "";
    std::string text = "";
    std::cin >> pattern >> text;

    AchoKorasic achoKorasic(pattern);

    std::vector< int > ans;
    achoKorasic.find(text, ans);

    for(int i = 0; i < ans.size(); ++i){
        std::cout << ans[i] << " ";
    }

    return 0;
}
