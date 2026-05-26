// cliente_calculadora.cpp
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 65432
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "ERROR: Uso: ./cliente <IP_servidor> <num1> <num2> <operacion>" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    double num1 = std::stod(argv[2]);
    double num2 = std::stod(argv[3]);
    int op = std::stoi(argv[4]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "ERROR: IP inválida" << std::endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "ERROR: No se pudo conectar al servidor" << std::endl;
        return 1;
    }

    std::string mensaje = std::to_string(num1) + "," + std::to_string(num2) + "," + std::to_string(op);
    send(sock, mensaje.c_str(), mensaje.size(), 0);

    char buffer[BUFFER_SIZE] = {0};
    int n = read(sock, buffer, BUFFER_SIZE - 1);
    
    if (n > 0) {
        std::cout << buffer << std::endl;  // Salida limpia para Python
    } else {
        std::cout << "Error: No se recibió respuesta del servidor" << std::endl;
    }

    close(sock);
    return 0;
}