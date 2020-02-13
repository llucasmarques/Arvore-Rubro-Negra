
 #include <stdio.h>
 #include <stdlib.h>

 /* Inclusao das funcoes que contem o metodo Rubro-negra */
 #include "arvRB.c"




 int main()
 {

    tipoRB *arv;
    arv = NULL;

    /*
    int v[] = {50, 80, 100, 120, 160, 200, 300, 400, 500, 350, 320, 310, 110, 105, 115, 109, 108, 35}; //, 25, 15, 5, 33, 34};
    int n = sizeof(v) / sizeof(int);
    int i;
    for (i = 0; i < n; ++i)
    {
        arv = insereVP(v[i],arv);
    }
  */
    arv = insereVP(20,arv);
    arv = insereVP(15,arv);
    arv = insereVP(57,arv);
    arv = insereVP(10,arv);
    arv = insereVP(40,arv);
    arv = insereVP(80,arv);
    arv = insereVP(30,arv);
    arv = insereVP(50,arv);
    arv = insereVP(90,arv);
   // remocao
  //  arv = removeVP(&arv,57);
  //  arv = insereVP(18,arv);
  // arv = removeVP(&arv,20);
 


    printf("Arvore Rubro Negra: \n");
    imprimeRB(arv,0);
    printf("\n\n\n");

}