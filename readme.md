# This readme is for final project submission

* The program is developed and tested on a 64-bit Linux server.
* The Abc's static library is uploaded by git larget file storage, if somehow the library
  could not be downloaded, compile the original Abc from source and set ABC_USE_NO_READLINE
  to 1 and place the complied library in lib/

  make ABC_USE_NO_READLINE=1
  make libabc.a

* To compile our code:

    make

* To reproduce our results:

    ./run_all

* The output circuits would be stored as alpha_publish/case*/circuit.v
