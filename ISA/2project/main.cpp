#include <utility>

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <regex>
#include <cmath>
#include <vector>
#include <map>

#define PORT 1339
#define BUFFER 4096

enum response_type {GET_BOARDS, POST_BOARDS, DELETE_BOARDS, GET_BOARD, POST_BOARD, PUT_ID, DELETE_ID};

std::string get_response_type (int response) {
    std::string arr[8] = {"GET_BOARDS", "POST_BOARDS", "DELETE_BOARDS", "GET_BOARD", "POST_BOARD", "PUT_ID", "DELETE_ID", "ERR"};
    if (response > 7){
        response = 7;
    }
    return arr[response];
}

void logger(const std::string tag, const std::string message) {
    std::cerr << "[" + tag + "]: " + message + "\n";
}

std::string get_ok_200() {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n";
}

std::string get_ok_201() {
    return "HTTP/1.1 201 OK\r\n";
}

std::string get_400(){
    return "HTTP/1.1 400 Bad Request\r\n";
}

std::string get_404() {
    return "HTTP/1.1 404 Not found\r\n";
}

std::string get_409(){
    return "HTTP/1.1 404 Conflict\r\n";
}

std::string get_413(){
    return "HTTP/1.1 413 Payload Too Large\r\n";
}

class Dashboard{
    public:
        explicit Dashboard(std::string new_name);
        void create_post(std::string message);
        void delete_post(int id);
        void edit_post(int id, std::string message);
        std::string get_all_posts();
    private:
        std::string name;
        std::vector<std::string> posts;
};

Dashboard::Dashboard(std::string new_name) {
    name = std::move(new_name);
}

void Dashboard::create_post(std::string message) {
    posts.push_back(message);
}

// TODO: Return code if post doesnt exist
void Dashboard::delete_post(int id) {
    posts.erase(posts.begin() + id - 1);
}

// TODO: Return code if post doesnt exist
void Dashboard::edit_post(int id, std::string message) {
    try {
        posts.at(id - 1) = message;
    } catch (const std::out_of_range& e) {

    }
}

std::string Dashboard::get_all_posts() {
    std::string all_posts;
    int i = 1;
    for (const std::string& post: posts) {
        all_posts.append(std::to_string(i));
        all_posts.append(". ");
        all_posts.append(post);
        all_posts.append("\n");
        i++;
    }
    return all_posts.substr(0, all_posts.length() - 1);
}

class Handler{
    public:
        std::string handle_request();
        void set_request_params(int response_type, std::string board_name, int post_id, std::string request_body);
    private:
        std::string get_boards();
        std::string post_boards();
        std::string delete_boards();
        std::string get_board();
        std::string post_board();
        std::string delete_id();
        std::string put_id();
        int response_type{};
        std::string board_name;
        int post_id{};
        std::string request_body;
        std::map<std::string, Dashboard*> dashboard_list;
};

void Handler::set_request_params(int response_type, std::string board_name, int post_id, std::string request_body) {
    Handler::response_type = response_type;
    Handler::board_name = std::move(board_name);
    Handler::post_id = post_id;
    Handler::request_body = std::move(request_body);
}

std::string Handler::handle_request() {
    switch (response_type){
        case GET_BOARDS:
            return get_boards();
        case POST_BOARDS:
            return post_boards();
        case DELETE_BOARDS:
            return delete_boards();
        case GET_BOARD:
            return get_board();
        case POST_BOARD:
            return post_board();
        case DELETE_ID:
            return delete_id();
        case PUT_ID:
            return put_id();
    }
    return get_404();
}

std::string Handler::get_boards() {
    std::string body;
    std::string response;
    for (auto & it : dashboard_list) {
        body.append(it.first);
        // TODO: SHOULD LAST BOARD NAME HAVE NEWLINE AT THE END?
        body.append("\n");
    }
    if (body.length() != 0) {
        response = get_ok_200();
        response.append("Content-Length: ");
        response.append(std::to_string(body.length() - 1));
        response.append("\r\n\r\n");
        response.append(body.substr(0, body.length() - 1));
        return response;
    } else {
        return get_404();
    }
}

std::string Handler::post_boards() {
    auto pos = dashboard_list.find(board_name);
    if (pos == dashboard_list.end()) {
        dashboard_list.insert(std::make_pair(board_name, new Dashboard(board_name)));
        return get_ok_201();
    } else {
        return get_409();
    }
}

std::string Handler::delete_boards() {
    auto pos = dashboard_list.find(board_name);
    if (pos != dashboard_list.end()){
        dashboard_list.erase(board_name);
        return get_ok_201();
    } else {
        return get_404();
    }
}

std::string Handler::get_board() {
    std::string response;
    auto pos = dashboard_list.find(board_name);
    if (pos != dashboard_list.end()){
        std::string body = pos->second->get_all_posts();
        // TODO: CHECK IF GOOD RETURN?
        if (body.length() == 0) {
            return get_404();
        }
        response = get_ok_200();
        response.append("Content-Length: ");
        response.append(std::to_string(body.length()));
        response.append("\r\n\r\n");
        response.append(body);
        return response;
    } else {
        return get_404();
    }
}

std::string Handler::post_board() {
    auto pos = dashboard_list.find(board_name);
    if (pos != dashboard_list.end()){
        pos->second->create_post(request_body);
        return get_ok_201();
    } else {
        return get_404();
    }
}

std::string Handler::delete_id() {
    auto pos = dashboard_list.find(board_name);
    if (pos != dashboard_list.end()){
        pos->second->delete_post(post_id);
        return get_ok_200();
    } else {
        return get_404();
    }
}

std::string Handler::put_id() {
    auto pos = dashboard_list.find(board_name);
    if (pos != dashboard_list.end()){
        pos->second->edit_post(post_id, request_body);
        return get_ok_200();
    } else {
        return get_404();
    }
}

class Body{
    public:
        std::string get_body();
        void set_body(std::string body_string);
        void append_body(char* request, int bytes_read);
    private:
        std::string body;
};

std::string Body::get_body() {
    return body;
}

void Body::set_body(std::string body_string) {
    body = std::move(body_string);
}

void Body::append_body(char *request, int bytes_read) {
    std::string body_string(request);
    body_string = body_string.substr(0, bytes_read);
    body.append(body_string);
}

class Header
{
    public:
        void create_request_string(char *request, int bytes_read);
        std::string get_request_string();
        void parse_first_line();
        int get_response();
        int parse_header();
        std::string get_board_name();
        int get_post_id();
        int check_content_length();
    private:
        int byte_length;
        std::string request_type;
        std::string board;
        std::string name;
        int response;
        long id;
        long content_length;
        std::string raw_request;
        std::string get_line_from_request();
        void parse_get();
        void parse_post();
        void parse_delete();
        void parse_put();

};

void Header::create_request_string(char *request, int bytes_read) {
    response = 404;
    content_length = -1;
    id = -1;
    byte_length = bytes_read;
    std::string request_string(request);
    request_string = request_string.substr(0, bytes_read);
    raw_request = request_string;
}

std::string Header::get_request_string() {
    return raw_request;
}

std::string Header::get_line_from_request() {
    std::string delimiter = "\n";
    size_t pos = 0;
    std::string token;
    if ((pos = raw_request.find(delimiter)) != std::string::npos) {
        token = raw_request.substr(0, pos + delimiter.length());
        raw_request.erase(0, pos + delimiter.length());
    }
    return token;
}

void Header::parse_first_line() {
    std::string line = get_line_from_request();
    std::regex request_regex ("(GET|POST|PUT|DELETE) */([a-zA-Z0-9]*)(/([a-zA-Z0-9]*)(/(\\d*))?)? *(HTTP/1.1)");
    std::smatch match;
    if (std::regex_search(line, match, request_regex)) {

        request_type = match[1];
        board = match[2];
        name = match[4];
        try {
            id = std::stoi(match[6]);
        } catch (...) {
            id = -1;
        }

        if (request_type == "GET") {
            parse_get();
        } else if (request_type == "POST") {
            parse_post();
        } else if (request_type == "PUT") {
            parse_put();
        } else if (request_type == "DELETE") {
            parse_delete();
        } else {
            response = 404;
        }

    }
    else {
        response = 404;
    }
}

int Header::parse_header() {
    std::string token;
    while(true) {
        token = get_line_from_request();
        if (token.empty()) {
            if (byte_length == BUFFER) {
                throw (std::length_error("Header too big"));
            }
            else {
                throw (std::invalid_argument("Header has wrong format"));
            }
        }

        std::smatch match;
        std::regex content_length_regex("CONTENT-LENGTH: *(\\d*)", std::regex_constants::icase);
        if (std::regex_search(token, match, content_length_regex)) {
            content_length = stol(match[1]);
        }
        if (token == "\r\n" || token == "\n"){
            break;
        }
    }
    int remaining_bytes = content_length - raw_request.length();
    return remaining_bytes;
}

void Header::parse_get() {
    if (board == "boards") {
        if (name.empty() && (id == -1)) {
            response = GET_BOARDS;
        } else {
            response = 404;
        }
    } else if (board == "board") {
        if (!name.empty() && (id == -1)) {
            response = GET_BOARD;
        }
        else {
            response = 404;
        }
    }
}

void Header::parse_post() {
    if (board == "boards") {
        if (!name.empty() && (id == -1)) {
            response = POST_BOARDS;
        } else {
            response = 404;
        }
    } else if (board == "board") {
        if (!name.empty() && (id == -1)) {
            response = POST_BOARD;
        }
        else {
            response = 404;
        }
    }
}

void Header::parse_put() {
    if (board == "board") {
        if (!name.empty() && (id >= 0)) {
            response = PUT_ID;
        } else {
            response = 404;
        }
    }
}

void Header::parse_delete() {
    if (board == "boards") {
        if (!name.empty() && (id == -1)) {
            response = DELETE_BOARDS;
        } else {
            response = 404;
        }
    } else if (board == "board") {
        if (!name.empty() && id >= 0) {
            response = DELETE_ID;
        } else {
            response = 404;
        }
    } else {
        response = 404;
    }
}

int Header::get_response() {
    return response;
}

std::string Header::get_board_name() {
    return name;
}

int Header::get_post_id() {
    return id;
}

int Header::check_content_length() {
    if (content_length == 0 && (request_type == "POST" || request_type == "PUT")){
        return 400;
    } else {
        return 200;
    }
}

int main() {
    Header header;
    Handler handler;
    sockaddr_in client;

    int server_socket, connection, bytes_read, optval;
    char header_buffer[BUFFER];
    char body_buffer[BUFFER];

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        logger("ERROR", "Cannot create socket");
        return errno;
    }

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0){
        logger("ERROR", "Setting socket option failed");
        close(server_socket);
        return errno;
    }

    if (fcntl(server_socket, O_NONBLOCK, 0) < 0){
        logger("ERROR", "Setting socket to non blocking failed");
        close(server_socket);
        return errno;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        logger("ERROR", "Binding of socket failed");
        close(server_socket);
        return errno;
    }

    if (listen(server_socket, 1) != 0) {
        logger("ERROR", "Failed to listen on socket");
        close(server_socket);
        return errno;
    }


    while(true) {
        auto addrlen = sizeof(client);
        logger("INFO", "Wating for connection");

        if ((connection = accept(server_socket, (struct sockaddr *) &client, (socklen_t *) &addrlen)) == -1) {
            logger("ERROR", "Accepting connection failed");
            close(server_socket);
            close(connection);
            return errno;
        }
        memset(header_buffer, 0, BUFFER);
        bytes_read = read(connection, header_buffer, BUFFER);
        logger("INFO", "I have read the message");
        header.create_request_string(header_buffer, bytes_read);
        header.parse_first_line();
        int response_code = header.get_response();
        std::string response;
        if (response_code == 404) {
            response = get_404();
            write(connection, response.c_str(), response.size());
            close(connection);
            continue;
        }
        int remaining_bytes;
        try {
            remaining_bytes = header.parse_header();
        } catch (std::length_error& e) {
            response = get_413();
            write(connection, response.c_str(), response.size());
            close(connection);
            continue;
        } catch (std::invalid_argument& e) {
            response = get_404();
            write(connection, response.c_str(), response.size());
            close(connection);
            continue;
        }
        if (header.check_content_length() == 400){
            response = get_400();
            write(connection, response.c_str(), response.size());
            close(connection);
            continue;
        }
        Body body;
        body.set_body(header.get_request_string());
        // TODO: READ BUFFER SIZE OR REMAINING_BYTES SIZE IF ITS LESS THAN BUFFER SIZE?
        while(remaining_bytes > 0) {
            memset(body_buffer, 0, BUFFER);
            bytes_read = read(connection, body_buffer, BUFFER);
            body.append_body(body_buffer, bytes_read);
            remaining_bytes -= bytes_read;
        }
        handler.set_request_params(response_code, header.get_board_name(), header.get_post_id(), body.get_body());
        response = handler.handle_request();
        write(connection, response.c_str(), response.size());
        close(connection);
    }
}