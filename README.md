# Compiler

Project specification (in Polish) can be found in `spec.pdf`.  
Example source codes (*.imp files) for compiler can be found in `programs` folder.

## Dependencies

| tool  | version |
|-------|---------|
| flex  | 2.6.4   |
| bison | 3.7.3   |
| g++   | 10.2.0  |
| make  | 4.3     |

## Usage

In order to build compiler and interpreter, in `compiler` and `virtual-machine` folders use `make` command.  
Compilator usage (in `compiler` folder):  
```
./kompilator <input_file.imp> <output_file.imp>
```

To run compiled program (in `virtual-machine` folder) use:
```
./maszyna-wirtualna <compiled_file.mr>
```
