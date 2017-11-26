  stata-reader-writer
=======================

Program to read and write Stata files

What I am working on is a library that will let me read and write Stata files to and from SQLite files.
All the solutions I have seen usually use just CSV files for import and output. Unfortunately you lose a lot 
of the richness that is provided by a full Stata dataset.

Reads format 117/118/119 (Stata 13, 14 and 15) and extracts the metadata. Only works on LSB datasets.

Writing output files is not started yet, I will focus on SQLite first.
