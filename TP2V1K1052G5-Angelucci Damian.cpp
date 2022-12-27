#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

typedef struct sNodoVta *tpListaVta; //Puntero lista venta
typedef char str20[21];
typedef unsigned short ushort;

struct sFec {
	ushort 	dd,	
					mm,
					aaaa;
}; 

struct sVen {
	ushort 	codVen,
					cant;
	str20  	descr;
	float  	preUni;
	sFec   	FecVta;
};

struct tInfoVta {
	ushort 	cont,
					totCan;
	float		totVen;
	sVen 		ventas[10];
};

struct sNodoVta{
	tInfoVta 		rInfo; 
	tpListaVta 	Sgte; //Referencia a sNodo campo Sgte
};

bool Abrir(FILE **Vtas){
	*Vtas = fopen("VentasFerreteria.Dat","rb");
}//Abre el archivo Dat

tpListaVta iif(bool exprLog, tpListaVta ptr1, tpListaVta ptr2) {
  if (exprLog)
    return ptr1;
  else
    return ptr2;	
} // iif

tpListaVta ExisteNodo (tpListaVta &Lista,tpListaVta &pAnt, short cVen) { 
  tpListaVta	pAct = Lista;

  pAnt = NULL;
  while (pAct && cVen > pAct->rInfo.ventas[0].codVen) {
    pAnt = pAct;
    pAct = pAct->Sgte;
  }
  return iif(pAct && cVen == pAct->rInfo.ventas[0].codVen, pAct, NULL);
} // ExisteNodo

void InsertaInicio(tpListaVta &Lista, tInfoVta rVentas){ 
	tpListaVta pNodo; 
	pNodo = new sNodoVta; 
	pNodo ->rInfo = rVentas; 
	pNodo ->Sgte = Lista; 
	Lista = pNodo; 
}; //Inserta Nodo al Inicio

void InsertaEnMedio (tpListaVta &pAnt, tInfoVta valor){ 
  tpListaVta pNodo = new sNodoVta;

  pNodo->rInfo = valor;
  pNodo->Sgte = pAnt->Sgte; 
  pAnt->Sgte = pNodo;
} //InsertaEnMedio

void SacarPrimerNodo (tpListaVta &List, tInfoVta &valor){
	tpListaVta pElim = List;
	
	valor = List->rInfo;
	List 	=	List->Sgte;
	delete pElim;
}

void CrearNodo(tpListaVta &Lista, tpListaVta &pAnt, tInfoVta valor) { 
  if (!pAnt)
    InsertaInicio(Lista,valor);
  else
    InsertaEnMedio(pAnt,valor);
} // CrearNodo

void IntCmb(sVen &rVta1, sVen &rVta2) {
	
	sVen 	auxVta = rVta1;
	rVta1 = rVta2;
	rVta2 = auxVta;
}

void OrdxBur(sVen ventas[], ushort card){

	bool 		ordenado;
	ushort 	li=0;
	
	do {
		li++;
		ordenado = true;
			
		for(int i=0; i<=card-li; i++) {
			if(ventas[i].FecVta.aaaa < ventas[i+1].FecVta.aaaa){
					IntCmb (ventas[i], ventas[i+1]);
					ordenado=false;
			}
			if (ventas[i].FecVta.aaaa == ventas[i+1].FecVta.aaaa && ventas[i].FecVta.mm < ventas[i+1].FecVta.mm){
					IntCmb (ventas[i], ventas[i+1]);
  				ordenado=false;
			}
			if (ventas[i].FecVta.aaaa == ventas[i+1].FecVta.aaaa && ventas[i].FecVta.mm == ventas[i+1].FecVta.mm 
			&& ventas[i].FecVta.dd < ventas[i+1].FecVta.dd){
					IntCmb (ventas[i], ventas[i+1]);
					ordenado=false;
			}
		}
	}		
	while(!ordenado);
} //OrdxBur cantidad total

void EnlazaInicioCant(tpListaVta &ListAux, tpListaVta &pNodo) {
    pNodo->Sgte = ListAux;
    ListAux = pNodo;
} // EnlazaInicioCant

void EnlazaEnMedioCant(tpListaVta &ListAux, tpListaVta &pNodo) {
  tpListaVta pAct = ListAux;
  while (pAct->Sgte != NULL && 	pNodo->rInfo.totCan < pAct->Sgte->rInfo.totCan)
    pAct = pAct->Sgte;
  pNodo->Sgte = pAct->Sgte;
  pAct->Sgte = pNodo;
}  // EnlazaEnMedioCant

void ReInsertaNodoCant(tpListaVta &ListAux, tpListaVta &pNodo)  {
  tpListaVta pAct;

  if (!ListAux || pNodo->rInfo.totCan > ListAux->rInfo.totCan)
    EnlazaInicioCant(ListAux, pNodo);
  else
    EnlazaEnMedioCant(ListAux, pNodo);
} // ReInsertaNodoCant

void ReOrdenarLstCant(tpListaVta &List) {
	
	tpListaVta 	LstAux = NULL,
			 				pNodo;

	while (List) {
		pNodo 	= List;
	  List = List->Sgte;
	  ReInsertaNodoCant(LstAux, pNodo);
	}
	 List = LstAux;
} // ReOrdenarLstCant

void EnlazaInicioImp(tpListaVta &ListAux, tpListaVta &pNodo) {
    pNodo->Sgte = ListAux;
    ListAux = pNodo;
} // EnlazaInicioImp

void EnlazaEnMedioImp(tpListaVta &ListAux, tpListaVta &pNodo) {
  tpListaVta pAct = ListAux;
  while (pAct->Sgte != NULL && 	pNodo->rInfo.totVen < pAct->Sgte->rInfo.totVen)
    pAct = pAct->Sgte;
  pNodo->Sgte = pAct->Sgte;
  pAct->Sgte = pNodo;
}  // EnlazaEnMedioImp

void ReInsertaNodoImp(tpListaVta &ListAux, tpListaVta &pNodo)  {
  tpListaVta	pAct;

  if (!ListAux || pNodo->rInfo.totVen > ListAux->rInfo.totVen)
    EnlazaInicioImp(ListAux, pNodo);
  else
    EnlazaEnMedioImp(ListAux, pNodo);
} // ReInsertaNodoImp

void ReOrdenarLstImp(tpListaVta &List) {
	
	tpListaVta 	LstAux = NULL,
			 				pNodo;

	while (List) {
		pNodo 	= List;
	  List = List->Sgte;
	  ReInsertaNodoImp(LstAux, pNodo);
	}
	 List = LstAux;
} // ReOrdenarLstImp

void ProcVentas(FILE *Vtas, tpListaVta &List) { 

	sVen 				rVentas;
	tInfoVta		rInfo;
	tpListaVta	pVta,
							pNodoAnt;
	char				car;
	
	while (fread(&rVentas, sizeof rVentas, 1, Vtas)) {
		pVta = ExisteNodo(List, pNodoAnt, rVentas.codVen);
		if(!pVta) { //pVta=NULL
			rInfo.cont = 0; //Tamaño arreglo
			rInfo.ventas[rInfo.cont] = rVentas;
			CrearNodo(List, pNodoAnt, rInfo);
		}
		else { //pVta apunta a un nodo
			pVta->rInfo.cont +=1;
			pVta->rInfo.ventas[pVta->rInfo.cont] 	= rVentas;
		} 
	}
}

void ListadoVtasAgrupVen(FILE *Vtas, tpListaVta &List)  {
	
	ofstream 			SldAF("Lista2x3AF.Txt");	
	tpListaVta		pAux;
								pAux = List;
	ushort				item = 0,
								auxCVen,
								codVenMyrVta;
	float					totVtas,
								totAux=0,
								totGral=0;
	
	SldAF.setf(ios::fixed);
	SldAF.precision(2);

	SldAF << "Listado (1) ordenado por Cod.Ven. y Fecha" << endl;
	while(pAux){
		totVtas=0;
				
		OrdxBur(pAux->rInfo.ventas, pAux->rInfo.cont);
		SldAF << endl << "Cod. Vendedor: "<< setw(3) << pAux->rInfo.ventas[0].codVen << endl;
		SldAF <<"#Item     Fecha     Cant.       Descripción          Pre.Unit.     Tot.Item " << endl;		
		for(ushort i=0; i <= pAux->rInfo.cont; i++){
			item = i+1; 
			SldAF << setw(3)  << item;
  		SldAF << setw(6)  << pAux->rInfo.ventas[i].FecVta.dd << "-" 
						<< setw(2)  << pAux->rInfo.ventas[i].FecVta.mm << "-"
						<< setw(4)  << pAux->rInfo.ventas[i].FecVta.aaaa;
			SldAF << setw(6)  << pAux->rInfo.ventas[i].cant;
			SldAF << setw(25) << pAux->rInfo.ventas[i].descr; 
			SldAF << setw(6)  <<"$" 
						<< setw(8)  << pAux->rInfo.ventas[i].preUni;		
			SldAF << setw(6)  <<"$"
						<< setw(8)  << pAux->rInfo.ventas[i].cant * pAux->rInfo.ventas[i].preUni << endl;		
		
			totVtas += pAux->rInfo.ventas[i].cant * pAux->rInfo.ventas[i].preUni; 			
			totGral += pAux->rInfo.ventas[i].cant * pAux->rInfo.ventas[i].preUni; 			
			pAux->rInfo.totCan += pAux->rInfo.ventas[i].cant;	
			
			if (totVtas > totAux) {
					totAux = totVtas;
					codVenMyrVta = pAux->rInfo.ventas[i].codVen;
			}
		}
	
		SldAF << endl << "Total Vendedor "
					<< setw(3)  << pAux->rInfo.ventas[0].codVen << ": "
					<< setw(48) << "$" 	
					<< setw(8)  << totVtas << endl;
		SldAF <<"-----------------------------------------------------------------------------------"<< endl << endl; 
		
		pAux->rInfo.totVen += totVtas;				
		pAux = pAux->Sgte;					
	}
	SldAF << "Total General:" << setw(53) << "$" 
														<< setw(9) 	<< totGral << endl;
	SldAF << "Cod.Vend.mayor venta de importe: "<< codVenMyrVta << endl << endl;
	SldAF <<"-----------------------------------------------------------------------------------"<< endl << endl; 
	
	SldAF.close();
}

void ListadoCantTot(tpListaVta &List) { 	

	ReOrdenarLstCant(List);
	ofstream 		SldAF("Lista2x3AF.Txt", ios::app);	
	tpListaVta	pAux;
							pAux = List;
							
		SldAF <<"Listado (2) ordenado decrecientemente por Cantidad Total de cada Código de Vendedor" <<endl;					
	SldAF <<"          Cód. Ven.          Cant.Tot."<<endl;
	while(pAux) {
		SldAF << setw(16) << pAux->rInfo.ventas[0].codVen << setw(19) << pAux->rInfo.totCan << endl;
		pAux = pAux->Sgte;	
	}	
	SldAF << "-----------------------------------------------------------------------------------" << endl << endl; 

	SldAF.close();
} //Emite ListadoCantTot

void ListadoImpTot(tpListaVta &List) { 

	ReOrdenarLstImp(List);
	ofstream 		SldAF("Lista2x3AF.Txt", ios::app);	
	tpListaVta	pAux;
							pAux = List;
	
	SldAF.setf(ios::fixed);
	SldAF.precision(2);		
	
	SldAF <<"Listado (3) ordenado decrecientemente por Importe Total de cada Código de Vendedor" <<endl;					
	SldAF <<"          Cód. Ven.          Importe Total"<<endl;

	while(pAux) {
		SldAF << setw(16) << pAux->rInfo.ventas[0].codVen 
					<< setw(16) << "$" 
					<< setw(8) << pAux->rInfo.totVen << endl;
		pAux = pAux->Sgte;	
	}
	SacarPrimerNodo(List, List->rInfo);
	SldAF << "-----------------------------------------------------------------------------------" << endl << endl; 

	SldAF.close();
} //Emite ListadoImpTot

int main() {
	FILE 				*VentasAF;
	tpListaVta 	ListaVend=NULL; //Puntero externo tipo tpListaVta

	Abrir(&VentasAF);	
	ProcVentas(VentasAF, ListaVend);	
	ListadoVtasAgrupVen(VentasAF, ListaVend); 
	ListadoCantTot(ListaVend); 
	ListadoImpTot(ListaVend); 

	fclose(VentasAF);

	return 0;
}
	
