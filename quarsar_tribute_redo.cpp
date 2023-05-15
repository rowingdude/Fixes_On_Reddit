#include "DuplicateLineRemover.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::vector;

struct Config {
    size_t outputSelection = 1;
    string inFilepathPrefix = "D:/Downloads/";
    string inFilepath = "";
    // string outFilepath = "D:/Downloads/DuplicateLineRemover output.txt"; // FIXME need to fix system call for this
};

Config parseArguments(int argc, char* argv[]) {
    Config config;
    if (argc >= 2) {
        string arg = argv[1];
        if (arg == "-c" || arg == "-0")
            config.outputSelection = 0;
        else if (arg == "-p" || arg == "-1")
            config.outputSelection = 1;
        else if (arg == "-d" || arg == "-2")
            config.outputSelection = 2;
        else
            config.inFilepath = arg;
    }
    return config;
}

LinePair processLine(const string& line) {
    if (!line.empty() && line[0] == '-')
        return LinePair{ line, line, line, true };

    string modifiedLine = line;
    string domain;

    size_t startPosToErase = modifiedLine.find("&commit=Search");
    if (startPosToErase != string::npos)
        modifiedLine.erase(startPosToErase, 14);
    startPosToErase = modifiedLine.find(" Sankaku Channel - Anime, manga & game related images & videos");
    if (startPosToErase != string::npos)
        modifiedLine.erase(startPosToErase - 2, 64);

    size_t domainPos = findDomainExtensionPos(modifiedLine);
    if (domainPos != string::npos) {
        domain = modifiedLine.substr(0, domainPos + 4);
    }

    if (domain == "redacted") {
        startPosToErase = modifiedLine.find("%20order%3Apopular");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 18);
        startPosToErase = modifiedLine.find("+order%3Apopular");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 16);
        startPosToErase = modifiedLine.find("order%3Apopular");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 15);

        startPosToErase = modifiedLine.find("&page=");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, modifiedLine.find(" ") - startPosToErase);

        startPosToErase = modifiedLine.find("%20 ");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 3);
        startPosToErase = modifiedLine.find("& ");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 1);
    }
    else if (domain == "redacted") {
        startPosToErase = modifiedLine.find("+order%3Afavcount");
        if (startPosToErase != string::npos)
            modifiedLine.erase(startPosToErase, 17);
        startPosToErase =
        modifiedLine.find("redacted/posts?page=");
        if (startPosToErase != string::npos) {
            startPosToErase = modifiedLine.find("page=");
            modifiedLine.erase(startPosToErase, modifiedLine.find("&") - startPosToErase + 1);
        }
    }

    startPosToErase = modifiedLine.find("+ ");
    if (startPosToErase != string::npos)
        modifiedLine.erase(startPosToErase, 1);

    modifiedLine.erase(modifiedLine.end() - 1);

    return LinePair{ modifiedLine.substr(0, line.find(" ") + 1), line, domain };
}

size_t findDomainExtensionPos(const string& url) {
    static const vector<string> domainExtensions = { ".com/", ".net/", ".xxx/", ".gov/", ".org/", ".edu/" };

    for (const auto& extension : domainExtensions) {
        size_t pos = url.find(extension);
        if (pos != string::npos)
            return pos;
    }
    return string::npos;
}

bool sameDomains(const string& domain1, const string& domain2) {
    static const vector<string> validDomains = { "redacted0", "redacted1", "redacted2" };

    if (domain1.empty() || domain2.empty())
        return false;

    if (std::find(validDomains.begin(), validDomains.end(), domain1) != validDomains.end() &&
        std::find(validDomains.begin(), validDomains.end(), domain2) != validDomains.end()) {
        return true;
    }
    return domain1 == domain2;
}

int main(int argc, char* argv[]) {
    Config config = parseArguments(argc, argv);
    cout << argv[0] << endl;

    if (config.inFilepath.empty()) {
        do {
            cout << "Enter file: " << std::flush;
            std::getline(std::cin, config.inFilepath);

            if (config.inFilepath == "-c" || config.inFilepath == "-0") {
                config.outputSelection = 0;
                cout << "outputSelection set to 0" << endl;
                continue;
            }
            if (config.inFilepath == "-p" || config.inFilepath == "-1") {
                config.outputSelection = 1;
                cout << "outputSelection set to 1" << endl;
                continue;
            }
            if (config.inFilepath == "-d" || config.inFilepath == "-2") {
                config.outputSelection = 2;
                cout << "outputSelection set to 2" << endl;
                continue;
            }

            if (!(config.inFilepath[0] == 'D' || config.inFilepath[0] == 'C') &&
                (config.inFilepath[1] == ':' && (config.inFilepath[2] == '/' || config.inFilepath[2] == '\\'))) {
                config.inFilepath = config.inFilepathPrefix + config.inFilepath;
            }
            if (config.inFilepath.substr(config.inFilepath.size() - 4) != ".txt")
                config.inFilepath += ".txt";
            cout << "Input file: " << config.inFilepath << endl;
        } while (!std::ifstream(config.inFilepath));
    }

    std::ifstream fileIn(config.inFilepath);
    if (!fileIn) {
        cout << "Failed to open input file." << endl;
        return 1;
    }

    vector<LinePair> linePairs;
    string line;
    while (std::getline(fileIn, line)) {
        linePairs.push_back(processLine(line
