/*
 * rawParser.h
 *
 *  Created on: Oct 15, 2013
 *      Author: speters
 */

#ifndef RAWPARSER_H_
#define RAWPARSER_H_
#include <complex>
#include <string>
#include "error_code.hpp"
#include "Data.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
using namespace std;



class rawParser {
public:
	rawParser();
	rawParser(const char* fname); //Oeffnet den File
	virtual ~rawParser();

	error_code Read(); //Liest die Datei in den RAM
	Data m_matrix; //matrix mit gespeicherten werten
	Data m_sync; //sync-matrix mit gespeicherten werten

private:
	error_code CalcSize(); //kalkoliert die Grosse der Daten

	error_code ReadPFile();
	error_code ExtractProtocol(); //Extrahiert informationen aus dem Protocol
	error_code ParseProtocol (const bool verbose = true, const bool dry = false);

	error_code ReadMFile();

	error_code preParseMeas(const bool verbose = true);
	error_code preParseMDH (const bool verbose = true);

	error_code AllocateMatrices (const bool verbose = true);

	error_code ParseScan (Data matrix,const size_t& c);
	error_code ParseMDH (const bool verbose = true);
	error_code ParseMeas(const bool verbose = true);

	error_code CleanUp(const bool verbose = true);


	ifstream m_ifs; //File der Ausgelsenden Datei
	error_code m_status; //Fehler Code
	size_t m_size; //Groesse der Gesammten Datei
	size_t m_dataSize; //Groesse der Messdaten;
	size_t m_fod; //Grosse des vorderen Protocol
	bool m_acqend;//sind die messdaten zuende
	char* m_pdata; //Protokoll Daten
	char* m_mdata; //Messdaten Daten
	bool m_palloc;//protocoll allociert
	bool m_malloc;//messdaten allociert
	std::string m_xprot;
	sMDH m_cur_mdh;//derzeiteige smdh
	size_t m_meas_col_bytes;//groesse der messdaten nach der eingabe
	size_t m_pos;//pos des lese zeigers
	bool m_initialised;//ist das program initialisiert
	bool m_verb;// Wird beschildert was vorgeht
#ifdef USE_OMP1
	int NumOfThreas;
	vector<int> ThreadPos;
	int ThreadNum;
	int ThreadSize;
#endif
};
#endif /* RAWPARSER_H_ */
