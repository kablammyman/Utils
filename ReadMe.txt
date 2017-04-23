UTILS:
All my common code in one sln! 

#FileUtils
Code to easily move, copy, count, read, and delete files.


#CFGUtils

I noticed I needed to parse a lot of files for my programs, especially for setting configuration options. I originally wrote this code in C# for a big Unity3D project I was working on, but since converting it to C++, it’s the only version I use and update.

Anyway, it’s pretty simple and straight forward. It basically works like:

optionName value

The value can be an int, float/double, string, or a list and the delimiter can be any char you want (I like pipes personally) It can also handle comma separated values which is useful for things like vectors. However, it cannot handle nested lists at this time. Maybe in the future if I fund a need for it, I’ll add support for it.

The visual studio 2015 project builds a lib and dll, and many of my projects use it. There are 32 and 64bit build options as well

Some examples of options that can be in the config file:

//it supports single line comments!

someOption : 10

anotherOption:some string

aCSVOption:1,2,3

//notice the space between the option and the '{'

listOption {
one,
two,
three
}

//these delimited values in the list can also be strings or floats

List2 {
One|1
Two|2
Three|3
}

The code has a test program that should show how to use the lib properly.