#! /bin/bash 
mkdir -p $1/bin/x64
cp -r $1/../nsengine/tests/import $1/bin/x64
cp -r $1/../nsengine/tests/plugins $1/bin/x64
cp -r $1/../nsengine/tests/resources $1/bin/x64
