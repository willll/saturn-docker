#!/bin/bash
set -e

declare -a versions=("8.4.0"
                      "9.5.0"
                      "10.4.0"
                      "12.2.0"
                      "12.3.0"
                      "13.2.0"
                      "13.3.0"
                      "14.1.0"
                      )

for version in "${versions[@]}"
do
    echo "Building : $version"
   ./build.sh $version
done

exit 0
