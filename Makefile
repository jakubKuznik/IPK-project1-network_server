CC = gcc
LD = gcc

CFLAGS = -std=gnu99 -pedantic -Wall -Wextra -pthread  
#CFLAGS = -g -std=c99 -pedantic -Wall -Wextra -Werror -lpthread   

all: hinfosvc hinfosvc.o 
#$< jmeno první závislosti		#$@ jmeno cile 		#$^ jmena vsech zavislosti

FILES = *

############## proj2 ###############
hinfosvc: hinfosvc.o
	$(CC) $(CFLAGS) $< -o $@
hinfosvc.o: hinfosvc.c hinfosvc.h
	$(CC) $(CFLAGS) -c $< -o $@ 
#####################################

clean:
	rm *.o hinfosvc
zip:
	zip xkuzni04.zip *.c *.h Makefile
git:
	sudo git add *.c *.h Makefile
	sudo git commit -m "Update."
	sudo git push

merlin:
	scp -r $(FILES) xkuzni04@merlin.fit.vutbr.cz:/homes/eva/xk/xkuzni04/ipk/proj1
