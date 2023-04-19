# Simple Shell
This is a simple implementation of a shell in C programming language. It supports executing basic commands, piping and redirection of input and output.

**There is a 2 pipe method in this code:**
#### 1. pipe through children -> that's mean, the shell is the parent and the children are execute the commands simultaneously
#### 2. pipe in a family tree ->that''s mean, the shell is the parent and the commands are executed in a 
####    tree (child ->grandchild ->great grandchild...).

## Prerequisites
To build and run this project, you need to have the following tools installed on your system:

* GCC compiler
* GNU Make

## Build Instructions
To build the project, open a terminal and run the following commands:

Copy code
```bash
$ make all
```
This will build the executable file shell in the current directory(and all the other files).

## Usage
To start the shell, run the following command:


Copy code
```bash
$ ./shell
```
The shell prompt will appear. You can type in commands at the prompt and execute them.

The shell supports the following commands:

* **cmp**: compares two files and prints the differences (requires two file names as arguments)
* **copy**: copies a file (requires two file names as arguments)
* **encode**: encodes a message using a codec (requires a codec name and a message as arguments)
* **decode**: decodes a message using a codec (requires a codec name and a message as arguments)
* **history**: prints the command history
* Other basic commands supported by the shell

## Examples
Here are some examples of how to use the shell:

* To compare two files using **cmp**:

Copy code
```bash
$ cmp file1.txt file2.txt
```
* To copy a file using **copy**:

```bash
$ copy file1.txt file2.txt
```
* To encode a message using **encode**:

Copy code
```bash
$ encode base64 "Hello World"
```
* To decode a message using **decode**:

Copy code
```bash
$ decode base64 "SGVsbG8gV29ybGQ="
```
* To print the command history using **history**:

Copy code
```bash
$ history
```

## Tools



### CMP Tool
The cmp tool is used to compare two files, and return 0 if they are equal, and 1 if they are not.
>The program opens the two files using the file I/O functions fopen() and fgetc(). It then reads each file byte by byte and compares the bytes. If the >bytes are not equal, the program returns 1, indicating that the files are different. If the end of one file is reached before the end of the other file, >the program also returns 1.

### Usage

Copy code
```bash
cmp file1 file2 [-v] [-i]
```
* file1 - The first file to compare.
* file2 - The second file to compare.
* -v - Optional flag to enable verbose output. Prints "equal" or "distinct" in addition to returning the int value.
* -i - Optional flag to ignore case. "AAA" and "aaa" are considered equal.

### Examples
Compare two files:

Copy code
```bash
cmp file1.txt file2.txt
```
Compare two files with verbose output:

Copy code
```bash
cmp file1.txt file2.txt -v
```
Compare two files ignoring case:
cmp file1.txt file2.txt -i
Compare two files with verbose output and ignoring case:

Copy code
```bash
cmp file1.txt file2.txt -v -i
```
### Return Value
* '0' - The files are equal.
* '1' - The files are not equal.
If the '-v' flag is used, the tool will print "equal" or "distinct" in addition to returning the int value.

### Errors
* Usage: cmp file1 file2 [-v] [-i] - The tool was not used correctly.

### Copy Tool
The Copy Tool is a command-line tool that allows you to copy a file from one location to another with optional flags for verbose output and overwriting existing files. The tool is written in C and returns a value of 0 on success or 1 on failure.
> The tool works by opening the source file in read binary mode and the destination file in write binary mode. It reads each character from the source file > and writes it to the destination file until the end of the file is reached. If the destination file already exists and the "-f" flag is not present, the > tool will exit with an error. If the copying process is successful, the tool will exit with a return value of 0. If there is an error during the copying > process, the tool will exit with a return value of 1.

### Usage
To use the Copy Tool, run the following command:

Copy code
```bash
./copy [source_file] [destination_file] [-v] [-f]
```
The tool requires at least two arguments: the source file and the destination file. If the destination file already exists, the tool will not overwrite it unless the -f flag is used.

### Flags
The Copy Tool supports the following optional flags:

* '-v' : Enables verbose output. If the file is copied successfully, the tool will output "success". If the destination file already exists, the tool will output "target file exist". If any other error occurs, the tool will output "general failure".
* '-f' : Enables force overwriting of the destination file. If the destination file already exists, it will be overwritten without prompting.

Examples
To copy a file named "file1.txt" to a new file named "file2.txt", run the following command:

Copy code
```bash
./copy file1.txt file2.txt
```
To copy a file named "file1.txt" to a new file named "file2.txt" with verbose output, run the following command:

Copy code
```bash
./copy file1.txt file2.txt -v
```
To copy a file named "file1.txt" to a new file named "file2.txt" and overwrite the destination file if it already exists, run the following command:

Copy code
```bash
./copy file1.txt file2.txt
```
To copy a file named "file1.txt" to a new file named "file2.txt" with verbose output, run the following command:

Copy code
```bash
./copy file1.txt file2.txt -v
```
To copy a file named "file1.txt" to a new file named "file2.txt" and overwrite the destination file if it already exists, run the following command:

Copy code
```bash
./copy file1.txt file2.txt -f
```
To copy a file named "file1.txt" to a new file named "file2.txt", overwrite the destination file if it already exists, and enable verbose output, run the following command:

Copy code
```bash
./copy file1.txt file2.txt -v -f
```
### How it works
The Copy Tool uses standard C file I/O functions to read the contents of the source file and write them to the destination file. If the destination file already exists and the -f flag is not used, the tool will exit without copying the file. If the -v flag is used, the tool will output status messages indicating whether the file was copied successfully or if any errors occurred. The tool returns a value of 0 on success or 1 on failure.

### Encode Tool 
The Encode Tool is a command-line tool that allows users to encode messages using one of two codecs: codecA and codecB. This tool is designed to be used in a Unix-like environment, and requires the use of shared libraries.

### Usage
The Encode Tool takes two command-line arguments: the codec to use (codecA or codecB) and the message to encode. The message can be provided as a command-line argument, or entered interactively (pipe) if no message is provided. To encode a message using codecA, the following command can be used:

Copy code
```bash
./encode codecA "hello world"
```
To encode a message using codecB, use the following command:

Copy code
```bash
./encode codecB "hello world"
```
If no message is provided as a command-line argument, the Encode Tool will prompt the user to enter a message interactively.

### Functionality
When the Encode Tool is executed, it loads the appropriate shared library (codecA or codecB) using the **dlopen()** function. Once the library is loaded, it retrieves a pointer to the **encode()** function using the **dlsym()** function. The **encode()** function is then called with the provided message as its argument, and the encoded message is stored in the **message** variable.

**decode/encode**

This function pointer type is used in both the Encode and Decode Tools. When the tool is executed, it loads the appropriate shared library using the dlopen() function. Once the library is loaded, it retrieves a pointer to the encode() or decode() function using the dlsym() function. The function pointer is then cast to the appropriate type (encode_func or decode_func) and stored in a variable. This variable can then be used to call the encode() or decode() function with the provided message.

**decode/encode**

After the message has been encoded, the shared library is closed using the **dlclose()** function. The encoded message is then printed to the console, and the program exits.

If no message is provided as a command-line argument, the **fgets()** function is used to read a message from standard input (i did it so it can be used in a pipe command). The resulting string is passed to the **encode()** function, and the encoded message is stored in the message variable.

### Error Handling
The Encode Tool provides some basic error handling. If the user provides an invalid codec as a command-line argument, an error message is printed to the console and the program exits. If there is an error loading or closing the shared library, an error message is also printed and the program exits.

#### in the same way for decode.

### Shared Libraries

codecA and codecB are two shared libraries used by the Encode and Decode tools to perform encoding and decoding operations on messages. These libraries are dynamically loaded by the tools at runtime using the dlopen() function.

#### Usage

The shared libraries are loaded by the Encode and Decode tools as needed. The tools automatically load the appropriate library based on the codec specified as a command-line argument.

#### Functionality
Both codecA and codecB provide an implementation of the encode() and decode() functions used by the Encode and Decode tools, respectively.

* codecA
> The encode() function in codecA takes a message as input and performs a simple substitution cipher on the message. Each letter in the message is replaced > by the letter immediately following it in the alphabet, with 'z' wrapping around to 'a'.

The decode() function in codecA performs the reverse operation on a message encoded with codecA, producing the original message.

* codecB
> The encode() function in codecB takes a message as input and performs a more complex substitution cipher on the message. Each letter in the message is > > > replaced by a corresponding letter according to a pre-defined mapping table.

The decode() function in codecB performs the reverse operation on a message encoded with codecB, producing the original message.

## Collab
@moshenh01 
@Yogev282
 
