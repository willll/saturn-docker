docker build -t saturn-docker . --file ./Dockerfile

docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash

https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn
