#  Storage Optimizer Architecture

# Modules:

- Scanner
- Analyzer
- Reporter
- Core
- Utilities

## Responsibilities

### Scanner
- It should scan the folder. For now i will just make it scan folders in my version 1 But in version 2 i will be looking to make it scan a whole drive.

### Analyzer
- Now what i am thinking is that it should get the scanned folder and analyze it in terms of the biggest file, size, path and also it should be able to sort them by the biggest size they would have along with the path included. For example if i scan a folder and i file total 20 gb of data out of which 8 gb has gone to a file something like abc.dll and the whole .dll has taken about 17 gb out of 20gb So, it should be able to sort like this.

### Reporter
- It should be able to take the input files and then display to the user like first i am going with CLI then after that i will make my GUI for it and then if so i will go to JSON.

### Core 
- now this is where all the working i want to happen like here the process will start like
                    Start -> Scan -> Analyze -> Report -> Exit
In which i want to make like the analyzer would not know how the files got scanned or the reporter will now know how the files got analyzed.

### Utilites
- Some smaller helper functions i will like to add just like a really imp but small function like a convertor of bytes to MB the easy convention or if the file is bigger then it will convert it to GB.


