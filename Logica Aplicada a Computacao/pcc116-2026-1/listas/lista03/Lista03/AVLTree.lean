import Mathlib.Data.Nat.Basic
import Mathlib.Tactic

/-  Árvores AVL indexadas por altura em Lean 4 com Tipos Dependentes.

    O invariante AVL é codificado no próprio tipo:
    - `AVLTree α h` é uma BST sobre α com altura h.
    - Três construtores de nó codificam o fator de balanceamento:
        · nodeL (esquerda com altura maior): filho esq. tem altura n+1, dir. tem altura n
        · nodeE (equilibrado): ambos os filhos têm altura n
        · nodeR (direita com altura maior): filho dir. tem altura n+1, esq. tem altura n

    Esta codificação evita `Nat.max` nos índices de tipo, que não reduz
    definitivamente em Lean 4 e causaria falhas de verificação de tipos.
-/

set_option autoImplicit false

inductive AVLTree (α : Type) : ℕ → Type where
  | leaf  :                                                          AVLTree α 0
  | nodeL {n} : α → AVLTree α (n + 1) → AVLTree α n       →       AVLTree α (n + 2)
  | nodeE {n} : α → AVLTree α n       → AVLTree α n       →       AVLTree α (n + 1)
  | nodeR {n} : α → AVLTree α n       → AVLTree α (n + 1) →       AVLTree α (n + 2)

-- Resultado de insert: a árvore pode manter a altura ou crescer 1.
inductive InsResult (α : Type) (h : ℕ) where
  | same   : AVLTree α h       → InsResult α h
  | taller : AVLTree α (h + 1) → InsResult α h

-- Resultado de delete: a árvore pode manter a altura ou diminuir 1.
-- `.shorter t` codifica "a árvore era de altura h+1 e passou a ter altura h".
inductive DelResult (α : Type) : ℕ → Type where
  | same    {h : ℕ} : AVLTree α h     → DelResult α h
  | shorter {h : ℕ} : AVLTree α h     → DelResult α (h + 1)

namespace AVLTree

-- ════════════════════════════════════════════
-- Altura e busca
-- ════════════════════════════════════════════

def height {α : Type} : {h : ℕ} → AVLTree α h → ℕ
  | h, _ => h

/-- Busca um elemento na AVL. Retorna `true` se encontrado. -/
def search {α : Type} [Ord α] (x : α) : {h : ℕ} → AVLTree α h → Bool :=
  sorry

-- ════════════════════════════════════════════
-- Inserção — funções de rebalanceamento
-- ════════════════════════════════════════════

/-
  balanceLeft_ins_* e balanceRight_ins_* rebalanceiam após inserção
  no filho esquerdo ou direito de um nó nodeE / nodeL / nodeR.

  Nomenclatura: o sufixo indica o tipo do nó pai (antes da inserção).
  · E : nodeE  (filhos iguais)
  · L : nodeL  (esquerda pesada)
  · R : nodeR  (direita pesada)

  Todas as funções retornam `InsResult α h` onde `h` é a altura original
  do nó pai, garantindo que a inserção resulta em "mesma altura" ou
  "1 a mais" — sem usar Nat.max.
-/

def balanceLeft_ins_E {α : Type} {n : ℕ}
    (v : α) (left : InsResult α n) (right : AVLTree α n)
    : InsResult α (n + 1) :=
  sorry

def balanceRight_ins_E {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α n) (right : InsResult α n)
    : InsResult α (n + 1) :=
  sorry

-- Inserção no filho esquerdo de um nó nodeL {n}
--   antes: v com esq.(n+1) dir.(n), altura n+2
--   depois: mesma altura n+2 (com possível rotação)
def balanceLeft_ins_L {α : Type} {n : ℕ}
    (v : α) (left : InsResult α (n + 1)) (right : AVLTree α n)
    : InsResult α (n + 2) :=
  sorry

-- Inserção no filho esquerdo de um nó nodeR {n}
def balanceLeft_ins_R {α : Type} {n : ℕ}
    (v : α) (left : InsResult α n) (right : AVLTree α (n + 1))
    : InsResult α (n + 2) :=
  sorry

-- Inserção no filho direito de um nó nodeL {n}
def balanceRight_ins_L {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α (n + 1)) (right : InsResult α n)
    : InsResult α (n + 2) :=
  sorry

-- Inserção no filho direito de um nó nodeR {n}
def balanceRight_ins_R {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α n) (right : InsResult α (n + 1))
    : InsResult α (n + 2) :=
  sorry

-- ════════════════════════════════════════════
-- Inserção
-- ════════════════════════════════════════════

/-- Insere um elemento na AVL, preservando o invariante AVL.
    Retorna `InsResult α h` indicando se a altura aumentou. -/
def insert {α : Type} [Ord α] (x : α) : {h : ℕ} → AVLTree α h → InsResult α h :=
  sorry

/-- Versão de `insert` que retorna a nova árvore como tipo existencial. -/
def insert' {α : Type} [Ord α] (x : α) {h : ℕ} (t : AVLTree α h) : Σ h', AVLTree α h' :=
  sorry

-- ════════════════════════════════════════════
-- Remoção — funções de rebalanceamento
-- ════════════════════════════════════════════

/-
  balanceLeft_del_* e balanceRight_del_*: rebalanceiam após remoção
  do filho esquerdo ou direito.  O `DelResult α h` carrega a nova
  altura h e indica se a subárvore encolheu (.shorter) ou não (.same).

  Retorno: `DelResult α H` onde H é a altura ORIGINAL do nó pai.
-/

def balanceLeft_del_E {α : Type} {n : ℕ}
    (v : α) (left : DelResult α n) (right : AVLTree α n)
    : DelResult α (n + 1) :=
  sorry

def balanceLeft_del_L {α : Type} {n : ℕ}
    (v : α) (left : DelResult α (n + 1)) (right : AVLTree α n)
    : DelResult α (n + 2) :=
  sorry

def balanceLeft_del_R {α : Type} {n : ℕ}
    (v : α) (left : DelResult α n) (right : AVLTree α (n + 1))
    : DelResult α (n + 2) :=
  sorry

def balanceRight_del_E {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α n) (right : DelResult α n)
    : DelResult α (n + 1) :=
  sorry

def balanceRight_del_L {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α (n + 1)) (right : DelResult α n)
    : DelResult α (n + 2) :=
  sorry

def balanceRight_del_R {α : Type} {n : ℕ}
    (v : α) (left : AVLTree α n) (right : DelResult α (n + 1))
    : DelResult α (n + 2) :=
  sorry

-- ════════════════════════════════════════════
-- removeMin e delete
-- ════════════════════════════════════════════

/-- Extrai e remove o menor elemento de uma AVL não-vazia.
    Devolve `(mínimo, DelResult)` onde o DelResult indica a nova altura. -/
def removeMin {α : Type} : {h : ℕ} → AVLTree α (h + 1) → α × DelResult α (h + 1) :=
  sorry

/-- Remove um elemento da AVL (se presente), preservando o invariante AVL. -/
def delete {α : Type} [Ord α] (x : α) : {h : ℕ} → AVLTree α h → DelResult α h :=
  sorry

/-- Versão de `delete` que retorna a nova árvore como tipo existencial. -/
def delete' {α : Type} [Ord α] (x : α) {h : ℕ} (t : AVLTree α h) : Σ h', AVLTree α h' :=
  sorry

-- ════════════════════════════════════════════
-- Predicado All e invariante BST
-- ════════════════════════════════════════════

/-- `All P t` — todo elemento de `t` satisfaz `P`. -/
def All {α : Type} (P : α → Prop) : {h : ℕ} → AVLTree α h → Prop
  | _, .leaf        => True
  | _, .nodeL v l r => P v ∧ All P l ∧ All P r
  | _, .nodeE v l r => P v ∧ All P l ∧ All P r
  | _, .nodeR v l r => P v ∧ All P l ∧ All P r

/-- Invariante BST: todo nó com valor `v` tem à esq. valores `< v`
    e à dir. valores `> v`. -/
def BST {α : Type} [Ord α] [LT α] : {h : ℕ} → AVLTree α h → Prop
  | _, .leaf        => True
  | _, .nodeL v l r => All (· < v) l ∧ All (v < ·) r ∧ BST l ∧ BST r
  | _, .nodeE v l r => All (· < v) l ∧ All (v < ·) r ∧ BST l ∧ BST r
  | _, .nodeR v l r => All (· < v) l ∧ All (v < ·) r ∧ BST l ∧ BST r

-- ════════════════════════════════════════════
-- Lemas de corretude
-- ════════════════════════════════════════════

section Correctness

variable {α : Type} [Ord α] [LT α]

@[simp]
theorem search_leaf (x : α) : search x (.leaf : AVLTree α 0) = false := by
  sorry

theorem insert_leaf (x : α) :
    insert x (.leaf : AVLTree α 0) = .taller (nodeE x .leaf .leaf) := by
  sorry

@[simp]
theorem delete_leaf (x : α) :
    delete x (.leaf : AVLTree α 0) = .same .leaf := by
  sorry

theorem insert_preserves_all
    (P : α → Prop) (x : α) (hx : P x) :
    ∀ {h : ℕ} (t : AVLTree α h),
      All P t →
      match insert x t with
      | .same   t' => All P t'
      | .taller t' => All P t' := by
  sorry

theorem delete_preserves_all
    (P : α → Prop) (x : α) :
    ∀ {h : ℕ} (t : AVLTree α h),
      All P t →
      match delete x t with
      | .same    t' => All P t'
      | .shorter t' => All P t' := by
  sorry

end Correctness

-- ════════════════════════════════════════════
-- Conversão para/de List
-- ════════════════════════════════════════════

/-- Travessia in-order: produz lista ordenada (se a árvore for BST). -/
def toList {α : Type} : {h : ℕ} → AVLTree α h → List α :=
  sorry

/-- Constrói uma AVL a partir de uma lista, inserindo cada elemento. -/
def fromList {α : Type} [Ord α] (xs : List α) : Σ h, AVLTree α h :=
  sorry

-- ════════════════════════════════════════════
-- Exemplos 
-- ════════════════════════════════════════════

section Examples

-- Exemplos de árvores construídas manualmente para referência:

-- Árvore equilibrada com 3 nós (altura 2):
--        2
--       / \
--      1   3
def exTree123 : AVLTree Nat 2 :=
  AVLTree.nodeE 2 (AVLTree.nodeE 1 .leaf .leaf) (AVLTree.nodeE 3 .leaf .leaf)

-- Verificações de tipos (não exigem implementação):
#check (exTree123 : AVLTree Nat 2)
#check (search : {α : Type} → [Ord α] → α → {h : ℕ} → AVLTree α h → Bool)
#check (insert : {α : Type} → [Ord α] → α → {h : ℕ} → AVLTree α h → InsResult α h)
#check (delete : {α : Type} → [Ord α] → α → {h : ℕ} → AVLTree α h → DelResult α h)
#check (toList : {α : Type} → {h : ℕ} → AVLTree α h → List α)
#check (fromList : {α : Type} → [Ord α] → List α → Σ h, AVLTree α h)

end Examples

end AVLTree
