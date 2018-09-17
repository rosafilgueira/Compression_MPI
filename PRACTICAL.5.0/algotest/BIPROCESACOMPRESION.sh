set -x
MESH="1 2 12 13 14"
TYPE="char int float double"
              	for mesh in $MESH 
		do
		for type in $TYPE 
		do
                ./PROCESAOUT compr.out.$mesh.$type.txt
                cp data data.compr 
                ./PROCESAOUT decompr.out.$mesh.$type.txt
                cp data data.decompr 
	   	P=$( wc -l < data.compr )	
	   	NUM_TOT=1
	   	while test $NUM_TOT -le $P #tantos saltos como mpirun
            	do  	
                 STRING1=`head -n $NUM_TOT data.compr | tail -1 | awk '{print $1}'`
                 STRING2=`head -n $NUM_TOT data.compr | tail -1 | awk '{print $2}'`
	         STRING3=$(echo "scale=10; $STRING1/$STRING2" | bc -l)
                 STRING4=`head -n $NUM_TOT data.compr | tail -1 | awk '{print $3}'`
                 STRING5=`head -n $NUM_TOT data.decompr | tail -1 | awk '{print $3}'`
	         STRING6=$(echo "scale=10; $STRING4 + $STRING5" | bc -l)
	         
                 echo $STRING3 $STRING6>> Data-$mesh.$type
		NUM_TOT=`expr $NUM_TOT + 1`
             done
                done
              done


TYPE="char int float double"
		for type in $TYPE 
		do
	   	P=$( wc -l < Data-1.$type )	
	   	NUM_TOT=1
	   	while test $NUM_TOT -le $P #tantos saltos como mpirun
            	do  	
			
                 	STRING1=`head -n $NUM_TOT Data-1.$type | tail -1 | awk '{print $1}'`
                 	STRING2=`head -n $NUM_TOT Data-2.$type | tail -1 | awk '{print $1}'`
                 	STRING3=`head -n $NUM_TOT Data-12.$type | tail -1 | awk '{print $1}'`
                 	STRING4=`head -n $NUM_TOT Data-13.$type | tail -1 | awk '{print $1}'`
                 	STRING5=`head -n $NUM_TOT Data-14.$type | tail -1 | awk '{print $1}'`
                 	STRING6=`head -n $NUM_TOT Data-1.$type | tail -1 | awk '{print $2}'`
                 	STRING7=`head -n $NUM_TOT Data-2.$type | tail -1 | awk '{print $2}'`
                 	STRING8=`head -n $NUM_TOT Data-12.$type | tail -1 | awk '{print $2}'`
                 	STRING9=`head -n $NUM_TOT Data-13.$type | tail -1 | awk '{print $2}'`
                 	STRING10=`head -n $NUM_TOT Data-14.$type | tail -1 | awk '{print $2}'`
		        echo $STRING1 $STRING2 $STRING3 $STRING4 $STRING5 $STRING6 $STRING7 $STRING8 $STRING9 $STRING10	>>Out.$type
		NUM_TOT=`expr $NUM_TOT + 1`
                done
	done	
