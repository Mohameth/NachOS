# NachOS Questions pdf

> Preview Markdown depuis **VSCODE**
 CTRL+SHIFT+M


> Conversion Markdown en PDF:
  Installer extension **VSCODE**:  yzane.markdown-pdf
Puis: CTRL+SHIFT+p   >taper: export pdf

## Etape 1

### 2.2 Initialisation du système

#### Indiquez sur le schéma le nom des classes C++ qui codent ces éléments. Précisez dans quelle mesure ces éléments appartiennent au mode noyau ou à la simulation de matériel.

> todo

#### Comment est créé ce premier thread noyau ? C’est-à-dire d’où viennent sa pile et ses registres ?

> L'instanciation d'une classe thread et l'appel de la fonction Fork(). La pile et le registre viennent du fichier machine.h.

#### Quel est le rôle (futur) de la structure de données allouée par `currentThread = new Thread("main");`

> Cette structure décrit le thread, elle contient les registres, piles ainsi que le status du thread. 

#### Les prochains threads noyaux pourront-ils être créés de la même façon ?

> Oui

### 2.3 Execution d'un programme utilisateur

#### Repérez dans le code de la fonction `Initialize()` l’allocation du processeur MIPS.

> `machine = new Machine (debugUserProg);`

#### Comment sont initialisés les registres de ce processeur ?

> Dans la classe machine du fichier *machine.cc*.

#### Quelle variable représente la mémoire MIPS ?

> `mainMemory = new char[MemorySize];`

#### `StartProcess()` emplacement

> *userprog/progtest.cc*   *test/halt.c* oui d'après le debugger

#### Machine::Run fonction qui exécute une instruction MIPS

> *machine/mipssim.cc*, Machine::OneInstruction execute une instruction
Ligne 144 RaiseException(OverflowException, 0); **OP_ADD**


#### Interruption lors de l'execution du programme.

> `while (CheckIfDue(FALSE))` dans la fonction `Interrup::OneTick()` fichier *interrupt.cc*.

### 3.1

#### Pourquoi la première instruction MIPS est elle exécutée au dixième tick d’horloge ? (cf la fonction Interrupt::OneTick() et l’initialisation du système)

> ?

### 3.2 Observation des threads noyau

#### Ajoutez le lancement d’un thread supplémentaire dans la fonction ThreadTest().

```cc
Thread *t = new Thread ("forked thread");
Thread *s = new Thread ("thread sylvestre");

t->Fork (SimpleThread, 1);
s->Fork (SimpleThread, 2);
```

#### Il marche toujours ?

> Oui, on peut observer l'execution de 3 threads.



