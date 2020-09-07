#!/usr/bin/env bash
cd $(dirname $0)
./cgal_create_CMakeLists
cmake -DCMAKE_BUILD_TYPE=Release .
make
echo "done"

