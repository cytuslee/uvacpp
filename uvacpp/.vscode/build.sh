#!/usr/bin/env bash
cd $(dirname $0)
./cgal_create_CMakeLists
make
echo "done"

