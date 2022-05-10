#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *entry_point(void *value){
	printf("%s\n", value);

	int i;
	for(i = 0; i < 100; i++){
		printf("%d - thread 1\n", i);
	}

	return NULL;
}

int main(int argc, char **argv){
	pthread_t thread;

	printf("main\n");


	pthread_create(&thread, NULL, entry_point, "thread 1");

	int i = 200;
	for(i = 0; i < 300; i++){
		printf("%d - thread main\n ", i);
	}

	
	pthread_join(thread, NULL);

	return 0;
}
