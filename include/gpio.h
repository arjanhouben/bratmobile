#pragma once

volatile unsigned int* setup_io();

namespace gpio
{
	template< int PIN >
	inline void input( volatile unsigned int *gpio )
	{
		gpio[ PIN / 10 ] &= ~( 7 << ( ( PIN % 10 ) * 3 ) );
	}

	inline void input( volatile unsigned int *gpio, unsigned int pin )
	{
		gpio[ pin / 10 ] &= ~( 7 << ( ( pin % 10 ) * 3 ) );
	}

	template< int PIN >
	inline void output( volatile unsigned int *gpio )
	{
		gpio[ PIN / 10 ] |= 7 << ( ( PIN % 10 ) * 3 );
	}

	inline void output( volatile unsigned int *gpio, unsigned int pin )
	{
		gpio[ pin / 10 ] |= 7 << ( ( pin % 10 ) * 3 );
	}

	template< int PIN >
	inline void on( volatile unsigned int *gpio )
	{
		gpio[ 7 ] = 1 << PIN;
	}

	inline void on( volatile unsigned int *gpio, unsigned int pin )
	{
		gpio[ 7 ] = 1 << pin;
	}

	template< int PIN >
	inline void off( volatile unsigned int *gpio )
	{
		gpio[ 10 ] = 1 << PIN;
	}

	inline void off( volatile unsigned int *gpio, unsigned int pin )
	{
		gpio[ 10 ] = 1 << pin;
	}

	template< int PIN >
	inline void read( volatile unsigned int *gpio )
	{
		gpio[ 13 ] &= 1 << PIN;
	}

	inline void read( volatile unsigned int *gpio, unsigned int pin )
	{
		gpio[ 13 ] &= 1 << pin;
	}
}

template< int PIN >
struct Pin
{
		Pin( volatile unsigned int *gpio ) :
			gpio_( gpio )
		{
			gpio::input< PIN >( gpio_ );
		}

	protected:

		volatile unsigned int * const gpio_;
};

template< int PIN >
struct InputPin : Pin< PIN >
{
		InputPin( volatile unsigned int *gpio ) :
			Pin< PIN >( gpio ) { }

		operator bool()
		{
			gpio::read< PIN >( Pin< PIN >::gpio_ );
		}
};

template< int PIN >
struct OutputPin : Pin< PIN >
{
		OutputPin( volatile unsigned int *gpio ) :
			Pin< PIN >( gpio )
		{
			gpio::output< PIN >( Pin< PIN >::gpio_ );
		}

		void on()
		{
			gpio::on< PIN >( Pin< PIN >::gpio_ );
		}

		void off()
		{
			gpio::off< PIN >( Pin< PIN >::gpio_ );
		}
};

class RaspberryPi
{
	public:
		RaspberryPi() :
			gpio_( setup_io() ) { }

		template< int PIN >
		InputPin< PIN > input()
		{
			return InputPin< PIN >( gpio_ );
		}

		template< int PIN >
		OutputPin< PIN > output()
		{
			return OutputPin< PIN >( gpio_ );
		}

		template< int PIN >
		OutputPin< PIN > output( bool state )
		{
			OutputPin< PIN > out( gpio_ );
			if ( state )
			{
				out.on();
			}
			else
			{
				out.off();
			}
			return out;
		}

		void output( unsigned int pin, bool state )
		{
			gpio::output( gpio_, pin );
			if ( state )
			{
				gpio::on( gpio_, pin );
			}
			else
			{
				gpio::off( gpio_, pin );
			}
		}

	private:

		volatile unsigned int * const gpio_;
};
