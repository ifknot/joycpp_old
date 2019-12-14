# joycpp a Joy interpretor written in C++

![joy logo](https://cldup.com/zIkm02BrOD.jpg)

Inspired by the [Joy programming language](https://github.com/ifknot/meh/wiki/Joy) by Manfred von thun (2001).

[Joy Synopsis](https://github.com/ifknot/joycpp/wiki/Joy-Synopsis)

joycpp is an, as yet, incomplete implementation of the Joy programming language as an interactive command line interpretor.

joycpp uses a token stack and 3 associative array dictionaries of functions that operate on that stack. 

The 3 dictionaries consist of: Joy language functions written in C++, Joy language functions written in Joy, and user functions written in Joy.

There is also a 4th  dictionary of utility commands that can be used to manage the 3 function dictionaries and the interpreter environment itself.

Commands:







