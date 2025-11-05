#include "converterJSON.h"
#include "exceptions.h"
#include <fstream>
#include <exception>
#define CONFIGMAIN



void ConverterJSON::setup(){
    std::ifstream fileConfig("config.json");
    if (fileConfig.is_open()){
        fileConfig >> config;
        fileConfig.close();
        if (config.contains(configMain)) {
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
    return config[configMain][configVersion];
}

std::string ConverterJSON::GetName() {
    return config[configMain][configName];
}

std::vector<std::string> ConverterJSON::GetTextDocuments(){
    return config[configFiles];
}

int ConverterJSON::GetResponsesLimit(){
    if (config[configMain].find(configMaxResponses) != config[configMain].end())
        return config[configMain][configMaxResponses];
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests(){
    return requests[requestsList];
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>& answers){
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
            dict[answersMain][curRequest][answersResult] = true;
            //std::map<size_t, float> m;
            nlohmann::json pair;
            if (answer.size() > 1) {
                
                for (auto& curAnswer : answer) {
                    pair += nlohmann::json::object({ {answersDocid, curAnswer.doc_id}, {answersRank, round(curAnswer.rank*1000)/1000} });
                }
                dict[answersMain][curRequest][answersRelevance] = pair;
            } else {
                pair = nlohmann::json::object({ {answersDocid, answer[0].doc_id}, {answersRank, round(answer[0].rank*1000)/1000} });
                dict[answersMain][curRequest] = pair;
            }

        } else {
            dict[answersMain][curRequest][answersResult] = false;
        }

        request++;
    }

    file << dict;
    file.close();
}
