.PHONY: clean

RM = rm -f

sugang_doumi: sugang_doumi.o login.o credit.o manage_schedule.o 
	gcc -o sugang_doumi sugang_doumi.o login.o credit.o manage_schedule.o 

sugang_doumi.o: sugang_doumi.c sugang_doumi.h
	gcc -Wall -g -c sugang_doumi.c

login.o: login.c sugang_doumi.h
	gcc -Wall -g -c login.c

credit.o: credit.c sugang_doumi.h
	gcc -Wall -g -c credit.c

manage_schedule.o: manage_schedule.c sugang_doumi.h
	gcc -Wall -g -c manage_schedule.c

clean:
	$(RM) *.o
