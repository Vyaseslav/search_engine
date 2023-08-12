#include "SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    vector<std::vector<RelativeIndex>> relativeIndex;
    vector<string> words;
//1. Разбивает поисковый запрос на отдельные слова.
    for (int i = 0; i < queries_input.size(); ++i) {
        std::istringstream iss(queries_input[i]);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
    }
//2. Формирует из них список уникальных.
    vector<string> queries;
    queries.push_back(words[0]);
    for (int i = 1; i < words.size(); ++i) {
        if (count(queries.begin(), queries.end(), words.at(i)) == 0) {
            queries.push_back(words.at(i));
        }
    }

    vector<string> sorted_queries;
    for (int i = 0; i < queries.size(); i++) {
        if (freq_dictionary.count(queries[i])) {
            sorted_queries.push_back(queries[i]);
        }
    }

    size_t total_count = 0;
    vector<int> count;
    vector<int> rec_id;

    rec_id.resize(sorted_queries.size());
    for (int i = 0; i < rec_id.size(); ++i) {
        rec_id[i] = i;
    }

// fix!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for (int i = 0; i < sorted_queries.size(); i++) {
        vector<Entry> pair = freq_dictionary[sorted_queries[i]];
        for (int j = 0; j < pair.size(); ++j) {
            total_count += pair[j].count;
        }
        count.push_back(total_count);
        total_count = 0;
    }

    for (int i = 0; i < sorted_queries.size(); i++) {
        for (int j = 0; j < sorted_queries.size(); ++j) {
            if (count[i] < count[j]) {
                swap(count[i], count[j]);
                swap(sorted_queries[i], sorted_queries[j]);
                swap(rec_id[i], rec_id[j]);
            }
        }
    }
/*
cout << "req count id" << endl;
for (int i = 0; i < sorted_queries.size(); ++i) {
    cout << sorted_queries[i] << " " <<count[i] << " " << rec_id[i] << endl;
}
cout << endl;
*/
    vector<RelativeIndex> buffer;
/*
cout << "(doc_id, count)" <<endl;
for(auto pair : freq_dictionary) {
    cout << pair.first << " ";
    for (auto sec: pair.second) cout << "(" << sec.doc_id << ", " << sec.count << ")";
    cout << endl;
}
*/

    for (int i = 0; i < sorted_queries.size(); ++i) {
        vector<Entry> pair = freq_dictionary[sorted_queries[i]];
        buffer.resize(freq_dictionary[sorted_queries[i]].size());
        for (int j = 0; j < pair.size(); ++j) {
            buffer[j].doc_id = pair[j].doc_id;
        }
        relativeIndex.push_back(buffer);
        buffer.clear();
    }

    float a = 0;

    for (int i = 0; i < sorted_queries.size(); ++i) {
        for (int j = 0; j < relativeIndex[i].size(); ++j) {
            if (rec_id[i] == j) {
                a += count[j];
            }
        }
        buffer = relativeIndex[i];
        buffer[i].rank = a / 10;
        relativeIndex[i] = buffer;
    }
/*
cout <<"doc_id, rank\n";
for (const auto& row : relativeIndex) {
    for (const auto& element : row) {
        std::cout << element.doc_id << " " << element.rank << "; ";
    }
    std::cout << std::endl;
}
*/
    return relativeIndex;
}

std::vector<std::vector<std::pair<int, float>>> SearchServer::relativeIndexToAnswer(vector<std::vector<RelativeIndex>> relativeIndex) {
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