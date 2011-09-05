# QAircrack

  QAircrack is a simple Qt4 frontend for Aircrack-ng (WEP only).

  Cracking process is splitted into these steps:

    1. Toggle monitor mode
    2. list available APs
    3. Choose AP and capture only from it
    4. Fake authentication
    5. Inyection
    6. Get password

  QAircrack GUI has one button for each of the previous steps so the 
  user must click on button at a time in order to retrieve the password.


## Screenshots

![Alt text](http://shoden.es/img/qaircrack-01.png "QAircrack screenshot 1")


## Compilation dependencies

  * Qt4 development libraries
  * CMake


## Runtime dependencies

  * Qt4 runtime libraries
  * Aircrack-ng


## Download source code

  <code> git clone git@github.com:shoden/qaircrack.git </code>


## Install from source

   <code> cd QAIRCRACK-SOURCES </code>

   <code> mkdir build </code>

   <code> cd build </code>

   <code> cmake .. </code>

   <code> make </code>

   <code> make install (as root) </code>


## Uninstall from source

  <code> cd QAIRCRACK-SOURCES/build </code>

  <code> make uninstall (as root) </code>


## Run

  <code> qaircrack </code>


## Project page

  <https://github.com/shoden/qaircrack>

