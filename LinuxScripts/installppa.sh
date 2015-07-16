#! /bin/sh
# Script to get all the PPA installed on a system
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid main restricted'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid-updates main restricted'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid universe'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid-updates universe'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid multiverse'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid-updates multiverse'
sudo apt-add-repository 'http://nl.archive.ubuntu.com/ubuntu/ vivid-backports main restricted universe multiverse'
sudo apt-add-repository 'http://security.ubuntu.com/ubuntu vivid-security main restricted'
sudo apt-add-repository 'http://security.ubuntu.com/ubuntu vivid-security universe'
sudo apt-add-repository 'http://security.ubuntu.com/ubuntu vivid-security multiverse'
sudo apt-add-repository ppa:pipelight/stable
sudo apt-add-repository ppa:team-xbmc/unstable
sudo apt-add-repository ppa:thomas.tsai/ubuntu-tuxboot
sudo apt-add-repository 'http://download.opensuse.org/repositories/isv:/ownCloud:/desktop/xUbuntu_15.04/ /'
sudo apt-add-repository ppa:diesch/testing
sudo apt-add-repository ppa:webupd8team/java
sudo apt-add-repository 'http://dl.google.com/linux/chrome/deb/ stable main'
