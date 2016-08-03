# udf-parse-json

This is a library of native implemented user-defined-functions for impalasql. 
Actually there is an json-parser for jsonstrings with key catching.

# Installation

<pre>
sudo apt-get install g++ cmake libboost-all-dev
wget http://archive.cloudera.com/cdh5/one-click-install/trusty/amd64/cdh5-repository_1.0_all.deb
sudo dpkg -i cdh5-repository_1.0_all.deb
sudo apt-get update
sudo apt-get install impala-udf-dev
</pre>

# Build library
<pre>
cmake .
make
</pre>

# Create Function

Copy library-file (*.so) from build-folder to hdfs. 
Use place like /user/hdfs/udfs

In terminal type
<pre>
impala-shell -q "create function if not exists parse_json(string, string) returns string location '/user/hdfs/libparsejson.so' SYMBOL='ParseJSON';"
</pre>

# Usage
The created function can be used in impalaSQL with
<pre>
SELECT parse_json(COLUMN, "key") as SOMETHING from TABLE;
</pre>