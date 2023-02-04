# libdbc

**libdbc** is a library for parsing [Vector's DBC File
Format](http://mcu.so/Microcontroller/Automotive/DBC_File_Format_Documentation.pdf).

These files are used for parsing CAN messages and signals and converting
CAN-IDs into human-readable and understandable names.

## Dependencies!

Yes, this library depends on other libraries! Scary!

### Heinous! Why would you add dependencies?

*The main reason behind the decision to include dependencies is to minimize the
amount of bugs I introduce into the code. I'm a bad programmer and will
introduce bugs. I wanted to let better programmers make decisions for me! I
would like to avoid reimplementing dynstrings, generic datastructures, etc.*

### But why not `glibc`?

I wanted to avoid `glibc` because I wanted this library to be as portable as
possible. Not all machines have `glibc`. Some of `C`'s utility lies exactly
where `glibc` doesn't exist, hence I wanted to avoid it.

### Alright, I don't like it, but what did you pull in?

* The wonderful [sds](https://github.com/antirez/sds) - *It's heavily used in
  Redis and if that doesn't convince you, what will?*

## Licensing

This whole repository, and everything in it is licensed under GPLv3, unless
you are a member, representative or in any way affiliated with [The Vector
Group](https://www.vector.com/us/en/). For said parties, the repository and its
contents are licensed under a strict non-commercial license available in the
file `LICENSE-Vector.txt`.

All parties using this repository and its contents, for whatever purpose, agree
to adhere to this.
