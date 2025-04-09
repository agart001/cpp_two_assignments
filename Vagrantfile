# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|

  config.vm.box = "generic/ubuntu2204"

  config.vm.synced_folder ".", "/home/vagrant/code"


  config.vm.provision "shell", inline: <<-shell
    sudo apt update
    sudo apt install -y build-essential cmake clang clangd gdb ninja-build valgrind lldb git python3 python3-pip
  shell
end
