#include "gpio.h"
#include <iostream>
#include <unistd.h>

using namespace gpio;
using namespace std;

int main( int, char *[] )
{
	try
	{
		RaspberryPi pi;

		auto pin = pi.output< 2 >();

		while( 1 )
		{
			pin.on();
			sleep( 1 );
			pin.off();
			sleep( 1 );
		}
	}
	catch ( const exception &err )
	{
		cerr << err.what() << endl;

		return -1;
	}

	return 0;
}
