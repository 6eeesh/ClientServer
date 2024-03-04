#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    setlocale(LC_ALL, "");
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024] = { 0 };
    const char* response = "Прекрасно";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Ошибка при инициализации Winsock" << endl;
        return -1;
    }

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        cout << "Ошибка при создании сокета" << endl;
        WSACleanup();
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Ошибка при привязке сокета" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Ошибка при ожидании подключений" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Сервер запущен. Ожидание подключений..." << endl;

    if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
        cout << "Ошибка при принятии подключения" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Получен запрос: " << buffer << endl;

    send(clientSocket, response, strlen(response), 0);
    cout << "Отправлен ответ: " << response << endl;

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
