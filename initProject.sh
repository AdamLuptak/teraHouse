#!/bin/bash

echo "Reinit clion arduino project"

platformio init --board=megaatmega2560 --ide=clion

strReplace="file(GLOB_RECURSE libs
        \"*.h\"
        \"*.cpp\"
        )
add_executable(\${PROJECT_NAME} \${SRC_LIST} \${libs})"

strFind="add_executable(\${PROJECT_NAME} \${SRC_LIST})"

replace "$strFind"  "$strReplace" --  CMakeLists.txt

