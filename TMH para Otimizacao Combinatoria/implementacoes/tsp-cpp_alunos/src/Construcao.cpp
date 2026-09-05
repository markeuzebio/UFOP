#include "Construcao.h"
#include "Aleatorio.h"
#include "Utilitarios.h"

#include <algorithm>  // std::min_element, std::sort, std::find
#include <numeric>    // std::iota
#include <limits>
#include <cstddef>

// ============================================================================
// [EXERCÍCIO] Vizinho Mais Próximo (guloso)
// Aula: Heurística Construtivas
// Slide ref: "PCV - Vizinho mais Próximo"
// Complexidade: O(n^2)
// ============================================================================
double constroiSolucaoGulosaVizinhoMaisProximo(const Instancia &inst,
                                              std::vector<int> &s)
{
    int n = inst.n;

    double fo = 0;
    // Cidades ainda não inseridas na rota (todas, exceto a cidade 0).
    std::vector<int> naoVisitadas(n - 1);

    // Preenche a lista com as cidades de 1 .. (n-1)
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    s.assign(n, -1);
    s[0] = 0; // a cidade origem é sempre a cidade 0

    //TODO
    while(naoVisitadas.size() != 0)
    {
    }

    return fo;
}

// ============================================================================
// [FRAMEWORK] Construção aleatória
// ============================================================================
double constroiSolucaoAleatoria(const Instancia &inst, std::vector<int> &s)
{
    int n = inst.n;

    // Cria a lista de cidades não visitadas
    std::vector<int> naoVisitadas(n - 1);

    // Preenche a lista com as cidades de 1 .. (n-1)
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    embaralhaVetor(naoVisitadas);

    // Redefine a solução com n posições
    s.assign(n, -1);

    // Insere a cidade de origem
    s[0] = 0;

    // Preenche com as demais cidades
    for (int j = 1; j < n; j++) {
        s[j] = naoVisitadas[j - 1];
    }

    return custo(inst, s);
}

// ============================================================================
// [EXERCÍCIO] Vizinho Mais Próximo parcialmente guloso (GRASP)
// ============================================================================
double constroiSolucaoParcialmenteGulosaVizinhoMaisProximo(const Instancia &inst,
                                                           std::vector<int> &s,
                                                           double alpha)
{
    int n = inst.n;
    double fo = 0;

    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    s.assign(n, -1);
    s[0] = 0;

    
    //TODO

    return fo;
}

// ============================================================================
// [EXERCÍCIO] Inserção Mais Barata (gulosa)
// ============================================================================
double constroiSolucaoGulosaInsercaoMaisBarata(const Instancia &inst,
                                              std::vector<int> &s)
{
    int n = inst.n;

    double fo = 0;
    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    std::vector<int> rota;
    rota.reserve(n);
    rota.push_back(0); // a cidade origem é sempre a cidade 0

    // Monta uma subrota inicial com 3 cidades usando o vizinho mais
    // próximo (mais simples do que aplicar inserção mais barata em uma
    // rota com menos de 3 cidades).

    //TODO

    // A cada passo, insere a cidade k -- entre duas cidades i e j
    // consecutivas já presentes na rota -- que resulta no menor custo de
    // inserção: d(i,k) + d(k,j) - d(i,j).

    //TODO

    return fo;
}

// ============================================================================
// [EXERCÍCIO] Inserção Mais Barata parcialmente gulosa (GRASP)
// ============================================================================
// ============================================================================
double constroiSolucaoParcialmenteGulosaInsercaoMaisBarata(const Instancia &inst,
                                                           std::vector<int> &s,
                                                           double alpha)
{
    int n = inst.n;
    double fo = 0;

    std::vector<int> naoVisitadas(n - 1);
    std::iota(naoVisitadas.begin(), naoVisitadas.end(), 1);

    std::vector<int> rota;
    rota.reserve(n);
    rota.push_back(0);

    //TODO

    return fo;
}
