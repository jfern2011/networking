#include "UdpConnection.h"
#include "abort.h"

#include <iostream>
#include <string>

bool run()
{
	net::UdpConnection conn1, conn2;
	AbortIfNot(conn1.bind(12345, "localhost"), false);
	AbortIfNot(conn2.connect(12345, "localhost"), false);

	std::string msg;
	char remote_input[1024];

	while (true)
	{
		std::getline(std::cin, msg);

		if (msg == "quit") break;

		int sent = conn2.send(msg.c_str(), msg.size());
		AbortIf(sent < 0, false);

		std::printf("sent     '%s' (%d bytes)\n",
			msg.c_str(), sent);
		

		int recvd = conn1.recv(remote_input, 1024, 2000);
		AbortIf(recvd < 0, false);

		if (recvd == 0)
			std::printf("nothing received.\n");
		else
		{
			remote_input[recvd] = '\0';
			std::printf("received '%s' (%d bytes)\n",
				remote_input, recvd);
		}

		std::fflush(stdout);
	}

	return true;
}

int main()
{
	run();
	return 0;
}
