# HardwareKitExample

**IMPORTANT NOTE**

Third-party hardware used in this example is not endorsed or supported by Phoenix Contact.

## Table of contents

<!-- TOC depthFrom:2 orderedList:true -->

- [Introduction](#introduction)
- [Guide details](#guide-details)
- [Hardware components](#hardware-components)
- [Procedure](#procedure)
- [C++ project description](#c-project-description)
- [FAQ](#faq)

<!-- /TOC -->

## Introduction

This project demonstrates the use of the left-side hardware extension kit AXC F XT KIT with an AXC F 3152 controller.

It is assumed that the user is familiar with developing Programs and Components for PLCnext Control devices using C++. Please refer to the [relevant sections of the Info Center](https://www.plcnext.help/te/Programming/Cpp/Cpp_programming/Cpp_programs_in_PLCnext.htm) if necessary.

## Guide details

|Description | Value |
|------------ |-----------|
|Created | 11.11.2022 |
|Last modified | 11.11.2022 |
|Controller | AXC F 3152 with firmware 2022.9 |
|IDE | Visual Studio 2019 (any edition) |
|PLCnext CLI | 22.6.0.1153 (22.6.0.1153) with the Visual Studio add-in installed |

## Hardware components

- [AXC F 3152](https://www.phoenixcontact.com/en-pc/products/controller-axc-f-3152-1069208)
- [AXC F XT KIT](https://www.phoenixcontact.com/en-pc/products/extension-module-axc-f-xt-kit-1383116)
- [Mini PCIe card with 2 x USB ports](https://www.inline-info.com/en/products/io-cards-cardreader/mini-pcie/9070/inline-mini-pcie-card-2x-usb-3.0)
- USB port adapter, e.g. [Delock USB 3.0 Pin Header female > 2 x USB 3.0 female](https://www.delock.de/produkt/41865/merkmale.html?setLanguage=en)
- [Neuftech USB RFID ID Card Reader Contact Los for EM4100](https://www.amazon.de/Neuftech-Reader-Kartenleseger%C3%A4t-Kartenleser-Kontaktlos/dp/B018OYOR3E/ref=asc_df_B018OYOR3E/), with tags.
- 24V DC PSU for the PLC and KIT module
- 5V DC and 12 VDC PSUs for the Mini PCI card
- Cables and connectors

## Procedure

1. Read the data sheets for all the hardware components carefully.

1. Assemble the hardware. It should look something like this (the I/O modules shown in the image are not used in this example):

   ![image](https://user-images.githubusercontent.com/13133969/201328414-77e8b198-26c5-41e5-a4f1-c83e2a09a9ac.png)

1. Apply power to the assembly.

1. Check that the RFID reader has been detected by the operating system, using the `lsusb` command on the PLC.

1. Clone this repository and open the Visual Studio solution.

1. Build the solution to generate a PLCnext Engineer library.

1. In PLCnext Engineer, create a new project for the AXC F 3152 v 2022.9.

1. Add the library to the PLCnext Engineer project.

1. Create an instance of the Program in a cyclic task.

1. Send the project to the PLC.

1. While online to the PLC, in Debug mode, add the GDS port variable (array) from the Program instance to the Watch window.

1. Scan an RFID tag.

1. The tag data should appear in the array.

You can now use the data from the RFID reader in your own PLCnext Engineer application.

## C++ project description

The Visual Studio solution includes a C++ project that contains one PLM component and one Program. The only purpose of the Program is to force the creation a PLM component instance.

The PLM component uses the `libusb` library, which is included in the SDK for the AXC F 3152.

The PLM component instance creates a Worker Thread that is executed every second.

On each execution of the worker thread, the *** method of the `libusb` library is used to check for new data from the RFID reader. When new data is detected, the new tag data is added to an array of strings. The array has the capacity for the five most recent strings and a null "terminator".

A program that reads the array can detect the arrival of new data by checking the location of the null entry in the array. When the location of the null entry changes, then new data has been received.

## FAQ

Q: Which PLCnext Technology controllers can the AXC F XT KIT be used with?
A: Only those controllers with left-side PCI express capability - currently  AXC F 2152 and 3152 devices.

Q: Can I use an AXC F 2152 for the above example?
A: Perhaps. Because of the different architecture it may be more difficult to get this example working on an AXC F 2152.

Q: Will other USB devices work with this setup?
A: Yes. For example it is possible to plug in a USB stick (thumb-drive) and mount the file system on the PLCnext Control device.

Q: Will it be easy to get other Mini PCIe cards working?
A: Perhaps. Perhaps not. In this case the `libusb` driver was already installed on the controller and included in the SDK. Other PCIe devices will probably need to have a separate driver installed.
