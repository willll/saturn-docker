#!/bin/bash
set -e

declare -a versions=("8.4.0"
                      "9.5.0"
                      "10.4.0"
                      "11.5.0"
                      "12.4.0"
                      "13.3.0"
                      "14.2.0"
                      )

for version in "${versions[@]}"
do
    echo "Building : $version"
   ./build.sh $version
done

exit 0
