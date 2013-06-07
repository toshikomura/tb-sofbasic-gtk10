// Arquivo meuAlocador.h
void *meuAlocaMem ( int num_bytes );
void  meuLiberaMem ( void *ptr );
void  imprMapa ( );

// Funções acrescentadas
void	*Cria_Raiz ( int num_bytes );
int Exite_Nodo_Desalocado( int num_bytes );
int *Encontrou_nodo_desalocado (int *p, int num_bytes );
int *Busca_desalocado ( int *p, int *nodo );
int *Sucessor ( int *p );
int *Antecessor ( int *p );
int *rot_lef ( int *p );
int *rot_rig ( int *p );
int modulo ( int n );
int *Arruma_Apontadores_Alocados ( int *p, int *end_desalocado );
int *Arruma_Apontadores_Alocados_R ( int *p, int *end_desalocado );
int *Arruma_Apontadores_Desalocados ( int *p, int *end_desalocado );
int *Arruma_Apontadores_Desalocados_R ( int *p, int *end_desalocado );
int *Arruma_Apontadores ( int *end_primeiro, int *end_ultimo_alocado );
int Nodo_esta_alocado ( int *p, int *end_desalocado );
int *Balanceamento ( int *p);
int h_arvore ( int *p );
void	imprime ( int *p, int h );
