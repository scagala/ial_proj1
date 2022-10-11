/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	
	list->firstElement = NULL;
	list->activeElement = NULL;

	while(list->lastElement != NULL) {
		DLLElementPtr last = list->lastElement;
		list->lastElement = list->lastElement->previousElement;
		free(last);
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

	DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
	if(newElem == NULL) {
		DLL_Error();
		return;
	}
	
	newElem->data = data;									// set the value of newElem
	newElem->nextElement = list->firstElement;				// nextElem will be the original first element
	newElem->previousElement = NULL;						// previousElem of the new head will be empty

	if(list->firstElement != NULL) { 						
		// list is not empty
		list->firstElement->previousElement = newElem;		// list isn't empty, so the prevElem pointer of the original first element will point to the new element
		list->firstElement = newElem;						// newElem becomes the new first element of the list
	}
	else {
		list->firstElement = newElem;
		list->lastElement = newElem;						// list was empty, so newElem becomes both the first and last element of the list
	}

}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	
	DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
	if(newElem == NULL) {
		DLL_Error();
		return;
	}

	newElem->data = data;									// set value of newElem
	newElem->nextElement = NULL;							// newElem will be the last element of the list, thus nextElem pointer will be empty
	newElem->previousElement = list->lastElement;			// Original last elem of the list will come before the new last element

	if(list->firstElement != NULL) {
		// list is not empty
		list->lastElement->nextElement = newElem;			// list is not empty, so nextElem ptr of the original last element will point to the new last elem
		list->lastElement = newElem;						// newElem becomes the new last element of the list
	}
	else {
		list->firstElement = newElem;
		list->lastElement = newElem; 						// list was empty, so newElem becomes both the first and last element
	}
	
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	
	if(list->firstElement == NULL) {
		//list is empty
		DLL_Error();
		return;
	}
	// list is not empty
	// set dataPtr to the value of firstElement
	*dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

	if(list->firstElement == NULL) {
		// list is empty
		DLL_Error();
		return;
	}
	// list is not empty
	// set dataPtr to the value of lastElement
	*dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	
	if(list->firstElement != NULL) { 									// list cannot be empty 

		if(list->firstElement == list->activeElement) {					// firstElement was active
			list->activeElement = NULL;									// cancel the active elem
		}

		DLLElementPtr first = list->firstElement;

		if(list->firstElement == list->lastElement) {					// there is only one element in the list
			list->firstElement = NULL;
			list->lastElement = NULL;									// we removed the only elem in the list, list is now empty
		}
		else {
			list->firstElement->nextElement->previousElement = NULL;	// remove the pointer to the previous element of the current second elem, since it will become the new first element
			list->firstElement = list->firstElement->nextElement;		// original second element becomes the first element; eliminating the original first
		}

		free(first);													// free the original first elem from memory
	}

}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	
	if(list->firstElement != NULL) {									// list cannot be empty

		if(list->lastElement == list->activeElement) {
			list->activeElement = NULL;									// cancel the active element if lastElem was active
		}

		DLLElementPtr last = list->lastElement;

		if(list->firstElement == list->lastElement) {					// there is only one element in the list
			list->firstElement = NULL;
			list->lastElement = NULL;									// we removed the only elem in the list, list is now empty
		}
		else {
			list->lastElement->previousElement->nextElement = NULL;		// remove the pointer to next elem of the second-to-last element, since it will become the new last element
			list->lastElement = list->lastElement->previousElement;		// original second-to-last element becomes the new last element; eliminating the previous last
		}
		free(last);														// free the original last elem from memory
	}


}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	
	if(list->activeElement != NULL && list->lastElement != list->activeElement) {					// continue only if the list is active and the active element is not the last elem of the list

		DLLElementPtr tmp_elem = list->activeElement->nextElement;
																									 
		if(tmp_elem == list->lastElement) {															// the next elem is the last one
			list->activeElement->nextElement = NULL;												// active elem becomes last, thus we can remove the next element
			list->lastElement = list->activeElement;
		}
		else {																						// next elem is not last in the list
			list->activeElement->nextElement = tmp_elem->nextElement;								// the active element will point to the element after the one we want to delete
			tmp_elem->nextElement->previousElement = list->activeElement;							// the elem before the element we want to delete will point to the current active element
		}
		free(tmp_elem);

	}

}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	
	if(list->activeElement != NULL && list->firstElement != list->activeElement) {					// continue only if the list is active and the active element is not the first elem of the list

		DLLElementPtr tmp_elem = list->activeElement->previousElement;

		if(tmp_elem == list->firstElement) {														// the elem we want to delete is the first elem of the list
			list->activeElement->previousElement = NULL;											// active elem first last, thus we can remove the previous element
			list->firstElement = list->activeElement;
		}
		else {																						// previous elem is not first in the list
			list->activeElement->previousElement = tmp_elem->previousElement;						// the active element will point to the element before the one we want to delete
			tmp_elem->previousElement->nextElement = list->activeElement;							// the elem after the element we want to delete will point to the current active element
		}
		free(tmp_elem);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	
	if(list->activeElement != NULL) {											//list has to be active

		DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
		if (newElem == NULL) {
			DLL_Error();
			return;
		}
		
		newElem->data = data;													// assign value to new element
		newElem->previousElement = list->activeElement;							// newElem will come after the active element, so nE.previous will point to the current active element

		if(list->activeElement == list->lastElement) {							// if the last elem of the list is the last one, newElem will become the new last element
			newElem->nextElement = NULL;
			list->lastElement = newElem;
		}
		else {
			list->activeElement->nextElement->previousElement = newElem;		// 
			newElem->nextElement = list->activeElement->nextElement;			// 
		}
		list->activeElement->nextElement = newElem;								// active elem will point the newElem as the next element


	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	
	if(list->activeElement != NULL) {
		
		DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
		if (newElem == NULL) {
			DLL_Error();
			return;
		}

		newElem->data = data;
		newElem->nextElement = list->activeElement;

		if(list->activeElement == list->firstElement) {
			newElem->previousElement = NULL;
			list->firstElement = newElem;
		}
		else {
			list->activeElement->previousElement->nextElement = newElem;
			newElem->previousElement = list->activeElement->previousElement;
		}
		list->activeElement->previousElement = newElem;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	
	if(list->activeElement == NULL) {					// check whether list is active
		DLL_Error();
		return;
	}
	*dataPtr = list->activeElement->data;				// set dataPtr to the value of the active element

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	
	if(list->activeElement != NULL) {							// do nothing if list is inactive
		list->activeElement->data = data;						// set the active elem's value to 'data'
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	
	if(list->activeElement != NULL) {								// Do nothing if list is inactive
		// if(list->activeElement == list->lastElement) {
		// 	list->activeElement = NULL;								// if last elem was the active one, list becomes inactive
		// }
		list->activeElement = list->activeElement->nextElement;		// next element becomes active
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

	if(list->activeElement != NULL) {								//DO nothing if list is inactive
		// if(list->activeElement == list->firstElement) {				
		// 	list->activeElement = NULL;								// if first elem was the active one, list becomes inactive
		// }
		list->activeElement = list->activeElement->previousElement;	// previous element becomes active
	}



}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return(list->activeElement != NULL);
}

/* Konec c206.c */