#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> config;
    std::vector<std::string> filePath;
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
        filePath.resize(2);

        if (dict["config"] != nullptr) {
            filePath[0] = dict["config"]["name"];
            filePath[1] = dict["config"]["version"];
            for (int i = 0; i < dict["files"].size(); i++) {
                filePath.push_back(dict["files"][i]);
            }
            config.resize(filePath.size());
        }
        for (int i = 2; i < filePath.size(); i++) {
            ifstream file(filePath[i]);
            if (!file)
            {
                cout << "File not found!\n";
            }
            else {
                //cout << "OK!\n";
                string str;
                while (!file.eof()){
                    getline(file, str);
                    config[i-2] += " " + str;
                }
                file.close();
            }
        }

/*
for (int i = 0; i < config.size(); ++i) {
    cout << config[i] << endl;
}*/
        //config.erase(config.begin()); //delete name
        //config.erase(config.begin()); //delete version
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
        if(!answers[i].empty()) {


            for (const auto &pair: answers[i]) {
                if(pair.second != 0) {
                    nlohmann::json relevanceJson;
                    requestJson["result"] = "true";
                    relevanceJson["doc_id"] = pair.first;
                    relevanceJson["rank"] = pair.second;

                    requestJson["relevance"].push_back(relevanceJson);
                }
            }
        }
        else {
            requestJson["result"] = "false";
        }
        json["answers"][requestKey] = requestJson;
    }

    std::ofstream file("json/answers.json");
    if (!file)
    {
        cout << "answers.json not found!\n";
        exit(0);
    }
    else {
        file << json.dump(GetResponsesLimit());
        file.close();
        std::cout << std::endl << "answers.json filled successfully." << std::endl;
    }
}