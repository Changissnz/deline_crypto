# Introduction

Engine that performs encoding and decoding of character sequences with the aid of
a given `n`-dimensional dataset (with non-missing values)
of decimal values.

Depending on usage, the encoding can be considered as
cryptographic.

# More info
See `dc_paper.pdf`. 

# Test datasets
For the test datasets `t4.csv` and `t5.csv` that are required
for some of the test functions to correctly execute, use the 
project at 
`github.com/changissnz/morebs` by the following:

- go to terminal.
- type in `python3 -i gen_t.py`.
- type in `gen_t4()` for `t4.csv`.
- type in `gen_t5()` for `t5.csv`.
- transfer those two files to the folder
    `deline_crypto/src/data`.

# Update 6/27/23
Further developments on project have been stalled.
Encoding procedure works.

# Update 8/12/23
First draft of paper on `deline_crypto` published 
to project. See `dc_paper.pdf`. 
