#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <impala_udf/udf.h>
#include <string>

using namespace impala_udf;

// Usage: > create function parse_json(string, string) returns string
//          location '/user/hdfs/libparsejson.so' SYMBOL='ParseJSON';
//        > select parse_json('{"test":"test2"}', "test");

StringVal ParseJSON(FunctionContext* context, const StringVal& arg1, const StringVal& arg2);
std::string evaluateObject(const std::string& s);
std::string makeLastCharValid(std::string s);
const std::string currentDateTime();
#endif
