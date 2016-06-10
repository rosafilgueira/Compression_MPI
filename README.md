# Compression_MPI
This repository is to store two versions of the same library for compressing at run-time MPI messages using different compression algorithms. The compression is adaptive, so it turns on and off dependeding on the: data size of the message, network speed, data type of the message.

The first version is called PRACTICAL 3.0 (PRAcTICaL-MPI (Portable Adaptive Compression Library- MPI) explained at the Euro-Par 2012 conference in the work published as "An Adaptive, Scalable, and Portable Technique for Speeding Up MPI-Based Applications". More details at: http://link.springer.com/chapter/10.1007%2F978-3-642-32820-6_72

 ```Abstract:
	This paper presents a portable optimization for MPI communications, called PRAcTICaL-MPI (Portable Adaptive Compression Library- MPI). 
	PRAcTICaL-MPI reduces the data volume exchanged among processes by using lossless compression and offers two main advantages. 
	Firstly, it is independent of the MPI implementation and the application used. 
	Secondly, it allows for turning the compression on and off and selecting the most appropriate compression algorithm at run-time, depending on the characteristics of each message and on network performance.
	
	We have validated PRAcTICaL-MPI in different MPI implementations and HPC clusters. 
	The evaluation shows that compressing MPI messages with the best algorithm and only when it is worthwhile, we obtain a great reduction in the overall execution time for many of the scenarios considered.
    ```

