* MPL file coping mip1
*    maximize    x +   y + 2 z
*    subject to  x + 2 y + 3 z <= 4
*                x +   y       >= 1
*                x, y, z binary
NAME          M1P1
ROWS
 N  OBJ
 E  ROW1
 G  ROW2
COLUMNS
    COL1      OBJ                1  ROW1                  1 
    COL1      ROW2               1
    COL2      OBJ                1  ROW1                  2 
    COL2      ROW2               1
    COL3      OBJ                2  ROW1                  3
RHS
    RHS1      ROW1               4
    RHS1      ROW2               1
BOUNDS
 BV BND1      COL1
 BV BND1      COL2
 BV BND1      COL3
ENDATA