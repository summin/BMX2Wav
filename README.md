
# BMX2Wav

## Description
BMX2Wav is a programm that extracts wave samples from the Jeskola Buzz Tracker project files with *bmx* extension. The current version of BMX2Wav unfortunately does not save 24 bit samples, and requires the bmx files to be saved in Jeskola Buzz Tracker version higher than 1.2 Beta. Meaning you will need to re-save your project files in any Buzz, that was released post 2009.

## Instructions
Copy BMX2Wav to your bmx files folder and run it. It will find all the files in the folder (non-recursively) and will extract samples to extractedSMP folder. The program will save the samples in original folders as they were on your old Windows machine.

We strongly recommend to copy all your bmx files into one folder and execute BMX2Wav there. Please, make sure that you have the backup copies of your bmx files.

## Compilation
We recommend to use xcode for compilation. Otherwise the code should compile without many problems for any other unix based os. Contact alexander.leontjev@gmail.com for binary.

## Licence MIT
The program is distributed unders MIT licence without any warranty or responsibility in regards to safety of your hardware, software and/or data. 
