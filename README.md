## Environment
This project works well under OS X 10.10, clang version 6.1.0, flex 2.5.35, bison 2.3.
I'm not sure it works well on other platform.

## Files 
smallc.l: the lexical analyzer
smallc.y: syntax analyzer
datastruct.h : the data structure used in the above files.
test[0-9]: test files

## Usage
<code>
	$ make
</code>

<code>
	$ ./main (input file) (output file)
</code>