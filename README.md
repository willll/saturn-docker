docker build -t saturn-docker . --file ./Dockerfile

docker image ls
REPOSITORY              TAG                 IMAGE ID       CREATED         SIZE
saturn-docker           latest              7d58ba05cd08   3 hours ago     2.67GB

docker run -it --rm -v $(pwd):/saturn saturn-docker /bin/bash

https://github.com/RetroReversing/retroReversing/tree/master/pages/sega/saturn
