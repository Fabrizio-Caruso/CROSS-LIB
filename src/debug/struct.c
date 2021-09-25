#include <stdio.h>
 
struct Test {
 
	int a;
	int b;
};
 
const struct Test globale = { 21, 42 };
 
const struct Test const * funzione() {
	return &globale;
}
 
void main(void) {
	const struct Test const* result = funzione();
	printf( "risultato = %d, %d", result->a, result->b );
}

