# Using alongside with Visual Studio

## Configuration

### List of required extensions

+ C/C++ by Microsoft
+ C/C++ Extension Pack by Microsoft
+ C/C++ Themes by Microsoft
+ Remote - SSH by Microsoft (optional)
+ ...

### Container Configuration, the easiest way

Pre-build images are available from docker.hub (https://hub.docker.com/r/willll/saturn-docker/tags

+ docker pull willll/saturn-docker:gcc_13.3.0
+ docker run -it -p 2222:22 --rm -v $(pwd):/saturn saturn-docker:gcc_13.3 /bin/bash
+ Note : $(pwd) being the local folder shared between the docker and your host computer.

### Container Configuration, the easy way

Portainer.io (https://www.portainer.io/) can be used to pull the content of github (https://docs.portainer.io/user/docker/stacks/add) :

+ Repository URL
 :https://github.com/willll/saturn-docker/
+ Repository reference : refs/heads/main
+ Compose path : compose.yaml
+ Use portainer.env (https://github.com/willll/saturn-docker/blob/main/portainer.env) to fine tune the variables

Once done, save, pull & redepoy. A new container shall be added and running once the image is built.

### Container Configuration, the hard way

Build you own image from github :

+ clone the Repository : git clone https://github.com/willll/saturn-docker.git
+ build the image : cd saturn-docker && docker build --build-arg GCC_VERSION_ARG=14.1.0 -t saturn-docker . --file ./Dockerfile
+ run it : docker run -it -p 2222:22 --rm -v /tmp/shared:/saturn saturn-docker /bin/bash
+ Note : /tmp/shared being the local folder shared between the docker and your host computer.

### Share your code between docker and Visual Studio

That does not apply if you are running docker on your computer, but for people using remote

![Hello World](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEYAAAAUCAAAAAAVAxSkAAABrUlEQVQ4y+3TPUvDQBgH8OdDOGa+oUMgk2MpdHIIgpSUiqC0OKirgxYX8QVFRQRpBRF8KShqLbgIYkUEteCgFVuqUEVxEIkvJFhae3m8S2KbSkcFBw9yHP88+eXucgH8kQZ/jSm4VDaIy9RKCpKac9NKgU4uEJNwhHhK3qvPBVO8rxRWmFXPF+NSM1KVMbwriAMwhDgVcrxeMZm85GR0PhvGJAAmyozJsbsxgNEir4iEjIK0SYqGd8sOR3rJAGN2BCEkOxhxMhpd8Mk0CXtZacxi1hr20mI/rzgnxayoidevcGuHXTC/q6QuYSMt1jC+gBIiMg12v2vb5NlklChiWnhmFZpwvxDGzuUzV8kOg+N8UUvNBp64vy9q3UN7gDXhwWLY2nMC3zRDibfsY7wjEkY79CdMZhrxSqqzxf4ZRPXwzWJirMicDa5KwiPeARygHXKNMQHEy3rMopDR20XNZGbJzUtrwDC/KshlLDWyqdmhxZzCsdYmf2fWZPoxCEDyfIvdtNQH0PRkH6Q51g8rFO3Qzxh2LbItcDCOpmuOsV7ntNaERe3v/lP/zO8yn4N+yNPrekmPAAAAAElFTkSuQmCC)
