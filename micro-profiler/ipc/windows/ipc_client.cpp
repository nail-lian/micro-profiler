#include "../client.h"

#include "common.h"

#include <windows.h>

using namespace std;

namespace micro_profiler
{
	namespace ipc
	{
		class client::impl : OVERLAPPED
		{
		public:
			impl(const char *server_name)
				: _server_name(c_pipe_ns + c_prefix + server_name), _completion(::CreateEvent(NULL, TRUE, FALSE, NULL), &::CloseHandle)
			{
				HANDLE pipe = INVALID_HANDLE_VALUE;

				do
				{
					if (::WaitNamedPipeA(_server_name.c_str(), NMPWAIT_WAIT_FOREVER))
						pipe = ::CreateFileA(_server_name.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
				} while (pipe == INVALID_HANDLE_VALUE);

				_pipe.reset(pipe, &::CloseHandle);
			}

			void call(const vector<byte> &input, vector<byte> &output)
			{
				*static_cast<OVERLAPPED*>(this) = zero_init();
				hEvent = _completion.get();
				::ResetEvent(_completion.get());
				::WriteFile(_pipe.get(), input.empty() ? NULL : &input[0], static_cast<DWORD>(input.size()), NULL, this);
				while (WAIT_IO_COMPLETION == ::WaitForSingleObjectEx(_completion.get(), INFINITE, TRUE))
				{	}

				DWORD dummy, message_length;

				::ReadFile(_pipe.get(), NULL, 0, &dummy, NULL);
				::PeekNamedPipe(_pipe.get(), NULL, 0, &dummy, &dummy, &message_length);

				output.resize(message_length);
				if (message_length)
					::ReadFile(_pipe.get(), &output[0], message_length, &dummy, NULL);
			}

		private:

		private:
			string _server_name;
			shared_ptr<void> _pipe, _completion;
		};

		client::client(const char *server_name)
			: _impl(new impl(server_name))
		{	}

		client::~client()
		{	}

		void client::call(const vector<byte> &input, vector<byte> &output)
		{	_impl->call(input, output);	}

		void client::enumerate_servers(vector<string> &servers)
		{
			BOOL more = TRUE;
			WIN32_FIND_DATAA fd = { 0 };
			
			servers.clear();
	
			for (const shared_ptr<void> h(::FindFirstFileA((c_pipe_ns + "*").c_str(), &fd), ::FindClose);
				h.get() != INVALID_HANDLE_VALUE && more;
				more = ::FindNextFileA(h.get(), &fd))
			{
				string name(fd.cFileName);

				if (0 == name.find(c_prefix))
					servers.push_back(name.substr(c_prefix.size()));
			}
		}
	}
}