import Pcc116.Aula12
import Mathlib.Data.Nat.Fib.Basic

set_option autoImplicit false
set_option tactic.hygienic false
set_option linter.hashCommand false

/-!
# Lista 04 — Verificação de Programas IMP com Lógica de Hoare

1. **Fibonacci iterativo** — calcula o n-ésimo número de Fibonacci por repetição.
2. **Divisão inteira** — calcula o quociente e o resto da divisão de n por m.
-/


/-! ## 1. Fibonacci Iterativo

Algoritmo em IMP:
```
a := 0;  b := 1;  i := 0;
while i ≠ n do
  tmp := a + b;
  a   := b;
  b   := tmp;
  i   := i + 1
```

**Invariante do laço**: `a = F(i) ∧ b = F(i+1) ∧ i ≤ n ∧ n = n₀`

**Pós-condição**: `a = Nat.fib n₀`
-/

def fibACom (n₀ : ℕ) : ACom :=
  .assign "a"   (.num 0)                        ;;ₐ
  .assign "b"   (.num 1)                        ;;ₐ
  .assign "i"   (.num 0)                        ;;ₐ
  .whl
    -- invariante: a = F(i), b = F(i+1), i ≤ n, n = n₀
    (fun s => s "a" = Nat.fib (s "i") ∧
              s "b" = Nat.fib (s "i" + 1) ∧
              s "i" ≤ s "n" ∧
              s "n" = n₀)
    -- guarda: i ≠ n
    (.not (.eq (.var "i") (.var "n")))
    -- corpo: passo de Fibonacci com troca de variáveis
    (.assign "tmp" (.add (.var "a") (.var "b"))  ;;ₐ
     .assign "a"   (.var "b")                    ;;ₐ
     .assign "b"   (.var "tmp")                  ;;ₐ
     .assign "i"   (.add (.var "i") (.num 1)))

/-- **Teorema de correção de Fibonacci**:
    Se a variável `"n"` vale `n₀` antes da execução,
    então após a execução a variável `"a"` vale `Nat.fib n₀`. -/
theorem fib_correct (n₀ : ℕ) :
    {{* (fun s => s "n" = n₀) *}}
        (erase (fibACom n₀))
    {{* (fun s => s "a" = Nat.fib n₀) *}} := by
    sorry 

/-! ## 2. Divisão Inteira

Algoritmo em IMP:
```
q := 0;  r := n;
while m ≤ r do
  r := r - m;
  q := q + 1
```

**Invariante do laço**: `n = q * m + r`

**Pós-condição**: `n = q * m + r  ∧  r < m`

O programa computa a divisão de Euclides: ao terminar, `q = n / m` e `r = n % m`
(para qualquer valor de `m`, inclusive `m = 0`, onde o laço nunca termina e a
tripla é vacuamente válida — correção parcial).
-/

/-- Programa IMP anotado que calcula o quociente e o resto de `n ÷ m`. -/
def divACom : ACom :=
  .assign "q" (.num 0)        ;;ₐ
  .assign "r" (.var "n")      ;;ₐ
  .whl
    -- invariante: n = q * m + r
    (fun s => s "n" = s "q" * s "m" + s "r")
    -- guarda: m ≤ r
    (.le (.var "m") (.var "r"))
    -- corpo: subtrai m de r e incrementa q
    (.assign "r" (.sub (.var "r") (.var "m"))  ;;ₐ
     .assign "q" (.add (.var "q") (.num 1)))

/-- **Teorema de correção da divisão**:
    Ao terminar, `n = q * m + r` e `r < m`. -/
theorem div_correct :
    {{* (fun _ => True) *}}
        (erase divACom)
    {{* (fun s => s "n" = s "q" * s "m" + s "r" ∧ s "r" < s "m") *}} := by
 sorry
