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

<!-- ### Vincenzo Di Florio -->

### 18/09/2026

---
# What we already did in Lab 0

Lab 0 (see [`00-environment_setup/`](../00-environment_setup/)) already covered:
- the **development environment** ([`Lab0_SetUp.md`](../00-environment_setup/Lab0_SetUp.md)): WSL / apptainer / Docker, the course container `quay.io/pjbaioni/amsc_mk:2025`, the shared folder, day-to-day usage (`start`, `exec`, `stop`);
- the **mk modules** basics: `source /u/sw/etc/bash.bashrc`, `module load gcc-glibc`, `module load <name>`, `env | grep mk`, `module list/avail/purge`;
- a first **compile & run**: `g++ -I ${mkEigenInc} test-installation.cpp -o test-installation`;
- an **introduction to Bash** ([`Lab0_IntroBash.md`](../00-environment_setup/Lab0_IntroBash.md)): shells and init files, essential commands, wildcards, redirection and pipes, filters (`wc`, `grep`, `cut`, `tr`, `sed`), `find`/`locate`, processes and jobs, regex, `man`/`--help`.

So in this lab we **do not repeat** those topics: we only check them, and we move on.
So in this lab we **do not repeat** those topics, and we move on: [`00-environment_setup/`](../00-environment_setup/) remains the reference for installation and troubleshooting, and for the Bash commands you will keep using every day.

---
# Outline
- [1. Completing the environment](#1-completing-the-environment)
  - [1.1 Text editing from the command line](#11-text-editing-from-the-command-line)
  - [1.2 Inspecting the machine](#12-inspecting-the-machine)
  - [1.3 SSH and authentication](#13-ssh-and-authentication)
- [2. Version management](#2-version-management)
  - [2.1 git](#21-git)
  - [2.2 GitHub](#22-github)
- [3. Compilation and automatic compilation tools](#3-compilation-and-automatic-compilation-tools)
  - [3.1 g++ and clang++](#31-g-and-clang)
  - [3.2 GNU Make](#32-gnu-make)
  - [3.3 CMake](#33-cmake)

---

- [4. Containerisation in HPC](#4-containerisation-in-hpc)
  - [4.1 Podman](#41-podman)
  - [4.2 Apptainer](#42-apptainer)
  - [4.3 HPCCM and Python envs](#43-hpccm-and-python-envs)
- [5. Modules and package managers](#5-modules-and-package-managers)
  - [5.1 mk modules](#51-mk-modules)
  - [5.2 spack](#52-spack-reading-only) <span style="color:red;">(reading only)</span>

The second part of the laboratory, on **Eigen**, is in [`Lab1b_IntroEigen.md`](./Lab1b_IntroEigen.md).

---
# 1. Completing the environment

All leading HPC systems run Linux-based operating systems — see [https://www.top500.org/lists/top500/2025/06/](https://www.top500.org/lists/top500/2025/06/) — and `bash` is the de facto standard shell there, which is why we started from it.

[Lab0_IntroBash](../00-environment_setup/Lab0_IntroBash.md) covered the shell itself. Three practical things were left out, and we will need all of them from now on: a text editor usable from the command line, the commands that tell you what hardware you are actually running on, and a pair of SSH keys.

For a further reading on the shell (interactive vs login shells, initialization files, regex, ...) see also
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/01-bash_modules_git/lab01.md).

---
## 1.1 Text editing from the command line

Any text editor can be used in the course. A very widespread one is `vscode` (a free/libre build is also available: [https://github.com/VSCodium/vscodium](https://github.com/VSCodium/vscodium)), and it is a good choice for writing your code, because it can open the shared folder of the container directly. Mind that a GUI editor cannot be called from inside the container as it is, and do not confuse a text editor with an IDE.

Sooner or later, though, you will work on a machine where the graphical editor is not available: on a login node reached through `ssh`, or inside a container. For the purposes of the course it is therefore essential that you are able to edit a file from the command line. `vim` (exit with `Esc` and then `:q!`), `emacs` (exit with `ctrl+x` `ctrl+c`) and `nano` are all already installed in the course container, so you can choose the one you prefer.

`nano` is definitely the simplest one, since its commands are listed at the bottom of the editor window. Its default settings are however quite uncomfortable for writing code, and a few options improve the experience considerably: line numbers, autoindent, and a sensible tab width.
```bash
nano -liST 2 filename.cpp
```
See `nano --help` or `man nano` for the meaning of each of them, and for the many others.

---
## 1.2 Inspecting the machine

In scientific computing you always want to know what hardware you are actually using, and how much of it you are consuming: the number of cores you can see is the upper bound of the speed-up you will be able to measure in the parallel laboratories, and the available memory is what decides the largest problem you can solve.

The static description of the processor is given by `lscpu` (or, in more detail, by `cat /proc/cpuinfo`), while `top` shows the running processes and the load they produce (`q` to quit). For the memory, `free -h` prints the amount used and available in human readable form, and it becomes much more informative if you keep it under observation while a program of yours is running:
```bash
watch -n 1 free -h
```
(`ctrl+c` to terminate). Disk occupation is reported by `df -h .` for the filesystem the current directory belongs to, and by `du -d 1 -h .` for the directories inside it.

Run `lscpu` and `free -h` inside the container now, and compare the numbers with the ones of your laptop: the container does not necessarily see all your cores nor all your memory, and that is a limit you should be aware of before drawing conclusions from a timing.

---
## 1.3 SSH and authentication

SSH (Secure Shell) is a protocol that provides encrypted remote login and secure file transfer on Linux, using public-key and/or password authentication to protect confidentiality and integrity. Public-key authentication relies on a pair of files: a private key, which stays on your machine and is never shared, and the corresponding public key, which you upload to the machines and the services you want to access.

Generate your pair with
```bash
ssh-keygen -t ed25519
```
accepting the default path, and you will find the two files in `~/.ssh`:
```bash
$ ls ~/.ssh
id_ed25519  id_ed25519.pub
```
`id_ed25519` is the private one, `id_ed25519.pub` the public one, and it is the latter that you will upload to GitHub in [Section 2.2](#22-github). As an extra, look at the permissions of the two files with `ls -l ~/.ssh` (or `stat`), and keep in mind that `ssh` deliberately refuses to use a private key that is readable by other users.

---
# 2. Version management

---
## 2.1 git
From `man git`, *Git is a fast, scalable, distributed revision control system with an unusually rich command set that provides both high-level operations and full access to internals.*
The must-have commands are:
```bash
git clone https://github.com/user/repo.git #https protocol
git clone git@github.com:user/repo.git #ssh protocol
git pull
git log
git status <path-to-file>
git diff <path-to-file>
git add <path-to-file(s)>
git commit -m "commit title"
git push
```

---
## 2.2 GitHub
You can use GitHub to:
- add an ssh key (or more), to ease authentication — use the `id_ed25519.pub` generated in [Section 1.3](#13-ssh-and-authentication);
- create a new repository;
- fork existing ones, create pull requests (more advanced).

Try the first two in class.
You can specify your name, mail, default editor... both locally and globally. For the latter, e.g.
```bash
git config --global core.editor "nano -ilST 2"
```
or edit `~/.gitconfig` manually.

---
### Exercise 1: clone the repository of the laboratories
The material of the laboratories is published on GitHub, at [`HPC-Courses/AMSC-Labs`](https://github.com/HPC-Courses/AMSC-Labs). Clone it inside the shared folder of the container: it contains the sources we compile in the rest of this lab.
```bash
cd ~/shared-folder
git clone https://github.com/HPC-Courses/AMSC-Labs.git
cd AMSC-Labs/Labs/2026-27/01-scientific_computing_tools
ls
```
Before each laboratory, run `git pull` inside `AMSC-Labs` to get the new material. This repository is read only for you: you can pull from it, but not push to it.

---
### Exercise 2: create a repository of your own, to practice
To try the commands that write to a repository, you need one where you are allowed to push. Create it now:
1. on GitHub, add the public key `id_ed25519.pub` generated in [Section 1.3](#13-ssh-and-authentication) to your account (*Settings → SSH and GPG keys*);
2. create a new, empty repository, e.g. `git-test`;
3. clone it in the shared folder with the ssh protocol, add a file, and push it:
```bash
cd ~/shared-folder
git clone git@github.com:<yourname>/git-test.git
cd git-test
echo "Hello, git!" > README.md
git status
git add README.md
git commit -m "first commit"
git push
```
Check on GitHub that the file is there, then modify it and go again through `status`, `diff`, `add`, `commit`, `push` to get used to the cycle.

---
# 3. Compilation and automatic compilation tools

---
## 3.1 g++ and clang++
Now that the sources are on your machine, we can compile them. Move to the directory of this lab in the repository you have just cloned, where you find `hello_world.cpp`, the simplest possible C++ program: have a look at it with `cat`, then compile and run it.
```bash
cd ~/shared-folder/AMSC-Labs/Labs/2026-27/01-scientific_computing_tools
cat hello_world.cpp
g++ hello_world.cpp -o hello_world
./hello_world
```
Without `-o`, the executable is called `a.out`. `clang++` is a different compiler with the same command line interface: if it is installed (`which clang++`), try `clang++ hello_world.cpp -o hello_world` and check that nothing changes for you.

---
### What the compiler did
The single command above hides several stages, that you can run separately to see what each one produces:
```bash
g++ -E hello_world.cpp -o hello_world.ii   # preprocessing: look at the size of the file!
g++ -S hello_world.cpp -o hello_world.s    # compilation to assembly
g++ -c hello_world.cpp -o hello_world.o    # assembling to object code
g++ hello_world.o -o hello_world           # linking
```
Look at the four files with `ls -l`, and with `file`. The first stage is where the `#include` of `iostream` is resolved: `wc -l hello_world.ii` tells you how much of the standard library one line of `#include` brings in.

---
### Useful flags
Basic compile/link flags:
```bash
g++ -I /path/to/non-std/include -c main.cpp
g++ -L /path/to/non-std/lib -llibrary_name main.o -o main
```
A few useful options: `-Wall -g -O0` when debugging (optionally `-ggdb`, with `gdb`), `-O3` in production.

Note you already used `-I` in Lab 0, with `-I ${mkEigenInc}`: that is exactly a non-standard include path, provided by the `eigen` mk module.

---
### Many files
The same program, split in three files, is in the `function-hello` directory of the lab:
```
function-hello/
├── hello.h      declares the function:  std::ostream & hello(std::ostream &);
├── hello.cpp    defines it:             #include <hello.h>
└── main.cpp     calls it:               #include <hello.h>
```
Move there and try to compile it exactly as you did for `hello_world.cpp`:
```bash
cd ~/shared-folder/AMSC-Labs/Labs/2026-27/01-scientific_computing_tools/function-hello
g++ hello.cpp main.cpp
```
**Do it**: the compilation fails, with an error like
```
hello.cpp:1:10: fatal error: hello.h: No such file or directory
```
even though `hello.h` is right there, in the same directory. Why?

---
### Where the compiler looks for a header
The two forms of `#include` search in different places:
- `#include "hello.h"` looks **first in the directory of the file that includes it**, then in the include path;
- `#include <hello.h>` looks **only in the include path**: the system directories (e.g. `/usr/include`, where `iostream` lives) plus the directories you add with `-I`.

Our sources use the angle brackets, as one does for a library, so the current directory must be added to the include path explicitly. `-I .` does it, where `.` is the current directory; an absolute path works just as well:
```bash
g++ -I . hello.cpp main.cpp
g++ -I ~/shared-folder/AMSC-Labs/Labs/2026-27/01-scientific_computing_tools/function-hello hello.cpp main.cpp
```
**Try both**, then execute with `./a.out`. You can also see the search path the compiler actually uses with `g++ -v -E -x c++ /dev/null 2>&1 | grep -A 10 "search starts here"`.

This is exactly what happened in Lab 0 with `-I ${mkEigenInc}`: `#include <Eigen/Dense>` is found because the `eigen` module puts its directory in that variable.

---
### Separate compilation and linking
The one-line command recompiles everything every time. For larger projects, each source is compiled to an object file on its own, and the objects are linked at the end:
```bash
g++ -c -I. hello.cpp     # produces hello.o
g++ -c -I. main.cpp      # produces main.o
g++ hello.o main.o -o hello
./hello
```
**Try it**, then change something in `main.cpp` (or just `touch main.cpp`), and recompile only `main.cpp` and re-link: `hello.o` is still valid, and there is no need to redo it. Deciding *which* files need to be recompiled after a change is the problem `make` solves.

---
## 3.2 GNU Make
GNU Make automates compilation and building. It reads a file called `Makefile`, which lists the **targets** to build, the **prerequisites** each target depends on, and the **recipe** (the commands) that builds it:
```makefile
target: prerequisite1 prerequisite2
	recipe
```
Make rebuilds a target only if it does not exist, or if any prerequisite is more recent than it: that is exactly the bookkeeping we did by hand in the previous slide. Mind that the recipe lines **must start with a tab**, not with spaces.

The most common user commands are
```bash
make            # builds the first target of the Makefile
make all
make clean      # removes the intermediate files
make distclean  # removes everything that was built
```

---
### A minimal Makefile
The `function-hello` directory contains one, `function-hello/Makefile`:
```makefile
CXX      = g++
CXXFLAGS = -Wall -std=c++20 -I .

EXE  = hello
OBJS = hello.o main.o

all: $(EXE)                       # the first target is the default one

$(EXE): $(OBJS)                   # link: the executable depends on the objects
	$(CXX) $(OBJS) -o $(EXE)

%.o: %.cpp hello.h                # compile: a pattern rule, one for all the .cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

.PHONY: all clean distclean       # these targets are not files
```
`$<` is the first prerequisite (the `.cpp`), `$@` is the target (the `.o`); `$(RM)` is `rm -f`.

---
### Try it
Move to `function-hello` and run `make` twice:
```bash
cd ~/shared-folder/AMSC-Labs/Labs/2026-27/01-scientific_computing_tools/function-hello
make
./hello
make
```
The second time nothing happens: everything is up to date. Now repeat the experiment of the previous slide, but leave the decisions to make:
```bash
touch main.cpp
make          # only main.o is recompiled, then the link
touch hello.h
make          # both objects are recompiled: they depend on hello.h
make distclean
ls
```
Try also `make -n`, that prints the commands without running them, and change `CXXFLAGS` to `-O3` to see that the flags propagate. Then note the limit of this Makefile: the dependency on `hello.h` is written by hand; for real projects the compiler generates it (`-MMD`), and this is one of the things CMake does for you.

For insights on writing Makefiles, as well as on compiling, especially with mk modules, see
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/02-compile/doc/](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/02-compile/doc/)

---
## 3.3 CMake
Writing and maintaining Makefiles becomes a complex labour too; CMake is a tool to automate Makefiles creation.
The basic usage is, starting from the root directory where `CMakeLists.txt` is,
```bash
mkdir build
cd build
cmake ..
make
```
We'll use CMake in a next lab; in the meantime, when needed, you can refer to
[https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/14-cmake/](https://github.com/pacs-course/pacs-Labs/blob/main/Labs/2025/14-cmake/)

---
# 4. Containerisation in HPC
In Lab 0 you **used** a container; here we look at how one is **built**.
- podman: OCI (as Docker) compatible, no sudo, sees the shared folder only
- apptainer: HPC oriented, derived from singularity, no sudo, uses FUSE to manage container file systems
- alternatives exist: docker (creation) + sarus (execution) [https://github.com/eth-cscs/sarus](https://github.com/eth-cscs/sarus)

---
### Before starting: install podman and apptainer
Neither tool is available **inside** the course container: you cannot build a container from within another one. Everything in this section is run on your **host** system, i.e. Linux, WSL, or macOS, in a terminal outside the container.

Both tools must be installed first. On Ubuntu (and on WSL with Ubuntu), `podman` is in the standard repositories:
```bash
sudo apt update
sudo apt install -y podman
podman --version
```
and `apptainer` is installed from its PPA, as done in [Lab 0](../00-environment_setup/Lab0_SetUp.md#21-install-apptainer) by Linux users:
```bash
sudo add-apt-repository -y ppa:apptainer/ppa
sudo apt update
sudo apt install -y apptainer
apptainer --version
```
On other distributions, follow the official guides: [https://podman.io/docs/installation](https://podman.io/docs/installation) and [https://apptainer.org/docs/admin/main/installation.html](https://apptainer.org/docs/admin/main/installation.html).


## 4.1 Podman
### Hello world
See `examples/hello-world.dockerfile`. Move to the `examples` directory of the lab first, since `podman build` looks for the sources to `COPY` in the current directory. Remember you are now on the host: the shared folder is at the path you chose in Lab 0, not at `~/shared-folder`.
```bash
cd /path/to/shared-folder/AMSC-Labs/Labs/2026-27/01-scientific_computing_tools/examples
podman build -t hello-world:v1 -f hello-world.dockerfile .
podman run --rm -it hello-world:v1 /usr/local/bin/hello
```

---
### Podman hello world - multistage
See `examples/hello-world-multistage.dockerfile`
```bash
podman build -t hello-world:v2 -f hello-world-multistage.dockerfile .
podman run --rm -it hello-world:v2 /usr/local/bin/hello
podman images
```
Compare the size of `v1` and `v2`: only the artifacts are kept in the final stage, not the toolchain.

---
## 4.2 Apptainer
(see files in the `examples` dir)
```bash
apptainer build ada.sif ada.apptainer_recipe
apptainer exec ada.sif gnatmake hello.adb && \
apptainer exec ada.sif gnatbind -x hello.ali && \
apptainer exec ada.sif gnatlink hello.ali && \
apptainer exec ada.sif ./hello
```

---
### Repositories & Apptainer
You can upload the container image built with podman to repositories such as [https://quay.io/repository/](https://quay.io/repository/) (free registration required to upload, and to follow the [https://quay.io/tutorial/](https://quay.io/tutorial/)).

Then, you can build apptainer containers from podman/docker images, e.g.
```bash
apptainer pull docker://quay.io/pjbaioni/amsc_mk:2025
```
which is exactly the command you ran in Lab 0.

---
## 4.3 HPCCM and Python envs
High-Performance Computing Container Maker provides a python based, higher level interface to container definitions, already following best practices for docker (default, podman compatible) and singularity (apptainer compatible).

It can be installed in the **recommended** way to install python packages:
```bash
python3 -m venv ~/hpccm
source ~/hpccm/bin/activate
pip install --upgrade pip
pip install hpccm
deactivate
```

---
### Example usage
```bash
source ~/hpccm/bin/activate
hpccm --recipe compilers.hpccm --format singularity > compilers.apptainer
hpccm --recipe compilers.hpccm --format docker > compilers.podman
deactivate
apptainer build compilers.sif compilers.apptainer
apptainer shell compilers.sif
```

---
# 5. Modules and package managers

---
## 5.1 mk modules
Let's redo the Lab 0 test, live, comparing before and after:
```bash
which $mkEigenInc
source /u/sw/etc/bash.bashrc
module load gcc-glibc
module load eigen
echo $mkEigenInc
```
and have a look at what else is there, since we will need it later in the course:
```bash
module avail
env | grep mk | sort
```

---
## 5.2 spack <span style="color:red;float:right">(reading only)</span>
Spack is a multi-platform package manager that builds and installs multiple versions and configurations of software. Spack is non-destructive: installing a new version of a package does not break existing installations, so many configurations of the same package can coexist.

Spack offers a simple "spec" syntax that allows users to specify versions and configuration options. Package files are written in pure Python, and specs allow package authors to write a single script for many different builds of the same package.

<span style="color:red;">**The slides of this section are not done in class: they are a guided reading, to be done at home.**</span> Nothing in the rest of the course depends on spack, but it is the tool you are most likely to meet on a cluster where mk modules are not installed.

---
### Installing <span style="color:red;float:right">(reading only)</span>

Clone the repository
```bash
git clone -c feature.manyFiles=true --depth=1 --branch \
releases/v0.23 https://github.com/spack/spack spack-0.23
```

---
### First steps <span style="color:red;float:right">(reading only)</span>

setup the environment
```bash
source spack-0.23/share/spack/setup-env.sh
```

list available packages (slow first time)
```bash
spack list
```

optional: tune configuration
```bash
find spack-0.23 -iname "config.yaml"
nano -liST 2 spack-0.23/etc/spack/defaults/config.yaml
```
(e.g., edit spack-stage, stage, test and cache dirs; see `examples/spack.diff`)

---
### Basic usage <span style="color:red;float:right">(reading only)</span>

```bash
spack info gcc
spack install gcc@14.2.0
which gcc
spack load gcc
which gcc
spack unload --all
```

---
### Specs (optional) <span style="color:red;float:right">(reading only)</span>

Default ones
```bash
spack spec -ll gcc
```
Setting specs
```bash
$ spack install mpileaks                           :  unconstrained
$ spack install mpileaks@3.3                       @: custom version
$ spack install mpileaks@3.3 %gcc@4.7.3            %: custom compiler
$ spack install mpileaks@3.3 %gcc@4.7.3 +threads   +/- build option
$ spack install mpileaks@3.3 cppflags="-O3 –g3"        set compiler flags
$ spack install mpileaks@3.3 target=cascadelake        target microarchitecture
$ spack install mpileaks@3.3 ^mpich@3.2 %gcc@4.9.3 ^: dependency constraints
```

---
### Compilers & toolchains <span style="color:red;float:right">(reading only)</span>

Get the compilers list: `spack compilers` or
```bash
spack config get compilers
```
equivalently
```bash
cat ~/.spack/linux/compilers.yaml
```
After having installed and loaded a new compiler, update the compilers list and check the result
```bash
spack load gcc@14.2.0
spack compiler find
spack config get compilers
```

---
### Compilers & toolchains <span style="color:red;float:right">(reading only)</span>
Check installed packages with `spack find`.
Now you can use that compiler to build new packages, e.g.
```bash
spack install intel-oneapi-tbb%gcc@14.2.0
```
Check installed packages again with `spack find`.

Thus, your installation is independent from the OS compiler version, and more portable and reproducible. Packages compiled with a specific compiler can be found with
```bash
spack find %<compiler>@version
```

---
### Reading: build your own module system <span style="color:red;float:right">(reading only)</span>

Requires either
- [https://github.com/envmodules/modules](https://github.com/envmodules/modules)
- [https://github.com/TACC/Lmod](https://github.com/TACC/Lmod)

Here we refer to `apt info environment-modules`, but they can be installed via spack too.
```bash
sudo apt install environment-modules
source /etc/profile.d/modules.sh
module avail
source spack-0.23/share/spack/setup-env.sh
spack module tcl refresh -y
module avail
module load gcc/14.2.0<...> && module load intel-oneapi-tbb/
module list
```

---
# Next: numerical linear algebra with Eigen

The second part of this laboratory is in [`Lab1b_IntroEigen.md`](./Lab1b_IntroEigen.md), in this same directory: we use the toolchain set up here to write our first programs with **Eigen**, the linear algebra library of the course, and we end with some image processing.

Keep at hand:
- the container and the mk modules of [`00-environment_setup/`](../00-environment_setup/);
- the `AMSC-Labs` repository cloned in [Section 2.2](#22-github), which contains the sources;
- a CLI editor, and `man`.
