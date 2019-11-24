#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>

#define BUFFER 4096

void print_help() {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << "-h\t\tShow help message" << std::endl;
    std::cout << "\t" << "-H HOST\t\tSet host for server" << std::endl;
    std::cout << "\t" << "-p PORT\t\tSet port for server" << std::endl;
}

std::string get_line_from_request(std::string *message) {
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    if ((pos = (*message).find(delimiter)) != std::string::npos) {
        token = (*message).substr(0, pos + delimiter.length());
        (*message).erase(0, pos + delimiter.length());
    }
    return token;
}

void print_response(char* response, int bytes_read) {
    std::string response_string(response);
    response_string = response_string.substr(0, bytes_read);
    std::string headers;
    std::string token;
    while (!(token = get_line_from_request(&response_string)).empty()) {
        if (token.compare("\r\n") == 0)
            break;
        headers.append(token);
    }
    std::cerr << headers;
    std::cout << response_string << std::endl;
}

class Parser {
    public:
        Parser(int argc, char* argv[]);
        void parse_params();
        int get_port();
        std::string get_request_string();
        std::string get_host();
    private:
        void parse_command();
        int argc;
        char** argv;
        int PORT;
        std::string HOST{};
        std::string what;
        std::string command;
        std::string name;
        std::string request_type;
        std::string request;
        std::string body;
        std::string id;
};

Parser::Parser(int argc, char* argv[]) {
    Parser::argc = argc;
    Parser::argv = argv;
    PORT = -1;
}

void Parser::parse_params() {
    if (argc < 6) {
        print_help();
        exit(-1);
    }
    if (std::string(argv[1]) == "-H") {
        HOST = std::string(argv[2]);
        if (HOST.empty()) {
            print_help();
            exit(-1);
        }
    } else if (std::string(argv[1]) == "-h") {
        print_help();
        exit(-1);
    }
    if (std::string(argv[3]) == "-p") {
        std::string param(argv[4]);
        std::size_t pos;
        PORT = std::stoi(param, &pos, 10);
        if (pos != param.size() || PORT < 0) {
            print_help();
            exit(-1);
        }
    }
    parse_command();

}

int Parser::get_port() {
    return PORT;
}

std::string Parser::get_host() {
    return HOST;
}

void Parser::parse_command() {
    what = std::string(argv[5]);
    request = " /boards";

    if (argc >= 8) {
        command = std::string(argv[6]);
        name = std::string(argv[7]);
        if (command == "add") {
            request_type = "POST";
        }
        if (command == "delete") {
            request_type = "DELETE";
        }
    }

    switch (argc){
        case 6: {
            if (what == "boards") {
                request_type = "GET";
            }
            break;
        }
        case 8: {
            if (what == "board") {
                if(command == "list") {
                    request_type = "GET";
                    request = " /board";
                }
            } else {
                request_type = std::string();
            }
            break;
        }
        case 9: {
            request = " /board";
            if (request_type == "POST") {
                body = std::string(argv[8]);
                break;
            } else if (request_type == "DELETE"){
                id = std::string(argv[8]);
                break;
            }
            request_type = std::string();
            break;
        }
        case 10: {
            request = " /board";
            if (command == "update") {
                request_type = "PUT";
                id = std::string(argv[8]);
                body = std::string(argv[9]);
                break;
            }
            request_type = std::string();
            break;
        }
    }
}

std::string Parser::get_request_string() {
    std::string response_string;
    if (request_type.empty()) {
        return std::string();
    }
    response_string.append(request_type);
    response_string.append(request);
    if (!name.empty()) {
        response_string.append("/");
        response_string.append(name);
    }
    if (!id.empty()) {
        response_string.append("/");
        response_string.append(id);
    }

    response_string.append(" HTTP/1.1\r\n");
    if (!body.empty()) {
        response_string.append("Content-Type: text/plain\r\n");
        response_string.append("Content-Length: ");
        response_string.append(std::to_string(body.length()));
        response_string.append("\r\n\r\n");
        response_string.append(body.substr(0, body.length()));
    } else {
        response_string.append("\r\n");
    }
    return response_string;
}

int main(int argc, char* argv[]) {
    Parser parser(argc, argv);
    parser.parse_params();
    std::string HOST = parser.get_host();

    struct hostent *servent;
    int client_socket;
    sockaddr_in server;
    char buffer[BUFFER];

    //Remove garbage
    memset(&server,0,sizeof(server));

    if ((servent = gethostbyname(HOST.c_str())) == nullptr) {
        std::cerr << "hostbyname error" << std::endl;
        return -1;
    }

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket error" << std::endl;
        return -1;
    }

    server.sin_family = AF_INET;
    memcpy(&server.sin_addr,servent->h_addr,servent->h_length);
    server.sin_port = htons(parser.get_port());

    //Create request based on params
    std::string request = parser.get_request_string();
    if (request.empty()) {
        std::cerr << "wrong request input" << std::endl;
        return -1;
    }

    if ((connect(client_socket, (struct sockaddr*)&server, sizeof(server))) < 0) {
        std::cerr << "cant connect" << std::endl;
        close(client_socket);
        return -1;
    }
    send(client_socket, request.c_str(), request.size(), MSG_DONTWAIT);

    int bytes_read = read(client_socket, buffer, BUFFER);
    close(client_socket);

    print_response(buffer, bytes_read);
    return 0;
}
