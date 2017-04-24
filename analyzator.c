#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void nacitaj_postupnost(int subor,unsigned char *n,float **x,int row){
	int i;
	float p;
	
	read(subor,n,sizeof(unsigned char));
	
	x[row] =(float *) malloc(sizeof(float) * (*n));
	
	for(i=0; i<(*n); i++){
		read(subor,&p,sizeof(float));
		x[row][i] = p;
	}
}

int pocet_postupnosti(int subor,int file_size){
	int i=0,pocet=0;
	unsigned char n;
	
	while (i<file_size){
		read(subor,&n,sizeof(unsigned char));
		lseek(subor,n*sizeof(float),SEEK_CUR);
		i += sizeof(unsigned char) + n*sizeof(float);
		pocet++;
	}
	return pocet;
}

float average(float *postupnost,unsigned char n){
	int i;
	float sum=0,priemer;
	
	for (i=0; i<n; i++){
		sum += postupnost[i];
	}
	priemer = sum / n;
	
	return priemer;
}

void vypis_postupnosti(float **postupnosti,unsigned char *velkost,int pocet_pos){
	int i,j;
	
	for (i=0; i<pocet_pos; i++){
		for (j=0; j<velkost[i]; j++){
			printf("%f ",postupnosti[i][j]);
		}
		printf("\n");
	}
}

int main( int argc, char *argv[] )  {
	int subor,file_size,pocet_pos,i,j;
	float priemer=0,q;
	
	subor = open(argv[1],O_RDONLY|O_BINARY,S_IWUSR);
	
	if (subor < 0){
		printf("Subor sa nepodarilo otvorit");
		return 0;
	}
	
	file_size = lseek(subor, 0, SEEK_END);
    lseek(subor,0,SEEK_SET);
    
    
    pocet_pos = pocet_postupnosti(subor,file_size);
    
    lseek(subor,0,SEEK_SET);
    
    float **postupnosti = (float **)malloc(pocet_pos * sizeof(float*));
    unsigned char *velkost_pos = (unsigned char*)malloc(pocet_pos * sizeof(unsigned char));
    
	for (i=0; i<pocet_pos; i++){
		nacitaj_postupnost(subor,velkost_pos+i*sizeof(unsigned char),postupnosti,i);
		priemer += average(postupnosti[i],velkost_pos[i]);
	}
	
	close(subor);
	
	priemer = priemer / pocet_pos;
	
	printf("Postupnosti je %d\n",pocet_pos);
	printf("Priemer priemerov postupnosti je %f\n\n----------\n\n",priemer);
	
	vypis_postupnosti(postupnosti,velkost_pos,pocet_pos);
}
