# MatrixCalculator

A project that allows for calculating matrix operations. I started this for my Systems Fundamentals course and added more to it.

The evaluate_expr_sf function accepts the path to a script as an argument and returns the last matrix that was created. Note that matrices must be named using a single uppercase character. There are 2 possible script instruction forms:
- Defining an m-by-n matrix by inputting dimensions and number of elements: A = m n [row1col1 row1col2 ... row1colN; row2col1 row2col2 ...... rowMcolN]
    - Rows are separated by semicolons (;) and entries within a row are separated by spaces
- Defining a matrix using an expression of other matrices: D = A + B * (B' - C)
    - These expressions can only use existing matrix names, addition (+), subtraction (-), matrix multiplication (*), transpose ('), and parentheses 

Here is what I needed to implement for the project
- Matrix addition, subtraction, multiplication, and transpose
- Conversion from infix to postfix and evaluating postfix to solve expressions
- Binary search tree to find named matrices
- Dynamic memory management (malloc/free) to save matrices
- File reading to parse the scripts
- Unit testing to ensure that functions worked as expected

