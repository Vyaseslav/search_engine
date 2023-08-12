//
// Created by slava on 03.08.2023.
//

#ifndef TEST_SEARCH_ENGINE_SEARCHSERVER_H
#define TEST_SEARCH_ENGINE_SEARCHSERVER_H

#include "InvertedIndex.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <thread>
using namespace std;

struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer() = default;

    SearchServer(InvertedIndex& idx) : _index(idx){
        freq_dictionary = _index.set_freq_dictionary();
    };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
    std::vector<std::vector<std::pair<int, float>>> relativeIndexToAnswer(vector<std::vector<RelativeIndex>> relativeIndex);

private:
    InvertedIndex _index;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};


#endif //TEST_SEARCH_ENGINE_SEARCHSERVER_H
