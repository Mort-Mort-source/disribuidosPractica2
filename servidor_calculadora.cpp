// servidor_calculadora.cpp
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 65432
#define BUFFER_SIZE 1024

std::string calcular(double num1, double num2, int op) {
    switch(op) {
        case 1: return "Resultado: " + std::to_string(num1 + num2);
        case 2: return "Resultado: " + std::to_string(num1 - num2);
        case 3: return "Resultado: " + std::to_string(num1 * num2);
        case 4: return (num2 == 0) ? "Error: División por cero" : "Resultado: " + std::to_string(num1 / num2);
        default: return "Error: Operación no válida";
    }
}

void atender_cliente(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    int n = read(client_fd, buffer, BUFFER_SIZE - 1);
    if (n <= 0) {
        close(client_fd);
        return;
    }

    std::string datos(buffer, n);  // Mejor manejo de longitud
    std::vector<std::string> partes;
    std::stringstream ss(datos);
    std::string item;

    while (std::getline(ss, item, ',')) {
        partes.push_back(item);
    }

    if (partes.size() != 3) {
        send(client_fd, "Error: Formato inválido", 23, 0);
        close(client_fd);
        return;
    }

    try {
        double num1 = std::stod(partes[0]);
        double num2 = std::stod(partes[1]);
        int op = std::stoi(partes[2]);
        std::string respuesta = calcular(num1, num2, op);
        send(client_fd, respuesta.c_str(), respuesta.size(), 0);
    } catch(...) {
        send(client_fd, "Error: Números inválidos", 24, 0);
    }
    close(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address, client_addr;
    int opt = 1;
    socklen_t addrlen = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Error en bind()\n";
        return 1;
    }
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Error en listen()\n";
        return 1;
    }

    std::cout << "Servidor C++ corriendo en puerto " << PORT << " (0.0.0.0)\n";
    std::cout << "Esperando conexiones...\n";

    while (true) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
        std::cout << "Conexión desde " << ip << ":" << ntohs(client_addr.sin_port) << std::endl;

        std::thread(atender_cliente, client_fd).detach();
    }

    close(server_fd);
    return 0;
}