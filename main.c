/********************************************
	Gas classification based on MLP Neural Network
	Author: Babak Djalaei
	published as a course computer assignment
	Embedded systems processing elements
	University of Tehran
*********************************************/

#include <stdio.h>
#include "data.h"
#include "param.h"
#include <sys/time.h>
//Activation Function
// Profiling.1.start
long long int sigmoid(long long int x)
{
	if(x>20000)
		return 30136;
	if(x<-100000)
		return 0;
	int i;
	long long int scalefactor = 1 << 15;
	x = -x;
	long long int y = scalefactor;
	long long int a = 1 << 30;

	long long int temp = scalefactor;
	for (i = 1; i < 100; i++)
	{
		temp = (temp * x) >> 15;
		temp /= i;
		y += temp;
	}
	y = a / (scalefactor + y);
	return y;
}
//Profiling.1.end

//Profiling.2.start
int predict(int *inputs)
{
	int scaleFactor = 1 << 15, p = 0;
	int i,j,res;
	long long int nOut1[10];
	long long int nOut2[6];
	long long int temp1=0;
	//First Layer
	//Profiling.3.start

	for (i = 0; i < 10; i++)
	{
		temp1 = 0;
		for (j = 0; j < 128; j++){
			//Profiling.6.start
			temp1 += W1[j][i] * inputs[j];	//MAC Instruction
		}

		temp1 = temp1 >> 15;
		temp1 += B1[i];
		nOut1[i] = sigmoid(temp1);			//Activation Function
	}
	//Profiling.3.end
	//Second Layer
	//Profiling.4.start
	for (i = 0; i < 6; i++)
	{
		temp1 = 0;
		for (j = 0; j < 10; j++)
			temp1 += W2[j][i] * nOut1[j];	//MAC Instruction
		temp1 = temp1 >> 15;
		temp1 += B2[i] ;
		nOut2[i] = temp1;
	}
	//Profiling.4.end
	//Output Determination According to Maximum Value Position 
	res = nOut2[0];

	for (i = 1; i < 6; i++){
		temp1 = nOut2[i];
		p = (temp1>res)*i + (temp1<=res)*p;
		res = (temp1>res)*temp1 + (temp1<=res)*res;
	}
	return p + 1;
}
//Profiling.2.end

int main(int argc, char *argv[])
{
	int c = 0,i,j;
	double total_time;
    struct timeval  tv1, tv2;
	gettimeofday(&tv1, NULL);
	
	for (i = 0; i < 100; i++)
		//Profiling.5.start
		if (predict(data[i]) == labels[i])		//Comparing MLP Outputs and Targets
			c++;
		//Profiling.5.end

	gettimeofday(&tv2, NULL);
    printf ("Total time = %f miliseconds\n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000 );

	printf("CCR: %d\n", c);
	// for (unsigned int i = 50000; i < 80000; i+=100)
	// {
		long long int a = sigmoid(-200000);
		printf(" i =: %d %lld\n", i, a);
	// }

	return 0;
}


