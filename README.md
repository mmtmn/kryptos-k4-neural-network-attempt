# Kryptos K4

The fourth message remains one of the most famous unsolved codes in the world. 

**This respository is dedicated to bruteforcing the fourth passage.**

<br>
<br>
<br>
<br>
<br>
<br>

# Work in Progress vs Biased

Work in progress is still experiencing some issues with validaded predicted outputs and decryptions predicted outputs. Both folders contain a Neural Network (NN), a RNN (Recurring Neural Network) and a LATT (Learning At Running Time model).

# Compile and run NN.c, RNN.c, LATT.c 

gcc -o NN NN.c -lm;./NN
gcc -o RNN RNN.c -lm;./RNN
gcc -o LATT LATT.c -lm;./LATT

<br>
<br>
<br>
<br>
<br>
<br>

# Update (June 10th,2024)

- two new folders have been added and a new unbiased neural network. 
- The new NN only consideres the positive and negative number shifts instead of letters.
- This attempts to avoid the previous attempts bias.
- You can find the old attempts in one of the folders, where the clues are placed correctly but the bias is still there.

The new neural network (unbiased_NN.c) is still training and it is not known if it'll get the clues's plain text in the right place as of right now.

<br>
<br>
<br>
<br>
<br>
<br>

# Why is the results biased for now?

On the older versions of the results are biased. You can find them in the folder biased_NN_&_RNN. The plain text response only includes letters present in the clues, suggesting a biased approach towards the bruteforced attempt. Further work would be required to **potentially** bruteforce the 30+ year old cipher (further work has been done and now this code sits in the biased folder). Even with such bias, the results are interesting, you find the plain text clues in the right place after 10k epochs, which gives the false hope of solving it. 

The path attempted in th e WIP folder is to use number displacements instead of using the alphabet, attempting to bruteforce a pattern istead of using the same letters for everything. There is certainly a valid argument that the same will happen with the displacements approach.



## Breaking Kryptos K4 (older models (biased) README.md)

change the EPOCHS variable to be above 10M for better results. the higher it is, the better the results.

