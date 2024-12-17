a basic implementation of the k nearest neighbors (knn) algorithm in C. Why? i dont know

### usage 
you can compile the code on your machine using the following command

```cmd
gcc knn.c -o knn -lm
```

you can enter the data manually but i prefer using the command line to enter the data as well in the following format

```plaintext
<number_of_dimensions>
<number_of_points_in_dataset>
<dataset>

<coordinates for test point>
```

this format can also be seen in the [input.txt](input.txt) file as well

this input file can be run as follows on a windows machine

```cmd
knn.exe < input.txt
```

and the output would look as follows

```plaintext
5 nearest neighbors for our testpoint
point 34 | distance : 0.200000 | label : Setosa
point 32 | distance : 0.244949 | label : Setosa
point 4 | distance : 0.264575 | label : Setosa
point 9 | distance : 0.387298 | label : Setosa
point 35 | distance : 0.435890 | label : Setosa


predicted label : Setosa
freed all memory - exiting
```

---