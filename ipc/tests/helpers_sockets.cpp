#include "helpers_sockets.h"

#include <arpa/inet.h>
#include <common/noncopyable.h>
#include <ipc/endpoint_sockets.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ut/assert.h>

namespace micro_profiler
{
	namespace ipc
	{
		namespace tests
		{
			socket_h::socket_h()
				: _socket(static_cast<int>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
			{	}

			socket_h::~socket_h()
			{	::close(_socket);	}

			socket_h::operator int() const
			{	return _socket;	}


			sender::sender(unsigned short port)
			{
				sockaddr_in service = { AF_INET, htons(port), inet_addr("127.0.0.1"), { 0 } };

				::connect(_socket, (sockaddr *)&service, sizeof(service));
			}

			sender::~sender()
			{	}

			void sender::operator ()(const void *buffer, size_t size_)
			{
				sockets::byte_representation<unsigned int> size;

				size.value = static_cast<unsigned int>(size_);
				size.reorder();
				::send(_socket, size.bytes, sizeof(size.bytes), 0);
				::send(_socket, static_cast<const char *>(buffer), static_cast<unsigned int>(size_), 0);
			}


			bool is_local_port_open(unsigned short port)
			{
				socket_h s;
				sockaddr_in service = { AF_INET, htons(port), inet_addr("127.0.0.1"), { 0 } };

				return 0 == ::connect(s, (sockaddr *)&service, sizeof(service));
			}
		}
	}
}
