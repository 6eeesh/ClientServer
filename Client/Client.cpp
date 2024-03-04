#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024] = { 0 };

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Ошибка при инициализации Winsock" << endl;
        return -1;
    }

    // Создание сокета
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cout << "Ошибка при создании сокета" << endl;
        WSACleanup();
        return -1;
    }

    // Настройка серверного адреса
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);

    // Подключение к серверу
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Ошибка при подключении к серверу" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Отправка запроса серверу
    const char* request = "Как дела?";
    int bytesSent = send(clientSocket, request, strlen(request), 0);
    if (bytesSent == SOCKET_ERROR) {
        cout << "Ошибка при отправке данных" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    cout << "Запрос отправлен: " << request << endl;

    // Получение ответа от сервера
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        cout << "Ошибка при получении данных" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    cout << "Ответ сервера: " << buffer << endl;

    closesocket(clientSocket); // Закрытие клиентского сокета
    WSACleanup(); // Освобождение ресурсов Winsock

    return 0;
}
