# numc

### Provide answers to the following questions.
- How many hours did you spend on the following tasks?
  - Task 1 (Matrix functions in C): 10
  - Task 2 (Speeding up matrix operations): 5
- Was this project interesting? What was the most interesting aspect about it?
  - This project was really interesting because we were doing something that we've already done before (namely mat_mul in RISC-V) but in C, and now with the ability to make it much faster by using "big word" techniques like "parallelism" and "vectorization." We found it really satisfying that we were able to see such drastic improvements in performance when we did small changes like SIMD and adding "#pragma omp parallel for." 
- What did you learn?
  - We learned how to debug even more so than we had learned in Project 1 by using cgdb and print statements. However, this process was still extremely annoying for the unittests.py because everything was random and we didn't quite understand how to attempt to debug through the unittests, so we had to write weird tests in mat_test.c to try to emulate what the unittests were doing. We reinforced our learning about how to make use of cache spatial locality by implementing transpose_matrix, which helped us a lot when we were vectorizing. Overall though, we learned how powerful using "#pragma omp parallel for" was because it really sped us up without us having to change our implementation so drastically.
- Is there anything you would change?
  - We would prefer that we were told algorithmic changes to be making (repeated squaring, transpose, etc) when initially implementing Task 1 because having to go back and change things again ended up messing with our correctness, and it was annoying to have to rethink and reimplement our functions multiple times. 
