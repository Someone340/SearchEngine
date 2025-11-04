# SearchEngine
## Description

This search engine was created to help you find files containing the word you are looking for.

## How does it work

The search engine operates as follows:
1. The names of the files the search engine will search are specified in the config* file before launching the application
2. The user submits a request via the JSON file requests.json*. The request is a set of words for which documents should be found
3. The search engine will automatically scan all files and
index them to find the most relevant documents for any search query
4. The request will be converted into a list of words
5. The index is searched for documents containing all of these words
6. The search results will be ranked, sorted, and returned to the user.
The maximum number of possible documents in the response is specified in the config* file.
7. Finally, the program generates the answers.json* file, in which it writes the search results.

> *All the json files can be found in src folder.

## List of currently supported versions of compilers, platforms, and build tools

|Dimension|Supported verion|
|:-----------|------------:|
|C++ Version|>= 17|
|CMake|>= 3.10|
|GCC|>= 7.5.0|

## How to use it

1. First of all place all the files you'll be working with in the resources folder.
2. In the project's root folder open the src folder. Add all the files you placed in the resources folder to config.json
3. Write your search requests to requests.json
4. Compile the project and run it. Once the program finishes its work you'll see ***Results were saved in answer.json*** message
5. Check the results in answer.json
 > Answers will be split to requests. Each request result is sorted by their relevance. Docid contains the number of the file in resources foulder. Rank is their relevance level