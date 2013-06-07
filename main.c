#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

//---------------------------------------------------------
// Inicio da Funções que imprime
//---------------------------------------------------------
/*----------------------------------------------------------------------------*/
/* Calcula altura da arvore                                                   */
/*----------------------------------------------------------------------------*/
int h_arvore (int *p) {

	if (p == NULL) {
		return 0;
	}

	int hl, hr, 
	des_esq = 3, des_dir = 4,  
	*p_esq = *(p + des_esq), *p_dir = *(p + des_dir);

	hl =h_arvore( p_esq );
	hr =h_arvore( p_dir );
	if (hl>hr) {
		return hl+1;
	}
	else {
		return hr+1; 
	}
}


//---------------------------------------------------------
// Função que imprime as árvores
//---------------------------------------------------------
void	imprime (int *p, int h) {

	int i, des_esq = 3, des_dir = 4, des_endi = 5, des_endf = 6;

	if (p == NULL) {
		for (i = 0; i < h; i++) 
			printf("      ");
		printf("*\n");
		return;
	}

	int *p_esq = *(p + des_esq), *p_dir = *(p + des_dir), 
	*p_endi = *(p + des_endi), *p_endf = *(p + des_endf);

	imprime ( p_dir, h + 1 );
	for (i = 0; i < h; i++) 
		printf("      ");
	printf("(p=%p)(i=%p)(f=%p)", p, p_endi, p_endf );
	printf("\n");

	imprime( p_esq, h + 1 );
}

void  imprMapa ( ) {
	int h, 
	*end_brk, *end_raiz, *end_alocado, *end_desalocado;

	end_brk = sbrk(0);
	end_raiz = *end_brk;
	end_alocado = end_raiz + 2;
	end_desalocado = end_raiz + 3;

	if ( *end_alocado != NULL) {
		h = h_arvore( *end_alocado );	
		imprime ( *end_alocado , h);
	}

	printf("-------------------------------------------------------------------\n");

	if ( *end_desalocado != NULL) {
		h = h_arvore( *end_desalocado );	
		imprime ( *end_desalocado , h);
	}
}
//---------------------------------------------------------
// Final das Funções que imprime
//---------------------------------------------------------

//---------------------------------------------------------
// Inicio da Função que cria a raiz
//---------------------------------------------------------
//---------------------------------------------------------
// Função que cria a raiz
//---------------------------------------------------------
Cria_Raiz ( int num_bytes) {

	printf("Criando a raiz\n");

	int des_ocup = 1, des_bal = 2, des_num= 3, des_esq = 4, des_dir = 5, des_endi = 6, des_endf = 7, 
	*end_brk, *end_brk_aux, *end_brk_ant, *end_raiz, *end_ultimo, i, a, b, sai =0;

	end_brk_ant = sbrk(0);
	sbrk(16);
	end_brk = sbrk(0);

	// Ao subir a brk atualiza o valor da raiz
	*end_brk = end_brk_ant;

	// O primeiro valor da heap é o endereço da raiz do primeiro nodo alocado
	*end_brk_ant = end_brk;

	// O segundo valor da heap é o endereço do maior endereço que foi alocadona na heap
	end_brk_ant = end_brk_ant + 1 ;
	*end_brk_ant = end_brk - 1 ;
	
	// Endereço da raiz da árvore dos nodos alocados
	end_brk_ant = end_brk_ant + 1 ;
	*end_brk_ant = sbrk(0);

	// Endereço da raiz da árvore dos nodos desalocados
	end_brk_ant = end_brk_ant + 1 ;
	*end_brk_ant = NULL;

	// Aloca o primeiro nodo
	// Guarda o antigo valor da brk para atribuir ao próximo valor da brk o endereço da raiz
	end_brk_ant = sbrk(0);

	// Somente para teste sempre joga a brk para cima 100 posições
	sbrk( (num_bytes * 20) + 24 );

	// Atualiza o endereço da raiz no novo valor da brk
	end_raiz = *end_brk_ant;
	end_brk = sbrk(0);	
	*end_brk = end_raiz;
	
	// Encontra o valor do último nodo alocado
	end_ultimo = *(end_raiz +1);
	
	// Coloca os valores da estrutura na heap
	*(end_ultimo + des_ocup) = 1; // Ocupado
	*(end_ultimo + des_bal) = 0; // Balanceamento inicial
	*(end_ultimo + des_num) = num_bytes; // Número de bytes alocados
	*(end_ultimo + des_esq) = NULL; // Endereço do filho esquerdo
	*(end_ultimo + des_dir) = NULL; // Endereço do filho direito
	*(end_ultimo + des_endi) = end_ultimo + des_endi + 2; // Endereço de inicio do nodo
	*(end_ultimo + des_endf) = end_ultimo + des_endi + 1 + num_bytes; // Endereço do fim do nodo	

	// Coloca o valor 20 aonde foi armazenado o que foi pedido
	for (i=0; i<num_bytes; i++) {
		*(end_ultimo + des_endi + 2 + i) = 20; 
	}
	
	// Imprime os valores na heap a partir do endereço do inicio do nodo
	printf("----------------------------------\n");
	for (i=0; i<(num_bytes + des_endf); i++) {
		a = 1 + end_ultimo  + i;  
		b = *(1 + end_ultimo + i);  
		printf("o endereço e %p\n",(void *) a);
		printf("o conteudo e %d\n",(void *) b);

	}
	printf("----------------------------------\n");
	
	// Atualiza o último alocado
	end_ultimo = *(end_ultimo + des_endf);
	*(end_raiz + 1 ) = end_ultimo;
	end_brk = sbrk(0);
	end_raiz = *end_brk;
	*(end_raiz + 1 ) = end_ultimo;

	printf("terminou de criar a raiz\n");
}
//---------------------------------------------------------
// Final da Função que cria a raiz
//---------------------------------------------------------


//---------------------------------------------------------
// Função que Procura um nodo na árvore dos desalocados
//---------------------------------------------------------
int *Encontrou_nodo_desalocado (int *p, int num_bytes) {

	printf("Iniciando procura nos nodos desalocados\n");

	int des_esq = 3, des_dir = 4, des_endi = 5, des_endf = 6, 
	*p_esq = *(p + des_esq), 
	*p_dir= *(p + des_dir ),
	*nodo, 
	diferenca = ( (*(p + des_endf) - *(p + des_endi ) )/4 )	;
	printf("p = %p\n", p);
	printf("A quantidade de bytes livre para p = %d\n", diferenca);

	if ( diferenca >= num_bytes + 24 ) {
		printf("encontrou diferenca = %d num_bytes = %d\n", diferenca, num_bytes + 24 );
		nodo = p;
		printf("p = %p nodo = %p\n", p, nodo);

		return nodo;

	}
	else {
		if (p_esq !=NULL )
 			nodo =Encontrou_nodo_desalocado ( p_esq, num_bytes );
		else
			nodo =NULL;
		if (p_dir !=NULL )
			nodo =Encontrou_nodo_desalocado ( p_dir, num_bytes );
		else
			nodo = NULL;
	}

	return nodo;

}


// Declaração das funções
int *Busca_desalocado (int *end_raiz_desalocado, int *nodo);
int *Arruma_Apontadores_Desalocados (int *end_raiz_alocado, int *nodo);


int Exite_Nodo_Desalocado (int num_bytes) {

	printf("Entrou na função para verificar se existe um nodo desalocado\n");

	int *end_brk, *end_raiz, *end_raiz_alocado, *end_raiz_desalocado, *nodo;
	int *achou;
	
	end_brk = sbrk(0);
	end_raiz = *end_brk;
	end_raiz_alocado = end_raiz + 2;
	end_raiz_desalocado = end_raiz + 3;
	
	// Se ainda não existe raiz na árvore dos desalocados
	if (*end_raiz_desalocado == NULL) {
		printf("Raiz dos desalocados NULL\n\n");
		return 0;
	}
	else {

		// Busca um nodo na árvore dos desalocados
 		nodo = Encontrou_nodo_desalocado ( *end_raiz_desalocado, num_bytes );
		printf("saiu da busca e nodo = %p\n", nodo);
		// Se encontra um nodo com a quantidade de bytes pedidos
		if ( nodo != NULL ) {
			printf("Retirando dos desalocados e inserindo nos alocados\n\n");
			// Busca o nodo desalocado e retira da árvore dos desalocados
			*end_raiz_desalocado =Busca_desalocado ( *end_raiz_desalocado, nodo);

			// Insere o nodo desalocado na árvore dos alocados
			Arruma_Apontadores_Desalocados ( end_raiz_alocado, nodo);

			// Informa que o nodo foi desalocado
			*nodo = 1;	

			return 1;
		}

		// Senão informa o programa que não encontrou
		else {
			printf("Não encontrou nenhum nodo para ser retirado dos desalocados\n\n");
			return 0;
		}	
	}
}


//---------------------------------------------------------
// Inicio da função que acerta os apontadores na árvore dos alocados
//---------------------------------------------------------
//---------------------------------------------------------
// Função que arruma os apontadores na árvore dos nodos alocados quando é inserido 
// um novo nodo
//---------------------------------------------------------
int	*Arruma_Apontadores (int *p, int *end_ultimo_alocado, int *colocou ) {

	int des_bal= 1, des_esq= 3, des_dir = 4, *p_esq, *p_dir;

	// Se o nodo corrente é NULL
	if ( p == NULL ) {
		*colocou = 1;
		return end_ultimo_alocado; // devolve o endereço do ultimo nodo alocado
	}

	// Escontra os apontadores para o filho esquerdo e direito do nodo corrente
	p_esq = *(p + des_esq);
	p_dir = *(p + des_dir);
	// Se ainda não chegou em um nodo NULL
	if ( !(*colocou) ) {
		// Testa se o endereço do último nodo alocado é menor que o nodo corrente
		// Então vai para esquerda 
		// Observação a comparação está sendo feito ao contrário e ainda não sei 
		// o por que se deve fazer isto
		if ( end_ultimo_alocado > p ) {
		// Se exite o nodo esquerdo e não existe o nodo direito
		//if ( (*(p + des_esq) != NULL) && (*(p + des_dir) == NULL) ) {
			printf("Entrou a direita\n");
			*(p + des_dir) = Arruma_Apontadores( p_dir , end_ultimo_alocado, colocou);
 			*(p + des_bal) =  *(p + des_bal) - 1 ;
		}
		// Senão o endereço do último alocado é maior, então vai para direita
		else {
			printf("Entrou a esquerda\n");
			*(p + des_esq) = Arruma_Apontadores( p_esq , end_ultimo_alocado, colocou);
 			*(p + des_bal) =  *(p + des_bal) + 1;
		}
	}
	return p;
	printf("Terminou a função\n");
}
//---------------------------------------------------------
// Final da função que acerta os apontadores na árvore dos alocados
//---------------------------------------------------------

//---------------------------------------------------------
// Inicio das funções que desalocam nodos
//---------------------------------------------------------
//---------------------------------------------------------
// Função que verifica se o nodo já foi alocado
//---------------------------------------------------------
int Nodo_esta_alocado (int *p, int *end_desalocado) {

	int des_esq= 3, des_dir = 4, 
	*p_esq = *(p + des_esq), 
	*p_dir= *(p + des_dir );

	if (p == end_desalocado)
		return 1;

	if (p_esq == NULL && p_dir == NULL)
		return 0;
	if (p_esq == NULL)
		return Nodo_esta_alocado (p_dir, end_desalocado);
	else
		return Nodo_esta_alocado (p_esq, end_desalocado);
}

//---------------------------------------------------------
// Busca o nodo que é o antecessor do nodo sendo retirado para substitui-lo
//---------------------------------------------------------
int *Antecessor (int *p) {

	int des_dir = 4, 
	*p_dir= *(p + des_dir );

	if ( p_dir == NULL)
		return p;

	return Antecessor ( p_dir );

}


//---------------------------------------------------------
// Busca o nodo que é o sucessor do nodo sendo retirado para substitui-lo
//---------------------------------------------------------
int *Sucessor (int *p) {

	int des_esq = 3, 
	*p_esq= *(p + des_esq );

	if ( p_esq == NULL)
		return p;

	return Sucessor ( p_esq );

}


//---------------------------------------------------------
// Função que busca o nodo desalocado na árvore dos desalocados e faz o nodo que 
// apontava pra ele apontar para NULL ou para outro nodo
//---------------------------------------------------------
int 	*Busca_desalocado (int *p, int *end_desalocado) {

	int des_esq = 3, des_dir = 4,
	*end_p_esq = p + des_esq, 
	*end_p_dir= p + des_dir,
	*end_desalocado_esq= end_desalocado + des_esq,
	*end_desalocado_dir= end_desalocado + des_dir,
	*p_esq = *(p + des_esq), *p_dir= *(p + des_dir );
	
	// Se encontrou o nodo que deve ser retirado da árvore dos alocados
	if ( p == end_desalocado ) {

		// Já que ele vai ser inserido na outra árvore então ele não tem filhos
		*end_desalocado_esq = NULL;
		*end_desalocado_dir = NULL;

		// Se é uma folha da árvore dos alocados
		if ( p_esq == NULL && p_dir == NULL) {
			return NULL;
		}
		else {

			// Se o nodo não tiver nodo esquerdo 
			if ( p_esq == NULL)
				return Sucessor ( p_dir );

			// Senão se não tiver nodo direito 
			else
				return Antecessor ( p_esq );
		}
	}

	if ( p_esq != NULL)
		*end_p_esq =Busca_desalocado ( p_esq, end_desalocado);

	if ( p_dir != NULL) {
		*end_p_dir =Busca_desalocado ( p_dir, end_desalocado);
	}

	return p;
}


//---------------------------------------------------------
// Função que arruma os apontadores na árvore dos nodos desalocados
// isto é inserindo o nodo na árvore dos desalocados
//---------------------------------------------------------
int	*Arruma_Apontadores_Desalocados_R (int *p, int *end_desalocado) {

	int des_num= 2, des_esq = 3, des_dir = 4, 
	*end_p_esq = end_desalocado + des_esq, 
	*end_p_dir= end_desalocado + des_esq;

	// Se chegar em nodo NULL pode inserir o nodo desalocado
	if ( p == NULL) {
		*end_p_esq= NULL;
		*end_p_dir= NULL;
		return end_desalocado;
	}

	int *p_num= p + des_num, *desalocado_num = end_desalocado + des_num,
	*p_esq= *(p + des_esq), *p_dir= *(p + des_esq);
	

	// Está sendo feito uma árvore AVL com ordem em quantidade de bytes alocados
	// Se o número de bytes do nodo desalocado for menor, vai para esquerda
	if ( *desalocado_num <= *p_num ) {
		*(p + des_esq) = Arruma_Apontadores_Desalocados_R( p_esq, end_desalocado);
	}

	// Se o número de bytes do nodo desalocado for maior ou igual, vai para direita
	else {
		*(p + des_dir) = Arruma_Apontadores_Desalocados_R( p_dir, end_desalocado);
	}	
	
	return p;
}

//---------------------------------------------------------
// Função que arruma os apontadores 
// tanto na árvore dos alocados e dos desalocados
//---------------------------------------------------------
int	*Arruma_Apontadores_Desalocados (int *p, int *end_desalocado) {

	int des_esq = 3, des_dir = 4, 
	*end_p_esq = end_desalocado + des_esq, 
	*end_p_dir= end_desalocado + des_esq, 
	h, *raiz_desalocado;

	// Se o conteúdo do ponteiro não aponta pra nada. no caso é um teste para a raiz
	if ( *p == NULL) {
		*p = end_desalocado;
		*end_p_esq= NULL;
		*end_p_dir= NULL;

		return p; 
	}

	// Senão procura um nodo NULL para fazer a inserção na árvore
	return Arruma_Apontadores_Desalocados_R ( *p, end_desalocado );
}


//---------------------------------------------------------
// Função que desaloca um nodo pedido
//---------------------------------------------------------
void  meuLiberaMem ( void *ptr ) {

	int des_ocup = -7, *ptrr = ptr, 
	*end_brk, *end_raiz, *end_raiz_alocado, *end_raiz_desalocado,
	*end_desalocado = ptrr + des_ocup, 
	*aux ;

	// Coloca o nodo na árvore dos desalocados
	end_brk = sbrk(0);
	end_raiz = *end_brk; 

	end_raiz_alocado = end_raiz + 2 ; // Raiz da árvore dos alocados
	end_raiz_desalocado = end_raiz + 3; // Raiz da árvore dos desalocados

	if ( Nodo_esta_alocado (*end_raiz_alocado, end_desalocado) ) {

		// Busca o nodo desalocado e retira da árvore dos alocados
		*end_raiz_alocado =Busca_desalocado ( *end_raiz_alocado, end_desalocado);

		// Insere o nodo desalocado na árvore dos desalocados
		Arruma_Apontadores_Desalocados ( end_raiz_desalocado, end_desalocado);

		// Informa que o nodo foi desalocado
		*ptrr = 0;	
	}
}
//---------------------------------------------------------
// Final das funções que desalocam nodos
//---------------------------------------------------------

//---------------------------------------------------------
// Função que aloca
//---------------------------------------------------------
main () {

	static int des_ocup = 1;
	static int des_bal = 2;
	static int des_num = 3;
	static int des_esq = 4;
	static int des_dir = 5;
	static int des_endi = 6;
	static int des_endf = 7;
	static int first = 1;
	int *end_brk, *end_brk_aux, *end_brk_ant, *end_raiz, *end_primeiro, 
	*end_ultimo, *end_ultimo_alocado, *colocou, i, a, b, num_bytes = 100, sai =0 , 
	teste= 2, *end_teste, abcd = 20, *end_ultimo_aux; 
	*colocou = 0;

	while (sai != 3) {
	sai++;
	num_bytes = num_bytes /2;
	abcd++;
	if ( first ) {
		Cria_Raiz ( num_bytes );
		first = 0; 
	}

	// Testa se precisa aumentar a brk
	if (! Exite_Nodo_Desalocado( num_bytes) ) {

		// Guarda o antigo valor da brk para atribuir ao próximo valor da brk o endereço da raiz
		end_brk_ant = sbrk(0);

		// Somente para teste sempre joga a brk para cima 100 posições
		sbrk(100000000000000);

		// Atualiza o endereço da raiz no novo valor da brk
		end_raiz = *end_brk_ant;
		end_brk = sbrk(0);	
		*end_brk = end_raiz;

		// Encontra o valor do último nodo alocado
		end_ultimo = *(end_raiz + des_ocup);
	
		// Para teste 
		if ( teste == 1 ) {
			end_teste = *(end_raiz + des_ocup);
		}
		teste--;

		// Coloca os valores da estrutura na heap
		end_ultimo_alocado = end_ultimo + des_ocup;	 // guardando o valor para arrumar na árvore
		*(end_ultimo + des_ocup) = 1; // Ocupado
		*(end_ultimo + des_bal) = 0; // Balanceamento inicial
		*(end_ultimo + des_num) = num_bytes; // Número de bytes alocados
		*(end_ultimo + des_esq) = NULL; // Endereço do filho esquerdo
		*(end_ultimo + des_dir) = NULL; // Endereço do filho direito
		*(end_ultimo + des_endi) = end_ultimo + des_endi + 2; // Endereço de inicio do nodo
		*(end_ultimo + des_endf) = end_ultimo + des_endi + 1 + num_bytes; // Endereço do fim do nodo	
	
		// Coloca o valor 20 aonde foi armazenado o que foi pedido
		for (i=0; i<num_bytes; i++) {
			*(end_ultimo + des_endi + 2 + i) = abcd; 
		}
	
		// Imprime os valores na heap a partir do endereço do inicio do nodo
		printf("----------------------------------\n");
		for (i=0; i<(num_bytes + des_endf); i++) {
			a = 1 + end_ultimo  + i;  
			b = *(1 + end_ultimo + i);  
			printf("o endereço e %p\n", a);
			printf("o conteudo e %d\n", b);

		}
		printf("----------------------------------\n");
		end_ultimo_aux = end_ultimo;
		// Atualiza o último alocado
		end_ultimo = *(end_ultimo + des_endf);
		*(end_raiz + 1 ) = end_ultimo;
		end_brk = sbrk(0);
		end_raiz = *end_brk;
		*(end_raiz + 1 ) = end_ultimo;

		// Arruma os ponteiros na árvore
		printf("Arrumando os ponteiros da árvore\n");
		end_brk = sbrk(0);
		end_raiz = *end_brk;
		end_primeiro = *end_raiz;
		Arruma_Apontadores ( end_primeiro, end_ultimo_alocado, colocou);
		printf("----------------------------------\n");
		printf("-apos estar arrumando ponteiros---\n");
		for (i=0; i<(num_bytes + des_endf); i++) {
			a = 1 + end_ultimo_aux  + i;  
			b = *(1 + end_ultimo_aux + i);  
			printf("o endereço e %p\n", a);
			printf("o conteudo e %d\n", b);

		}
		printf("----------------------------------\n");
		*colocou = 0;
		}

	printf("Imprime as árvores 1\n");
	printf("------------------------------------------------------------------\n");
	imprMapa ( );
	printf("------------------------------------------------------------------\n");

		if ( teste == 1 ) {
			printf("desalocando um nodo ====================================\n");
			printf("teste = %p\n", end_ultimo_alocado + 7);
			meuLiberaMem ( end_ultimo_alocado + 7 );
		}

	printf("Imprime as árvores 2\n");
	printf("------------------------------------------------------------------\n");
	imprMapa ( );
	printf("------------------------------------------------------------------\n");

	}

	// No final retirar o número 7 pois está sendo passando para a função 
	// O endereço de inicio da alocacção dos bytes que o usuário vai utilizar
	meuLiberaMem ( end_ultimo_alocado + 7 );
	meuLiberaMem ( end_teste + 8 );
	
	printf("Imprime as árvores\n");
	printf("------------------------------------------------------------------\n");
	imprMapa ( );
	printf("------------------------------------------------------------------\n");
}
