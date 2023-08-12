//
// Created by slava on 02.08.2023.
//

#ifndef TEST_SEARCH_ENGINE_INVERTEDINDEX_H
#define TEST_SEARCH_ENGINE_INVERTEDINDEX_H

#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <thread>
using namespace std;
#include "ConverterJSON.h"


struct Entry {
    size_t doc_id, count;
// Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    void UpdateDocumentBase(std::vector<std::string> input_docs);

    std::vector<Entry> GetWordCount(const std::string& word);

    std::map<std::string, std::vector<Entry>> set_freq_dictionary();

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    std::vector<std::string> docs; // список содержимого документов
};




#endif //TEST_SEARCH_ENGINE_INVERTEDINDEX_H
