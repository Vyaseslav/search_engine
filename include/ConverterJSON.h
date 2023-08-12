//
// Created by slava on 02.08.2023.
//

#ifndef TEST_SEARCH_ENGINE_CONVENTERJSON_H
#define TEST_SEARCH_ENGINE_CONVENTERJSON_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class ConverterJSON {
public:
    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
};



#endif //TEST_SEARCH_ENGINE_CONVENTERJSON_H
