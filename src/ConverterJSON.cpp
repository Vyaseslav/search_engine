#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> config;
    ifstream input("json/config.json");
    if (!input)
    {
        cout << "config.json not found!\n";
        exit(0);
    }
    else {
        //cout << "Fine!\n";

        json dict;
        input >> dict;
        config.resize(2);
        if (dict["config"] != nullptr) {
            config[0] = dict["config"]["name"];
            config[1] = dict["config"]["version"];
            for (int i = 0; i < dict["files"].size(); i++) {
                config.push_back(dict["files"][i]);
            }
        }
        for (int i = 2; i < config.size(); i++) {
            ifstream file(config[i]);
            if (!file)
            {
                cout << "File not found!\n";
            }
            else {
                //cout << "OK!\n";
                getline(file, config[i]);
                file.close();
            }
        }
        input.close();
/*
for (int i = 0; i < config.size(); ++i) {
    cout << config[i] << endl;
}*/
        return config;
    }
    return{};
}

int ConverterJSON::GetResponsesLimit() {
    ifstream input("json/config.json");
    if (!input)
    {
        cout << "config.json not found!\n";
        exit(0);
    }
    else {
        //cout << "Fine!\n";

        json dict;
        input >> dict;
        input.close();
        return dict["config"]["max_responses"];
    }
}

std::vector<std::string> ConverterJSON::GetRequests() {
//int limit = GetResponsesLimit();
    std::vector<std::string> requests;
    ifstream input("json/requests.json");
    if (!input)
    {
        cout << "requests.json not found!\n";
        exit(0);
    }
    else {
        //cout << "Fine!\n";

        json dict;
        input >> dict;
        for (int i = 0; i < dict["requests"].size(); i++) {
            requests.push_back(dict["requests"][i]);
        }
        input.close();
        return requests;
    }
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::json json;
    for (int i = 0; i < answers.size(); i++) {
        std::string requestKey = "request" + std::to_string(i + 1);
        nlohmann::json requestJson;

        for (const auto &pair: answers[i]) {
            nlohmann::json relevanceJson;
            relevanceJson["docid"] = pair.first;
            relevanceJson["rank"] = pair.second;

            requestJson["relevance"].push_back(relevanceJson);
        }

        requestJson["result"] = "true";

        json["answers"][requestKey] = requestJson;
    }

    std::ofstream file("json/answers.json");
    if (!file)
    {
        cout << "answers.json not found!\n";
        exit(0);
    }
    else {
        file << json.dump(4);
        file.close();
        std::cout << "answers.json filled successfully." << std::endl;
    }
}