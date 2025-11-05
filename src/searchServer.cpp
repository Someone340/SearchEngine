#include "searchServer.h"

void SearchServer::queueSearch(const std::string& queue, InvertedIndex& _index,
            std::mutex& relativeRelevanceLock, std::vector<std::vector<RelativeIndex>>& relativeRelevance) {

    std::vector<std::string> words;
    std::vector<RelativeIndex> curRelativeRelevance;
    std::string splitWords;
    for (size_t i = 0; i <= queue.length(); i++) {
        if (queue[i] != '\0' && queue[i] != ' ') {
            splitWords += queue[i];
        } else {
            auto it = std::find(words.begin(), words.end(), splitWords);
            if (it == words.end())
                words.push_back(splitWords);
            splitWords.clear();
        }
    }

    std::map<size_t, std::vector<std::string>> frequency;
    size_t foundTimes = 0;

    for (auto& wordFromDict : words) {
        std::vector<Entry> entryGetCount = _index.GetWordCount(wordFromDict);

        int foundTimes = 0;
        for (auto& curEntry : entryGetCount)
            foundTimes += curEntry.count;

        if (foundTimes != 0)
            frequency[foundTimes].push_back(wordFromDict);

        foundTimes = 0;
    }

    std::vector<Entry> absoluteRelevance;
    for (auto& curFrequency : frequency) {
        for (auto& curWord : curFrequency.second) {
            if (absoluteRelevance.empty())
                absoluteRelevance = _index.GetWordCount(curWord);
            else {
                auto addRelevance = _index.GetWordCount(curWord);
                for (auto& test : addRelevance) {
                    bool found = false;
                    for (auto& test2 : absoluteRelevance) {
                        if (test.doc_id == test2.doc_id) {
                            test2.count += test.count;
                            found = true;
                        }
                    }
                    if (!found) {
                        absoluteRelevance.push_back(test);
                    }
                }
            }
        }
    }

    float maxAbsoluteRelevance = 0;
    for (auto& i : absoluteRelevance) {
        if (maxAbsoluteRelevance < i.count)
            maxAbsoluteRelevance = i.count;
    }

    int answersAmount = 0;
    for (auto& curRelevance : absoluteRelevance) {
        RelativeIndex rInd;
        rInd.doc_id = curRelevance.doc_id;
        rInd.rank = curRelevance.count / maxAbsoluteRelevance;
        curRelativeRelevance.push_back(rInd);
        answersAmount++;
        if (answersAmount == 5)
            break;
    }

    for (int i = 0; i < curRelativeRelevance.size(); i++) {
        bool flag = true;
        for (int j = 0; j < curRelativeRelevance.size() - (i + 1); j++) { 
            if (curRelativeRelevance[j].rank < curRelativeRelevance[j + 1].rank) {
            flag = false;
            std::swap(curRelativeRelevance[j], curRelativeRelevance[j + 1]);
            }
        }
        if (flag) {
            break;
        }
    }
            
    relativeRelevanceLock.lock();
    relativeRelevance.push_back(curRelativeRelevance);
    relativeRelevanceLock.unlock();
}

void SearchServer::updateMaxResponse(const int& maxResponse){
    max_response = maxResponse;
};

std::vector<std::vector<RelativeIndex>> SearchServer::search
        (const std::vector<std::string>& queries_input) {
    std::mutex relativeRelevanceLock;
            
    std::vector<std::vector<RelativeIndex>> relativeRelevance;
    std::vector<std::thread> threadsVector;

    for (auto& queue : queries_input) {
        threadsVector.push_back(std::thread(&SearchServer::queueSearch, this, queue, std::ref(_index), std::ref(relativeRelevanceLock), std::ref(relativeRelevance)));
    }

    for (auto& curThread : threadsVector) {
        curThread.join();
    }

    return relativeRelevance;
};
