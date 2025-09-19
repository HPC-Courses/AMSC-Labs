# Environment setup
For the laboratories of the courses of
- Advanced Methods for Scientific Computing (prof. Luca Formaggia),
- Numerical Linear Algebra (prof. Paola F. Antonietti),
- Numerical Methods for Partial Differential Equations (prof. Alfio Quarteroni).

You will need two things: a text editor and a unix/linux-based development environment, equipped with a set of scientific libraries used in the laboratories. This document will guide you through the installation of those libraries.

## Table of contents

You should read the section corresponding to your OS, and then jump to the last section to test if the installation was successful.

- [1. Windows users](#1-windows-users)
- [2. Linux users](#2-linux-users)
  - [2.1. Install apptainer](#21-install-apptainer)
  - [2.2. Pull the container image](#22-pull-the-container-image)
  - [2.3. Using the container](#23-using-the-container)
  - [2.4. Using the libraries inside the container](#24-using-the-libraries-inside-the-container)
- [3. MacOS users](#3-macos-users)
  - [3.1. Install Docker](#31-install-docker)
  - [3.2. Run the Docker container for the first time](#32-run-the-docker-container-for-the-first-time)
  - [3.3. Day-to-day usage of the Docker container](#33-day-to-day-usage-of-the-docker-container)
  - [3.4. Troubleshooting Docker issues](#34-troubleshooting-docker-issues)
- [4. Test the installation](#4-test-the-installation)

## 1. Windows users

If you are using Windows, you need to install the [Windows Subsystem for Linux (WSL)](https://learn.microsoft.com/en-us/windows/wsl/install). For the Linux distribution, we recommend Ubuntu 24.04 LTS.

WSL provides a Linux environment directly on Windows. After installing it, go to the section for [Linux users](#2-linux-users) and follow the instructions there.

If you use WSL, we recommend to always work inside the Linux filesystem, as it will also be slightly more efficient. There exist plugins for major editors for compatibility with WSL (see e.g. [here](https://code.visualstudio.com/docs/remote/wsl) for Visual Studio Code).

From the WSL terminal you can access your folders by changing your working directory to `/mnt/c`. For instance, if you want to access to the directory `C:\Users\Name\Documents` in your WSL terminal you can execute `cd /mnt/c/Users/Name/Documents/`.

## 2. Linux users

### 2.1. Install apptainer

The libraries are provided through a container. We recommend using [apptainer](https://apptainer.org/) to install it. Therefore, as a first step you need to install apptainer.

On Ubuntu, you can use the following commands:
```bash
sudo apt update
sudo apt install -y software-properties-common
sudo add-apt-repository -y ppa:apptainer/ppa
sudo apt update
sudo apt install -y apptainer
```
On other distributions, you can follow the instructions on the [official guide](https://apptainer.org/docs/admin/main/installation.html).

### 2.2. Pull the container image

To pull the container, use the following commands:
```bash
mkdir -p $HOME/apptainer-tmp/
mkdir -p $HOME/apptainer-cache/
export APPTAINER_TMPDIR=$HOME/apptainer-tmp/
export APPTAINER_CACHEDIR=$HOME/apptainer-cache/
apptainer pull docker://quay.io/pjbaioni/amsc_mk:2025
```
Beware that the last command may take a while, and will download around 2.5GB of data.

### 2.3. Using the container

To start a shell inside the container, use the command
```bash
apptainer shell /path/to/amsc_mk_2025.sif
```
where you must replace `/path/to/` with the actual path where the file `amsc_mk_2025.sif` has been downloaded (it will be the folder where you executed `apptainer pull`).

Next time, you just have to run `apptainer shell` on the same file (you don't need to download apptainer or the container image again).

### 2.4. Using the libraries inside the container

The container already has many of the libraries installed in system paths (e.g. Eigen). By default, you will find several environment variables with prefix `mk` (e.g. `mkEigenInc`) which point to the paths where those libraries are found.

Additionally, it provides the [mk modules](https://github.com/pcafrica/mk), which bundle a set of scientific libraries compiled under the same toolchain. 

To enable `mk` modules, you should type each time you open the terminal `source /u/sw/etc/bash.bashrc`. 

Once enabled, they provide the command module, that has several subcommands:
```bash
module load gcc-glibc
```
loads the toolchain, making other modules available, and
```bash
module load <module name> 
```
loads the requested module. This creates or overrides a set of environment variables storing relevant paths for that library (e.g. `mkEigenPrefix`, `mkEigenInc`, ...). Use
- `env | grep mk`  to obtain a list of all the environment variables relative to mk modules
- `module list`: to show a list of currently loaded modules
- `module avail`: to show a list of all available modules (loaded or not)
- `module purge`: to unload all loaded modules
- `module --help`: to show a list of all the commands

## 3. MacOS users

### 3.1. Install Docker

As a first step, install the [Docker container environment](https://www.docker.com/). You can follow the instruction on the [official guide](https://docs.docker.com/get-docker/). **Please read it thoroughly**.

`Docker` is relatively heavy on RAM, and it might help to go to `Settings` and enabling the *Virtualization framework* under *Experimental features*.

From a terminal, run the command
```bash
docker pull quay.io/pjbaioni/amsc_mk:2025
```
Verify that the image has been downloaded with
```bash
docker images
```

### 3.2. Run the Docker container for the first time

To start a shell inside the container, use the command
```bash
docker run -it --name amsc -v /path/to/host/shared-folder:/path/to/container/shared-folder quay.io/pjbaioni/amsc_mk:2025 /bin/bash
```
where you must replace both `/path/to/container/shared-folder` with the actual path of a folder on your MacOS machine that you want to share with the Docker container (e.g. `/home/your-username/amsc_mk_25-shared-folder`), and `/path/to/container/shared-folder` with the path where you want the folder to be mounted inside the container, e.g. `/home/ubuntu/shared-folder`. 

**WARNING:** Could be that recent versions of Mac have the following issue `WARNING: The requested image's platform (linux/amd64) does not match the detected host platform (linux/arm64/v8) and no specific platform was requested`. To solve this problem substitute the command above with
```bash
docker run --platform linux/amd64 -it --name amsc -v /path/to/host/shared-folder:/path/to/container/shared-folder quay.io/pjbaioni/amsc_mk:2025 /bin/bash
```

**WARNING:** to avoid problems paths should not contain white spaces or special characters. 

**WARNING:** use global paths starting with `/`, avoid using `~`.

### 3.3. Day-to-day usage of the Docker container

To start the container, run
```bash
docker start amsc
```
To open a shell into the container run:
```bash
docker exec -it amsc /bin/bash
```
You can leave the container and return to your OS with `exit`. You can check your containers and their status with the command
```bash
docker ps -las
```
If the status of the container is `UP`, you can stop it with
```bash
docker stop amsc
```
Once you have created your container remember **not** to use again the command `run` but just `start`. Otherwise you will create every time a new container. If you want to remove a container you created by mistake you can run:
```bash
docker rm <name-of-the-container>
```
you can also remove the image with
```bash
docker rmi <image-id>
```
You can refer to
```bash
docker --help
```
for a brief documentation.

### 3.4. Troubleshooting Docker issues
When running MPI on Docker, you may experience errors like the following:
```
Read -1, expected <some_number>, errno = 1 error
```
This is a known issue (see, e.g., [here](https://github.com/open-mpi/ompi/issues/4948)), that can be solved by typing the following command on the terminal:
```bash
export OMPI_MCA_btl_vader_single_copy_mechanism=none
```
```bash
nano $HOME/.bashrc
```
then go to the end of the file and paste the following commands,
```bash
export OMPI_MCA_btl_vader_single_copy_mechanism=none
```
Then, 
```bash
source $HOME/.bashrc
```
to reload.

## 4. Test the installation

1. Using a text editor, create a file `test-installation.cpp` with content:

```cpp
#include <Eigen/Eigen>
#include <iostream>

int main(int argc, char** argv)
{
        std::cout << "Successfully included Eigen." << std::endl;
        return 0;
}
```

2. If you are not already there, from inside the terminal move to the folder containing the file `test-installation.cpp`. Here some useful commands:
  * `cd` to change directory, E.g. `cd shared-folder`
  * `ls` to list the content of the current working directory
  * `pwd` to show the current working directory

3. Compile and run the test. Once in the correct folder execute the two following commands:
```bash
g++ -I ${mkEigenInc} test-installation.cpp -o test-installation
./test-installation
```
## 5. Extras

You can personalize the shell with, for example
```bash
export PS1='\[$(tput bold)\]\[$(tput setaf 1)\]\u@\h\[$(tput setaf 4)\] \W \$\[$(tput sgr0)\] '
```
If you want to know how the container image was built, you can have a look at the [Dockerfile](https://github.com/HPC-Courses/AMSC-Labs/tree/main/Labs/2025-26/00-environment_setup/extras/Dockerfile)
