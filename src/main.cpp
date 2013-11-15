#include "wiiuse.h"
#include <iostream>

using namespace std;

int main( int argc, char *argv[] )
{
	wiimote_t **wiimote = wiiuse_init( 1 );

	auto found = wiiuse_find( wiimote, 1, 5 );

	cout << found << endl;

	return 0;
}
