#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <iostream>

using namespace std;

class Client
{
private:
  int init_socket;
  sockaddr_in *client_addr;
  int rc;
  void setup_client();
  void setup_server();

public:
  int push(string send_string);
  string get();
  Client();
};

Client::Client()
{
  // Иницилизация соrета
  this->init_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (this->init_socket == -1)
  {
    cout << "[err] Trobles with socket: " << endl;
    exit(0);
  }

  setup_client();
  setup_server();
};

void Client::setup_client()
{
  client_addr = new (sockaddr_in);
  client_addr->sin_family = AF_INET;
  client_addr->sin_port = 0;
  client_addr->sin_addr.s_addr = 0;
  rc = bind(this->init_socket,(const sockaddr*) client_addr,sizeof(sockaddr_in));

  if (rc == -1)
  {
    cout << "[err] Error binding on client side: " << endl;
    exit(0);
  }
}

// Параметры сервера

void Client::setup_server()
{
    sockaddr_in *server_addr;
    server_addr = new (sockaddr_in);
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(4747);
    server_addr->sin_addr.s_addr = htonl((127<<24)+1); // 127.0.0.1 - LOCALHOST
    rc=connect(this->init_socket,(const sockaddr*)server_addr,sizeof(sockaddr_in));
    if (rc==-1)
    {
      cout << "[err] Error binding on server side: " << endl;
      exit(0);
    }
}

// Отправка строки на серверв

int Client::push(string send_string )
{
  rc=send(this->init_socket,(const void*)send_string.data(),send_string.size(),0);
  return rc;
};

// Получение сообщения от сервера, может быть несколько строк

string Client::get()
{
  char * buffer = new char[257];
  int rc = recv(this->init_socket, (void*)buffer, 256, 0);
  return string(buffer);
};

int main(int argc, char ** argv)
{
  Client new_client;
  string in("DoYourHomework\n");
  new_client.push(in);
  cin >> in;
  new_client.push(in);
  cout << new_client.get() << endl;
  return 0;
}
