//funciones a implementar {SWAP,MUL,DIV};

typedef struct{
char pos;
int tamano;
}TDD;

typedef void Funciones2op(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void ADD(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SUB(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SWAP(long int op1,long int op2,char top1,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);// terminarla
void MUL(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void DIV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void CMP(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SHL(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void SHR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void AND(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void OR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void XOR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);

Funciones2op *funciones[]={MOV,ADD,SUB,0,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR};
// AND OR XOR, afectan al registro CC, pero no se de q forma lo afecta?????

long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]);
void Ultima_operacion(long int TablaRegistros[],long int nz);

void Pre_Funcion(short int cant,long int op1,long int op2,char top1,char top2,char operacion,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){

    if (cant==2){
        long int valor=Operando2(op2,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
        if (operacion!=3)
            funciones[operacion](op1,top1,valor,TablaMemoria,TablaRegistros,TablaDeDatos);
        else
            SWAP(op1,op2,top1,top2,TablaMemoria,TablaRegistros,TablaDeDatos);
    }else{

    }
}

long int Operando2(long int op2,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
 char tiporeg;
 long int resultado;
    if (top2==2){
        tiporeg=op2>>4;
        if (tiporeg==3)
            resultado=(TablaRegistros[(op2&0x0F)]&0x0000FFFF);
        if (tiporeg==2)
            resultado=((TablaRegistros[(op2&0x0F)]&0x0000FF00)>>8);
        if (tiporeg==1)
            resultado=(TablaRegistros[(op2&0x0F)]&0x000000FF);
        if (tiporeg==0)
            resultado=TablaRegistros[(op2&0x0F)];
    }
    if (top2==0)
       // if ((op2&0x00FF0000)==0)
          // resultado=TablaMemoria[TablaDeDatos[1].pos + (op2&0x00FFFF)]; //DS+offset
       // else
            resultado=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op2&0x000F0000)>>16)]+(op2&0x0000FFFF)];//valor registro +offset
    if (top2==1)
        resultado=op2;
    return resultado;
}// lo que hace la funcion es ya tomar directamente el valor de el operando 2 y dejarlo listo para oprerar (en caso de ser posible lo deja en los bytes menos significativos)

void MOV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    if (top1==0){
        //if (op1&0x00FF0000==0)
          //  TablaMemoria[TablaDeDatos[1].pos+op1&0x00FFFF]=valor;
      //  else
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=valor;
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0)
                TablaRegistros[(op1&0x0F)]=0;
            if (tiporeg==1){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
                valor=(valor&0x000000FF);
            }
            if (tiporeg==2){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
                valor=((valor&0x000000FF)<<8);
            }
            if (tiporeg==3){
                TablaRegistros[(op1&0x0F)]=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
                valor=(valor&0x000FFFF);
            }
            TablaRegistros[(op1&0x0F)]+=valor;
        }
}
void ADD(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
        //if ((op1&0x00FF0000)==0){
           // TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]+=valor;
          //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }
      //  else{
        TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]+=valor;
        nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]+valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)+(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)+((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)+(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void SUB(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
      //  if ((op1&0x00FF0000)==0){
         //   TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]-=valor;
         //   nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }else{
        TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]-=valor;
        nz= TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]-valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)-(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)-((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)-(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void SWAP(long int op1,long int op2,char top1,char top2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
long int aux;
    if (top1==top2){
        if (top1==0){
            aux=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op2&0x000F0000)>>16)]+(op2&0x0000FFFF)];
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op2&0x000F0000)>>16)]+(op2&0x0000FFFF)]=aux;
        }else{








        }




    }
}
void MUL(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
       // if ((op1&0x00FF0000)==0){
         //   TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]*=valor;
          //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }else{
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]*=valor;
            nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=TablaRegistros[(op1&0x0F)]*valor;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)*(valor&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)*((valor&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)*(valor&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);
}
void DIV(long int op1,char top1,long int valor,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if(valor!=0){
        if (top1==0){
            //if ((op1&0x00FF0000)==0){
             //   TablaRegistros[9]=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]%valor;
             //   TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]/=valor;
              //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]; //pos inicial DS mas offset de mem

           // }else{
                TablaRegistros[9]=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]%valor;
                TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]/=valor;
                nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]; //pos inicial DS mas offset mas direccion que se encutra en tabla de reg
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0){
                TablaRegistros[9]=TablaRegistros[(op1&0x0F)]%valor;
                aux=TablaRegistros[(op1&0x0F)]/valor;
                nz=aux;
            }
            if (tiporeg==1){
                TablaRegistros[9]=(TablaRegistros[(op1&0x0F)]&0x000000FF)%(valor&0x000000FF);
                aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)/(valor&0x000000FF);
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
            }
            if (tiporeg==2){
                TablaRegistros[9]=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8)%(valor&0x000000FF);
                aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8)/((valor&0x000000FF));
                aux=(aux<<8)&0x0000FF00;
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
            }
            if (tiporeg==3){
                TablaRegistros[9]=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)%(valor&0x000FFFF);
                aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)/(valor&0x000FFFF);
                nz=aux;
                aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
            }
            TablaRegistros[(op1&0x0F)]=aux;
        }
        Ultima_operacion(TablaRegistros,nz);
    }
}
void CMP(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int valor;
    if (top1==0){
      //  if ((op1&0x00FF0000)==0)
      //      valor=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
      //  else
        valor=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0)
            valor=TablaRegistros[(op1&0x0F)];
        if (tiporeg==1)
            valor=(TablaRegistros[(op1&0x0F)]&0x000000FF);
        if (tiporeg==2)
            valor=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
        if (tiporeg==3)
            valor=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
    }
    Ultima_operacion(TablaRegistros,valor-valor2);
}
void SHL(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux;
    if (top1==0){
      //  if ((op1&0x00FF0000)==0)
       //     TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]=((TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)])<<valor2);
     //   else
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=((TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]<<valor2));
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0)
               aux=TablaRegistros[(op1&0x0F)]<<valor2;
               TablaRegistros[(op1&0x0F)]=0;
            if (tiporeg==1){
                aux=(TablaRegistros[(op1&0x0F)]&0x000000FF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFFFF00;
                aux=(aux<<valor2)&0x000000FF;
            }
            if (tiporeg==2){
                aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF00FF;
                aux=((aux<<valor2)&0x000000FF)<<8;
            }
            if (tiporeg==3){
                aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF0000;
                aux=(aux<<valor2)&&0x0000FFFF;
            }
        TablaRegistros[(op1&0x0F)]+=aux;
        }
}
void SHR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux;
    if (top1==0){
      //  if ((op1&0x00FF0000)==0)
     //       TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]=((TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)])>>valor2);
      //  else
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=((TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]>>valor2));
        }else{
            tiporeg=op1>>4;
            if (tiporeg==0)
               aux=TablaRegistros[(op1&0x0F)]>>valor2;
               TablaRegistros[(op1&0x0F)]=0;
            if (tiporeg==1){
                aux=(TablaRegistros[(op1&0x0F)]&0x000000FF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFFFF00;
                aux=(aux>>valor2)&0x000000FF;
            }
            if (tiporeg==2){
                aux=((TablaRegistros[(op1&0x0F)]&0x0000FF00)>>8);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF00FF;
                aux=((aux>>valor2)&0x000000FF)<<8;
            }
            if (tiporeg==3){
                aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF);
                TablaRegistros[(op1&0x0F)]=TablaRegistros[(op1&0x0F)]&0xFFFF0000;
                aux=(aux>>valor2)&&0x0000FFFF;
            }
        TablaRegistros[(op1&0x0F)]+=aux;
        }
}
void AND(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
    char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
       // if ((op1&0x00FF0000)==0){
           // TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)])&(valor2);
          //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }
       // else{
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)])&(valor2);
            nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])&(valor2);
            TablaRegistros[(op1&0x0F)]=0;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)&(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)&((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)&(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}
void OR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
       // if ((op1&0x00FF0000)==0){
           // TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)])|(valor2);
          //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }
       // else{
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)])|(valor2);
            nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])|(valor2);
            TablaRegistros[(op1&0x0F)]=0;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)|(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)|((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)|(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}
void XOR(long int op1,char top1,long int valor2,char *TablaMemoria,long int TablaRegistros[],TDD TablaDeDatos[]){
char tiporeg;
    long int aux=0,nz=0;
    if (top1==0){
       // if ((op1&0x00FF0000)==0){
           // TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)])^(valor2);
          //  nz=TablaMemoria[TablaDeDatos[1].pos+(op1&0x00FFFF)];
       // }
       // else{
            TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)]=(TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)])^(valor2);
            nz=TablaMemoria[TablaDeDatos[1].pos+TablaRegistros[((op1&0x000F0000)>>16)]+(op1&0x0000FFFF)];
    }else{
        tiporeg=op1>>4;
        if (tiporeg==0){
            aux=(TablaRegistros[(op1&0x0F)])^(valor2);
            TablaRegistros[(op1&0x0F)]=0;
            nz=aux;
        }
        if (tiporeg==1){
            aux=(TablaRegistros[(op1&0x0F)]&0x000000FF)^(valor2&0x000000FF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFFFF00);
        }
        if (tiporeg==2){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FF00)^((valor2&0x000000FF)<<8);
            nz=aux>>8;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF00FF);
        }
        if (tiporeg==3){
            aux=(TablaRegistros[(op1&0x0F)]&0x0000FFFF)^(valor2&0x000FFFF);
            nz=aux;
            aux+=(TablaRegistros[(op1&0x0F)]&0xFFFF0000);
        }
        TablaRegistros[(op1&0x0F)]=aux;
    }
    Ultima_operacion(TablaRegistros,nz);//no se si opera asi, por si las dudas lo dejo en stand by.
}
void Ultima_operacion(long int TablaRegistros[],long int nz){
    //efectuo los cambios en el CC.
    TablaRegistros[8]=0;// seteo el cc en 0 por si las dudas
    if (nz==0)
        TablaRegistros[8]=0x40000000; //segundo bit mas significativo en 1 por ser una operacion que dio 0
    else
        TablaRegistros[8]=0x80000000;  //primer bit mas significativo en 1 por ser una operacion que dio <0
}

