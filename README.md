# Thero
Headers only utility collection for modern C++

## Contents

The utilities provided follow no particular theme but is more a collection of very reusable components that I have found the need for more than once. Maybe some of them will be of use to others too.

### General

* [TH_ASSERT](https://github.com/therocode/thero/wiki/TH_ASSERT) - A handy assert macro which shows line/function/file of the assertion with a custom message, as well as ensures that gdb breaks on it instead of just quitting

### Polymorphic helpers

* [PolymorphicWrapper](https://github.com/therocode/thero/wiki/PolymorphicWrapper) - A wrapper to store a polymorphic object as a base type in a way that ensures proper copy-mechanics
* [ModuleSet](https://github.com/therocode/thero/wiki/ModuleSet) - A way to store polymorphic modules in a way that ensures that it is easy to access the separate instances too

### Random

* [RandomSelector](https://github.com/therocode/thero/wiki/RandomSelector) - A templated container for a set of values. On access, a random value is selected based on uniform distribution by default. Weights to skew the item representation can be added.

## Installation

This library uses CMake and is headers only so it is really simple to get working. Start off by generating the needed CMake files (install prefixes can be set according to CMake conventions):

```
cmake .
```

Now install the headers like you would usually do. Example for a unix-based environment:

```
sudo make install
```

Now just include the headers in your project where you want to use the library. There is a FindThero.cmake file in "cmake/modules" which can be used to locate the installed headers in the recommended way.

## Usage

Follow the links in the beginning of this document to go to the repository wiki to read about using the classes provided.
