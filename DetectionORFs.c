#include <stdio.h>
#include <stdlib.h>
#include "DetectionORFs.h"




tyListeORFs *findORF(tySeqADN *pS){

	/* Creation struc tyListeORFs a remplir */
	/* Il faut initiliser le premier ORF du 1 au premier stop en phase 1 */
	tyListeORFs *pLORFs=NULL;

	/* Creation des 3 listes d'indices des stop (3 phases) ==================== */
	/* liste d'indices de stop impossible a depasser theoriquement */
	int *lStop1 = NULL, *lStop2 = NULL, *lStop3 = NULL;
	int taille = pS->lg;
	lStop1 = malloc(taille  * sizeof(int));
	lStop2 = malloc(taille  * sizeof(int));
	lStop3 = malloc(taille  * sizeof(int));
	/* Vérifiation de l'allocation */
	if (lStop1 == NULL || lStop2 == NULL || lStop3 == NULL){
		exit(1);
	}
	/* =========================================================================*/

	int nbrStop1=0, nbrStop2=0, nbrStop3=0;
	int i;
	char *codonPh1, *codonPh2, *codonPh3;

	/* Parcours de la seq; Recuperation des indices des stop */

	for (i=0; i<taille-5; i+=3){

		codonPh1 = &pS->seq[i];
		codonPh2 = &pS->seq[i+1];
		codonPh3 = &pS->seq[i+2];

		if (estStop(codonPh1) ==1){ // Si le codon est un codon stop
			nbrStop1 +=1; //nbr codon stop phase 1 ++
			lStop1[nbrStop1-1] = i;
		}
		if (estStop(codonPh2) ==1){ // Si le codon est un codon stop
			nbrStop2 +=1; //nbr codon stop phase 1 ++
			lStop2[nbrStop2-1] = i+1;
		}
		if (estStop(codonPh3) ==1){ // Si le codon est un codon stop
			nbrStop3 +=1; //nbr codon stop phase 1 ++
			lStop3[nbrStop3-1] = i+2;
		}

	}


	/* Parcours des listes : début = stop+3 ; stop = stop
	 * on pose qu'il faut au min 1 codon dans l'ORF : ORF de 2 codons
	 */

	int c1;


	int debut1, stop1;
	int debut2, stop2;
	int debut3, stop3;


	for (c1=0; c1<taille; c1++){
		debut1 = lStop1[c1]+3;
		stop1 = lStop1[c1+1];

		debut2 = lStop2[c1]+3;
		stop2 = lStop2[c1+1];

		debut3 = lStop3[c1]+3;
		stop3 = lStop3[c1+1];



		if ( debut1 != 0 &&\
			 stop1 != 0  &&\
			 stop1-debut1>=3 ){
			//printf("Phase 1 Debut : %d ;stop : %d\n", debut1, stop1);
			pLORFs = ajouterORF(pLORFs, debut1, stop1+4, -1, pS);
		}
		if ( debut2 != 0 &&\
			 stop2 != 0  &&\
			 stop2-debut2>=3 ){
			//printf("Phase 2 Debut : %d ;stop : %d\n", debut2, stop2);
			pLORFs = ajouterORF(pLORFs, debut2, stop2+4, -1, pS);
		}
		if ( debut3 != 0 &&\
			 stop3 != 0  &&\
			 stop3-debut3>=3 ){
			//printf("Phase 3 Debut : %d ;stop : %d\n", debut3, stop3);
			pLORFs = ajouterORF(pLORFs, debut3, stop3+4, -1, pS);
		}

	}

	free(lStop1);
	free(lStop2);
	free(lStop3);



	return pLORFs;

}


void TrouveLesPremiersStarts(tyListeORFs *lesORF){

	tyListeORFs *ORF;

	ORF = lesORF;
	int i,taille, debut, stop;
	char *pCodon=NULL;
	char *seq=NULL;
	seq = ORF->pORF->pSeq->seq;

	while (ORF->pSuiv != NULL){
		debut = ORF->pORF->debut;
		stop = ORF->pORF->stop;
		taille = stop-debut;

		for(i=debut; i< stop; i+=3){
			pCodon = &seq[i];
			if (estStart(pCodon) ==1){
				ORF->pORF->start = i;
				break;
				
			}
		}


		ORF = ORF->pSuiv;

	}
}
