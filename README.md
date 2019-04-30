# FuturesLib
FuturesLib aims to provide an API that allows users to implement their own
custom trading modules. It designed specifically for futures and options on futures.
It is designed from the ground up and makes several assumptions for better
performance. It is inspired by the hffix and QuantLib projects on Github. The first 
version is developed solely for use on CME Group Products. Suggestions are welcome.

### Version 0.1.0

#### 4/29/19

GenerateTextFiles is now a member function of FutureFactory. Considering making FutureFactory
a singleton and have it be responsible for construction of all FuturesContracts. Should I make
a separate class FutureUpdater?

#### 4/22/19 Design changes:

The important data-containing objects in the library will now be composed of smaller "building blocks".
Each building block is a POD struct. The data within each struct should be obtained from the same source
at the same time.

### Version 0.0.0

Anything done while this repo is still private will be version 0.
