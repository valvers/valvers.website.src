# RPI CM4 Linux CNC

This article provides instructions and information on using the CM4 as a Linux CNC board.

## Installing Linux CNC

Go to the [Linux CNC ISO website](https://www.linuxcnc.org/iso/) and download the latest `pi4` ISO image. It'll be named something like `linuxcnc-2.8.1-pi4.zip`.

Unzip the archive

## HAL File

The first thing we need to configure for the RPi is an INI and HAL File so that we can get some stepper motors working. Let's have a look at how that might look:

We need an [INI file](https://linuxcnc.org/docs/2.6/html/config/ini_config.html) and a HAL file