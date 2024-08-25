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

That does not apply if you are running docker on your computer, but for people using remote servers.

+ Share using sshfs : sudo sshfs -o allow_other root@192.168.1.161:/saturn /tmp/saturn/ -p 2222
+ Note : This command will map the directory /saturn hosted on the SSH server 192.168.1.161, using the port 2222, to the local folder /tmp/saturn/
+ Note2 : Default SSH credentials for SSH are root/root

### Visual Studio Code Configuration

Lets try to build https://github.com/willll/saturn_helloworld

+ clone the repository, either from VSCode (https://code.visualstudio.com/docs/sourcecontrol/intro-to-git) or CLI, make sure to clone it into the folder shared with the docker images
+ If you are using SSH, you need to configure it (https://code.visualstudio.com/docs/remote/ssh) before going any further :
  - Install the Remote-SSH extension (https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh)
  - Install Remote Explorer extention (https://marketplace.visualstudio.com/items?itemName=ms-vscode.remote-explorer)
  - In Remote Explorer add a SSH connection ![Remote Explorer](Images/VSCode-Remote_Server.png)
  - Enter the connection string you would normally used to connect to your server, for example : ssh root@192.168.1.161 -p 2222
  - It should update your configuration file such as : ![SSH Config](Images/VSCode-SSH_Configuration.png)
  - You will be prompted for the SSH user account password (eventually),
  - Once connected it should look like this : ![SSH Config](Images/VSCode-Remote_Server_Connected.png)
+ 
