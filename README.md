# FuturesLib
FuturesLib is a package for interacting with futures and options on futures.
Since this is my first Github project the main goal will be to practice coding
structure, style, and readability. The first version will be developed solely
to CME Globex. All suggestions are welcome.

### Version 0.1.0

#### 4/28/19

Completed secdef parsing functions. I now have a FuturesContract constructor that can read lines from secdef.dat.
Once I figure out the proper design pattern for the FutureFactory class and FilterSecdef function, the factory
will be a friend of FuturesContract and the constructor will be a member func of the factory instead.

#### 4/22/19 Design changes:

The important data-containing objects in the library will now be composed of smaller "building blocks".
Each building block is a POD struct. The data within each struct should be obtained from the same source
at the same time.

### Version 0.0.0

Anything done while this repo is still private will be version 0.