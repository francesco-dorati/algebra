#include <stdio.h>
#include <stdlib.h>

#define MAX_MAT 10

typedef struct {
  int num;
  int den;
} frz;

typedef struct {
  frz mat[MAX_MAT][MAX_MAT];
  int righe;
  int colonne;
} mat;

// MENU
int ask_int(int, int);
int matrici_menu();

// FRAZIONI
frz frazione(int, int); // scompone in minimi
frz inversa_f(frz);
frz somma_f(frz, frz);
frz differenza_f(frz, frz);
frz prodotto_f(frz, frz);
frz prodotto_scal_f(frz, int);
frz divisione_f(frz, frz);
int confronta_f(frz, frz); // -1 "a" piu grande, 1 "b" piu grande
frz estrai_f(char *);
void stampa_f(frz, const char *);

// MATRICI
mat richiedi_matrice();
mat somma_m(mat, mat);
mat prodotto_m(mat, mat);
mat prodotto_scal_m(mat, frz);
mat trasposta_m(mat);
mat scambia_righe_m(mat, int, int);
mat moltiplica_riga(mat, int, frz);
mat differenza_righe_m(mat, int, frz, int);
mat scala_m(mat);
int rango_m(mat);
frz determinante_m(mat);
mat inversa_m(mat);
void stampa_m(mat, const char *);
// void stampa_matrice(mat);

// SISTEMI

/*
TODO
  - workspace con varaibili 
  - salvate in un file
  - possibilit√† di cambiare/eliminare workspace

Numeri Reali
  - frz = rad(2)/rad(5)

MATRICI
  - inversa di matrice
  - salva matrici risultato

FUNZIONI LINEARI (?)
salvate come matrici e vettori
  - stampa sistema lista di stringhe
  - crea sistema da matrice / stringhe
  - ker e imm
  - cabio di base
  - polinomio caratteristico
  - diagonalizzata

*/

/* MAIN */
int main() {
  int s;
  while (1) {
    printf("\nMATH\n\n");
    printf("Opzioni:\n");
    printf("1) Matrici\n");
    printf("2) Numeri Immaginari\n");
    printf("3) Piani/Rette\n");
    printf("0) Esci\n\n");
    s = ask_int(0, 3);

    switch (s) {
      case 1:
        // Matrici
        matrici_menu();
        break;
      case 2:
        // Numeri Immaginari
        break;
      case 3:
        // Piani/Rette
        break;
      case 0:
        return 0;
    }
  }
}

int ask_int(int from, int to) {
  int s;
  do {
    fflush(stdin);
    printf("> ");
  } while (!scanf("%d", &s) || s < from || s > to);
  return s;
}

/* FRAZIONI */
frz frazione(int n, int d) {
  frz f;
  int min, i, abs_num = abs(n), abs_den = abs(d);
  
  f.num = n;
  f.den = d;
  
  // divisione per 0
  if (!d) {
    f.num = 0;
    return f;
  }

  // 0
  if (!n) {
    f.den = 1;
    return f;
  }

  // gestisci il segno
  if (d < 0) {
    if (n < 0) {
      // tutti negativi
      f.num = abs(f.num);
      f.den = abs(f.den);
    } else {
      // denominatore negativo
      f.num = -abs(f.num);
      f.den = abs(f.den);
    }
  }
  
  // semplifica i termini
  min = abs_num < abs_den ? abs_num : abs_den;
  for (i = min; i > 1; i--) {
    if (f.num % i == 0 && f.den % i == 0) {
      f.num /= i;
      f.den /= i;
    }
  }

  return f;
}

frz inversa_f(frz f) {
  return frazione(f.den, f.num);
}

frz somma_f(frz a, frz b) {
  if (a.den == b.den) 
    return frazione(a.num+b.num, a.den);
  
  return frazione(a.num * b.den + b.num * a.den, a.den * b.den);
}

frz differenza_f(frz a, frz b) {
  if (a.den == b.den) 
    return frazione(a.num-b.num, a.den);
  
  return frazione(a.num * b.den - b.num * a.den, a.den * b.den);
}

frz prodotto_f(frz a, frz b) {
  return frazione(a.num * b.num, a.den * b.den);
}

frz prodotto_scal_f (frz f, int n) {
  return frazione(f.num * n, f.den);
}

frz divisione_f (frz a, frz b) {
  return prodotto_f(a, inversa_f(b));
}

int confronta_f(frz a, frz b) {
  int common_den = a.den * b.den;
  if (a.num * common_den == b.num * common_den)
    return 0;
  else if (a.num * common_den > b.num * common_den)
    return -1;
  else
    return 1;
  
}

frz estrai_f(char *s) {
  int i, frz_index = -1, a, b = 1;
  for (i = 0; s[i] != '\0'; i++)
    if (s[i] == '/') frz_index = i;

  a = atoi(s);
  if (frz_index != -1)
    b = atoi(s + frz_index + 1);

  return frazione(a, b);
}

void stampa_f(frz f, const char *a) {
  if (f.den == 1) {
    printf("%d%s", f.num, a);
    return;
  }
  printf("%d/%d%s", f.num, f.den, a);
}



/* MATRICI */
int matrici_menu() {
  int scelta;
  mat m[2] = {{{}, 0, 0}, {{}, 0, 0}}, tmp, scal;

  while (1) {
    printf("\n\nMATRICI\n\n");
    printf("A:\n");
    m[0].righe ? stampa_m(m[0], "\n\n") : printf("Non ancora definita.\n\n");
    printf("B:\n");
    m[1].righe ? stampa_m(m[1], "\n\n") : printf("Non ancora definita.\n\n");
    
    if (!m[0].righe && !m[1].righe) {
      printf("1) Nuova Matrice\n");
      printf("0) Indietro\n");
      scelta = ask_int(0, 1);
    } else {
      printf("1) Nuova Matrice\n");
      printf("2) Operazioni\n");
      printf("3) Riduci a scala\n");
      printf("4) Rango\n");
      printf("5) Trasposta\n");
      printf("6) Determinante\n");
      printf("7) Inversa\n");
      printf("0) Indietro\n");
      scelta = ask_int(0, 7);
    }

    switch (scelta) {
      case 1:
        // nuova matrice
        tmp = richiedi_matrice();
        printf("\nMatrice inserita:\n");
        stampa_m(tmp, "\n\n");
        printf("Dove vuoi salvarla?\n");
        printf("1) A %s\n", m[0].righe ? "(sovrascrivi)" : "");
        printf("2) B %s\n", m[1].righe ? "(sovrascrivi)" : "");
        printf("0) Annulla\n");
        switch (ask_int(0, 2)) {
          case 1:
            m[0] = tmp;
            break;
          case 2:
            m[1] = tmp;
            break;
        }
        break;
      case 2:
        // operazioni
        printf("\n\n1) A + B\n");
        printf("2) A - B\n");
        printf("3) A * B\n");
        printf("0) Indietro\n");
        switch(ask_int(0, 3)) {
          case 1:
            break;
        }
        break;
      case 3:
        // scala
        printf("\n\nQuale matrice vuoi ridurre a scala?\n");
        printf("1) A %s\n", m[0].righe ? "" : "[non definita]");
        printf("2) B %s\n", m[1].righe ? "" : "[non definita]");
        printf("0) Indietro\n");

        scelta = ask_int(0, 2);
        switch (scelta) {
          case 1:
            if (!m[0].righe) {
              printf("\nMatrice non definita.\n");

            } else {
              printf("\nMatrice A ridotta a scala:\n");
              scal = scala_m(m[0]);
              stampa_m(scal, "\n\n");
            }
            break;
          case 2:
            if (!m[1].righe) 
              printf("\nMatrice non definita.\n");
            else {
              printf("\nMatrice B ridotta a scala:\n");
              scal = scala_m(m[1]);
              stampa_m(scal, "\n\n");
            }
            break;
        }
        if (scelta == 1 || scelta == 2) {
          printf("1) Salva in A %s\n", m[0].righe ? "(sovrascrivi)" : "");
          printf("2) Salva in B %s\n", m[1].righe ? "(sovrascrivi)" : "");
          printf("0) Non salvare.\n");
          switch (ask_int(0, 2)) {
          case 1:
            m[0] = scal;
            break;
          case 2:
            m[1] = scal;
            break;
          }
        }
        break;

      case 4:
        // rango
        printf("\n\nRango A: %d\n\n", rango_m(m[0]));
        break;
      case 5:
        // trasposta
        // printf("\nRango della Matrice: %d\n\n", rango_m(a));
        // ask_int(0, 0);
        printf("\n\nTrasposta A: \n");
        stampa_m(trasposta_m(m[0]), "\n\n");
        ask_int(0, 0);
        break;
      case 6:
        // determinante
        printf("\n\nDeterminante A: ");
        stampa_f(determinante_m(m[0]), "\n\n");
        ask_int(0, 0);
        break;
      case 7:
        // inversa
        printf("\n\nInversa A: \n");
        stampa_m(inversa_m(m[0]), "\n\n");
        ask_int(0, 0);
        break;
      case 0:
        return 0;
    }
  }
  return 0;
}

mat richiedi_matrice() {
  int righe, colonne, r, c;
  mat m;

  // dimensione
  printf("\nInserisci dimensione matrice [massimo %d]:\n", MAX_MAT);
  do {
    fflush(stdin);
    printf("righe> ");
  } while (!scanf("%d", &righe) || righe < 0 || righe > MAX_MAT);
  do {
    fflush(stdin);
    printf("colonne> ");
  } while (!scanf("%d", &colonne) || colonne < 0 || colonne > MAX_MAT);

  m.righe = righe;
  m.colonne = colonne;

  printf("\nMatrice %dx%d:\n", m.righe, m.colonne);

  // inserimento valori
  for (r = 0; r < m.righe; r++) {
    fflush(stdin);
    printf("Inserisci elementi della riga %d [%d valori]: ", r+1, m.colonne);
    for (c = 0; c < m.colonne; c++) {
      char s[10];
      scanf("%s", s);
      m.mat[r][c] = estrai_f(s);
    }
  }
  printf("\n");

  return m;
}

mat somma_m(mat a, mat b) {
  mat m = {{}, 0, 0};
  int r, c;

  if (a.righe != b.righe || a.colonne != b.colonne) return m;
  
  m.righe = a.righe;
  m.colonne = a.colonne;
  for (r = 0; r < a.righe; r++)
    for (c = 0; c < a.colonne; c++)
      m.mat[r][c] = somma_f(a.mat[r][c],  b.mat[r][c]);

  return m;
}

mat prodotto_m(mat a, mat b) {
  mat m = {{}, 0, 0};
  int r, c, i;

  if (a.colonne != b.righe) return m;

  m.righe = a.righe;
  m.colonne = b.colonne;
  for (r = 0; r < a.righe; r++) {
    for (c = 0; c < b.colonne; c++) {
      m.mat[r][c] = frazione(0, 1);
      for (i = 0; i < a.colonne; i++) {
        m.mat[r][c] = somma_f(m.mat[r][c], prodotto_f(a.mat[r][i], a.mat[i][c]));
      }
    }
  }

  return m;
}

mat prodotto_scal_m(mat m, frz f) {
  int r, c;
  for (r = 0; r < m.righe; r++)
    for (c = 0; c < m.colonne; c++)
      m.mat[r][c] = prodotto_f(m.mat[r][c], f);
  return m;
}

mat trasposta_m(mat m) {
  int rt, ct;
  mat tr = {{}, m.colonne, m.righe};

  for (rt = 0; rt < tr.righe; rt++)
    for (ct = 0; ct < tr.colonne; ct++)
      tr.mat[rt][ct] = m.mat[ct][rt];

  return tr;
}

mat scambia_righe_m(mat m, int r1, int r2) {
  frz tmp_swap;
  int c;
  for (c = 0; c < m.colonne; c++) {
    tmp_swap = m.mat[r1][c];
    m.mat[r1][c] = m.mat[r2][c];
    m.mat[r2][c] = tmp_swap;
  }
  return m;
}

mat moltiplica_riga(mat m, int r, frz coeff) {
  int c;

  for (c = 0; c < m.colonne; c++)
    m.mat[r][c] = prodotto_f(coeff, m.mat[r][c]);

  return m;
}

mat differenza_righe_m(mat m, int r1, frz coeff, int r2) {
  int c;

  for (c = 0; c < m.colonne; c++)
    m.mat[r1][c] = differenza_f(m.mat[r1][c], prodotto_f(coeff, m.mat[r2][c])); 

  return m;
}

mat scala_m(mat m) {
  int r, c, ic = 0, ir = 0;
  frz coeff;

  while (ic < m.colonne && ir < m.righe) {
    // se il primo elemento della prima riga √® nullo e non √® l'ultima riga
    if (!m.mat[ir][ic].num && ir < m.righe - 1) {
      // ricerca la prima riga che ha il primo elemento non nullo
      r = ir;
      for (c = ic; c < m.colonne && !m.mat[r][c].num; c++) {
        for (r = ir; r < m.righe && !m.mat[r][c].num; r++) {}
        if (!m.mat[r][c].num) ic++;
      }

      // scambia la prima riga con la prima non nulla
      m = scambia_righe_m(m, ir, r);
    }

    // riduci le righe sotto la prima
    for (r = ir+1; r < m.righe; r++) {
      // se il primo elemento √® nullo passa a quello dopo
      if (!m.mat[r][ic].num) continue;

      // determina i coefficenti
      coeff = divisione_f(m.mat[r][ic], m.mat[ir][ic]);

      // riduci tutti gli elementi della riga
      m = differenza_righe_m(m, r, coeff, ir);
    }

    ir++;
    ic++;
  }
  return m;
}

int rango_m(mat m) {
  int pivot = 0, r = 0, c;
  mat ms = scala_m(m);

  for (c = 0; c < ms.colonne; c++) {
    if (ms.mat[r][c].num) {
      pivot++;
      r++;
    }
  }

  return pivot;
}

frz determinante_m(mat m) {
  frz det = frazione(0, 1);
  int i, r, c;

  if (m.righe != m.colonne) 
    return frazione(0, 0);

  if (m.righe == 1) 
    return m.mat[0][0];
  
  for (i = 0; i < m.righe; i++) {
    mat d = m;
    d.righe--;
    d.colonne--;
    for (c = i; c < d.colonne; c++)
      for (r = 0; r < d.colonne; r++)
        d.mat[r][c] = d.mat[r][c+1];
    
    det = somma_f(
      det, 
      prodotto_scal_f(
        prodotto_f(
          m.mat[m.righe-1][i], 
          determinante_m(d)
        ), 
        ((m.righe+i)%2 ? -1 : 1) // segno
      ) 
    );
  }

  return det;
}

mat inversa_m(mat m) {
  int r, c, ic = 0, ir = 0;
  frz coeff;
  mat inv = {{}, 0, 0};

  if (m.righe != m.colonne) return inv;
  if (confronta_f(determinante_m(m), frazione(0, 1)) == 0) return inv; // determinante != 0

  // inv = identita
  inv.righe = m.righe;
  inv.colonne = m.colonne;
  for (r = 0; r < inv.righe; r++)
    for (c = 0; c < inv.colonne; c++)
      inv.mat[r][c] = (r == c) ? frazione(1, 1) : frazione(0, 1); // 1 nella diagonale, 0 nel resto
  
  // riduci totalmente a scala m e applica le stesse operazioni a inv
  while (ic+1 < m.colonne && ir+1 < m.righe) {
    // 1 - se il primo elemento della prima riga √® nullo e non √® l'ultima riga
    if (!m.mat[ir][ic].num && ir < m.righe - 1) {
      // ricerca la prima riga che ha il primo elemento non nullo
      r = ir;
      for (c = ic; c < m.colonne && !m.mat[r][c].num; c++) {
        for (r = ir; r < m.righe && !m.mat[r][c].num; r++) {}
        if (!m.mat[r][c].num) ic++; // se tutta la riga √® 0 vai a prossima colonna
      }

      // scambia la prima riga con la prima non nulla 
      m = scambia_righe_m(m, ir, r);
      inv = scambia_righe_m(inv, ir, r);
    }

    // 2 - riduci le righe sotto la prima
    for (r = ir+1; r < m.righe; r++) {
      // se il primo elemento √® nullo passa a quello dopo
      if (!m.mat[r][ic].num) continue;

      // determina il coefficente
      coeff = divisione_f(m.mat[r][ic], m.mat[ir][ic]);

      // riduci tutti gli elementi della riga
      m = differenza_righe_m(m, r, coeff, ir);
      inv = differenza_righe_m(inv, r, coeff, ir);
    }
    ir++;
    ic++;
  }

  // riparti al contrario
  while (ir >= 0 && ic >= 0) {
    // 1 - rendi l'elemento uguale a 1
    coeff = inversa_f(m.mat[ir][ic]);
    m = moltiplica_riga(m, ir, coeff);
    inv = moltiplica_riga(inv, ir, coeff);

    // 2 - riduci la riga sopra
    for (r = ir-1; r >= 0; r--) {
      // se l'elemento √® nullo passa a quello dopo
      if (!m.mat[r][ic].num) continue;

      // determina il coefficente
      coeff = divisione_f(m.mat[r][ic], m.mat[ir][ic]);

      // riduci tutti gli elementi della riga
      m = differenza_righe_m(m, r, coeff, ir);
      inv = differenza_righe_m(inv, r, coeff, ir); 
    }
    ir--;
    ic--;
  }

  return inv;
}


void stampa_m(mat m, const char *a) {
  int c, r;
  for (r = 0; r < m.righe; r++) {
    printf("|\t");
    for (c = 0; c < m.colonne; c++) {
      stampa_f(m.mat[r][c], "\t");
    }
    printf("|\n");
  }
  printf("%s", a);
  return;
}