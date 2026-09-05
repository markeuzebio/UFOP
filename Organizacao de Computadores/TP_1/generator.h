#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction* generateMultiplicationInstructions(int, int);
Instruction* readInstructions(char*, int*);
Instruction* gerarMultiplicacaoViaInstrucoes(int, int);
Instruction* gerarDivisaoViaInstrucoes(int dividendo, int divisor);
Instruction* gerarExponenciacaoViaInstrucoes(int base, int expoente);

#endif // !GENERATOR_H
