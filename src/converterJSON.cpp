#include "converterJSON.h"
#include "exceptions.h"
#include <fstream>
#include <exception>


void ConverterJSON::setup() {
    std::ifstream fileConfig(configFile);
    if (fileConfig.is_open()){
        fileConfig >> config;
        fileConfig.close();
        if (config.contains(configMain)) {
            std::ifstream fileRequest(requestsFile);
            fileRequest >> requests;
            fileRequest.close();
        } else {
        throw EmptyConfigError();
        }
    } else {
        throw ConfigIsMissingError();
    }
};

std::string ConverterJSON::GetVersion() const {
    return config[configMain][configVersion];
}

std::string ConverterJSON::GetName() const {
    return config[configMain][configName];
}

std::vector<std::string> ConverterJSON::GetTextDocuments() const {
    return config[configFiles];
}

int ConverterJSON::GetResponsesLimit() const {
    if (config[configMain].find(configMaxResponses) != config[configMain].end())
        return config[configMain][configMaxResponses];
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() const {
    return requests[requestsList];
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>& answers){
    std::ofstream file(answerFile);
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

    std::ifstream answer("answer.json");
    if (answer.is_open())
        std::cout << "Results were saved in answer.json" << std::endl;
    else
        std::cout << "Couldn't save results" << std::endl;
    answer.close();
}
