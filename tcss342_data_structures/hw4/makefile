#
# ====================================================================
# Author:    Jesse Bannon
# Date:      04/25/15
# Class:     TCSS 342: Data Structures
# School:    University of Washington Tacoma
# Desc:      Uses Huffman Trees to compress text files.
# Copyright: Use for educational purposes only.
#====================================================================
#
CC=gcc
all: compressor

compressor: main.o huffman_tree.o
	$(CC) -o Compress main.o huffman_tree.o

main.o: Main.c
	$(CC) -c -o main.o Main.c

huffman_tree.o: HuffmanTree.c HuffmanTree.h
	$(CC) -c -o huffman_tree.o HuffmanTree.c

#Need dis

