all: instructionsGenerator.o instructionsGeneratorNotSoRandom.o
	@gcc instructionsGenerator.o -o instructionsGenerator.exe 
	@gcc instructionsGeneratorNotSoRandom.o -o instructionsGeneratorNotSoRandom.exe
	@rm instructionsGenerator.o instructionsGeneratorNotSoRandom.o
instructionsGenerator.o:
	@gcc -c instructionsGenerator.c -Wall
instructionsGeneratorNotSoRandom.o:
	@gcc -c instructionsGeneratorNotSoRandom.c -Wall
