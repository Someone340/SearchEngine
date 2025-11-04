#include <iostream>
#include <iomanip>
#include <fstream>
#include "nlohmann/json.hpp"
#include "converterJSON.h"
#include "exceptions.h"
#include "invertedIndex.h"
#include "searchServer.h"

#define CURRENT_VERSION "0.1"
using namespace std;

void versionCheck(string configVersion) {
    if (configVersion != CURRENT_VERSION)
        throw IncorrectVersionError();
}

int main(int, char**){
    try {
        ConverterJSON* convert = new ConverterJSON;
        versionCheck(convert->GetVersion());
        cout << "---------------" << convert->GetName() << "---------------" << endl;
        cout << setw(50) << right << convert->GetVersion() << endl;

        vector<string> files, docs;
        files = convert->GetTextDocuments();
        for (auto file : files) {
            ifstream fileContent(file);
            string line;
            getline(fileContent, line);
            docs.push_back(line);
            fileContent.close();
        }

        InvertedIndex index;
        index.UpdateDocumentBase(docs);

        SearchServer server(index);
        server.updateMaxResponse(convert->GetResponsesLimit());
        vector<string> request = convert->GetRequests();
        std::vector<vector<RelativeIndex>> result = server.search(request);

        convert->putAnswers(result);

        cout << "Results were saved in answer.json" << endl;
        delete convert;
    }
    catch(const ConfigIsMissingError &x) {
        cerr << "Caught exception: " << x.what() << endl;
    }
    catch(const EmptyConfigError &x) {
        cerr << "Caught exception: " << x.what() << endl;
    }
    catch(const IncorrectVersionError &x) {
        cerr << "Caught exception: " << x.what() << endl;
    }
}