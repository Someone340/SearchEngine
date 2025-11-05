#include "invertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::vector<std::string>& input_docs) {
    std::string curWord;
    for (size_t i = 0; i < input_docs.size(); i++) {
        for (size_t j = 0; j <= input_docs[i].length(); j++) {
            if (input_docs[i][j] != '\0' && input_docs[i][j] != ' ') {
                curWord += input_docs[i][j];
            } else {
                std::vector<Entry> newIndex;

                if (freq_dictionary.count(curWord) > 0) {
                    bool idIsFound = false;
                    auto it = freq_dictionary.find(curWord);
                    newIndex = it->second;
                    for (auto& curEntry : newIndex) {
                        if (curEntry.doc_id == i) {
                            curEntry.count = curEntry.count + 1;
                            idIsFound = true;
                        }
                    }
                
                    if (!idIsFound) {
                        newIndex.push_back({i, 1});
                    }
                
                    freq_dictionary[curWord] = newIndex;
                } else {
                    newIndex.push_back({i, 1});
                    freq_dictionary[curWord] = newIndex;
                }
                curWord.clear();
            }
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    std::vector<Entry> result;
    if (freq_dictionary.count(word) > 0) {
        auto it = freq_dictionary.find(word);
        result = it->second;
    }
    return result;
}