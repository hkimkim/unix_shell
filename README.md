# My Own UNIX Shell
Northeastern Summer 2021

## Program Description
This program emulates an UNIX shell. It executes four built-in functions (cd, help, quit, and joke) and other UNIX commands provided in `/bin` directory of the computer system. 

## Run Program
```bash
$ make shell

make all
gcc shell.c -w -o shell
./shell .
k-sea-shell>>

```

## Command Names

### 1. `cd`: Change Directory
This command changes the directory from current to given directory.  

I used `chdir()` c library to change directory and `getcwd()` to print the changed directory. 

Example Usage:
```console
k-sea-shell>> cd ../ 
Directory changed to: /home/heekyung622/heekyung622_CS5008 

```

### 2. `help`: Print description of the built-in commands
This command is used to print the description of the built-in commands.  

I used `put` to print all the command descriptions. 

Example Usage:
```console
k-sea-shell>> help 

List of Built-in Commands provided:
> cd [name of directory]: changes directory to given directory
> help : prints all the built-in commands provided
> exit : terminate the shell
> joke : tells a random joke
> And all other unix commands

```
### 3. `exit`: Ends the current shell 
This command terminates the current shell.  

I used `exit()` to implement the command. 

Example Usage: 
```console
k-sea-shell>> exit
```

### 4. `joke`: Tells a random joke 
This command tells a random joke to the user.  

I implemented this command by first creating a list of strings of jokes and a list of strings of answers which were corresponding to each other by indexes. Then I used `rand()` to generate a random number between 0 and 6 (there were 6 jokes in total). The randomly generated number was used as indexes to first print the joke and then print the answer. I also made the shell `sleep` for 1 sec after the joke is printed so that there is some thinking time before the answer is told. 

Example Usage: 
```
k-sea-shell>> joke

What was Beethoven's favorite fruit?
> Ba-na-na-NAAA!

```



