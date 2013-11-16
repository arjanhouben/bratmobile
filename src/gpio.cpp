#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <sstream>

#include <errno.h>

volatile unsigned int* setup_io()
{
	void *gpio_map = 0;
	int  mem_fd = 0;
	const unsigned int BLOCK_SIZE = 4 * 1024;
	const unsigned int BCM2708_PERI_BASE = 0x20000000;
	const unsigned int GPIO_BASE = BCM2708_PERI_BASE + 0x200000;

	/* open /dev/mem */
	if ( ( mem_fd = open( "/dev/mem", O_RDWR | O_SYNC ) ) < 0 )
	{
		throw std::logic_error( "can't open /dev/mem" );
	}

	/* mmap GPIO */
	gpio_map = mmap(
		NULL,					// Any adddress in our space will do
		BLOCK_SIZE,				// Map length
		PROT_READ|PROT_WRITE,	// Enable reading & writting to mapped memory
		MAP_SHARED,				// Shared with other processes
		mem_fd,					// File to map
		GPIO_BASE				// Offset to GPIO peripheral
	);

	close( mem_fd ); //No need to keep mem_fd open after mmap

	if ( gpio_map == MAP_FAILED )
	{
		std::stringstream stream;
		stream << errno;
		throw std::logic_error( "mmap error: " + stream.str() );
	}

	// Always use volatile pointer!
	return (volatile unsigned int*)gpio_map;
}
