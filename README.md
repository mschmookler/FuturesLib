# FuturesLib
FuturesLib aims to provide an API that allows users to implement their own custom
trading modules. It is developed specifically for interacting with futures and options
on futures through CME Group exchanges.

I'll start by building simple, non-trading tools that I feel provide some value and
then move on to actual trading modules.

## Design goals
1. Transparent and uncomplicated architecture. Plain old data structs and struct pointers are used
to package data together and pass references.
2. Minimize or eliminate heap allocation. Strings are used sparingly and mostly to simplify the
design process. My goal is to remove all non-constant string operations in later versions.
3. Complete logging.

## More info
This is my first non-trivial C++ project as well as my first Github project so
it is a work in progress. I drew inspiration from two projects in particular: hffix and
QuantLib. Both are available on Github.

All suggestions are welcome.

#### Version 0.2.0
