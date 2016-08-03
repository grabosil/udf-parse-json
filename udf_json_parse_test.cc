#include <iostream>

#include <impala_udf/udf-test-harness.h>
#include "parse-json.h"

using namespace impala;
using namespace impala_udf;
using namespace std;

int main(int argc, char** argv)
{
    bool passed = true;
    char* json = "{\"firstTime\":\"01/Aug/2016:14:40:35.916 +0000\",\"testString\":\"tested\"}                                                                                                                                                                                                                                                                ";
    char* key = "testString";
    char* value = "\"tested\"";

    passed &= UdfTestHarness::ValidateUdf<StringVal, StringVal, StringVal>(
        ParseJSON, StringVal((uint8_t*)json, strlen(json)), StringVal((uint8_t*)key, strlen(key)), StringVal((uint8_t*)value, strlen(value)));

    cout << "Tests " << (passed ? "Passed." : "Failed.") << endl;
    return !passed;
}
