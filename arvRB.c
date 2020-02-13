/*
 * É uma arvore binaria com mecanismo de balanceamento baseado na coloração
  dos nós. O processso de balanceamento é o mesmo das arvores AVL, ou seja,
  rotações simples ou duplas. No entanto, o conceito de balanceamento se
  da por meio do relacionamento das cores dos nós.
 * 
 * Considere as seguintes regras fundamentais em que uma arvore vermelho e preto: 
 *
 * a) se um nó v é "externo"(nulo), entao v é preto.
 * b) a raiz sempre é preto.
 * c)  Qual quer caminho da raiz ate um nó externo tem uma numero igual de nós pretos.
 * d) O pai de uma nó vermelho é preto
 * e) se um nó é vermelho, entao seus filhos(ambos) sao pretos.
 * 
 * Uma arvore vemelho-preto que atende a todos esses itens é dita
  equilibrada/balanceada.
 *
 *
 * Uma probabilidade obtida por consequencia as propriedades apresentadas, 
 é que nao ira ocorrer de nós consecutivos serem vermelhos.
 * 
 * 
 * Inserção: O procedimento de Inserção segue a logica da Inserção em uma arvore binária. Em relação ao controle do balanceamento, todo nó da arvore guarda sua cor, um novo nó inserido tem como cor inicial o vermelho. A cada insercao é preciso verificar se as regras continuam a ser atendiadas, caso algumas delas seja desobedecidas, será executada uma rotação.
 * 
 * A cada insercao é preciso verificar se o nó inserido é vermelho e nao é raiz,
 entao seu pai deve ser preto. Temos os seguintes casos:
 * 
 * 1) Se o pai é preto, entao a arvore esta balanceada.
 * 2) Se o pai é vermelho, entao é preciso balancear a arvore. Temos 2 sub casos:
 *     * 2.1) Se o pai do novo nó é vermelho e o seu tio
 *          Tambem é vermelho,entao ocorre atualizacao  das cores. 
 *           Exemplo no caderno...
 *      * 2.2) Se o pai é vermelho mas o tio é preto entao uma
 * rotacao deve ser aplicada.
 */





 /*Definicao de dois tipos de cores da arvore */
typedef enum{PRETO,VERMELHO} tipoCor;

/* Definicao do tipo da estrutura da arvore Rubro Negra */
struct arvoreRB
{
  struct arvoreRB *esq;
  struct arvoreRB *dir;
  int valor;
  tipoCor cor;
};
typedef struct arvoreRB tipoRB;

//Prototipos das funcoes da RB 
int cor(tipoRB **arv);

// 2) Funcao de alocao
tipoRB *aloca(int valor);
void imprimeRB(tipoRB *arv, int nivel);
tipoRB *insereVP (int valor, tipoRB *arv);
int balanceamento(tipoRB *no, tipoRB *pai, tipoRB *avo, tipoRB *bisavo, tipoRB **raiz);
tipoRB *buscaRemove(tipoRB *arv, tipoRB *noChave, int *flag);


/* Funcao que faz a alocacao do valor e de suas cores necessarias */
tipoRB* aloca(int valor)
{
    tipoRB *novoNo;
 	
    novoNo = (tipoRB*) malloc(sizeof(tipoRB));
    if(novoNo)
    {
        novoNo->valor = valor;
        novoNo->dir = NULL;
        novoNo->esq = NULL;
        novoNo->cor = VERMELHO;// inicializa a cor vermelha para identificar o no
   	}
    return novoNo;

}




/*
 *
 * Funcao principal de insercao em Arvore Rubro-Negra.
 *
*/
tipoRB *insereVP (int valor, tipoRB *arv) {
	tipoRB *raiz = arv;
	
	insereEmVP(valor, arv, NULL, NULL, &raiz); /*
	Nó pai e avo passado como parametro com nulo, pois nao ha pai e avo na
	raiz */


	raiz->cor = 0; /* Inicializacao com a raiz com preto */

	return raiz;
}


/*
 * Funcao de insercao em Arvore Rubro-Negra
 */
int insereEmVP(int valor, tipoRB *no, tipoRB *pai, tipoRB*avo, tipoRB **raiz) {
	tipoRB *noADescer;
	int flagSucesso;
	
	/*Verifica se alcancou a posicao onde o novo valor
	deve ser armazenado (alocando um novo nó). */
	if (no == NULL)
	{

		no =  aloca(valor); /*inicializa o nó com vermelho, mas na funcao anterior
		a raiz sera identificada como preto */

		if (*raiz == NULL) // momento da alocacao
		{
			*raiz = no;
		} 
		else
		{
			if ( valor < pai->valor ) 
			{
				pai->esq = no;
			}
			else
			{
				pai->dir = no;
			}
		}
		return 1; 	//Retorna sucesso para a insercao.
	}
	else 
	{	//Caso nao seja nulo( ou seja tem que descer na arvore).
		if ( valor != no->valor ) {
			if ( valor < no->valor ) {
				noADescer = no->esq;
			} else {
				noADescer = no->dir;
			}
			flagSucesso = insereEmVP(valor, noADescer, no, pai, &(*raiz) );
			switch (flagSucesso) {
				case 0:	
					return 1;
					break;
				case 1:		//Se a insercao ocorreu com sucesso, executa o balanco.
					if (noADescer == NULL) {
						if (valor < no->valor)
							noADescer = no->esq;
						else
							noADescer = no->dir;
					}
					return balanceamento(noADescer, no, pai, avo, raiz);
					break;
				case 2:	/*Caso receba o sinal '2'
				 		significa que ocorreu uma rotacaoo no nivel inferior,
				 		e os ponteiros estao desestruturados neste nivel.*/
					return 2;
					break;
			}
		} else {
			//Entao o no ja esta presente na arvore,entao nao faz nada...
			return 0;
		}
	}
}


/**
 * Funcao que verifica o balanceamento da Arvore Rubro-Negra
 */
int balanceamento(tipoRB *no, tipoRB *pai, tipoRB *avo, tipoRB *bisavo, tipoRB **raiz)
{
	

	tipoRB *tio, *raizSubArv;
	int flagBalanco; /* Sera utilizada para sinalizar
	 o tipo de ajuste executado (atualizacao de cores = 1, rotacao =2) */
	flagBalanco = 2;
	
	if (pai != NULL) { //testa para verificar se eh verdadeiro
		if ( pai->cor == VERMELHO ) { /*no caso de ser vermelho
			(vai ter que atualizar as cores ou realizar rotacao)*/
			
			/* Casos para descobri quem eh o tio do nó */
			if ( pai == avo->esq) {
			/*Identifica o tio do nó(caso seu pai e
			eh filho esquerdo, entao o seu irmao(tio) sera o filho direito do avo)*/
				tio = avo->dir; 
			} else { //caso contrario, seu tio sera o filho esq do seu avo
				tio = avo->esq;
			}
			
			if ( tio == NULL || tio->cor == PRETO ) { //Há necessidade de rotacao 
				// identifica qual rotacao devera ser feita
				if ( pai == avo->esq ) //identifica que o no esta pesando mais para o lado esquerdo
				{
					if ( no == pai->esq ) {	//Identifica rotacao 'Simples' a direita
						raizSubArv = pai;
						avo->esq = pai->dir;
						pai->dir = avo;
						pai->cor = PRETO;
						avo->cor = VERMELHO;
					} else {	//Identifica rotacao 'Dupla' a direita
						raizSubArv = no;
						pai->dir = no->esq;
						no->esq = pai;
						avo->esq = no->dir;
						no->dir = avo;
						no->cor = PRETO;
						avo->cor = VERMELHO;
					}
				} else {
					if ( no == pai->dir ) {	//Identifica rotacao 'Simples Esquerda'
						raizSubArv = pai;
						avo->dir = pai->esq;
						pai->esq = avo;
						pai->cor = PRETO;
						avo->cor = VERMELHO;
					} else {	//Identifica rotacao 'Dupla' a esquerda
						raizSubArv = no;
						pai->esq = no->dir;
						no->dir = pai;
						avo->dir = no->esq;
						no->esq = avo;
						no->cor = PRETO;
						avo->cor = VERMELHO;
					}
				}
				
				if ( bisavo != NULL) {
					if ( no->valor < bisavo->valor ) {
						bisavo->esq = raizSubArv;
					} else {
						bisavo->dir = raizSubArv;
					}
				} else {
					*raiz = raizSubArv;
				}
			} else {//Se tio eh VERMELHO, entao atualizar as cores dos pai, tio e avo.
				pai->cor = PRETO;
				tio->cor = PRETO;
				avo->cor = VERMELHO;
				flagBalanco = 0;
			}
		}
		return flagBalanco;
	} else {
		//Caso o pai seja nulo, nao há mais nada a fazer, estamos em um nÃ³ filho da raiz.
		return 2;
	}



}

/* Funcao que realiza a impressao no design de arvore */
void imprimeRB(tipoRB *arv, int nivel)
{
	int i;
	if( arv != NULL)
	{
		imprimeRB(arv->dir,nivel+1);
		for( i = 0; i < nivel;i++)
		{
			printf("\t");
		}
		printf("[%i, COR: %i]\n",arv->valor,arv->cor);
		imprimeRB(arv->esq,nivel+1);
	}

}

/*Funcao para a remocao da Arvore RB
tipoRB *removeVP(tipoRB **arv, int valor)
{
	tipoRB *raiz;
	raiz = (*arv);

	removeEmVP(&raiz,valor,*arv,NULL,NULL);

	if (raiz != NULL)
	{
		raiz->cor = PRETO;
	}
	return raiz;
}

int removeEmVP(tipoRB **raiz, int valor, tipoRB *no, tipoRB *pai, tipoRB *avo)
{
	int flagRemove;
	tipoRB *noADescer, *aux;
	if (no == NULL)
	{
		return -1;//valor n encontrado
	}
	//CONSIDERANDO QUE ESTEJA VERIFICANDO UM NÓ VALIDO NA ARVORE

	if(valor != no->valor)
	{
		if (valor < no->valor)
			noADescer = no->esq;
		else
			noADescer = no->dir;

		flagRemove = removeEmVP(raiz, valor, noADescer,no,pai);

		//Verifica a necessidade de balanceamento 
		switch(flagRemove){
			case -1:
				return -1;
				break;
			case 0:
				return 1;
				break;
			case 1:
				return balanceamento(noADescer, no, pai, avo,raiz);
				break;
			case 2:
				return 2;
				break;

		}//endSwitch

	}








	else { // caso o valor tenha sido encontrado
		//Verifica se o nó é folha (caso mais simples)
		if ( (no->esq == NULL) && (no->dir == NULL) )
		{
			/* avalia se há apenas a raiz na arvore */
			//if (no == *raiz)
			//{
			// 	free(no);
			// 	raiz = NULL;
			// 	return 2;
			// }
			// if(no == pai->esq)
			// 	pai->esq = NULL;
			// else 
			// 	pai->dir = NULL;
			// pai->cor = VERMELHO;
			// free(no);
			// return 0;
		//}else { // Caso seja um nó em um nivel intermediario da arvore
		//	if (no->esq == NULL)
		//	{
				/* Se o filho esq eh nulo,
				entao basta subir com o filho direito */
		///		aux = no;
			//	pai->cor = VERMELHO;
			//	if (no == pai->esq)
			//	{
			//		pai->esq = no->dir;
					// Aqui é preciso implementar a chamada de balanceamento,faça!
					//Atencao com o irmao 
			//	}else
			//	{
					//pai->dir = no->dir;
					// Aqui eh preciso implementar, a chamada recursiva, faça!
					// Atencao com o irmao
			//	}
			//	free(aux);
			//	return 0;
			//} else if(no->dir == NULL)
			//{
			//	aux = no;
				/*Faca  a implementacao, seguindo a mesma logica apresentada
				acima, mas para o outro lado */
			//} else
			//{
				/* Caso a nó tenha os dois filhos, entao selecione o filho
				mais a direita da subarvore esquerda */
				/* Sera preciso implementar uma funcao busca remove(como
				na da arvore binaria),em que se econtra o nó mais a direita
				da subarvore esquerda (o maior daquela subarvore) e Entao
				sobre escrever o no a ser removido, e finalmente desaloca o noA
				onde estava o maior valor. *
			}
		}
	}
	

}
*/














// void balanceamentoRemove(tipoRB **noADescer,tipoRB **pai)
// {
// 	if ( (*noADescer) != NULL)
// 	{
// 		if( (*noADescer)->cor == PRETO)
// 		{
// 			if( (*noADescer)->esq != NULL)
// 			{
// 				if( (*noADescer)->esq->cor == VERMELHO)
// 				{
// 					rotacaoDireita();
// 				}
// 			}else
// 			{
// 				if( (*noADescer)->dir != NULL)
// 				{
// 					if( (*noADescer)->dir->cor == VERMELHO)
// 					{
// 						rotacaoEsqDir();	
// 					}
// 				}else
// 				{
// 					(*noADescer)->cor = VERMELHO;
// 					(*pai)->cor = PRETO;
// 				}
// 			}
// 		}
// 	}

// }

// void rotacaoDireita(tipoRB )
// {
// 	raizSubArv = pai;
// 	avo->esq = pai->dir;
// 	pai->dir = avo;
// 	pai->cor = PRETO;
// 	avo->cor = VERMELHO;
// }



// //Funcao para a remocao da Arvore RB
// tipoRB *removeVP(tipoRB **arv, int valor)
// {
// 	tipoRB *raiz;
// 	raiz = (*arv);

// 	removeEmVP(&raiz,valor,*arv,NULL,NULL);

// 	if (raiz != NULL)
// 	{
// 		raiz->cor = PRETO;
// 	}
// 	return raiz;
// }

// int removeEmVP(tipoRB **raiz, int valor, tipoRB *no, tipoRB *pai, tipoRB *avo)
// {
// 	int flagRemove;
// 	tipoRB *noADescer, *aux;
// 	if (no == NULL)
// 	{
// 		return -1;//valor n encontrado
// 	}
// 	//CONSIDERANDO QUE ESTEJA VERIFICANDO UM NÓ VALIDO NA ARVORE

// 	if(valor != no->valor)
// 	{
// 		if (valor < no->valor)
// 			noADescer = no->esq;
// 		else
// 			noADescer = no->dir;

// 		flagRemove = removeEmVP(raiz, valor, noADescer,no,pai);

// 		//Verifica a necessidade de balanceamento 
// 		switch(flagRemove){
// 			case -1:
// 				return -1;
// 				break;
// 			case 0:
// 				return 1;
// 				break;
// 			case 1:
// 				return balanceamento(noADescer, no, pai, avo,raiz);
// 				break;
// 			case 2:
// 				return 2;
// 				break;
// 			case 3:
// 				if (noADescer == pai->esq)
// 				{
// 					noADescer = pai->dir;
// 				}else
// 					noADescer = pai->esq;
// 				return balanceamentoRemove(noADescer,&pai);

// 		}//endSwitch

// 	}


// 	else { // caso o valor tenha sido encontrado
// 		//Verifica se o nó é folha (caso mais simples)
// 		if ( (no->esq == NULL) && (no->dir == NULL) )
// 		{
// 			 avalia se há apenas a raiz na arvore 
// 			if (no == *raiz)
// 			{
// 				free(no);
// 				raiz = NULL;
// 				return 2;
// 			}
// 			if(no == pai->esq)
// 				pai->esq = NULL;
// 			else 
// 				pai->dir = NULL;
// 			pai->cor = VERMELHO;
// 			free(no);
// 			return 3;
// 		}else { // Caso seja um nó em um nivel intermediario da arvore
// 			if (no->esq == NULL)
// 			{
// 				/* Se o filho esq eh nulo,
// 				entao basta subir com o filho direito 
// 				aux = no;
// 				pai->cor = VERMELHO;
// 				if (no == pai->esq)
// 				{
// 					pai->esq = no->dir;
// 					// Aqui é preciso implementar a chamada de balanceamento,faça!
// 					//Atencao com o irmao 
// 				}else
// 				{
// 					pai->dir = no->dir;
// 					// Aqui eh preciso implementar, a chamada recursiva, faça!
// 					// Atencao com o irmao
// 				}
// 				free(aux);
// 				return 0;
// 			} else if(no->dir == NULL)
// 			{
// 				aux = no;
// 				/*Faca  a implementacao, seguindo a mesma logica apresentada
// 				acima, mas para o outro lado */
// 			} else
// 			{
// 				/* Caso a nó tenha os dois filhos, entao selecione o filho
// 				mais a direita da subarvore esquerda */
// 				/* Sera preciso implementar uma funcao busca remove(como
// 				na da arvore binaria),em que se econtra o nó mais a direita
// 				da subarvore esquerda (o maior daquela subarvore) e Entao
// 				sobre escrever o no a ser removido, e finalmente desaloca o noA
// 				onde estava o maior valor. */ 

// 				buscaRemove(no->esq,no,&flagRemove);

// 			}
// 		}
// 	}
	

// }

// tipoRB *buscaRemove(tipoRB *arv, tipoRB *noChave, int *flag){
//    tipoRB *noAux;
//    //busca pelo nó mais a direita
  
//    if (arv->dir != NULL){
//       arv->dir = buscaRemove ( arv->dir, noChave, flag);
      
//    } else { //encontrou o nó mais à direita da árvore
     
//       noChave->valor = arv->valor;

//       noAux = arv;
     
//       arv = arv->esq;
 	  
// 	  free(noAux);
//       *flag = 0; //para propagar a informação da mudança da árvore, para que os outros nós possam verificar o FB.
//    }
//    return arv;
// }




