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

1\)![png tester1][pngtester1] 2\)![png tester2](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAHoAAABkCAYAAABJhSQPAAAACXBIWXMAAAsTAAALEwEAmpwYAAADf0lEQVR42u3dW2vTYBzH8eeUPDm0adN2adeddNMpo2ObXoypsDvFd+WbEfRSUUHvxIFOEXG7UEFR5xybulO3tU3XpF4JIiJ43Pw/v+8LKP3nQ54nIaTlC2fOXGKIfAKHANAI0AjQCNAI0AjQCNAI0AjQgEaARoBGgEaARoBGgEaARoBGgAY0AjQCNAI0AjQCNAI0AjQCNKARoBGgEaARoNE/T+EQHL4SwXhsCbnrKWvHU3bdV3rHV3rPlXrPkbqppY5tYXUkVx3JZSo4Z4wxkXa7KukmKul2dDvdd+Mk9ltJ7DeTGNAHXFML+Slnu6slnVkpOfm1og5bttC/8lmp4LwtuGhbzGo40t1kFs7ogyjljNV9ZS9V3OB11Su97XUrWLqJFFtcLEdu9vmRTPSq3+vDHk2oli3k66qXWzie7V8r6AIuxogty+/KbvbxydzActmJcNVNrIYW6uloED0ay4/i9opg64GlH4yHgwe57wL6L/YhtN17k4Xh95HT8z99b0D/xBl891Rx5DDuv4D+AzW1kHMThaFnRzOD//McgP5BT0aD6N5UYYzCLID+Th/ztnPzXFSr+ypDZSZAf3MvPF/LVw/7rRKgf6NtX9nXZsvjW1krS3E+QDPGXgz64e2ZngnKMxoPfXeqMPh0NBimPqex0G3FxfXZythKSZdMmNdI6B1XWlcu9J1uauGYMrNx0OuBpS9f7JsxbW6joD+EtnvlfHXaxFVMABnQpJZrk5GNgN51pDJxTzYKuiM5v3q+epoh2tA3zkUn91zpgpkw9P3xfHWp4pZBTBj6bcXNUnwCBeivatlCXpstY1+mDn1nuucYWIlDv+z3cm+qbi9YCUO3FRe3zkZTICUOPV8L+8BJHLruKevJiWAEnMSh5ybDI6AkDr2VUfbLAR/LNnXo+Vo4AEbi0E0t5IshH9DUoRdHggiEBkA/rOWPg5A49GpBeynHD+KRh148lsUjSOrQKWfs2dHMEPiIQ28ElgM6A6Df9Ho50BkA/arfw20VdeiUM7ZW1EXQEYduaIl3uk2A3sjhQswI6PWc7YHNAOjNwAK0CdBbGUAbAb3r4RUbI6BbWtpgMwC6rbgFNgOgv/z1DyIOLdJuF2wGQNud7j7YDIB24qQNNgOgM42kCTYDoPO7+w2wGQAd1gFtBHRxuw1oE6AL2/stsBkA7cVJB2w/32c7r8DNq/e3jAAAAABJRU5ErkJggg==)

_1\) the referenced base64 block stops being parsed right at the point where 'a new chunk of image' starts, the green part `... So4Z4wxkXa7KukmKul2dDvd ...`  
2\) by passing the block directly we get the whole image_

This was tested with http://tmpvar.com/markdown.html. I don't know how it behaves on github. [The issue](https://github.com/github/markup/issues/270) has been already well discussed.

[pngtester1]:data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAHoAAABkCAYAAABJhSQPAAAACXBIWXMAAAsTAAALEwEAmpwYAAADf0lEQVR42u3dW2vTYBzH8eeUPDm0adN2adeddNMpo2ObXoypsDvFd+WbEfRSUUHvxIFOEXG7UEFR5xybulO3tU3XpF4JIiJ43Pw/v+8LKP3nQ54nIaTlC2fOXGKIfAKHANAI0AjQCNAI0AjQCNAI0AjQgEaARoBGgEaARoBGgEaARoBGgAY0AjQCNAI0AjQCNAI0AjQCNKARoBGgEaARoNE/T+EQHL4SwXhsCbnrKWvHU3bdV3rHV3rPlXrPkbqppY5tYXUkVx3JZSo4Z4wxkXa7KukmKul2dDvdd+Mk9ltJ7DeTGNAHXFML+Slnu6slnVkpOfm1og5bttC/8lmp4LwtuGhbzGo40t1kFs7ogyjljNV9ZS9V3OB11Su97XUrWLqJFFtcLEdu9vmRTPSq3+vDHk2oli3k66qXWzie7V8r6AIuxogty+/KbvbxydzActmJcNVNrIYW6uloED0ay4/i9opg64GlH4yHgwe57wL6L/YhtN17k4Xh95HT8z99b0D/xBl891Rx5DDuv4D+AzW1kHMThaFnRzOD//McgP5BT0aD6N5UYYzCLID+Th/ztnPzXFSr+ypDZSZAf3MvPF/LVw/7rRKgf6NtX9nXZsvjW1krS3E+QDPGXgz64e2ZngnKMxoPfXeqMPh0NBimPqex0G3FxfXZythKSZdMmNdI6B1XWlcu9J1uauGYMrNx0OuBpS9f7JsxbW6joD+EtnvlfHXaxFVMABnQpJZrk5GNgN51pDJxTzYKuiM5v3q+epoh2tA3zkUn91zpgpkw9P3xfHWp4pZBTBj6bcXNUnwCBeivatlCXpstY1+mDn1nuucYWIlDv+z3cm+qbi9YCUO3FRe3zkZTICUOPV8L+8BJHLruKevJiWAEnMSh5ybDI6AkDr2VUfbLAR/LNnXo+Vo4AEbi0E0t5IshH9DUoRdHggiEBkA/rOWPg5A49GpBeynHD+KRh148lsUjSOrQKWfs2dHMEPiIQ28ElgM6A6Df9Ho50BkA/arfw20VdeiUM7ZW1EXQEYduaIl3uk2A3sjhQswI6PWc7YHNAOjNwAK0CdBbGUAbAb3r4RUbI6BbWtpgMwC6rbgFNgOgv/z1DyIOLdJuF2wGQNud7j7YDIB24qQNNgOgM42kCTYDoPO7+w2wGQAd1gFtBHRxuw1oE6AL2/stsBkA7cVJB2w/32c7r8DNq/e3jAAAAABJRU5ErkJggg==
