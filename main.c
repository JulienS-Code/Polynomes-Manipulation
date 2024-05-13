#include <stdio.h>
#include <stdlib.h>

/**** Structure d'un rationnel ****/
struct rationnel{
    int num;
    int den;};
typedef struct rationnel Rationnel;

/**** Méthodes d'un rationnel ****/

/* Fonction qui attribue en backend la valeur 1 au dénominateur si l'utilisateur ne l'a pas renseigné */
void DefaultValueRationnel(Rationnel *r) {
    if (r->den == 0) r->den = 1;
}

/* Plus grand diviseur commun de deux entiers */
int Pgcd(int n, int m) {
    while (m != 0) {
        int temp = m;
        m = n % m;
        n = temp;
    }
    return n;
}

/* Fonction qui permet d'écrire un rationnel de la façon la plus simplifiée en utilisant le pgcd */
void SimplifyRationnel(Rationnel *r) {
    int pgcd = Pgcd(abs(r->num), abs(r->den)); // la valeur absolue permet d'éviter les erreurs inattendues
    r->num /= pgcd;
    r->den /= pgcd;
}

/* Lecture d'un rationnel depuis la console de l'utilisateur */
void LectureRationnel(Rationnel *r) {
    printf("Entrez un rationnel (num/den): ");
    scanf("%d/%d", &r->num, &r->den);
    DefaultValueRationnel(r);
    if (r->den < 0) { // si dénominateur négatif, permute le signe sur numérateur
        r->num = -r->num;
        r->den = -r->den;
    }
}

/* Affichage d'un rationnel sur la console */
void PrintRationnel(Rationnel *r) {
    SimplifyRationnel(r);
    if (r->den < 0) { // si dénominateur négatif, permute le signe sur numérateur
        r->num = -r->num;
        r->den = -r->den;
    }
    if (r->den == 1) { // évite d'écrire une fraction qui a pour dénominateur 1
        if (r->num < 0) { // rajoute les parenthèses pour éviter que des signes (+ ou -) se touchent
            printf("(%d)", r->num);
        } else printf("%d", r->num);  
    }
    else printf("(%d/%d)", r->num, r->den);
}

/* Fonction Somme de deux rationnels */
Rationnel SommeRationnel(
    Rationnel r1, 
    Rationnel r2) 
    {
    Rationnel result;
    if (r1.den != r2.den) { // multiplication de chaque rationnel par le dénominateur voisin pour ainsi avoir le même dénominateur
        int b = r1.den; // stockage d'un dénominateur pour éviter de perdre la donnée
        r1.num *= r2.den;
        r1.den *= r2.den;
        r2.num *= b;
        r2.den *= b;
    } 
    result.num = r1.num + r2.num;
    result.den = r1.den;
    SimplifyRationnel(&result); // simplification du rationnel après calculs
    return result;
}

/* Fonction Produit de deux rationnels */
Rationnel ProduitRationnel(
    Rationnel r1,
    Rationnel r2)
    {
    Rationnel result;
    result.num = r1.num * r2.num; // pour le produit il s'agit de multiplier chaque numérateur et 
    result.den = r1.den * r2.den; // dénominateur entre eux, ce qui créer un nouveau rationnel
    return result;
}

/* Fonction Produit d'un rationnel et d'un entier */
Rationnel ProduitRationnelInt(Rationnel r, int x) {
    Rationnel result;
    result.num = r.num * x; // l'entier peut s'écrire comme un rationnel avec un dénominateur de 1 : x = x/1
    result.den = r.den; // donc le dénominateur se voit multiplié par 1
    return result;
}

/* Fonction Puissance d'un entier */
int PuissanceInt(int base, int exponent) { // Fonction power() de math.h qui renvoi un entier
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }    
    return result;
}


/**** Structure d'un polynome ****/
struct polynome{
    int degre;
    Rationnel * poly;};
typedef struct polynome Polynome;

/**** Méthodes d'un polynome ****/

/* 1. Lecture de polynômes depuis le clavier */
void LecturePoly(Polynome *p) {
    printf("Lecture d'un polynome.\n");
    printf("Degré du polynome : ");
    scanf("%d", &p->degre);
    p->poly = (Rationnel*)malloc((p->degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    for (int i = p->degre; i >= 0; i--) {
        LectureRationnel(&p->poly[i]); // création d'un rationnel par degré du polynôme
    } // le polynôme est modifié depuis ses pointeurs, aucun return
}

/* 2. Affichage d'un polynôme sur la console */
void PrintPoly(Polynome p) {
    for (int i = p.degre; i >= 0; i--) {
        if (i != p.degre) printf("+"); // rajoute le signe de l'addition sauf pour le premier terme
        PrintRationnel(&p.poly[i]);
        if (i == 1) { // si degré = 1, pas besoin d'écrire sous forme de puissance
            printf("x");
        } else if (i != 0) { // si degré = 0, x^0 = 1, donc aucun x à écrire
            printf("x^%d", i);
        } 
    }
}

/* 3. Calcul de la somme de deux polynômes */
Polynome SommePoly(
    Polynome *p1, 
    Polynome *p2) 
    {
    Polynome result;
    result.degre = (p1->degre > p2->degre) ? p1->degre : p2->degre; // le résultat sera du degré max d'un des deux polynômes
    result.poly = malloc((result.degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    for (int i = result.degre; i >= 0; i--) {
        DefaultValueRationnel(&p1->poly[i]); // évite les erreurs
        DefaultValueRationnel(&p2->poly[i]);
        result.poly[i] = SommeRationnel(p1->poly[i], p2->poly[i]); // Somme les rationnels du même degré
    }
    return result;
}

/* Affichage de la fonction `SommePoly` en console */
void PrintSommePoly(
    Polynome p1,
    Polynome p2,
    Polynome *result)
    {
    printf("La somme des polynômes ");
    PrintPoly(p1); printf(" et "); PrintPoly(p2); printf(" vaut : ");
    *result = SommePoly(&p1, &p2); // affectation à la variable et non à son adresse
    PrintPoly(*result); printf(".\n\n");
}

/* 4. Calcul du produit de deux polynômes */
Polynome ProduitPoly(
    Polynome *p1,
    Polynome *p2)
    {
    Polynome result;
    result.degre = p1->degre + p2->degre;
    result.poly = malloc((result.degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    for (int i = 0; i <= result.degre; i++) { // Initialise un tableau de valeurs nulles
        result.poly[i].num = 0;
        result.poly[i].den = 1;
    }
    for (int i = p1->degre; i >= 0; i--) {
        for (int j = p2->degre; j >= 0; j--) {
            int degre_terme = i + j; // un produit avec des puissances est une addition des exposants
            Rationnel produit = ProduitRationnel(p1->poly[i], p2->poly[j]); // puis un produit des rationnels de chaque exposant
            Rationnel somme = SommeRationnel(result.poly[degre_terme], produit); // puis une somme de ces produits
            result.poly[degre_terme] = somme;
        }
    }
    return result;
}

/* Affichage de la fonction `ProduitPoly` en console */
void PrintProduitPoly(
    Polynome p1,
    Polynome p2,
    Polynome *result)
    {
    printf("Le produit des polynômes ");
    PrintPoly(p1); printf(" et "); PrintPoly(p2); printf(" vaut : ");
    *result = ProduitPoly(&p1, &p2); // affectation à la variable et non à son adresse
    PrintPoly(*result); printf(".\n\n");
}

/* 5. Evaluation d'un polynôme en un point donné */
Rationnel EvaluationPoint(Polynome p, int x) {
    Rationnel result, somme;
    result.num = 0; result.den = 1; // initialisation d'un rationnel valant 0 (0/1)
    for (int i = p.degre; i >= 0; i--) {
        if (i == 0) somme = p.poly[i]; // pas de puissance à calculer, rationnel brut rajouté dans l'addition
        else somme = ProduitRationnelInt(p.poly[i], PuissanceInt(x, i));
        result = SommeRationnel(result, somme); // somme des anciens termes avec le nouveau calculé grâce au point donné
    }
    SimplifyRationnel(&result); // simplification du rationnel après calculs
    return result;
}

/* Affichage de la fonction `EvaluationPoint` en console */
void PrintEvalPoint(
    Polynome p,
    Rationnel *result)
    {
    int x;
    printf("Entrez un entier à tester sur le polynome : ");
    scanf("%d", &x);
    printf("En remplaçant x par %d dans le polynome ", x); PrintPoly(p); printf(", on obtient ");
    *result = EvaluationPoint(p, x); // affectation à la variable et non à son adresse
    PrintRationnel(result); printf(".\n\n");
}

/* 6. Calcul de la dérivée d'un polynôme */
Polynome DeriveePoly(Polynome p) {
    Polynome result;
    result.degre = p.degre - 1; // degré de la dérivée d'un polynome équivaut au degré -1
    result.poly = malloc((result.degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    if (p.degre == 0) {
        result.degre = 0;
        Rationnel r0; r0.num = 0; r0.den = 1;
        result.poly[0] = r0;
        return result;
    }
    for (int i = result.degre; i >= 0; i--) { // boucle qui permet de s'arréter avant le dernier terme de p (le rationnel n'étant pas facteur de x)
        result.poly[i] = ProduitRationnelInt(p.poly[i+1], i + 1); // produit du rationnel avec le degré +1 
    } // similaire à faire le produit rationnel du même degré puis -1
    return result;
}

/* Affichage de la fonction `DeriveePoly` en console */
void PrintDeriveePoly(
    Polynome p,
    Polynome *result)
    {
    printf("La dérivée du polynome "); PrintPoly(p); printf(" vaut ");
    *result = DeriveePoly(p); // affectation à la variable et non à son adresse
    PrintPoly(*result); printf(".\n\n");
}

/* Fonction de nettoyage de la mémoire d'un polynôme */
void CleanMemory(Polynome *p) {
    free(p->poly); // libère la mémoire allouée
    p->poly = NULL; // évite les accès à la mémoire non souhaités
}

/**** Fonctions du menu ****/

/* Fonction `Lecture` du menu */
void L_Menu() {
    Polynome p1;
    LecturePoly(&p1);
    CleanMemory(&p1);
}

/* Fonction `Affichage` du menu */
void A_Menu() {
    Polynome p1;
    LecturePoly(&p1);
    printf("Votre polynôme est "); PrintPoly(p1); printf(".\n\n");
    CleanMemory(&p1);
}

/* Fonction `Somme` du menu */
void S_Menu() {
    Polynome p1, p2, p3;
    LecturePoly(&p1); LecturePoly(&p2);
    PrintSommePoly(p1, p2, &p3);
    CleanMemory(&p1); CleanMemory(&p2); CleanMemory(&p3);
}

/* Fonction `Produit` du menu */
void P_Menu() {
    Polynome p1, p2, p3;
    LecturePoly(&p1); LecturePoly(&p2);
    PrintProduitPoly(p1, p2, &p3);
    CleanMemory(&p1); CleanMemory(&p2); CleanMemory(&p3);
}

/* Fonction `Evaluation` du menu */
void E_Menu() {
    Polynome p1;
    Rationnel r1;
    LecturePoly(&p1);
    PrintEvalPoint(p1, &r1);
    CleanMemory(&p1);
}

/* Fonction `Dérivée` du menu */
void D_Menu() {
    Polynome p1, p2;
    LecturePoly(&p1);
    PrintDeriveePoly(p1, &p2);
    CleanMemory(&p1); CleanMemory(&p2);
}

/* Fonction `Tests Automatisés` du menu */
void T_Menu() {
    Polynome p1;
    p1.degre = 2;
    p1.poly = (Rationnel*)malloc((p1.degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    p1.poly[0].num = 1; p1.poly[0].den = 6; // au degré 0, rationnel 1/6 = (1/6)
    p1.poly[1].num = 2; p1.poly[1].den = 5; // au degré 1, rationnel 2/5 = (2/5)x
    p1.poly[2].num = 3; p1.poly[2].den = -1; // au degré 2, rationnel 3/-1 = (-3)x^2

    printf("Test de l'affichage d'un polynôme :\n"); // Fonction Affichage
    printf("Affichage du polynôme initial : "); PrintPoly(p1); printf("\n\n"); // (-3)x^2+(2/5)x+(1/6)

    printf("Test de la somme de deux polynômes :\n"); // Fonction Somme de polynômes
    Polynome p2;
    p2.degre = 2;
    p2.poly = (Rationnel*)malloc((p2.degre + 1) * sizeof(Rationnel)); // Allocation mémoire du tableau
    p2.poly[0].num = 1; p2.poly[0].den = -6; // au degré 0, rationnel 1/-6 = (-1/6)
    p2.poly[1].num = 4; p2.poly[1].den = 3; // au degré 1, rationnel 4/3 = (4/3)x
    p2.poly[2].num = 5; p2.poly[2].den = -2; // au degré 2, rationnel 5/-2 = (-5/2)x^2
    printf("Deuxième polynôme : "); PrintPoly(p2); printf("\n"); // (-5/2)x^2+(4/3)x+(-1/6)
    Polynome somme_poly;
    PrintSommePoly(p1, p2, &somme_poly); // (-11/6)x^2+(26/15)x+0

    Polynome produit_poly; 
    PrintProduitPoly(p1, p2, &produit_poly); // (15/2)x^4+(-5)x^3+(37/60)x^2+(7/45)x+(-1/36)

    printf("Test de l'évaluation d'un polynôme en un point donné :\n"); // Fonction Evaluation 
    int x = 10;
    printf("Valeur de x : %d\n", x);
    printf("Évaluation du polynôme initial en x = %d : ", x);
    Rationnel result_eval = EvaluationPoint(p1, x);
    PrintRationnel(&result_eval); printf("\n\n"); // (-1775/6)

    Polynome derivee_poly;
    PrintDeriveePoly(p1, &derivee_poly); // (-6)x+(2/5)

    // Nettoyage de la mémoire
    CleanMemory(&p1);
    CleanMemory(&p2);
    CleanMemory(&somme_poly);
    CleanMemory(&produit_poly);
    CleanMemory(&derivee_poly);
}


void main() {
    char input, continu;
    do {
        do {
            printf("\n\n****************** Menu ******************\n");
            printf("L --> Lecture simple d'un polynôme\n");
            printf("A --> Affichage d'un polynôme\n");
            printf("S --> Somme de deux polynômes\n");
            printf("P --> Produit de deux polynômes\n");
            printf("E --> Evaluation d'un polynôme en un point\n");
            printf("D --> Dérivée d'un polynôme\n");
            printf("T --> Tests automatisés (pour professeurs)\n");
            printf("******************************************\n\n");
            scanf(" %c", &input);
            
            switch (input)
            {
            case 'L':
                L_Menu();
                break;
            case 'A':
                A_Menu();
                break;
            case 'S':
                S_Menu();
                break;
            case 'P':
                P_Menu();
                break;
            case 'E':
                E_Menu();
                break;
            case 'D':
                D_Menu();
                break;
            case 'T':
                T_Menu();
                break;
            default:
                printf("Veuillez entrer une lettre du menu\n\n");
            }
        } while (input != 'L' && input != 'A' 
                && input != 'S' && input != 'P' 
                && input != 'E' && input != 'D' 
                && input != 'T');

        printf("Voullez-vous continuer ? (o/n) : \n");
        scanf(" %c", &continu);

    } while (continu == 'o');
    printf("Vous avez quitté\n");
}