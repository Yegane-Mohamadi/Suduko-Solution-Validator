# Sudoku Solution Validator 🧩

This is a multithreaded Sudoku solution validator written in C using `pthread`.  
The program checks whether a given 9x9 Sudoku grid is a valid solution by spawning multiple threads to validate:

- All 9 rows
- All 9 columns
- All 9 subgrids (3x3)

## ✅ Features

- Multithreading with 11 threads:
  - 1 thread to validate all rows
  - 1 thread to validate all columns
  - 9 threads for the 3x3 subgrids
- Fast and efficient logic
- Easy to modify the input grid

