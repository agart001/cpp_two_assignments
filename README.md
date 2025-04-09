# CPP2 Assignments
![Build](https://github.com/kstatz12/cpp_two_assignments/actions/workflows/build.yml/badge.svg)

## Visual Studio
if you want to use a full featured IDE like visual studio or CLion make sure you have the CMake tools installed
[link](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170) and loading this project _should_ just work

## Environment Setup

### Vagrant
#### Install
- install Vagrant from [link](https://developer.hashicorp.com/vagrant/docs/installation)

- make sure you have a CPU that allows virtualization
  - Enable Virtualization [link](https://support.microsoft.com/en-us/windows/enable-virtualization-on-windows-c5578302-6e43-4b4b-a449-8ced115f58e1)

### Virtualbox
#### Install 
- install Virtualbox for your system
  - [link](https://www.virtualbox.org/wiki/Downloads)
  
  - note if you are on windows you can use [hyper-v](https://learn.microsoft.com/en-us/windows-server/virtualization/hyper-v/hyper-v-overview?pivots=windows-server) instead of the virtualbox hypervisor, but it is integrated into windows to the extent that if the VM crashes, it will also crash the host (your computer)

  - note for Mac Silicone users: you have to jump through some hoops. You can either switch out the base box (ubuntu 22.04) with an ARM version or use the `qemu` provider to run under virtualization AND emulation
  
  - if you are running linux i assume you can setup `libvrt` on your own

### Visual Studio Code
#### Install
- download [link](https://code.visualstudio.com/download)

#### Plugins
- remote development plugin [link](https://code.visualstudio.com/docs/remote/remote-overview)

- c++ tools [link](https://code.visualstudio.com/docs/languages/cpp)

### Git
#### Install

- install git bash for your platform [link](https://git-scm.com/downloads)

## Spinning Up

open a [windows terminal](https://github.com/microsoft/terminal)/powershell/cmd/git bash window and navigate to your project

``` shell
cd ~/{path to your project}
```

spin up vagrant

``` shell
vagrant up
```

if you are using virtualbox for a provider you have to pass that in

``` shell
vagrant up --provider virtualbox
```

## Connecting your editor

- in your shell, in the root directory of the project enter `vagrant ssh-config` and copy the output. It should look something like 

``` shell
Host default
  HostName 192.168.122.144
  User vagrant
  Port 22
  UserKnownHostsFile /dev/null
  StrictHostKeyChecking no
  PasswordAuthentication no
  IdentityFile /home/karl/src/cpp_two_assignments/.vagrant/machines/default/libvirt/private_key
  IdentitiesOnly yes
  LogLevel FATAL
```

- Launch Visual Studio Code and hit Ctrl+Shift+P to open the command pallete

- Choose `Open SSH Configuration File`

- Paste the config

- Change the `Host default` to something like `Host cpp_assignments`

- Open the command pallete again (Ctrl+Shift+P) and search for `Remote SSH: Connect To Host` and choose your host name from the previous step

- This will open a new VSCode window and start installing tools on the remote server

- Once you are fully connected choose `Open Folder` and then choose on `/home/vagrant/code`

- Now you should be able to edit files as if they were local

## Compiling

### SSH
- in your shell type `vagrant ssh`

- cd into `/home/vagrant/code`

- Enter the following commands to build and compile your project

``` shell
cmake -S . build

cmake --build build
```

then to run all the tests

``` shell
./build/ctest
```

### In Editor

to build the project you can use the built in hotkey: Ctrl+Shift+B


to compile choose `Run Tasks` and choose `cmake compile`

to test run `cmake test`


