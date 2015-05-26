
# Huffman Tree Encoder
## An elegant way to compress text files

### Install:
Simply clone the repository and build the executable by running `make` in its directory within a terminal.

### Usage:
`./Compress [-c]  [txt_name] [bin_output]`  Compresses text file to binary file

`./Compress [-cs] [txt_name] [bin_output]`  Compresses and displays hash table statistics

`./Compress [-d]  [bin_name] [txt_output]`  Decompresses binary file to text file

### About
This assignment's goal was to speed up our existing Huffman Encoding program (found within [hw3](https://github.com/jmbannon/Schoolwork/tree/master/tcss342_data_structures/hw3)).  Instead of building the tree using ASCII characters, it uses English words. Any string of characters including 0-9, a-Z, -, or ' is considered a 'word'. Any other character is considered its own word.  Each word is then stored within a hash table that uses quadratic probing.  See below for the analysis of quadratic versus linear probing.

I have included the text file of *War and Peace* as a test file. 

### Analysis of Hash Table Collision Strategies
A PDF of the analysis can be found [here](https://github.com/jmbannon/Schoolwork/blob/master/tcss342_data_structures/hw4/hash_collision_analysis.pdf).

### License
Use for educational purposes only.
