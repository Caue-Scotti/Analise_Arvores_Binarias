/*

Trabalho realizado por:
  Cauê Scotti - 00338597
  Rafael Morschbacher - 334857

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ABP{

    char nome_alimento[50];
    float calorias;
    struct ABP *esq;
    struct ABP *dir;

};typedef struct ABP ptABP;

struct AVL{

    char nome_alimento[50];
    int calorias;
    struct AVL *esq;
    struct AVL *dir;
    int FB;

};typedef struct AVL ptAVL;

//Variáveis globais:

int comp_abp, comp_avl, ok, rot;


int true_strcmp (char str1[50], char str2[50]){     //Corrige alguns critérios da função strcmp

    int i;
    char str1_aux[50], str2_aux[50];                //Transforma o caracter ' em espaço e torna todas as letras
    strcpy (str1_aux, str1);                        //minúsculas, para que não haja erro no ordenamento alfabetico
    strcpy (str2_aux, str2);

    for (i = 0; i < 48; i++){
        if (!isalpha(str1_aux[i]))
            str1_aux[i] = ' ';
        if (!isalpha(str2_aux[i]))
            str2_aux[i] = ' ';
        str1_aux[i] = tolower(str1_aux[i]);
        str2_aux[i] = tolower(str2_aux[i]);
    }

    return (strcmp(str1_aux, str2_aux));
}


ptABP* insere_abp (ptABP *p_abp, char nome[50], float cal){     //Insere um nodo na ABP passada como parâmetro (igual ao moodle)

    if (p_abp == NULL){
        p_abp = (ptABP*)malloc(sizeof(ptABP));
        strcpy (p_abp -> nome_alimento, nome);
        p_abp -> calorias = cal;
        p_abp -> esq = NULL;
        p_abp -> dir = NULL;
        return p_abp;
    }

    else{
        if (true_strcmp(nome, p_abp -> nome_alimento) == -1)
            p_abp -> esq = insere_abp (p_abp -> esq, nome, cal);
        else
            p_abp -> dir = insere_abp (p_abp -> dir, nome, cal);
    }

    return p_abp;
}


ptABP* Consulta_ABP (ptABP *p_abp, char nome[50]){      //Recebe o nome de algum alimento para consultar na abp (igual ao moodle)

    while (p_abp != NULL){
        comp_abp++;
        if (!strcmp(p_abp -> nome_alimento, nome)){
            comp_abp++;
            return p_abp;
        }
        else {
            comp_abp++;
            if (strcmp(p_abp -> nome_alimento, nome)>0)
                p_abp = p_abp -> esq;
            else
                p_abp = p_abp -> dir;
        }
    }

    return NULL;
}


ptAVL* Consulta_AVL (ptAVL *p_avl, char nome[50]){      //Recebe o nome de algum alimento para consultar na avl (igual ao moodle)

    while (p_avl != NULL){
        comp_avl++;
        if (!strcmp(p_avl -> nome_alimento, nome)){
            comp_avl++;
            return p_avl;
        }
        else {
            comp_avl++;
            if (strcmp(p_avl -> nome_alimento, nome)>0)
                p_avl = p_avl -> esq;
            else
                p_avl = p_avl -> dir;
        }
    }

    return NULL;
}


ptABP* le_arquivo_abp (ptABP* p_abp, char nome_arq[50]){     //Pega os dados do arquivo das calorias e salva na árvore

    char tst[50] = {0}, *p, alimento[50];
    float caloria;
    int i = 0, j;

    //O trecho abaixo serve para transformar a string de tamanho 50 em uma string com tamanho definido pela existência ou não
    //de caracteres. Para isso, toda a string é percorrida e, enquanto existirem caracteres, o contador i é aumentado. Quando
    //a string termina, temos então o tamanho real da string com o nome do arquivo para ser aberto

    while (isalpha(nome_arq[i]) != 0 || isdigit(nome_arq[i]) != 0 || nome_arq[i] == '_' || nome_arq[i] == '.' || nome_arq[i] == '-' || nome_arq[i] == ' ')
        i++;
    char read_nom_tr[i+1];
    for (j = 0; j < i; j++)
        read_nom_tr[j] = nome_arq[j];
    read_nom_tr[i] = '\0';

    //Com o nome do arquivo já otimizado, é possível abrir o arquivo desejado

    FILE *arq;
    arq = fopen (read_nom_tr, "r");
    if (arq == NULL){
        printf("Não foi possivel abrir a base de dados indicada");
        return NULL;
    }

    while (feof(arq) == 0){
        fgets(tst, 50, arq);            //Utilizando tokenização para pegar os nomes
        p = strtok (tst, ";");          //dos alimentos e suas calorias correspondentes
        strcpy (alimento, p);           //Em seguida, os dados são inseridos na abp
        p = strtok (NULL, ";");
        caloria = atof (p);

        p_abp = insere_abp (p_abp, alimento, caloria);
    }

    fclose(arq);

    return p_abp;
}


int altura_abp(ptABP* p_abp){       //Cálculo de altura da ABP

    int altura_esq, altura_dir;

    if (p_abp == NULL)
        return 0;

    else{
        altura_esq = 1 + altura_abp(p_abp->esq);
        altura_dir = 1 + altura_abp(p_abp->dir);

        if(altura_esq >= altura_dir)
            return altura_esq;
        else
            return altura_dir;
    }

}


int numero_nodos_abp(ptABP* p_abp){     //Cálculo do número de nodos da ABP
    int num_nodos_esq, num_nodos_dir;

    if (p_abp == NULL)
        return 0;
    else{
        num_nodos_esq = 1 + numero_nodos_abp(p_abp->esq); //Somo +1 apenas no esquerdo para não contar 2 vezes o mesmo nodo
        num_nodos_dir = numero_nodos_abp(p_abp->dir);

        return num_nodos_esq + num_nodos_dir;

    }

}


void salva_arquivo (ptABP* arv_abp, ptAVL* arv_avl, char r_arq[50], char w_arq[50]){      //Cria o arquivo com a saída da alimentação diária

    ptABP *nodo;
    ptAVL *nodo1;
    float g_ing, cal, cal_total = 0;
    char linha[50] = {0}, *b, consulta[50];
    b = NULL;
    int i = 0, j;

    //O trecho a seguir é igual ao trecho comentado na função le_arquivo

    while (isalpha(r_arq[i]) != 0 || isdigit(r_arq[i]) != 0 || r_arq[i] == '_' || r_arq[i] == '.' || r_arq[i] == '-' || r_arq[i] == ' '){
        i++;
    }
    char read_nom_tr[i+1];
    for (j = 0; j < i; j++)
        read_nom_tr[j] = r_arq[j];
    read_nom_tr[i] = '\0';

    //Abre o arquivo dos alimentos ingeridos no dia informado pelo usuário

    FILE *read = fopen(read_nom_tr, "r");
    if (read == NULL){
        printf("\nImpossivel escrever arquivo texto");
        return;
    }

    while (isalpha(w_arq[i]) != 0 || isdigit(w_arq[i]) != 0 || w_arq[i] == '_' || w_arq[i] == '.' || w_arq[i] == '-' || w_arq[i] == ' ')
        i++;
    char write_nom_tr[i+1];
    for (j = 0; j < i; j++)
        write_nom_tr[j] = w_arq[j];
    write_nom_tr[i] = '\0';


    //Abre o arquivo de saída e começa a escrevê-lo

    FILE *write = fopen (write_nom_tr, "w");
    if (write == NULL){
        printf("Nao criou o arquivo");
        return;
    }

    fprintf(write, "==================== ANALISE DOS DADOS ====================\n\n");

    while (feof(read) == 0){

        fgets(linha, 50, read);             // Através da tokenização, os dados passados no arquivo de
        b = strtok (linha, ";");            //leitura são comparados com os dados já importados anteriormente,
        strcpy (consulta, b);               //que foram alocados na árvore
        b = strtok (NULL, ";");
        g_ing = atof (b);

        nodo1 = Consulta_AVL(arv_avl, consulta);
        nodo = Consulta_ABP(arv_abp, consulta);

        if (nodo != NULL){
            cal = ((nodo -> calorias)*g_ing) / 100;   //Regra de 3 para saber quantas calorias foram ingeridas
            fprintf(write, "%.0fg de %s (%.0f calorias por 100g) = %.2f calorias\n", g_ing, consulta, nodo -> calorias, cal);
            cal_total += cal;
        }

    }

    fprintf(write, "\nTotal de %.2f calorias consumidas no dia\n", cal_total);

    fprintf(write,"\n========  ESTATÍSTICAS ABP ============\n");
    fprintf(write, "\nNumero de nodos: %d \n", numero_nodos_abp(arv_abp));
    fprintf(write, "\nAltura: %d \n", altura_abp(arv_abp));
    fprintf(write, "\nRotações: 0\n");
    fprintf(write, "\Comparações: %d\n", comp_abp);

    fprintf(write, "\n=======  ESTATÍSTICAS AVL ============\n");
    fprintf(write, "\nNumero de nodos: %d \n", numero_nodos_avl(arv_avl));
    fprintf(write, "\nAltura: %d \n", altura_avl(arv_avl));
    fprintf(write, "\nRotações: %d\n", rot);
    fprintf(write, "\Comparações: %d\n", comp_avl);

    fclose (write);
    fclose (read);

    return;
}

/*-----------------------PARTE DE IMPLEMENTAÇÃO DE AVL------------------------------------

 As funções abaixo constituem o grupo de funções que implementam a avl. As funções le_arquivo_avl
 e Insere_AVL são iguais às de abp. As funções são semelhantes às disponibilizadas no moodle
*/

int altura_avl (ptAVL *a)
{
    int Alt_Esq, Alt_Dir;
    if (a == NULL)
      return 0;
    else
    {
       Alt_Esq = altura_avl (a->esq);
       Alt_Dir = altura_avl (a->dir);
       if (Alt_Esq > Alt_Dir)
         return (1 + Alt_Esq);
       else
         return (1 + Alt_Dir);
     }
}


int numero_nodos_avl(ptAVL* p_avl){
    int num_nodos_esq, num_nodos_dir;

    if (p_avl == NULL)
        return 0;
    else{
        num_nodos_esq = numero_nodos_abp(p_avl->esq);
        num_nodos_dir = numero_nodos_abp(p_avl->dir);

        return 1+ num_nodos_esq + num_nodos_dir;

    }

}


int Calcula_FB(ptAVL *a)
{
    return (altura_avl(a->esq) - altura_avl(a->dir));
}


ptAVL* rotacao_direita(ptAVL *pt){
   ptAVL* ptu;

   ptu = pt->esq;
   pt->esq = ptu->dir;
   ptu->dir = pt;
   pt->FB = 0;
   pt = ptu;
   rot++;
   return pt;
}


ptAVL* rotacao_esquerda(ptAVL *pt){
   ptAVL* ptu;

   ptu = pt->dir;
   pt->dir = ptu->esq;
   ptu->esq = pt;
   pt->FB = 0;
   pt = ptu;
   rot++;
   return pt;
}


ptAVL* rotacao_dupla_direita (ptAVL* pt){
   ptAVL* ptu, *ptv;

   ptu = pt->esq;
   ptv = ptu->dir;
   ptu->dir = ptv->esq;
   ptv->esq = ptu;
   pt->esq = ptv->dir;
   ptv->dir = pt;
   if (ptv->FB == 1)   pt->FB = -1;
      else pt->FB = 0;
   if (ptv->FB == -1)  ptu->FB = 1;
      else ptu->FB = 0;
   pt = ptv;
   rot ++;
   return pt;
}


ptAVL* rotacao_dupla_esquerda (ptAVL* pt){
   ptAVL *ptu, *ptv;

   ptu = pt->dir;
   ptv = ptu->esq;
   ptu->esq = ptv->dir;
   ptv->dir = ptu;
   pt->dir = ptv->esq;
   ptv->esq = pt;
   if (ptv->FB == -1) pt->FB = 1;
     else pt->FB = 0;
   if (ptv->FB == 1) ptu->FB = -1;
      else ptu->FB = 0;
   pt = ptv;
   rot ++;
   return pt;
}


ptAVL* Caso1 (ptAVL* a , int *ok)
{
   ptAVL *ptu;

	ptu = a->esq;
	if (ptu->FB == 1)
    {
        a = rotacao_direita(a);
     }
    else
    {
        a = rotacao_dupla_direita(a);
    }

    a->FB = 0;
	*ok = 0;
	return a;
}

ptAVL* Caso2 (ptAVL *a , int *ok)
{
    ptAVL *ptu;

	ptu = a->dir;
	if (ptu->FB == -1)
    {
       a=rotacao_esquerda(a);
    }
    else
    {
       a=rotacao_dupla_esquerda(a);
    }
	a->FB = 0;
	*ok = 0;
	return a;
}

ptAVL* InsereAVL (ptAVL *a, char nome[50], int cal, int *ok)
{

     if (a == NULL)
     {
     	a = (ptAVL*) malloc(sizeof(ptAVL));
     	strcpy(a->nome_alimento, nome);
     	a->calorias = cal;
        a->esq = NULL;
        a->dir = NULL;
        a->FB = 0;
	    *ok = 1;
     }
     else
     if (true_strcmp(nome, a->nome_alimento) == -1)
     {
		a->esq = InsereAVL(a->esq, nome, cal, ok);
        if (*ok)
        {
   		    switch (a->FB) {
        	  case -1:  a->FB = 0; *ok = 0; break;
			  case  0:  a->FB = 1;  break;
			  case  1:  a=Caso1(a,ok); break;
            }
         }
     }
	 else
     {
  		    a->dir = InsereAVL(a->dir,nome,cal,ok);
            if (*ok)
            {
              switch (a->FB) {
                case  1:  a->FB = 0; *ok = 0; break;
                case  0:  a->FB = -1; break;
			    case -1:  a = Caso2(a,ok); break;
             }
            }
     }
     return a;
}


ptAVL* le_arquivo_avl (ptAVL* p_avl, int* ok, char nome_arq[50]){

    char tst[50] = {0}, *p, alimento[50];
    int caloria;
    int i, j;

    while (isalpha(nome_arq[i]) != 0 || isdigit(nome_arq[i]) != 0 || nome_arq[i] == '_' || nome_arq[i] == '.' || nome_arq[i] == '-' || nome_arq[i] == ' ')
        i++;
    char read_nom_tr[i+1];
    for (j = 0; j < i; j++)
        read_nom_tr[j] = nome_arq[j];
    read_nom_tr[i] = '\0';

    FILE *arq;
    arq = fopen (read_nom_tr, "r");
    if (arq == NULL){
        printf("Não foi possivel abrir a base de dados indicada");
        return NULL;
    }

    while (feof(arq) == 0){
        fgets(tst, 50, arq);
        p = strtok (tst, ";");
        strcpy (alimento, p);
        p = strtok (NULL, ";");
        caloria = atof (p);
        p_avl = InsereAVL(p_avl, alimento, caloria, ok);
    }

    fclose(arq);

    return p_avl;
}



//--------------------------FIM IMPLEMENTAÇÃO DE AVL-------------------------------------//


int main (int argc, char *argv[]){

    char base_dados[50] = {0}, relatorio_diario[50] = {0}, saida[50] = {0};

    ptABP *arvoreABP;
    arvoreABP = NULL;

    ptAVL *arvoreAVL;
    arvoreAVL = NULL;

    if (argc != 4){
        printf("Quantidade invalida de parametros");
        return 0;
    }

    strcpy(base_dados, argv[1]);
    strcpy(relatorio_diario, argv[2]);
    strcpy(saida, argv[3]);

    arvoreABP = le_arquivo_abp (arvoreABP, base_dados);
    if (arvoreABP == NULL)
        return 0;
    arvoreAVL = le_arquivo_avl (arvoreAVL, &ok, base_dados);

    salva_arquivo(arvoreABP, arvoreAVL, relatorio_diario, saida);

    return 0;

}
