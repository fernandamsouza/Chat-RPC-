#include <stdio.h>
#include <stdlib.h>

#include "hw.h"
#include "tools.h"

char* readingFile(char *nameFileMessage, char *file_content);

int main(int argc, char *argv[]) {
  
  // Estrutura RPC de comunicação 
  CLIENT *c1 = NULL;

  // Estrutura para armazenar o nickname do cliente
  usuario *user = NULL;
  
  // Auxiliares
  int opcao = 0;
  int id_usuario;

  // Verificação de parâmetros do console
  if (argc != 3) {
    printf("Uso: ./client host nickname\n");
    return 1;
  }
  
  // Retorna um id único identificando o usuário conectado
  id_usuario = conectar_rpc(&c1, argv[1], &user, argv[2]);

  printf("\nRPC - Fernanda Maria de Souza\n");
  // Menu para navegação 
  do {
    opcao = menu();
    switch(opcao) {
      case 0:
        break;
      case 1:
        // Envio de mensagens para o servidor
        envio_mensagens(id_usuario, argv[2], c1);
        break;
      case 2:
        // Para receber as mensagens, os clientes devem consultar o servidor de tempos em tempos (polling)
        recebimento_mensagens(c1);
        break;
      default:
        break;
    }
  } while(opcao != 0);
}

int menu() {
  int num;
  printf("\n------------------------------");
  printf("\n1 - Enviar mensagem\n");
  printf("\n2 - Receber mensagem\n");
  printf("\n0 - Sair\n");
  printf("\n------------------------------");

  scanf("\n%d", &num);
  return num;
}

// Conectar ao servidor RPC retornando um id único para identificação do cliente
int conectar_rpc(CLIENT **client, char *host, usuario **user, char *nickname) {
  *user = malloc(sizeof(usuario));
  *client = clnt_create(host, PROG, VERS, "udp");

  strcpy((*user)->nickname, nickname);
  return *conectar_usuario_1(*user, *client);
}

// Envio de mensagens ao servidor
void envio_mensagens(int id_usuario, char *nickname, CLIENT *client) {
   
  msg mensagem;
  FILE *fileR;

  // Auxiliares
  char* file_content = NULL;
  char chat[50] = "chat";
  char auxchat[10] = "-01.chat";

  // Montagem do nome de arquivo estipulado na descrição do trabalho 
  strcat(chat, nickname);
  strcat(chat, auxchat);

  // Leitura do arquivo a ser enviado e passagem para a struct
  fileR = fopen(chat, "r");
  file_content = readingFile(chat, fileR);
  strcpy (mensagem.conteudo_mensagem, file_content);
  mensagem.id_usuario = id_usuario;

  // Passagem da mensagem lida para o servidor
  enviar_mensagem_1(&mensagem, client);
}

// Leitura de arquivo .chat
char* readingFile(char *nomeDoArquivo, char *conteudoDoArquivo) {

	FILE *fileR;
	fileR = fopen(nomeDoArquivo, "r");
	
	if (fileR == NULL) {
		printf("Erro na abertura de arquivo.");
		exit(1);
	}

	conteudoDoArquivo = readline(fileR);
	
	fclose(fileR);
	return conteudoDoArquivo;
}

// Polling para recebimento de mensagens e consequente retransmissão aos outros clientes
void recebimento_mensagens(CLIENT *client) {

  // Definições auxiliares
  FILE *fileW;
  msg *mensagem;
  usuario *user;
  int i;
  int mensagens = 0;
  void *p;

  mensagens = *retornar_numero_mensagens_1(p, client);

  for (i = 0; i < mensagens; i++) {

    // Resgate no servidor da mensagem para retransmissão aos clientes conectados
    mensagem = resgatar_mensagem_1(&i, client);

    // Resgate no servidor do usuário atual
    user = resgatar_usuario_1(&(mensagem->id_usuario), client);

    // Adequação do nome do arquivo conforme descrição do trabalho
    char auxiliar2[20] = "-0";
    char auxiliar3[20] = ".client";
    sprintf(auxiliar2, "%d", i);
    strcat(user->nickname, auxiliar2);
    strcat(user->nickname, ".client");
    sprintf(auxiliar3, "%d", mensagem->id_usuario);
    strcat(user->nickname, auxiliar3);

    // Escrita da mensagem no arquivo
    fileW = fopen(user->nickname, "w+");

    if (fileW == NULL) {
		  printf("Erro na abertura de arquivo.");
		  exit(1);
	  } 
    
    writeline(mensagem->conteudo_mensagem, fileW);
    fclose(fileW);
  }
}  
