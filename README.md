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

## Contributing
  ...
