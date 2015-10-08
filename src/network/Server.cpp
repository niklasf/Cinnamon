/*
    Cinnamon UCI chess engine
    Copyright (C) Giuseppe Cannella

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Server.h"
#include "Client.h"

void Server::run() {

    int read_size;
    c = sizeof(struct sockaddr_in);

    char client_message[MAX_MSG_SIZE];
    debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "accept...", client_message);
    while (1) {
        client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);

        assert(client_sock != -98691);
        assert(client_sock);
        debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "socket create: ", client_sock);

        while ((read_size = recv(client_sock, client_message, Server::MAX_MSG_SIZE, 0)) > 0) {
            debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "Server::read", client_message);
            write(client_sock, _def::OK.c_str(), strlen(_def::OK.c_str()) + 1);
            assert(client_sock != -98691);
            assert(client_sock);
            parser->parser(client_message);
            debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "ok");
        }
        assert(client_sock != -98691);
        assert(client_sock);
    }
}

void Server::dispose() {
    if (sockfd >= 0)close(sockfd);
}

void Server::endRun() {
    dispose();
}

Server::~Server() {
    dispose();
}

Server::Server(int portno, Iparser *parser1) {
    debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "Server");
    port = portno;
    parser = parser1;
    struct sockaddr_in server;
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    assert (socket_desc != -1);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(portno);
    int on = 0;
    assert(setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) >= 0);
    debug<LOG_LEVEL::INFO, false>(LINE_INFO, "binding..");

    assert (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) >= 0);
    debug<LOG_LEVEL::INFO, false>(LINE_INFO, "ok");
    listen(socket_desc, 3);


}

void Server::sendMsg(const string &msg) {
    debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "sendmsgaaaa");
    Message m(msg);

    string s = m.getSerializedString();

    assert(client_sock != -98691);
    assert(client_sock);

    debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "write to server ", s, " , socket: ", client_sock);
    write(client_sock, s.c_str(), strlen(s.c_str()) + 1);

    assert(client_sock != -98691);
    assert(client_sock);
    debug<LOG_LEVEL::DEBUG, false>(LINE_INFO, "ok, socket: ", client_sock);
}
