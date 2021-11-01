# saturn-docker
docker for  SEGA  Saturn development

WIP :

Build it :

docker build -t saturn-docker . --file ./Dockerfile

run it :

docker run -it --rm -v $(pwd):/saturn saturn-docker:latest /bin/bash

