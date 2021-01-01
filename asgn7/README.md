To compile the program, type make or make all to generate the executables for
encode and decode. For both encode and decode, specify input and output file
with -i and -o, respectively, with the names after them. Otherwise, default
input will be STDIN and default output will be STDOUT. To print the statistics
for the program, type argument -v, which will print the compressed and
uncompressed file sizes along with the ratio.

Make infer generates generates 1 memory leak for encode and 2 memory leaks for
decode that are tied to trie-node-create and word-append-sym function in word.c,
respectively. However, they are most likely false positives as said 
function has measures to make sure that pointers to allocated spaces of memory 
cannot be NULL.
