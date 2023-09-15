#include "SearchServer.h"

vector<vector<string>> SearchServer::getUniqRequests(const std::vector<std::string> &queries_input){
    vector<vector<string>> requests;
    vector<string> words;
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
    return requests;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    vector<std::vector<RelativeIndex>> relativeIndex;
    vector<vector<string>> requests = getUniqRequests(queries_input);

    //3. Сортирует слова в порядке увеличения частоты встречаемости: от самых
    //редких до самых частых. По возрастанию значения поля count поля
    //freq_dictionary
    vector<vector<pair<string, int>>> sorted_queries;
    sorted_queries.resize(requests.size());

    for (int i = 0; i < requests.size(); ++i) {
        for (int j = 0; j < requests[i].size(); ++j) {
            auto it = freq_dictionary.find(requests[i][j]);
            if (it == freq_dictionary.end())
            {
                sorted_queries[i].emplace_back(make_pair(requests[i][j], 0));
            }
                else if (it->first == requests[i][j])
                {
                    for(auto sec : it->second)
                        count += sec.count;
                    sorted_queries[i].emplace_back(make_pair(it->first, count));
                }
            count = 0;
        }
    }

    if(sorted_queries.empty()){
        relativeIndex.resize(1);
        relativeIndex[0].resize(1);
        relativeIndex[0][0].rank = 0;
        relativeIndex[0][0].doc_id = 0;
        return relativeIndex;
    }
    else {
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
        vector<vector<pair<string, vector<int>>>> docs_queries;
        docs_queries.resize(sorted_queries.size());

        for (int i = 0; i < sorted_queries.size(); ++i) {
            for (int j = 0; j < sorted_queries[i].size(); ++j) {
                for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
                    if (it->first == sorted_queries[i][j].first) {
                        vector<int> docs;
                        for (auto sec: it->second) {
                            docs.push_back(sec.doc_id);
                        }
                        docs_queries[i].emplace_back(make_pair(sorted_queries[i][j].first, docs));
                        docs.clear();
                    }
                }
            }
        }

        vector<vector<pair<int, int>>> frequency;
        frequency.resize(docs_queries.size());

        count = 0;
        for (int i = 0; i < docs_queries.size(); ++i) {
            for (int j = 0; j < docs_queries[i].size(); ++j) {
                for (int k = 0; k < docs_queries[i][j].second.size(); ++k) {
                    for (auto it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
                        for (auto sec: it->second) {
                            if (sec.doc_id == docs_queries[i][j].second[k] && it->first == docs_queries[i][j].first) {
                                count = sec.count;
                            }
                        }
                    }
                    if (count != 0)
                        frequency[i].emplace_back(make_pair(docs_queries[i][j].second[k], count));
                    count = 0;
                }
            }
        }

        for (int i = 0; i < frequency.size(); ++i) {
            for (int j = 0; j < frequency[i].size(); ++j) {
                for (int k = j+1; k < frequency[i].size(); ++k) {
                    if (frequency[i][j].first == frequency[i][k].first) {
                        count = frequency[i][k].second;
                        frequency[i][j].second += count;
                        frequency[i][k].second = 0;

                    }
                }
            }
        }

        for (int i = 0; i < frequency.size(); ++i) {
            for (int j = 0; j < frequency[i].size(); ++j) {
                for (int k = 0; k < frequency[i].size(); ++k) {
                    if (frequency[i][k].second < frequency[i][j].second) {
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
                //cout << frequency[i][j].second << " , ";
                relativeIndex[i][j].rank = ((float) frequency[i][j].second / (float) frequency[i][0].second);
                //cout << frequency[i][0].second << endl;
            }
        }
    }
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