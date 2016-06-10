# Compression_MPI
In this repository I have stored two versions of the same library for compressing at run-time MPI messages using different compression algorithms. The compression is adaptive, so it turns on and off dependeding on the: data size of the message, network speed, data type of the message. Both versions have instructions (README.txt) about how to install them.

The first version is called PRACTICAL 3.0 (PRAcTICaL-MPI -- Portable Adaptive Compression Library-MPI --), and it has two subversions, one for using with MPICH and othe for using with OPENMPI. This library was explained at the Euro-Par 2012 conference in the work published as "An Adaptive, Scalable, and Portable Technique for Speeding Up MPI-Based Applications". This vesion includes LZ0, RLE, HUFFMAN, SHANNON, RICE compression algorithms. More details at: http://link.springer.com/chapter/10.1007%2F978-3-642-32820-6_72

	```Abstract:
	This paper presents a portable optimization for MPI communications, called PRAcTICaL-MPI (Portable Adaptive Compression Library- MPI). 
	PRAcTICaL-MPI reduces the data volume exchanged among processes by using lossless compression and offers two main advantages. 
	Firstly, it is independent of the MPI implementation and the application used. 
	Secondly, it allows for turning the compression on and off and selecting the most appropriate compression algorithm at run-time, depending on the characteristics of each message and on network performance.
	
	We have validated PRAcTICaL-MPI in different MPI implementations and HPC clusters. 
	The evaluation shows that compressing MPI messages with the best algorithm and only when it is worthwhile,
	we obtain a great reduction in the overall execution time for many of the scenarios considered.
	```
The second version is called PRACTICAL 4.0, which includes previous compression algorithms plus Snappy and LZ4 agorithms. This version, was presented as part of the work of "Applying Selectively Parallel I/O Compression to Parallel Storage Systems", in Euro-PAR 2014. More details at "http://link.springer.com/chapter/10.1007%2F978-3-319-09873-9_24"

	```Abstract:
	This paper presents a new I/O technique called Selectively Parallel I/O Compression (SPIOC)
	for providing high-speed storage and access to data in QoS enabled parallel storage systems. 
	SPIOC reduces the time of I/O operations by applying transparent compression between the computing and the storage systems. 
	SPIOC can predict whether to compress or not at runtime, allowing parallel or sequential compression techniques, 
	guaranteeing QoS and allowing partial and full reading by decompressing the minimum part of the file. 
	SPIOC maximises the measured efficiency of data movement by applying run-time customising compression before storing data in the Papio storage system
	```
