#!/bin/bash

echo "Deleting data and database generated source from tests"

rm -rf ./basic/data ./basic/database/ ./advanced/data ./advanced/database/ ./multi_stage_base/data ./multi_stage_base/database/
