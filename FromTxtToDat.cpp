#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

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
	str20		descr;
	float 	preUni;
	sFec 		FechaVta;
};

bool LeeUnaVta(ifstream &VentasTXT, sVen &rVen) {
	
	VentasTXT >> rVen.codVen;
	VentasTXT >> rVen.cant;
	VentasTXT.get(rVen.descr,21);
	VentasTXT.ignore();
	VentasTXT >> rVen.preUni;
	VentasTXT >> rVen.FechaVta.dd;
	VentasTXT >> rVen.FechaVta.mm;
	VentasTXT >> rVen.FechaVta.aaaa;
	VentasTXT.ignore();

  return VentasTXT.good();
} // LeeUnaVta


int main(){
	
	ifstream	VentasTXT("VentasFerreteria.txt", ios::in);
	ofstream	VentasDAT("VentasFerreteria.dat", ios::binary);
	sVen 			rVentas; // =rVendededores
	
	while(LeeUnaVta(VentasTXT, rVentas)) 
		VentasDAT.write((char *)&rVentas, sizeof (rVentas));
	VentasDAT.write((char *)&rVentas, sizeof (rVentas));

	VentasTXT.close();
	VentasDAT.close();

	return 0;
}
