#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => isto é uma instrução
    // 02|33|12|01 => isto é outra instrução
            
    // 0 => salvar na memória
    // 1 => opcode => somar
    // 2 => opcode => subtrair
    //-1 => halt
    
    // 22 => significa um endereço da RAM (10 endereço) 
    // 13 => significa 2o endereço
    // 45 => significa 3o endereco
    //ramSize => ESTA FORA DO INTERVALO DE 0 A ramSize DA MEMÓRIA RAM

    Instruction* instructions = (Instruction*) malloc(10 * sizeof(Instruction));

    for (int i=0; i<9; i++){
        instructions[i].opcode = rand() % 4; //0, 1, 2, 3
        instructions[i].info1 = rand() % ramSize; //0 ... RAM_SIZE
        do {
            instructions[i].info2 = rand() % ramSize; //0 ... RAM_SIZE
        } while (instructions[i].info1 == instructions[i].info2);
        instructions[i].info3 = rand() % ramSize; //0 ... RAM_SIZE
    }
    
    //inserindo a ultima instrucao do programa que nao faz nada que presta
    instructions[9].opcode =-1;
    instructions[9].info1 = -1;
    instructions[9].info2 = -1;
    instructions[9].info3 = -1;
    
    return instructions;
}

Instruction* gerarMultiplicacaoViaInstrucoes(int multiplicador, int multiplicando)
{
	// As primeirasgenerator.c.bkp_2 duas posições são os valores que serão multiplicados.
	// A última, a instrução de HALT. Logo, aloca mais 3 posições.
	Instruction* instructions = (Instruction*) malloc((multiplicador + 3) * sizeof(Instruction));

	// A primeira instrução é a de MOVER (opcode = 0) o multiplicando
	// para a posição 0 da RAM (info2 = 0).
	instructions[0].opcode = 0;
	instructions[0].info1 = multiplicando;
	instructions[0].info2 = 0;


	// A segunda instrução é a de MOVER (opcode = 0) o valor 0
	// para a posição 1 da RAM (info1 = 0).
	instructions[1].opcode = 0;
	instructions[1].info1 = 0;
	instructions[1].info2 = 1;

	// "Cria" instruções a partir da terceira posição.
	// Essas instruções estão somando o que está na posição
	// 0 da RAM (multiplicando) com o que está na posição
	// 1 da RAM (inicialmente, o valor 0) e armazenando o
	// resultado na posição 1 da RAM.
	for(int i = 0 ; i < multiplicador ; i++)
	{
		instructions[i + 2].opcode = 1;
		instructions[i + 2].info1 = 0;
		instructions[i + 2].info2 = 1;
		instructions[i + 2].info3 = 1;
	}	

	instructions[multiplicador + 2].opcode = -1;
	instructions[multiplicador + 2].info1 = -1;
	instructions[multiplicador + 2].info2 = -1;

	return instructions;
}

Instruction* gerarDivisaoViaInstrucoes(int dividendo, int divisor)
{
	// As primeiras duas posições são os valores que serão multiplicados.
	// A terceira diz respeito à memória na RAM que receberá o resultado de uma subtração continuamente.
	// A quarta é a instrução HALT.
	Instruction* instructions = (Instruction*) malloc((dividendo/divisor + 4) * sizeof(Instruction));
	int PC = 0;

	instructions[0].opcode = 0;
	instructions[0].info1 = dividendo;
	instructions[0].info2 = 0;

    instructions[1].opcode = 0;
    instructions[1].info1 = divisor;
	instructions[1].info2 = 1;

	for(int i = dividendo ; i >= divisor ; i -= divisor)
	{
		instructions[PC + 2].opcode = 2;
		instructions[PC + 2].info1 = 0;
		instructions[PC + 2].info2 = 1;
		instructions[PC + 2].info3 = 0;

		PC++;
	}

	instructions[dividendo/divisor + 2].opcode = 0;
	instructions[dividendo/divisor + 2].info1 = PC;
    instructions[dividendo/divisor + 2].info2 = 2;

	instructions[dividendo/divisor + 3].opcode = -1;
    instructions[dividendo/divisor + 3].info1 = -1;
    instructions[dividendo/divisor + 3].info2 = -1;
    instructions[dividendo/divisor + 3].info3 = -1;

	return instructions;
}

Instruction* gerarExponenciacaoViaInstrucoes(int base, int expoente)
{
	int gruposDeInstrucoesAlocadas = expoente;
	int counter = 0;

	Instruction **instruction_aux = (Instruction**) malloc(gruposDeInstrucoesAlocadas * sizeof(Instruction*));
	
	instruction_aux[0] = (Instruction*) malloc(2 * sizeof(Instruction));
	(instruction_aux[0])[0].opcode = 0;
	(instruction_aux[0])[0].info1 = base;
	(instruction_aux[0])[0].info2 = 1;

	(instruction_aux[0])[1].opcode = -1;
    (instruction_aux[0])[1].info1 = -1;
    (instruction_aux[0])[1].info2 = -1;
    (instruction_aux[0])[1].info3 = -1;

	counter++;

	while(counter < gruposDeInstrucoesAlocadas)
	{
        instruction_aux[counter] = gerarMultiplicacaoViaInstrucoes(base, base);

		counter++;
	}

	for(int i = 1 ; i < gruposDeInstrucoesAlocadas ; i++)
	{
		(instruction_aux[i])[0].opcode = 3;
		(instruction_aux[i])[0].info1 = 1;
		(instruction_aux[i])[0].info2 = 0;
	}

	// Primeiro 1: uma instrução alocada para colocar o valor base na memória.
	// gruposDeInstrucoesAlocadas - 1: quantidade de vezes que foi necessário multiplicar o valor.
	// base + 2: cada multiplicação precisa de base + 3 instruções, só que 1 é descontada por conta do OPCode, que não é capturado.
	// Último 2: necessário para se ter duas instruções a mais: uma levando o valor resultante para a posição 0 da RAM.
	//			 e o outro por conta do comando HALT
	int quantidadeTotalDeInstrucoes = 1 + ((gruposDeInstrucoesAlocadas - 1) * (base + 2)) + 2;
	int k = 0;

	Instruction *instructions = (Instruction*) malloc((quantidadeTotalDeInstrucoes) * sizeof(Instruction));

	for(int i = 0 ; i < gruposDeInstrucoesAlocadas ; i++)
	{
		for(int j = 0 ; (instruction_aux[i])[j].opcode != -1 ; j++)
		{
			instructions[k] = (instruction_aux[i])[j];

			k++;
		}
	}

	instructions[quantidadeTotalDeInstrucoes - 2].opcode = 3;
    instructions[quantidadeTotalDeInstrucoes - 2].info1 = 1;
    instructions[quantidadeTotalDeInstrucoes - 2].info2 = 0;

	instructions[quantidadeTotalDeInstrucoes - 1].opcode = -1;
    instructions[quantidadeTotalDeInstrucoes - 1].info1 = -1;
    instructions[quantidadeTotalDeInstrucoes - 1].info2 = -1;
    instructions[quantidadeTotalDeInstrucoes - 1].info3 = -1;

	return instructions;
}

Instruction* readInstructions(char* fileName, int* ramSize) {
    printf("FILE -> %s\n", fileName);
    FILE* file = fopen(fileName, "r"); // Abrindo arquivo no modo leitura
    
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto.\n");
        exit(1);
    }

    int n, i = 0;
    fscanf(file, "%d %d", ramSize, &n);
    Instruction* instructions = (Instruction*) malloc(n * sizeof(Instruction));
    while (i < n) {
        fscanf(file, "%d %d %d %d", &instructions[i].opcode, &instructions[i].info1, &instructions[i].info2, &instructions[i].info3);
        i++;
    }
    fclose(file); // Fechando o arquivo

    return instructions;
}
