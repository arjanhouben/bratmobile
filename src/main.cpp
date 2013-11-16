#include "wiiuse.h"
#include <iostream>
#include <thread>

#define _USE_MATH_DEFINES
#include <cmath>

#if _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;

bool volatile run = true;

template < class T >
T clamp( T value, T mi, T ma )
{
	return min( max( mi, value ), ma );
}

double timer()
{
#ifdef _WIN32
	LARGE_INTEGER frequency, count;
	QueryPerformanceFrequency( &frequency );
	QueryPerformanceCounter( &count );
	return static_cast< double >( count.QuadPart ) / static_cast< double >( frequency.QuadPart );
#else
	timeval tv;
	gettimeofday( &tv, 0 );
	return static_cast< double >( tv.tv_sec ) + ( static_cast< double >( tv.tv_usec ) / 1e6 );
#endif
}

const double timeout = 1;

void controllerLoop( wiimote_t *wiimote );

void connected( wiimote_t *wiimote )
{
	wiiuse_set_leds( wiimote, WIIMOTE_LED_1 );
	wiiuse_rumble( wiimote, true );
	usleep( 100000 );
	wiiuse_rumble( wiimote, false );

	// motion sensing
	wiiuse_motion_sensing( wiimote, true );

	controllerLoop( wiimote );

	wiiuse_disconnect( wiimote );
}

template < class Device, class Button >
bool pressed( Device dev, Button button )
{
	return ( ( dev.btns & button ) == button );
}

template < class Device, class Button >
bool pressed( Device *dev, Button button ) { return pressed( *dev, button ); }

template < class Device, class State >
bool is_set( Device dev, State state )
{
	return ( ( dev.state & state ) == state );
}

template < class Device, class State >
bool is_set( Device *dev, State state ) { return is_set( *dev, state ); }

class InputValues
{
	public:

		InputValues() :
			x_( 0 ),
			y_( 0 ),
			last_( 0 ) {}

		void set( double x, double y )
		{
			x_ = clamp( x, -1.0, 1.0 );
			y_ = clamp( y, -1.0, 1.0 );
			last_ = timer();
		}

		double x()
		{
			checkTimeout();
			return x_;
		}

		double y()
		{
			checkTimeout();
			return y_;
		}

		void checkTimeout()
		{
			if ( last_ && timer() - last_ > timeout )
			{
				x_ = 0;
				y_ = 0;
				last_ = 0;
			}
		}

	private:

		double x_, y_;
		double last_;

} values;

void controllerLoop( wiimote_t *wiimote )
{
	while ( run && is_set( wiimote, WIIMOTE_STATE_CONNECTED ) )
	{
		if ( wiiuse_poll( &wiimote, 1 ) )
		{
			float throttle = wiimote->exp.classic.ljs.y;
			if ( throttle >= 0 ) throttle = std::max( throttle, wiimote->exp.classic.r_shoulder );
			values.set( wiimote->exp.classic.rjs.x, throttle );
		}
		else
		{
			usleep( 1000 );
		}
	}

	values.set( 0, 0 );
}

void printThread()
{
	while ( run )
	{
		static float x = values.x(),
				y = values.y();

		if ( x != values.x() || y != values.y() )
		{
			x = values.x();
			y = values.y();
			cout << x << '\t' << y << endl;
		}

		usleep( 10000 );
	}
}

void signal_handler( int )
{
	run = false;
}

int main( int, char *[] )
{
	for ( int i = 1; i <= SIGUSR2; ++i ) signal( i, signal_handler );

	wiimote_t **wiimotes = wiiuse_init( 1 );

	thread pt( printThread );

	while ( run )
	{
		if ( wiiuse_find( wiimotes, 1, 2 ) )
		{
			if ( wiiuse_connect( wiimotes, 1 ) == 1 )
			{
				connected( *wiimotes );
			}
		}
	}

	wiiuse_cleanup( wiimotes, 1 );

	return 0;
}
