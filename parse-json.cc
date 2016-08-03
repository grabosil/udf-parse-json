#include "parse-json.h"
#include "json.hpp"

#include <cctype>
#include <algorithm>
#include <string>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <boost/algorithm/string.hpp>
using namespace std;
using json = nlohmann::json;

StringVal ParseJSON(FunctionContext* context, const StringVal& arg1, const StringVal& arg2)
{

    if (arg1.is_null || arg2.is_null) {
        return StringVal::null();
    }

    stringstream jsonStream;
    for (size_t i = 0; i < arg1.len; ++i) {
        jsonStream << arg1.ptr[i];
    }
    string castJson = jsonStream.str();

    boost::trim(castJson);
    string jsonString = evaluateObject(castJson);
    if (jsonString.size() == 0 ) {
        return StringVal::null();
    }

    stringstream keystream;
    for (size_t i = 0; i < arg2.len; ++i) {
        keystream << arg2.ptr[i];
    }
    string key = keystream.str();

    auto jsonObject = json::parse(jsonString);

    string value = jsonObject[key].dump();

    if (value.empty()) {
        return StringVal::null();
    }

    StringVal result(context, value.size());
    memcpy(result.ptr, value.c_str(), value.size());

    return result;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

string evaluateObject(const string& s)
{
    size_t objectOpen = std::count(s.begin(), s.end(), '{');
    size_t objectClose = std::count(s.begin(), s.end(), '}');
    if (objectOpen == objectClose) {
        return makeLastCharValid(s);
    }
    else {
        ofstream errorFile;
        errorFile.open("udf-parse-json-error.log", ofstream::out | ofstream::in | ofstream::app);
        errorFile << currentDateTime() << " structure validation failed\n";
        errorFile << currentDateTime() << " string: " << s << "\n";
        errorFile.close();
        return string();
    }
}

string makeLastCharValid(string s)
{
    string oldString = s;
    if(s.size() == 0 || s == "null") {
        return string();
    }
    char c = s[s.size() - 1];
    while (c != '}' && s.size() > 0) {
        s.pop_back();
        c = s[s.size() - 1];
    }

    if (s.size() == 0) {
        ofstream errorFile;
        errorFile.open("udf-parse-json-error.log", ofstream::out | ofstream::in | ofstream::app);
        errorFile << currentDateTime() << " jsonstring '" << oldString << "' is empty after modify \n";
        errorFile.close();
        return string();
    }
    return s;
}