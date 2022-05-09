#include <stdio.h>
#include <math.h>

//Cabeçalho com definições para estrutura do código
#define BINNARYLENGTH 16         //Tamanho do numero binário (16 bits)
#define MAXVALUEPOSITIVE 32767    //Maior valor inteiro representado pelo binario de 16 bits signed
#define MAXVALUENEGATIVE -32767   //Menor valor inteiro representado pelo binario de 16 bits signed
#define TRUE 1
#define FALSE 0

//Funcao para mapeamento de avisos do software
char msg[4][255] = {
    "Um numero decimal excede a quantidade de bits suportada quando transformado em binario\n", 
    "Estouro de Bits para o calculo\n",
    "Opcao Invalida\n",
    "Finalizando Calculadora\n"
};
void msgShow(int code){
    code = code - 1;
    printf("\n[---WARNING---]: %s\n", msg[code]);
}

//Funcao para imprimir um numero binario ao usuario
void showBinary(int *bin){
    int i;
    for(i = 0; i < BINNARYLENGTH; i++){
        printf("%d", bin[i]);
    }
}

//Funcao para a coleta de numeros decimais
int getDecimalNumber(int op){
    int decimalNumber;
    char opcoes[4][20] = {"Multiplicador", "Multiplicando", "Dividendo", "Divisor"};
    printf("\nDigite o %s em decimal: ", opcoes[op]);
    scanf("%d", &decimalNumber);
    return decimalNumber;
}

//Funcao para o deslocamento à direita realizado na multiplicacao
int shiftMult(int *A, int *Q, int inicialA){
    int resto, i;
    resto = Q[BINNARYLENGTH - 1];
    for(i = BINNARYLENGTH - 1; i >= 0; i--){
        if(i == 0){
            Q[i] = A[BINNARYLENGTH - 1];
        }else{
            Q[i] = Q[i - 1];
        }
        
    }
    for(i = BINNARYLENGTH - 1; i >= 0; i--){
        if(i == 0){
            A[i] = inicialA;
        }else{
            A[i] = A[i - 1];
        }
        
    }
    return resto;
}

//Funcao para o deslocamento à esquerda realizado na divisao
void shiftDiv(int *A, int *Q){
    int i;
    for(i = 0; i < BINNARYLENGTH; i++){
        if(i == BINNARYLENGTH-1){
            A[i] = Q[0];
        }else{
            A[i] = A[i + 1];
        }
        
    }
    for(i = 0; i < BINNARYLENGTH; i++){
        if(i == BINNARYLENGTH-1){
            Q[i] = 0;
        }else{
            Q[i] = Q[i + 1];
        }
    }
}

//Funcao para realizar a soma de dois numeros binarios e retornar o carry (se houver)
int sumBinnary(int *bin1, int *bin2, int *result){
    int i, carry = 0;
    for(i = BINNARYLENGTH-1; i >= 0; i--){
        if(carry == 1){
            if(bin1[i] == 1 && bin2[i] == 1){
                result[i] = 1;
                carry = 1;
            }else if(bin1[i] == 1 || bin2[i] == 1){
                result[i] = 0;
                carry = 1;
            }else{
                result[i] = 1;
                carry = 0;
            }
        }else{
            if(bin1[i] == 1 && bin2[i] == 1){
                result[i] = 0;
                carry = 1;
            }else if(bin1[i] == 1 || bin2[i] == 1){
                result[i] = 1;
                carry = 0;
            }else{
                result[i] = 0;
                carry = 0;
            }
        }
    }
    return carry;
}

//Funcao para criar o complemento de dois de um numero binario
void getBinaryComplement(int *binary, int *complement){
    int realComplement[BINNARYLENGTH];
    int binOne[BINNARYLENGTH];
    int binOneComplement[BINNARYLENGTH];

    getBinaryNumber(1, binOne, binOneComplement, FALSE);

    int i;
    for(i = 0; i < BINNARYLENGTH; i++){
        if(binary[i] == 1){
            complement[i] = 0;
        }else{
            complement[i] = 1;
        }
    }

    sumBinnary(complement, binOne, realComplement);

    for(i = 0; i < BINNARYLENGTH; i++){
        complement[i] = realComplement[i];
    }
}

//Funcao que transforma um numero decimal em binario
int getBinaryNumber(int decimal, int *binary, int *complement, int getComp){
    int i = 0, j = 0;
    int needComplement = FALSE;
    int num[BINNARYLENGTH];

    //Verificação do numero decimal, se for maior que o numero de bits suportado, retorna erro
    if(decimal > MAXVALUEPOSITIVE || decimal < MAXVALUENEGATIVE){
        return FALSE;
    }

    //Verificação do sinal do numero decimal
    if(decimal < 0){
        decimal = -decimal;
        needComplement = TRUE;
    }

    //Preparaçao do numero binario final
    for(int w = 0; w < BINNARYLENGTH; w++){
        binary[w] = 0;
        num[w] = 0;
        complement[w] = 0;
    }
    
    //Conversao Binaria
    while(decimal > 0){
        binary[i] = decimal % 2;
        decimal = decimal / 2;
        i++;
    }

    j = BINNARYLENGTH-i;
    for(i=i-1;i>=0;i--,j++){
        num[j] = binary[i]; 
    }

    //Reorganização do numero binario final
    for(i = 0; i < BINNARYLENGTH; i++){
        binary[i] = num[i];
    }

    if(getComp){
        getBinaryComplement(binary, complement);
    }

    if(needComplement){
        for(i = 0; i < BINNARYLENGTH; i++){
            binary[i] = complement[i];
            complement[i] = num[i];
        }
        return TRUE;
    }else{
        return TRUE;
    }
}

//Funcao CORE que realiza todas as operacoes necessarias para a multiplicacao em complemento de dois
void multProcess(){
    int q_1 = 0, z, initial = 0;
    int a, b, controller = FALSE;
    //Inicializaçao do numero A
    int A[BINNARYLENGTH];

    //Inicializaçao do Multiplicador e do Multiplicando
    int Q[BINNARYLENGTH], M[BINNARYLENGTH];

    //Inicializacao dos vetores para o complemento de dois
    int Qcomp[BINNARYLENGTH], Mcomp[BINNARYLENGTH];

    //Vetor auxiliar para a resposta proveniente da soma
    int resSum[BINNARYLENGTH];

    //Coleta dos numeros decimais
    a = getDecimalNumber(0);
    b = getDecimalNumber(1);

    if (getBinaryNumber(a, Q, Qcomp, TRUE)){
        if (getBinaryNumber(b, M, Mcomp, TRUE)){
            controller = TRUE;
        }
    }

    //Reset do vetor binario para A
    for(int i = 0; i < BINNARYLENGTH; i++){
        A[i] = 0;
    }

    //Execucao do algoritmo de Booth para a multiplicacao
    if(controller){
        printf("\nMultiplicador: ");
        showBinary(Q);
        printf("\nMultiplicando: ");
        showBinary(M);
        printf("\nComplemento Binario Multiplicando: ");
        showBinary(Mcomp);
        printf("\n-------------------------------------------------");
        printf("\n[Valores Iniciais]");
        printf("\nA: ");
        showBinary(A);
        printf("\nQ: ");
        showBinary(Q);
        printf("\nQ_1: %d", q_1);
        printf("\nM: ");
        showBinary(M);
        for(int i = 0; i < BINNARYLENGTH; i++){
            initial = 0;
            printf("\n-------------------------------------------------");
            printf("\n[Ciclo %d]", i+1);
            if(Q[BINNARYLENGTH-1] == 1 && q_1 == 0){
                printf("\nA <- A - M");
                sumBinnary(A, Mcomp, resSum);
                initial = 1;
                for(z = 0; z < BINNARYLENGTH; z++){
                    A[z] = resSum[z];
                }
                printf("\nA: ");
                showBinary(A);
                printf("\nQ: ");
                showBinary(Q);
                printf("\nQ_1: %d", q_1);
                printf("\nM: ");
                showBinary(M);
            }else if(Q[BINNARYLENGTH-1] == 0 && q_1 == 1){
                printf("\nA <- A + M");
                sumBinnary(A, M, resSum);
                initial = 0;
                for(z = 0; z < BINNARYLENGTH; z++){
                    A[z] = resSum[z];
                }
                printf("\nA: ");
                showBinary(A);
                printf("\nQ: ");
                showBinary(Q);
                printf("\nQ_1: %d", q_1);
                printf("\nM: ");
                showBinary(M);
            }
            q_1 = shiftMult(A, Q, initial);
            printf("\nDeslocamento a direita");
            printf("\nA: ");
            showBinary(A);
            printf("\nQ: ");
            showBinary(Q);
            printf("\nQ_1: %d", q_1);
            printf("\nM: ");
            showBinary(M);
        }
        printf("\n-------------------------------------------------");
        printf("\n\nResultado da multiplicacao: ");
        showBinary(A);
        showBinary(Q);
    }else{
        msgShow(1);
    }
}

//Funcao CORE para a divisao em complemento de dois, juntamente com suas operacoes internas
void divProcess(){
    int a, b, controller = FALSE;
    int isZero = FALSE;
    //Inicializaçao do numero A
    int A[BINNARYLENGTH];

    //Inicializaçao do Divisor e do Dividendo
    int Q[BINNARYLENGTH], M[BINNARYLENGTH];

    //Inicializacao dos vetores para o complemento de dois
    int Qcomp[BINNARYLENGTH], Mcomp[BINNARYLENGTH];

    //Vetor auxiliar para a resposta proveniente da soma
    int resSum[BINNARYLENGTH];

    //Coleta dos numeros decimais
    a = getDecimalNumber(2);
    b = getDecimalNumber(3);

    if (getBinaryNumber(a, Q, Qcomp, TRUE)){
        if (getBinaryNumber(b, M, Mcomp, TRUE)){
            controller = TRUE;
        }
    }

    //Reset do vetor binario para A
    for(int i = 0; i < BINNARYLENGTH; i++){
        A[i] = 0;
    }

    //Execucao do algoritmo da divisao segundo fluxograma utilizado
    if(controller){
        printf("\nDividendo: ");
        showBinary(Q);
        printf("\nDivisor: ");
        showBinary(M);
        printf("\nComplemento Binario Divisor: ");
        showBinary(Mcomp);
        printf("\n-------------------------------------------------");
        printf("\n[Valores Iniciais]");
        printf("\nA: ");
        showBinary(A);
        printf("\nQ: ");
        showBinary(Q);
        printf("\nM: ");
        showBinary(M);
        printf("\nM': ");
        showBinary(Mcomp);
        for(int i = 0; i < BINNARYLENGTH; i++){
            isZero = TRUE;
            printf("\n-------------------------------------------------");
            printf("\n[Ciclo %d]", i+1);

            printf("\n\nDeslocamento a esquerda");
            shiftDiv(A, Q);
            printf("\nA: ");
            showBinary(A);
            printf("\nQ: ");
            showBinary(Q);
            printf("\nM: ");
            showBinary(M);
            printf("\nM': ");
            showBinary(Mcomp);

            printf("\n\nA <- A - M");
            sumBinnary(A, Mcomp, resSum);
            for(int i = 0; i < BINNARYLENGTH; i++){
                A[i] = resSum[i];
            }
            printf("\nA: ");
            showBinary(A);
            printf("\nQ: ");
            showBinary(Q);
            printf("\nM: ");
            showBinary(M);
            printf("\nM': ");
            showBinary(Mcomp);

            if(A[0] == 1){
                printf("\n\nA < 0: Q0 = 0, A <- A + M");
                Q[BINNARYLENGTH - 1] = 0;
                sumBinnary(A, M, resSum);
                for(int i = 0; i < BINNARYLENGTH; i++){
                    A[i] = resSum[i];
                }
                printf("\nA: ");
                showBinary(A);
                printf("\nQ: ");
                showBinary(Q);
                printf("\nM: ");
                showBinary(M);
                printf("\nM': ");
                showBinary(Mcomp);
            }else{
                printf("\n\nA >= 0: Q0 = 1");
                Q[BINNARYLENGTH - 1] = 1;
                printf("\nA: ");
                showBinary(A);
                printf("\nQ: ");
                showBinary(Q);
                printf("\nM: ");
                showBinary(M);
                printf("\nM': ");
                showBinary(Mcomp);
            }
        }
        printf("\n-------------------------------------------------");
        printf("\n\nResultado da divisao: ");
        printf("\nResto:");
        showBinary(A);
        printf("\nResultado:");
        showBinary(Q);
    }else{
        msgShow(1);
    }
}

//Funcao para mostrar o menu ao usuario
int showOptions(){
    int op = -1;
    do{ 
        printf("\n[---OPCOES---]:\n");
        printf("1 - Multiplicacao\n");
        printf("2 - Divisao\n");
        printf("0 - Sair\n");
        printf("[---OPCAO---]: ");
        scanf("%d", &op);
        if(op < 0 || op > 2){
            msgShow(3);
        }
    }while(op < 0 || op > 2);

    return op;
}

//Funcao main para o looping do programa
int main(){
    int resp = 0;
    do{
        resp = showOptions();
        switch (resp){
            case 0:
                msgShow(4);
            break;

            case 1:
                multProcess();
            break;

            case 2:
                divProcess();
            break;

            default:
            break;
        }
    }while (resp != 0);
    return 0;
}