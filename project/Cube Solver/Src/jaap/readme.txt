Rubik's Cube solver

Name:   Jaap Scherphuis
email:  jaapsch _a_t_ yahoo _d_o_t_ com
From:   Delft, the Netherlands
Source: cubecompo.cpp

Notes:

Solving method:
This program solves the cube using Thistlethwaite's four stage
algorithm.

The four stages of the Thistlethwaite are as follows.

Stage 1: Go from the group <U,D,R,L,F,B> to the group <U,D,R,L,F2,B2>.
          Solves edge orientation.
Stage 2: Go from the group <U,D,R,L,F2,B2> to the group <U,D,R2,L2,F2,B2>.
          Solves corner orientation and separates middle layer edges.
Stage 3: Go from the group <U,D,R2,L2,F2,B2> to the group <U2,D2,R2,L2,F2,B2>.
          Separates edge slices, separates corner tetrads, and fixes
          permutation parity and tetrad relative twist.
Stage 4: Go from the group <U2,D2,R2,L2,F2,B2> to the trivial group.
          Permutes edges slices and corner tetrads.

Solution length:
About 31.3 moves on average.

Programming language:
C++ (mostly straight C, but with some very minor C++ flourishes)

Program:
The cube is stored in memory as a 20 character array containing the
piece permutation, and a similar array for the orientation. Most arrays
in the program are char arrays, because this allows them to be
initialised with a string. This is shorter than using a list of numbers.

According to the rules of the competition, any precomputed tables cannot
be saved between solves, so have to be regenerated each time. I thought
it might take too long to make full tables of each of the four stages
(though I haven't tested this assumption). Therefore I decided to do one
or two smaller tables in each stage, and use them as pruning tables in
an iterative deepening search.

The tables that are calculated in each stage are as follows:

Stage 1:
   Uses table of size 4096=2^12, though only half its entries are filled
   since total edge flip is zero. For program simplicity, a second dummy
   table of length 1 is used too.
Stage 2:
   Uses table of size 6561=3^8 for corner orientation, though only 1/3
   its entries are filled since total twist is zero. Uses table of size
   4096=2^12 for middle layer edge selection, though only 12!/8!4!=495
   are filled.
Stage 3:
   Uses table of size 256=2^8 for edge slice selection, though only
   8!/4!4!=70 are filled. Uses table of size 1536=6*2^8 for tetrad
   selection, parity and twist, though only 6*8!/4!4!=540 are filled.
Stage 4:
   Uses tables of sizes 13824=4!^3 and 576=4!^2 for the 5 orbits of
   pieces. Originally I used one with the three edges orbits, and one
   with the two corner orbits. This did not work as well as using one
   corner orbit in each table.


There is occasionally some cancellation possible by combining the last
move of one stage with the first move of the next. I did not bother
doing the cancellation since the improvement would be undone by the
increase in program length.


Program Length:
About 2066 characters, when variable/function names and C words are each
counted as one, comments and whitespace ignored.

Compilation:
Program compiled without errors or warnings in Visual Studio 5.

Running:
Program takes the the input from the command line as described in the
contest instructions. Two lines of code are commented out which can be
used to read from stdin instead.
