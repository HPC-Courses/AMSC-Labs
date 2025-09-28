---
marp: true
#size: 4:3
style: |
    img[alt~="center"] {
      display: block;
      margin: 0 auto;
    }
---

# **Laboratory 01**
## Scientific computing tools in HPC  

### Paolo Joseph Baioni

### 26/09/2025

---
# Outline
- [1. Linux OS hands-on](#1-linux-os-hands-on)
  - [1.1 Introduction](#11-introduction)
  - [1.2 The Shells](#12-shells)
  - [1.3 Text Editing](#13-text-editing)
  - [1.4 SSH and authentication](#14-ssh-and-authentication)
- [2. Compilation and automatic compilation tools](#2-compilation-and-automatic-compilation-tools)
  - [2.1 g++ and clang++](#21-g++-and-clang++)
  - [2.2 GNU Make](#22-gnu-make)
  - [2.3 CMake](#23-cmake)

---

- [3. Version management](#3-version-management)
  - [3.1 git](#31-git)
  - [3.2 GitHub](#32-github)
- [4. Containerisation in HPC](#4-containerisation-in-hpc)
  - [4.1 Apptainer](#41-apptainer)
  - [4.2 Podman](#42-podman)
  - [4.3 HPCCM and Python Envs](#43-hpccm-and-python-envs)
- [5. Modules and package managers](#5-modules-and-package-managers)
  - [5.1 mk modules](#51-mk-modules)
  - [5.2 spack (readings)](#52-spack-readings)

---
# 1. Linux OS hands-on


---
## 1.1 Introduction
All leading HPC systems run Linux-based operating systems — see [https://www.top500.org/lists/top500/2025/06/](https://www.top500.org/lists/top500/2025/06/)

We will also use Linux-based environments, adopting a learn‑by‑doing approach. Many references are available at every level; for a beginner introduction, see e.g. [linux essentials book](https://www.bibliosearch.polimi.it/permalink/39PMI_INST/1nvvje2/alma9956393108776), that can be read online at PoliMi library website.

---
## 1.2 Shells

It is of utmost importance to learn the basics of `bash`, as is the most widely used and de facto standard shell on HPC systems, and it's commonly available on compute/login nodes and in user environments.

Provided bash is known, others shells may be used as well.

Some essentials commands follow; for a first further reading (interactive, login, regex...) you can check also
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md).

---
### Essential commands
  - `echo` displays a string or a variable.
  - `pwd` stands for **Print working directory**. The command displays the current path of the shell.
  - `ls` stands for a **List** and it lists the content of a directory.
  - `cd` stands for **Change directory** and changes the active directory to the path specified.
#### Example:
- `cd ..` change path to the above directory.
- `ls .` list files in the current directory.

---
### Essential commands
- `mkdir` stands for **Make directory** and it is used to make a new directory or a folder.
- `mv` stands for **Move** and it moves one or more files or directories from one place to another. It is also used to *rename* files.
- `touch` command is used to create new, empty files. It is also used to change the timestamps on existing files and directories.
- `rm` stands for **Remove** and it removes files. 
- `export` is employed to export variables to child processes.

#### Example
- `rm -r myDir` removes the directory `myDir`.
- `mv temp.txt tmp.txt` renames a file.

---
### Essential commands
- `cat` stands for **Concatenate** and it reads a file and outputs its content. It can read any number of files, and hence the name concatenate.
- `grep` stands for **Global regular expression print**. It searches for lines with a given string or looks for a pattern in a given input stream.
- `head` shows the first lines of a file.
- `tail` shows the last lines of a file.
- `file` reads the files specified and performs a series of tests in attempt to classify them by type.

---
### Run a script
To run a script you need to change the access permissions of the file. To make a file executable run:

```bash
chmod +x script_file
```
(see also `chown` and `stat`).

Finally, remember that the **first line of the script** tells the shell which interpreter to use while executing the file. For example, if your script starts with `#!/bin/bash` it will be run by `Bash`, if is starts with `#!/usr/bin/env python` it will be run by `python`.

---
### Redirection and Pipes
We can add operators between commands in order to chain them together.
- The pipe operator `|`, forwards the output of one command to another. E.g. `cat /etc/passwd | grep user` checks system information about `user`.
- The redirect operator `>` sends the standard output of one command to a file.
- The append operator `>>` appends the output of one command to a file.
- The operator `&>` sends the standard output and the standard error to file.
- `&&`  pipe is activated only if the return status of the first command is 0.
- `||` pipe is activated only if the return status of the first command is different from 0.

---
### Advanced commands: sed, find
```bash
sed -i 's/<target-string>/<new-string>/' filename
```

You can use regex to comment some lines:
```bash
sed -i '5,7s/^/\/\//' hello_world.cpp
sed -i '5,7s/^\/\///' hello_world.cpp 
```
You can look for a file, discarding errors
```bash
find ~ -iname "*tools*md" 2>/dev/null
```

---
### Processes, memory
You can monitor resources with `top` for processes (`q` for quit), and compare with information from `lscpu` (or `cat /proc/cpuinfo`).

Similarly, `free -h` for memory. You can also use `watch -n 1 free -h` (`ctrl+c` to terminate).

For checking occupied and available space check also
```bash
df -h .
du -d -1 -h .
```

---
## 1.3 Text editing
Any text editor can be used in the course; a very widespread one is `vscode` (also a free/libre version is available: [https://github.com/VSCodium/vscodium](https://github.com/VSCodium/vscodium)).
### Do not confuse text editor with IDE! 
Feel free to use the editor you prefer; mind that GUI editor can't be directly called by the container as it is.

---
### A simple CLI editor: nano
For the purposes of the course, is essential that you can use a CLI editor.
`vim` (exit with `Esc` and `:q!`), `emacs` (exit with `ctrl+x` and `ctrl+c`) and `nano` are already available in the course container; feel free to choose the one you prefer.
`nano` is definitely the simpler, and its commands are summarised in the editor window. You can better the UX with a few options, such as
```bash
nano -liST 2 filename.cpp
```
see `nano --help` or `man nano` for more details.

---
## 1.4 SSH and authentication
SSH (Secure Shell) is a protocol that provides encrypted remote login and secure file transfer on Linux, using public‑key and/or password authentication to protect confidentiality and integrity.
Generate with:
```bash 
ssh-keygen -t ed25519
```
as we will use it later.
You will find:
```bash
$ ls ~/.ssh
id_ed25519  id_ed25519.pub
```
extra: see also the permissions, with `ls -l` or `stat`.

---
# 2. Compilation and automatic compilation tools

---
## 2.1 g++ and clang++
Basic compile/link flags:
```bash
g++ -I /path/to/non-std/include -c main.cpp
g++ -L /path/to/non-std/lib -llibrary_name main.o -o main
```
A few useful options: `-Wall -g -O0` when debugging (optionally, `-ggdb` with `gdb`), `-O3` in production.

---
### Many files
One-line, recompile all:
```bash
g++ -I . hello.cpp main.cpp 
```
execute with `./a.out`.

Separate compilation and linking stages:
```bash
g++ -c -I. hello.cpp
g++ -c -I. main.cpp
g++ hello.o main.o
```
---
## 2.2 GNU Make
GNU Make automates compilation and building. We write our own `Makefiles` during the next lab sessions; in the meantime, the more common user commands are
```bash
make
make all
make clean
make distclean
```
For insights on Makefiles writing, as well as on compiling, especially with mk modules, you can see 
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/02-compile/doc/](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/02-compile/doc/)

---
## 2.3 CMake
Makefiles writing and maintenance becomes a complex labour too; CMake is a tool to automate Makefiles creation.
The basic usage is, starting from the root directory where the `CMakeLists.txt` is, 
```bash
mkdir build
cd build
cmake ..
make
```
We'll use CMake in a next lab; in the meantime, when needed you can refer to
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/14-cmake/](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/14-cmake/)

---
# 3. Version management

---
## 3.1 git
From `man git`, *Git is a fast, scalable, distributed revision control system with an unusually rich command set that provides both high-level operations and full access to internals.*
The must-have commands are:
```bash
git clone https://github.com/user/repo.git #https protocol
git clone git@github.com:user/repo.git #ssh protol
git pull
git log
git status <path-to-file>
git diff <path-to-file>
git add path-to-file(s)
git commit -m "commit title"
git push
```

---
## 3.2 GitHub
You can use GitHub to:
 - add an ssh key (or more), to ease auth. 
 - create a new repository
 - fork existent ones, create pull requests (more advanced)
Try to do the first two in class
You can specify your name, mail, default editor... both locally and globally. For the latter, eg
```bash
git config --global core.editor "nano -ilST 2"
```
or manually edit `~/.gitconfig`

---
# 4. Containerisation in HPC
 - podman: OCI (as Docker) compatible, no sudo, sees the shared folder only
 - apptainer: HPC oriented, derived from singularity, no sudo, uses FUSE to manage container file systems
 - alternatives exist: docker (creation) + sarus (execution) [https://github.com/eth-cscs/sarus](https://github.com/eth-cscs/sarus)


---
## 4.1 Podman
### Hello world
See hello-world.dockerfile
```
podman build -t hello-world:v1 -f hello-world.dockerfile .
podman run --rm -it hello-world:v1 /usr/local/bin/hello
```

---
## Podman hello world - multistage
See hello-world-multistage.dockerfile
```
podman build -t hello-world:v2 -f hello-world-multistage.dockerfile .
podman run --rm -it hello-world:v2 /usr/local/bin/hello
podman images
```

---
## 4.2 Apptainer
(see files in `examples` dir)
```bash
apptainer build ada.sif ada.apptainer_recipe
apptainer exec ada.sif gnatmake hello.adb && \
apptainer exec ada.sif gnatbind -x hello.ali && \
apptainer exec ada.sif gnatlink hello.ali && \
apptainer exec ada.sif ./hello
```
---
## Repositories & Apptainer
You can upload the container image built with podman to repositories such as [https://quay.io/repository/](https://quay.io/repository/) (free registration required to upload, and to follow the [https://quay.io/tutorial/](https://quay.io/tutorial/) ).

Then, you can build apptainer containers from podman images, eg
```bash
apptainer pull docker://quay.io/pjbaioni/amsc_mk:2025
```

---
## 4.3 HPCCM and Python Envs
High-Performace Computing Container Maker provides a python based higher level interface to container definitions, already following best practices for docker (default, podman compatible) and singularity (apptainer compatible)
It can be installed in the **recommended** way to install python packages 
```bash
python3 -m venv ~/hpccm
source ~/hpccm/bin/activate
pip install --upgrade pip
pip install hpccm
deactivate
```

---
## Example usage
```bash
source ~/hpccm/bin/activate
hpccm --recipe compilers.hpccm --format singularity > compilers.apptainer
hpccm --recipe compilers.hpccm --format docker > compilers.podman
deactivate
apptainer build compilers.sif compilers.apptainer
apptainer shell compilers.sif
```

---
# 5. Modules & package managers

---
## 5.1 mk modules
Let's try live the Lab 0 test, comparing
```bash
which $mkEigenInc
source /u/sw/etc/bash.bashrc
module load gcc-glibc
module load eigen
echo $mkEigenInc
```


---
## 5.2 spack (readings)
Spack is a multi-platform package manager that builds and installs multiple versions and configurations of software. Spack is non-destructive: installing a new version of a package does not break existing installations, so many configurations of the same package can coexist.

Spack offers a simple "spec" syntax that allows users to specify versions and configuration options. Package files are written in pure Python, and specs allow package authors to write a single script for many different builds of the same package. 

---
## Installing

Clone the repository
```
git clone -c feature.manyFiles=true --depth=1 --branch \
releases/v0.23 https://github.com/spack/spack spack-0.23
```

---
## First steps

setup the environment
```
source spack-0.23/share/spack/setup-env.sh
```

list available packages (slow first time)
```
spack list
```

optional: tune configuration
```
find spack-0.23 -iname "config.yaml"
nano -liST 2 spack-0.23/etc/spack/defaults/config.yaml 
```
(eg, edit spack-stage, stage, test and cache dirs; see spack.diff)

---
## Basic usage

```
spack info gcc
spack install gcc@14.2.0
which gcc
spack load gcc
which gcc 
spack unload --all
```

---
## Specs (optional)

Default ones
```
spack spec -ll gcc
```
Setting specs
```
$ spack install mpileaks                           :  unconstrained
$ spack install mpileaks@3.3                       @: custom version
$ spack install mpileaks@3.3 %gcc@4.7.3            %: custom compiler
$ spack install mpileaks@3.3 %gcc@4.7.3 +threads   +/- build option
$ spack install mpileaks@3.3 cppflags="-O3 –g3"        set compiler flags
$ spack install mpileaks@3.3 target=cascadelakeset     target microarchitecture
$ spack install mpileaks@3.3 ^mpich@3.2 %gcc@4.9.3 ^: dependency constraints
```

---
## Compilers & toolchains

Get compilers list: `spack compilers` or
```
spack config get compilers
```
equivalently
```
cat ~/.spack/linux/compilers.yaml 
```
After having installed and loaded a new compiler, update compilers list and check the result
```
spack load gcc@14.2.0
spack compiler find
spack config get compilers
```

---
## Compilers & toolchains
Check installed packages with `spack find`
Now you can use that compiler to build new packages, eg
```
spack install intel-oneapi-tbb%gcc@14.2.0
```
Check again installed packages with `spack find`

Thus, your installation is independent from OS compiler version and more portable and reproducible. 
Packages compiled with a specific compiler can be found with 
```spack find %<compiler>@version```

---
## Reading: build your own module system

Requires either
 - [https://github.com/envmodules/modules](https://github.com/envmodules/modules)
 - [https://github.com/TACC/Lmod](https://github.com/TACC/Lmod)   

Here we refer to `apt info environment-modules`, but they can be installed via spack too.
```
sudo apt install environment-modules
source /etc/profile.d/modules.sh 
module avail
source spack-0.23/share/spack/setup-env.sh
spack module tcl refresh -y 
module avail
module load gcc/14.2.0<...> && module load intel-oneapi-tbb/
module list
```
