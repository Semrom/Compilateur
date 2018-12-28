#include <stdio.h>
#include <string.h>
#include "syntabs.h"
#include "util.h"
#include "dico.h"

void affiche_table_symboles(n_prog *n);

void table_l_instr(n_l_instr *n);

void table_instr(n_instr *n);

void table_instr_si(n_instr *n);

void table_instr_tantque(n_instr *n);

void table_instr_faire(n_instr *n);

/* MODIFIE POUR EVAL */
void table_instr_pour(n_instr *n);

/* MODIFIE POUR EVAL */
void table_instr_affect(n_instr *n);

void table_instr_appel(n_instr *n);

void table_instr_retour(n_instr *n);

void table_instr_ecrire(n_instr *n);

void table_l_exp(n_l_exp *n);

void table_exp(n_exp *n);

void table_varExp(n_exp *n);

void table_opExp(n_exp *n);

void table_intExp(n_exp *n);

void table_lireExp(n_exp *n);

void table_appelExp(n_exp *n);

void table_l_dec(n_l_dec *n);

void table_dec(n_dec *n);

void table_foncDec(n_dec *n);

void table_varDec(n_dec *n);

void table_tabDec(n_dec *n);

void table_var(n_var *n);

void table_var_simple(n_var *n);

void table_var_indicee(n_var *n);

void table_appel(n_appel *n);

int size_n_l_dec(n_l_dec *n);

int size_n_l_exp(n_l_exp *n);

int contexte = C_VARIABLE_GLOBALE;
int adresseLocaleCourante;
int adresseArgumentCourant;

/*-------------------------------------------------------------------------*/

void affiche_table_symboles(n_prog *n) {
    table_l_dec(n->variables);
    table_l_dec(n->fonctions);
}

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

void table_l_instr(n_l_instr *n) {
    if (n) {
        table_instr(n->tete);
        table_l_instr(n->queue);
    }
}

/*-------------------------------------------------------------------------*/

void table_instr(n_instr *n) {
    if (n) {
        if (n->type == blocInst) table_l_instr(n->u.liste);
        else if (n->type == affecteInst) table_instr_affect(n);
        else if (n->type == siInst) table_instr_si(n);
        else if (n->type == tantqueInst) table_instr_tantque(n);
        else if (n->type == faireInst) table_instr_faire(n);
        else if (n->type == pourInst) table_instr_pour(n);
        else if (n->type == appelInst) table_instr_appel(n);
        else if (n->type == retourInst) table_instr_retour(n);
        else if (n->type == ecrireInst) table_instr_ecrire(n);
    }
}

/*-------------------------------------------------------------------------*/

void table_instr_si(n_instr *n) {

    table_exp(n->u.si_.test);
    table_instr(n->u.si_.alors);
    if (n->u.si_.sinon) {
        table_instr(n->u.si_.sinon);
    }
}

/*-------------------------------------------------------------------------*/

void table_instr_tantque(n_instr *n) {

    table_exp(n->u.tantque_.test);
    table_instr(n->u.tantque_.faire);
}

/*-------------------------------------------------------------------------*/

void table_instr_faire(n_instr *n)          /* MODIFIE POUR EVAL */
{                                             /* MODIFIE POUR EVAL */
    table_instr(n->u.faire_.faire);           /* MODIFIE POUR EVAL */
    table_exp(n->u.faire_.test);              /* MODIFIE POUR EVAL */
}                                             /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void table_instr_pour(n_instr *n)                /* MODIFIE POUR EVAL */
{                                                  /* MODIFIE POUR EVAL */
    table_instr(n->u.pour_.init);                  /* MODIFIE POUR EVAL */
    table_exp(n->u.pour_.test);                    /* MODIFIE POUR EVAL */
    table_instr(n->u.pour_.faire);                 /* MODIFIE POUR EVAL */
    table_instr(n->u.pour_.incr);                  /* MODIFIE POUR EVAL */
}                                                  /* MODIFIE POUR EVAL */

/*-------------------------------------------------------------------------*/

void table_instr_affect(n_instr *n) {
    table_var(n->u.affecte_.var);
    table_exp(n->u.affecte_.exp);
}

/*-------------------------------------------------------------------------*/

void table_instr_appel(n_instr *n) {
    table_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void table_appel(n_appel *n) {
    int nb_param, i;
    if ((i = rechercheExecutable(n->fonction)) != -1) {
        desc_identif *fonc = &dico.tab[i];
        nb_param = size_n_l_exp(n->args);
        if (nb_param != fonc->complement) {
            erreur_1s("Erreur dans la table des symboles", "Nombre arguments invalides");
            printf("%s \n", n->fonction);
            exit(EXIT_FAILURE);
        }
        if (rechercheExecutable("main") == -1) {
            erreur_1s("Erreur dans la table des symboles", "Fonction main non trouvée");
            exit(EXIT_FAILURE);
        }
        if (dico.tab[rechercheExecutable("main")].complement != 0) {
            erreur_1s("Erreur dans la table des symboles",
                      "Fonction main non valide, la fonction main ne doit pas avoir d'arguments");
            exit(EXIT_FAILURE);
        }
        entreeFonction();
        table_l_exp(n->args);
        sortieFonction();
        return;
    }
    printf("%s \n", n->fonction);
    erreur_1s("Erreur dans la table des symboles", "Fonction non déclarée");
    exit(EXIT_FAILURE);
}

/*-------------------------------------------------------------------------*/

void table_instr_retour(n_instr *n) {
    table_exp(n->u.retour_.expression);

}

/*-------------------------------------------------------------------------*/

void table_instr_ecrire(n_instr *n) {
    table_exp(n->u.ecrire_.expression);
}

/*-------------------------------------------------------------------------*/

void table_l_exp(n_l_exp *n) {

    if (n) {
        table_exp(n->tete);
        table_l_exp(n->queue);
    }
}

/*-------------------------------------------------------------------------*/

void table_exp(n_exp *n) {

    if (n->type == varExp) table_varExp(n);
    else if (n->type == opExp) table_opExp(n);
    else if (n->type == intExp) table_intExp(n);
    else if (n->type == appelExp) table_appelExp(n);
    else if (n->type == lireExp) table_lireExp(n);
}

/*-------------------------------------------------------------------------*/

void table_varExp(n_exp *n) {
    table_var(n->u.var);
}

/*-------------------------------------------------------------------------*/
void table_opExp(n_exp *n) {
    if (n->u.opExp_.op1 != NULL) {
        table_exp(n->u.opExp_.op1);
    }
    if (n->u.opExp_.op2 != NULL) {
        table_exp(n->u.opExp_.op2);
    }
}

/*-------------------------------------------------------------------------*/

void table_intExp(n_exp *n) {

}

/*-------------------------------------------------------------------------*/
void table_lireExp(n_exp *n) {

}

/*-------------------------------------------------------------------------*/

void table_appelExp(n_exp *n) {
    table_appel(n->u.appel);
}

/*-------------------------------------------------------------------------*/

void table_l_dec(n_l_dec *n) {

    if (n) {

        table_dec(n->tete);
        table_l_dec(n->queue);
    }
}

/*-------------------------------------------------------------------------*/

void table_dec(n_dec *n) {

    if (n) {
        if (n->type == foncDec) {
            table_foncDec(n);
        }
        else if (n->type == varDec) {
            table_varDec(n);
        }
        else if (n->type == tabDec) {
            table_tabDec(n);
        }
    }
}

/*-------------------------------------------------------------------------*/

void table_foncDec(n_dec *n) {
    int nb_param;
    if (rechercheDeclarative(n->nom) == -1) {
        nb_param = size_n_l_dec(n->u.foncDec_.param);
        ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_FONCTION, 0, nb_param);
        contexte = C_ARGUMENT;
        table_l_dec(n->u.foncDec_.param);

        entreeFonction();
        table_l_dec(n->u.foncDec_.variables);
        table_instr(n->u.foncDec_.corps);
        affiche_dico();
        sortieFonction();

        return;
    }

    printf("%s \n", n->nom);
    erreur_1s("Erreur dans la table des symboles", "Fonction déja déclarée");
    exit(EXIT_FAILURE);

}

/*-------------------------------------------------------------------------*/

void table_varDec(n_dec *n) {
    if (rechercheDeclarative(n->nom) == -1) {
        switch (contexte) {
            case C_VARIABLE_GLOBALE:
                ajouteIdentificateur(n->nom, contexte, T_ENTIER, dico.sommet, -1);
                break;
            case C_VARIABLE_LOCALE:
                ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseLocaleCourante, -1);
                adresseLocaleCourante += 4;
                break;
            case C_ARGUMENT:
                ajouteIdentificateur(n->nom, contexte, T_ENTIER, adresseArgumentCourant, -1);
                adresseArgumentCourant += 4;
                break;
        }
        return;
    }
    printf("%s \n", n->nom);
    erreur_1s("Erreur dans la table des symboles", "Variable déjà déclarée");
    exit(EXIT_FAILURE);
}

/*-------------------------------------------------------------------------*/

void table_tabDec(n_dec *n) {
    if (rechercheDeclarative(n->nom) == -1) {
        ajouteIdentificateur(n->nom, C_VARIABLE_GLOBALE, T_TABLEAU_ENTIER, dico.sommet, n->u.tabDec_.taille);
        return;
    }
    printf("%s \n", n->nom);
    erreur_1s("Erreur dans la table des symboles", "Tableau déja déclaré");
    exit(EXIT_FAILURE);
}

/*-------------------------------------------------------------------------*/

void table_var(n_var *n) {
    if (n->type == simple) {
        table_var_simple(n);
    }
    else if (n->type == indicee) {
        table_var_indicee(n);
    }
}

/*-------------------------------------------------------------------------
 * Nous testons dans cette fonction si la variable utilisée est hors contexte et si elle est bien déclarée
 * ------------------------------------------------------------------------*/
void table_var_simple(n_var *n) {
    int i;
    if ((i = rechercheExecutable(n->nom)) != -1) {
        desc_identif *var = &dico.tab[i];
        if (contexte == C_VARIABLE_LOCALE) {
            if (var->classe == C_VARIABLE_LOCALE)
                return;
            else if (var->classe == C_ARGUMENT)
                return;
            else {
                printf("%s, %d, %d \n", n->nom, contexte, var->classe);
                erreur_1s("Erreur dans la table des symboles", "Variable hors contexte");
                exit(EXIT_FAILURE);
            }
        }
        else if (contexte == C_VARIABLE_GLOBALE && var->classe != C_VARIABLE_GLOBALE) {
            printf("%s, %d, %d \n", n->nom, contexte, var->classe);
            erreur_1s("Erreur dans la table des symboles", "Variable hors contexte");
            exit(EXIT_FAILURE);
        }
        if (var->type != T_ENTIER) {
            printf("%s \n", n->nom);
            erreur_1s("Erreur dans la table des symboles", "mauvais type de variable, variable simple attendue");
            exit(EXIT_FAILURE);
        }
        return;
    }
    printf("%s \n", n->nom);
    erreur_1s("Erreur dans la table des symboles", "Variable non déclarée");
    exit(EXIT_FAILURE);
}

/*-------------------------------------------------------------------------*/
void table_var_indicee(n_var *n) {
    int i;
    if ((i = rechercheExecutable(n->nom)) != -1) {
        desc_identif *var = &dico.tab[i];
        if (var->type != T_TABLEAU_ENTIER) {
            printf("%s \n", n->nom);
            erreur_1s("Erreu dans la table des symboles", "Tableau déclarée comme entier");
            exit(EXIT_FAILURE);
        }
        table_exp(n->u.indicee_.indice);
        return;
    }
    printf("%s \n", n->nom);
    erreur_1s("Erreur dans la table des symboles", "Tableau non déclaré");
    exit(EXIT_FAILURE);
}

/*-------------------------------------------------------------------------*/

int size_n_l_dec(n_l_dec *n) {
    if (n == NULL)
        return 0;
    return 1 + size_n_l_dec(n->queue);
}

int size_n_l_exp(n_l_exp *n) {
    if (n == NULL)
        return 0;
    return 1 + size_n_l_exp(n->queue);
}
