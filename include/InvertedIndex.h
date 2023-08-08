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

    void UpdateDocumentBase(std::vector<std::string> input_docs){
        input_docs.erase(input_docs.begin()); //delete name
        input_docs.erase(input_docs.begin()); //delete version
        std::vector<std::string> words;
        docs = input_docs;
        //split text to words
        for (int i = 0; i < input_docs.size(); ++i) {
            std::istringstream iss(input_docs[i]);
            std::string word;
            while (iss >> word) {
                words.push_back(word);
            }
        }

        for (int i = 0; i < words.size(); ++i) {
            //cout << words[i] << " ";
            freq_dictionary.insert(std::make_pair(words[i], GetWordCount(words[i])));
        }
/*
        cout << "(doc_id, count)" <<endl;
        for(auto pair : freq_dictionary) {
            cout << pair.first << " ";
            for(auto sec : pair.second) cout << "(" << sec.doc_id << ", " << sec.count << ")";
            cout << endl;
        }*/

    }

    std::vector<Entry> GetWordCount(const std::string& word){
        Entry entry;
        vector<Entry> entries;
        entry.doc_id = 0;

        for(int i = 0; i < docs.size(); i++){
            entry.count = 0;
            std::istringstream iss(docs[i]);
            std::string docsWord;
            while (iss >> docsWord) {
                if(word == docsWord){
                    entry.count++;
                }
            }
            if(entry.count != 0)
            entries.push_back(entry);
            entry.doc_id++;
        }
        return entries;
    }

    std::map<std::string, std::vector<Entry>> set_freq_dictionary(){
        return freq_dictionary;
    }

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    std::vector<std::string> docs; // список содержимого документов
};




#endif //TEST_SEARCH_ENGINE_INVERTEDINDEX_H
