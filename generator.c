#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>

void napis_postupnost(int subor){
	int i;
	float x;
	unsigned char n;
	
	n = rand() % 11 + 10;
	write(subor,&n,sizeof(unsigned char));
	
	for (i=0; i<n; i++){
		x = rand() + (float)rand()/((float)RAND_MAX);
		write(subor,&x,sizeof(float));
	}
}


int main( int argc, char *argv[] )  {
	int n = atoi(argv[2]);
	int subor,i;
	
	if(argc != 3){
		printf("Expected 2 argument");
		return 0;
	}
	
	srand((unsigned int) time(NULL));
	
	subor = open(argv[1],O_RDWR|O_CREAT|O_BINARY,S_IWUSR);
	
	if (subor < 0) printf("Subor sa nepodarilo otvorit");
    
    for (i=0; i<n; i++){
    	napis_postupnost(subor);
	}
	
	close(subor);
}
