#include "parse-json.h"
#include "lib/json.hpp"

#include <cctype>
#include <algorithm>
#include <string>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <boost/algorithm/string.hpp>
using namespace std;
using json = nlohmann::json;

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

string makeLastCharValid(FunctionContext* context, string toValidateString)
{
    string oldString = toValidateString;
    if(toValidateString.size() == 0 || toValidateString == "null") {
        return string();
    }
    char c = toValidateString[toValidateString.size() - 1];
    while (c != '}' && toValidateString.size() > 0) {
        toValidateString.pop_back();
        c = toValidateString[toValidateString.size() - 1];
    }

    if (toValidateString.size() == 0) {
        stringstream warningStream;
        warningStream << "jsonstring '" + oldString + "' is empty after modify";
        context->AddWarning( warningStream.str().c_str());
        return string();
    }
    return toValidateString;
}

string evaluateObject(FunctionContext* context, const string& evaluateString)
{
    size_t objectOpen = std::count(evaluateString.begin(), evaluateString.end(), '{');
    size_t objectClose = std::count(evaluateString.begin(), evaluateString.end(), '}');
    if (objectOpen == objectClose) {
        return makeLastCharValid(context, evaluateString);
    }
    else {
        stringstream warningStream;
        warningStream << "count of { is '" << objectOpen << "' and count of } is '" << objectClose << "'";
        context->AddWarning( warningStream.str().c_str());
        return string();
    }
}

StringVal ParseJSON(FunctionContext* context, const StringVal& tmpString, const StringVal& tmpKey)
{

    if (tmpString.is_null || tmpKey.is_null) {
        return StringVal::null();
    }

    stringstream jsonStream;
    for (size_t i = 0; i < tmpString.len; ++i) {
        jsonStream << tmpString.ptr[i];
    }
    string realJson = jsonStream.str();

    boost::trim(realJson);
    string jsonString = evaluateObject(context, realJson);
    if (jsonString.size() == 0 ) {
        return StringVal::null();
    }

    stringstream keystream;
    for (size_t i = 0; i < tmpKey.len; ++i) {
        keystream << tmpKey.ptr[i];
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