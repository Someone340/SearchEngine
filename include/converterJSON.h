#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"
#include "searchServer.h"


/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
    nlohmann::json config = nullptr;
    nlohmann::json requests = nullptr;
    const std::string configFile = "config.json";
    const std::string requestsFile = "requests.json";
    const std::string answerFile = "answer.json";
    const std::string configMain = "config";
    const std::string configVersion = "version";
    const std::string configName = "name";
    const std::string configFiles = "files";
    const std::string configMaxResponses = "max_responses";
    const std::string requestsList = "requests";
    const std::string answersMain = "answers";
    const std::string answersResult = "result";
    const std::string answersRelevance = "relevance";
    const std::string answersDocid = "docid";
    const std::string answersRank = "rank";
    
public:
    ConverterJSON() {setup();};

    std::string GetVersion() const;
    std::string GetName() const;
    void setup();
        /**
        * Метод получения содержимого файлов
        * @return Возвращает список с содержимым файлов перечисленных
        * в config.json
        */
    std::vector<std::string> GetTextDocuments() const;
        /**
        * Метод считывает поле max_responses для определения предельного
        * количества ответов на один запрос
        * @return
        */
    int GetResponsesLimit() const;
        /**
        * Метод получения запросов из файла requests.json
        * @return возвращает список запросов из файла requests.json
        */
    std::vector<std::string> GetRequests() const;
        /**
        * Положить в файл answers.json результаты поисковых запросов
        */
    void putAnswers(std::vector<std::vector<RelativeIndex>>& answers);
};