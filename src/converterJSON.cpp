#include "converterJSON.h"
#include "exceptions.h"
#include <fstream>
#include <exception>


void ConverterJSON::setup(){
    std::ifstream fileConfig("config.json");
    if (fileConfig.is_open()){
        fileConfig >> config;
        fileConfig.close();
        if (config.contains("config")) {
            std::ifstream fileRequest("requests.json");
            fileRequest >> requests;
            fileRequest.close();
        } else {
        throw EmptyConfigError();
        }
    } else {
        throw ConfigIsMissingError();
    }
};

std::string ConverterJSON::GetVersion() {
    return config["config"]["version"];
}

std::string ConverterJSON::GetName() {
    return config["config"]["name"];
}

std::vector<std::string> ConverterJSON::GetTextDocuments(){
    return config["files"];
}

int ConverterJSON::GetResponsesLimit(){
    if (config["config"].find("max_responses") != config["config"].end())
        return config["config"]["max_responses"];
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests(){
    return requests["requests"];
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers){
    std::ofstream file("answer.json");
    nlohmann::json dict;

    int request = 1;
    for (auto& answer: answers) {
        std::string curRequest = "Request";

        if (request / 10.0 < 1.0)
            curRequest += ("00" + std::to_string(request));
        else if (request / 100.0 < 1.0)
            curRequest += ("0" + std::to_string(request));
        else
            curRequest += std::to_string(request);        


        if (!answer.empty()) {
            dict["answers"][curRequest]["result"] = true;
            //std::map<size_t, float> m;
            nlohmann::json pair;
            if (answer.size() > 1) {
                
                for (auto& curAnswer : answer) {
                    pair += nlohmann::json::object({ {"docid", curAnswer.doc_id}, {"rank", round(curAnswer.rank*1000)/1000} });
                }
                dict["answers"][curRequest]["relevance"] = pair;
            } else {
                pair = nlohmann::json::object({ {"docid", answer[0].doc_id}, {"rank", round(answer[0].rank*1000)/1000} });
                dict["answers"][curRequest] = pair;
            }

        } else {
            dict["answers"][curRequest]["result"] = false;
        }

        request++;
    }

    file << dict;
    file.close();
}
