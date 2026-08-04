import Mathlib.Data.Nat.Basic

/- Sorted lists -/

inductive Sorted : List ℕ → Prop where
  | nil  : Sorted []
  | single (x : ℕ) : Sorted [x]
  | two_or_more (x y : ℕ)
                {zs : List ℕ}
                (hle : x ≤ y)
                (hsorted : Sorted (y :: zs)) :
      Sorted (x :: y :: zs)

/- Permutations -/

inductive Perm {a : Type} : List a → List a → Prop where
  | nil  : Perm [] []
  | skip (x : a) {xs ys : List a} :
      Perm xs ys →
      Perm (x :: xs) (x :: ys)
  | swap (x y : a) (xs : List a)  :
      Perm (x :: y :: xs) (y :: x :: xs)
  | trans {xs ys zs : List a}     :
      Perm xs ys →
      Perm ys zs →
      Perm xs zs

infix:50 " ~ " => Perm

lemma Perm_refl {a : Type} : ∀ (xs : List a), xs ~ xs := by
  intro xs
  induction xs with
  | nil          => exact Perm.nil
  | cons x xs IH =>
    apply Perm.skip
    assumption

lemma Perm_sym {a : Type} : ∀ (xs ys : List a), xs ~ ys → ys ~ xs := by
  intros xs ys H
  induction H with
  | nil => exact Perm.nil
  | skip x _ IH =>
    apply Perm.skip
    assumption
  | swap x y zs =>
    apply Perm.swap
  | trans _ _ IH1 IH2 =>
    apply Perm.trans
    · assumption
    · assumption


-- ─────────────────────────────────────────────
-- Exercício 1.  removeMin
-- ─────────────────────────────────────────────

/-- A função removeMin remove o menor elemento de 
    uma lista não-vazia. O resultado deve ser um 
    par (m, rs), em que m é o valor mínimo e rs 
    os demais elementos da lista. -/

def removeMin : List ℕ → Option (ℕ × List ℕ)
  := sorry  

-- ─────────────────────────────────────────────
-- Exercício 2.  selectionSort
-- ─────────────────────────────────────────────

/-- Ordena uma lista de naturais por seleção 
    sucessiva do mínimo. -/

def selectionSort : List ℕ → List ℕ 
  := sorry 

-- ─────────────────────────────────────────────
-- Casos de teste
-- ─────────────────────────────────────────────

#eval removeMin [3, 1, 4, 1, 5, 9]   -- some (1, [3, 4, 1, 5, 9])
#eval selectionSort [3, 1, 4, 1, 5, 9, 2, 6]  -- [1, 1, 2, 3, 4, 5, 6, 9]
#eval selectionSort ([] : List Nat)   -- []
#eval selectionSort [42]              -- [42]

-- ─────────────────────────────────────────────
-- Exercício 3.  Lemas sobre removeMin
-- ─────────────────────────────────────────────

theorem removeMin_nonempty 
    {xs : List ℕ} 
    {m rest} 
    (h : removeMin xs = some (m, rest)) 
    : xs ≠ [] := by
  intro heq; simp [heq, removeMin] at h

theorem removeMin_length 
    {xs : List ℕ} 
    {m rest}
    (h : removeMin xs = some (m, rest)) 
    : rest.length < xs.length := sorry 

theorem removeMin_perm 
    {xs : List ℕ} 
    {m rest}
    (h : removeMin xs = some (m, rest)) 
    : xs.Perm (m :: rest) := sorry 

theorem removeMin_min 
    {xs : List ℕ} 
    {m rest}
    (h : removeMin xs = some (m, rest)) 
    : ∀ y ∈ rest, m ≤ y := sorry 

-- ─────────────────────────────────────────────
-- Exercício 4.  Prova de Permutação
-- ─────────────────────────────────────────────

theorem selectionSort_perm 
  : ∀ xs : List ℕ, xs ~ (selectionSort xs) := sorry 

-- ─────────────────────────────────────────────
-- Exercício 5.  Prova de Ordenação
-- ─────────────────────────────────────────────

def allGe (lb : ℕ) (xs : List ℕ) : Prop 
  := ∀ x ∈ xs, lb ≤ x

theorem selectionSort_allGe 
    {lb : Nat} 
    {xs : List ℕ}
    (h : allGe lb xs) 
    : allGe lb (selectionSort xs) := sorry  

theorem selectionSort_sorted 
  : ∀ xs : List ℕ, Sorted (selectionSort xs) := sorry 

theorem selectionSort_correct (xs : List Nat) :
    xs ~ (selectionSort xs) ∧ 
    Sorted (selectionSort xs) := sorry
