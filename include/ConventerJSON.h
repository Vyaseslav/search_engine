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
    std::vector<std::string> GetTextDocuments()
    {
        std::vector<std::string> config;
        ifstream input("C:\\Users\\slava\\CLionProjects\\test_search_engine\\config.json");
        json dict;
        input >> dict;
        config.resize(2);
        if(dict["config"] != nullptr){
            config[0] = dict["config"]["name"];
            config[1] = dict["config"]["version"];
            for(int i = 0; i < dict["files"].size(); i++){
                config.push_back(dict["files"][i]);
            }
        }
        for(int i = 2; i < config.size(); i++){
            ifstream file(config[i]);
            getline(file, config[i]);
            file.close();
        }
        input.close();
        /*
        for (int i = 0; i < config.size(); ++i) {
            cout << config[i] << endl;
        }*/
        return config;
    }

    int GetResponsesLimit() {
        ifstream input("C:\\Users\\slava\\CLionProjects\\test_search_engine\\config.json");
        json dict;
        input >> dict;
        input.close();
        return dict["config"]["max_responses"];
    }

    std::vector<std::string> GetRequests() {
        //int limit = GetResponsesLimit();
        std::vector<std::string> requests;
        ifstream input("C:\\Users\\slava\\CLionProjects\\test_search_engine\\requests.json");
        json dict;
        input >> dict;
        for(int i = 0; i < dict["requests"].size(); i++){
            requests.push_back(dict["requests"][i]);
        }
        input.close();
        return requests;
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
    {
        nlohmann::json json;

        for (int i = 0; i < answers.size(); i++)
        {
            std::string requestKey = "request" + std::to_string(i + 1);
            nlohmann::json requestJson;

            for (const auto& pair : answers[i])
            {
                nlohmann::json relevanceJson;
                relevanceJson["docid"] = pair.first;
                relevanceJson["rank"] = pair.second;

                requestJson["relevance"].push_back(relevanceJson);
            }

            requestJson["result"] = "true";

            json["answers"][requestKey] = requestJson;
        }

        std::ofstream file("C:\\Users\\slava\\CLionProjects\\test_search_engine\\answers.json");
        file << json.dump(4);
        file.close();

        std::cout << "answers.json filled successfully." << std::endl;
    }
private:
};



#endif //TEST_SEARCH_ENGINE_CONVENTERJSON_H
