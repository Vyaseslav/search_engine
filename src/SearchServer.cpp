#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input, int responsesLimit) {
    vector<std::vector<RelativeIndex>> relativeIndex;
    vector<string> words;
    vector<vector<string>> requests;


// Разбивает поисковый запрос на отдельные слова.
    for (int i = 0; i < queries_input.size(); ++i) {
        stringstream ss(queries_input[i]);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        requests.push_back(words);
        words.clear();
    }

// 2. Формирует из них список уникальных
    for (int i = 0; i < requests.size(); ++i) {
        for (int j = 0; j < requests[i].size()-1; ++j) {
            if(requests[i][j] == requests[i][j+1]){
                requests[i].erase(requests[i].begin() + j + 1);
            }
        }
    }

    //std::cout << std::endl;
    //for (const auto& request : requests) {
    //    for (const auto& param : request) {
    //        std::cout << param << " ";
    //    }
    //    std::cout << std::endl;
    //}


    //3. Сортирует слова в порядке увеличения частоты встречаемости: от самых
    //редких до самых частых. По возрастанию значения поля count поля
    //freq_dictionary

    vector<vector<pair<string, int>>> sorted_queries;
    sorted_queries.resize(requests.size());

    int count = 0;

    for (int i = 0; i < requests.size(); ++i) {
        for (int j = 0; j < requests[i].size(); ++j) {
            for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
                if (it->first == requests[i][j])
                {
                    for(auto sec : it->second){
                        count += sec.count;
                    }
                    sorted_queries[i].push_back(make_pair(it->first, count));
                }
                count = 0;
            }
        }
    }


    for (int i = 0; i < sorted_queries.size(); ++i) {
        for (int j = 0; j < sorted_queries[i].size(); ++j) {
            for (int k = 0; k < sorted_queries[i].size(); ++k) {
                if (sorted_queries[i][k].second < sorted_queries[i][j].second) {
                    swap(sorted_queries[i][k].second, sorted_queries[i][j].second);
                    swap(sorted_queries[i][k].first, sorted_queries[i][j].first);
                }
            }
        }
    }

    //4. По первому, самому редкому слову из списка находит все документы, в которых
    //встречается слово.

    vector<vector<pair<string,vector<int>>>> docs_queries;
    docs_queries.resize(sorted_queries.size());

    for (int i = 0; i < sorted_queries.size(); ++i) {
        for (int j = 0; j < sorted_queries[i].size(); ++j) {
            for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
                if (it->first == sorted_queries[i][j].first) {
                    vector<int> docs;
                    for (auto sec: it->second) {

                        docs.push_back(sec.doc_id);
                    }
                    docs_queries[i].push_back(make_pair(sorted_queries[i][j].first, docs));
                    docs.clear();
                }
            }
        }
    }
    /*
    cout << endl;
    for (const auto& inner_vector : sorted_queries) {
        for (const auto& pair : inner_vector) {
            std::cout << pair.first << ": " << pair.second << " ";
        }
        cout << endl;
    }*/
    /*
    cout << endl;
    for (const auto& inner_vector : docs_queries) {
        for (const auto& pair : inner_vector) {
            std::cout << pair.first << ": ";
            for (const auto& value : pair.second) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
        cout << endl;
    }*/

    vector<vector<int>> uniq_docs;
    uniq_docs.resize(sorted_queries.size());

    for (int i = 0; i < docs_queries.size(); ++i) {
        for (int j = 0; j < docs_queries[i].size(); ++j) {
            for (int k = 0; k < docs_queries[i][j].second.size(); ++k) {
                uniq_docs[i].push_back(docs_queries[i][j].second[k]);
            }
        }
    }

    for (int i = 0; i < uniq_docs.size(); ++i) {
        for (int j = 0; j < uniq_docs[i].size()-1; ++j) {
            if(uniq_docs[i][j] == uniq_docs[i][j+1]){
                uniq_docs[i].erase(uniq_docs[i].begin() + j + 1);
            }
        }
    }
    /*cout << endl;
    for (int i = 0; i < docs_queries.size(); ++i) {
        for (int j = 0; j < docs_queries[i].size(); ++j) {
            cout << uniq_docs[i][j] << " ";
        } cout << endl;
    }cout <<endl;
    */

    vector<vector<pair<int, int>>> frequency;
    frequency.resize(docs_queries.size());

    for (int i = 0; i < uniq_docs.size(); ++i) {
        for (int j = 0; j < uniq_docs[j].size(); ++j) {
            for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {

                    for (auto sec: it->second) {
                        if(it->first == docs_queries[i][j].first) {
                            count += sec.count;
                        }
                    }

                }
            if(count != 0)
            frequency[i].push_back(make_pair(uniq_docs[i][j], count));
            count = 0;
        }
    }


    for (int i = 0; i < frequency.size(); ++i) {
        for (int j = 0; j < frequency[i].size(); ++j) {
            for (int k = 0; k < frequency[i].size(); ++k) {
                if(frequency[i][k].second < frequency[i][j].second){
                    swap(frequency[i][k].second, frequency[i][j].second);
                    swap(frequency[i][k].first, frequency[i][j].first);

                }
            }
        }
    }


    relativeIndex.resize(frequency.size());
    for (int i = 0; i < relativeIndex.size(); ++i) {
        relativeIndex[i].resize(frequency[i].size());
    }


    for (int i = 0; i < frequency.size(); ++i) {
        for (int j = 0; j < frequency[i].size(); ++j) {
            relativeIndex[i][j].doc_id = frequency[i][j].first;
            relativeIndex[i][j].rank = ((float)frequency[i][j].second/(float)frequency[i][0].second);

        }

    }
    /*
    for (const auto& innerVector : frequency) {
        for (const auto& pair : innerVector) {
            std::cout << "(" << pair.first << ", " << pair.second << ") ";
        }
        std::cout << std::endl;
    }*/



/*
    vector<vector<int>> docs_count;
    docs_count.resize(docs_queries.size());

    for (int i = 0; i < docs_queries.size(); ++i) {
        for (int j = 0; j < docs_queries[i].size(); ++j) {
            for (int k = 0; k < docs_queries[i][j].second.size(); ++k) {

                for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
                    for (auto sec: it->second) {
                        if(docs_queries[i][j].second[k] == sec.doc_id){
                            count+=sec.count;
                        }
                    }
                }
                docs_count[i].push_back(count);
                count = 0;
            }
        }
    }

    for (int i = 0; i < docs_count.size(); ++i) {
        for (int j = 0; j < docs_count[j].size(); ++j) {
            cout << docs_count[i][j] << " ";
        }
        cout << endl;
    }*/


    return relativeIndex;
}

std::vector<std::vector<std::pair<int, float>>>
SearchServer::relativeIndexToAnswer(vector<std::vector<RelativeIndex>> relativeIndex) {
    std::vector<std::vector<std::pair<int, float>>> answers;
    for (const auto &vec: relativeIndex) {
        std::vector<std::pair<int, float>> temp;

        for (const auto &index: vec) {
            temp.emplace_back(index.doc_id, index.rank);
        }
        answers.push_back(temp);
    }
    return answers;
}