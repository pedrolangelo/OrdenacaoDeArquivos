#include <stdio.h>
#include <string.h>
#include <locale.h>

typedef struct _Endereco Endereco;

struct _Endereco {
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void* c1, const void* c2){
    
    return strncmp(((Endereco*)c1)->cep,((Endereco*)c2)->cep,8);

}

int main(int argc, char**argv) {
    FILE *f = fopen ("cep.dat", "r"), *arq;
    long tamanho, quantidade, partes, quantidade2;
    Endereco *e;

    fseek (f, 0, SEEK_END);
    tamanho = ftell(f);
    quantidade = tamanho/sizeof(Endereco);
    partes = quantidade/16;

    rewind(f);

    printf("Gerando arquivos...");

    for (int i = 1; i <=16; i++){
        if (i<16)quantidade2 = partes;
        else quantidade2 = quantidade - (partes*15);

        e = (Endereco*) malloc(quantidade2*sizeof(Endereco));

        char arquivo[200];
        sprintf(arquivo,"cep_%d.dat",i);

        arq = fopen (&arquivo, "w");

        fread(e, sizeof(Endereco), quantidade2, f);
        qsort(e,quantidade2,sizeof(Endereco),compara);
        fwrite (e, sizeof(Endereco), quantidade2, arq);

        fclose(arq);
        free(e);

    }

    
    FILE *arq1, *arq2, *arq3;
    int x = 1, y = 17;
    Endereco e2, e3;

    printf("Concatenando...");

    while (y<32){
        char aux1[200], aux2[200], concatenado[200];

        sprintf(aux1,"cep_%d.dat",x++);
        sprintf(aux2,"cep_%d.dat",x++);
        if (y==31) sprintf(concatenado,"cep_ordenado.dat", y++);
        else sprintf(concatenado,"cep_%d.dat",y++);

        arq1 = fopen(aux1,"r");
        arq2 = fopen(aux2,"r");
        arq3 = fopen(concatenado,"w");

        fread(&e2,sizeof(Endereco),1,arq1);
	    fread(&e3,sizeof(Endereco),1,arq2);

        while(!feof(arq1) && !feof(arq2)){
            if(compara(&e2,&e3) < 0){
                fwrite(&e2,sizeof(Endereco),1,arq3);
                fread(&e2,sizeof(Endereco),1,arq1);
            }
            else{
                fwrite(&e3,sizeof(Endereco),1,arq3);
                fread(&e3,sizeof(Endereco),1,arq2);
            }
        }

        while(!feof(arq1)){
            fwrite(&e2,sizeof(Endereco),1,arq3);
            fread(&e2,sizeof(Endereco),1,arq1);		
        }

        while(!feof(arq2)){
            fwrite(&e3,sizeof(Endereco),1,arq3);
            fread(&e3,sizeof(Endereco),1,arq2);		
            }

    fclose(arq1);
	fclose(arq2);
	fclose(arq3);
    }
    printf("Arquivo Ordenado");
}