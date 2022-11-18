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
void stampa_f(frz, const char *);

// MATRICI
mat richiedi_matrice();
mat somma_m(mat, mat);
mat prodotto_m(mat, mat);
mat prodotto_scal_m(mat, frz);
mat scala_m(mat);
int rango_m(mat);
int determinante_m(mat);
mat inversa_m(mat);
void stampa_m(mat, const char *);
// void stampa_matrice(mat);

// SISTEMI

/*
TODO
  - leggi frazioni da richiedi matrice (leggi stringa)
  - determinante
  - inversa di matrice
  - salva matrici risultato
  
  - sistemi
    - equazioni come stringhe
    - trovare matrice

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
  mat m = {{}, 0, 0};

  while (1) {
    
    printf("\n\nMATRICI\n\n");
    if (!m.righe) {
      printf("1) Nuova Matrice");
      printf("0) Indietro");
      scelta = ask_int(0, 1);
    } else {
      stampa_m(m, "\n\n");
      printf("1) Nuova Matrice");
      printf("2) Somma");
      printf("3) Prodotto");
      printf("4) Riduci a scala");
      printf("5) Rango");
      printf("6) Inversa");
      printf("0) Indietro");
      scelta = ask_int(0, 6);
    }

    switch (scelta) {
      case 1:
        m = richiedi_matrice();
        break;
      case 2:
        printf("\n\nMatrice ridotta a scala:\n");
        stampa_m(scala_m(m), "\n\n");
        ask_int(0, 0);
        break;
      case 3:
        printf("\nRango della Matrice: %d\n\n", rango_m(m));
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
      int v;
      scanf("%d", &v);
      m.mat[r][c] = frazione(v, 1);
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

mat scala_m(mat m) {
  int r, c, ic = 0, ir = 0;
  frz tmp_swap, coeff;

  while (ic < m.colonne && ir < m.righe) {
    // se il primo elemento della prima riga è nullo e non è l'ultima riga
    if (!m.mat[ir][ic].num && ir < m.righe - 1) {
      // ricerca la prima riga che ha il primo elemento non nullo
      for (c = ic; c < m.colonne && !m.mat[r][c].num; c++) {
        for (r = ir; r < m.righe && !m.mat[r][c].num; r++) {}
        if (!m.mat[r][c].num) ic++;
      }

      // scambia la prima riga con la prima non nulla
      for (c = ic; c < m.colonne; c++) {
        tmp_swap = m.mat[ir][c];
        m.mat[ir][c] = m.mat[r][c];
        m.mat[r][c] = tmp_swap;
      }

      // passaggio
      // printf("r%d <-> r%d;\n", ir, r);
    }

    // riduci le righe sotto la prima
    for (r = ir+1; r < m.righe; r++) {
      // se il primo elemento è nullo passa a quello dopo
      if (!m.mat[r][ic].num) continue;

      // determina i coefficenti
      coeff = divisione_f(m.mat[r][ic], m.mat[ir][ic]);

      // riduci tutti gli elementi della riga
      for (c = ic; c < m.colonne; c++) 
        m.mat[r][c] = differenza_f(m.mat[r][c], prodotto_f(coeff, m.mat[ir][c]));

      // passaggio
      // printf("r%d %c", r, coeff.n>0?'+':'\0');
      // stampa_frazione(coeff);
      // printf(" r%d;\n", ir);
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