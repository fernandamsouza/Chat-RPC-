#include <stdio.h>
#include <string.h>
#include "hw.h"
#include "tools.h"

int novo_usuario(usuario *u);

// Declaração de tamanhos máximo para as structs
usuario usuarios[50];
msg mensagens[10000];

// Definição de parâmetros auxiliares
int numeroUsuarios = 0;
int numeroMensagens = 0;

// Criação de um novo usuário consequentemente o relacionando com um identificador único
int * conectar_usuario_1_svc(usuario *user, struct svc_req *req) {
  static int id_user;
  int id_usuario = novo_usuario(user);
  id_user = id_usuario;
  return &id_user;
}

// Função auxiliar para criação de um novo usuário(cliente) no servidor
int novo_usuario(usuario *u) {
  usuarios[numeroUsuarios] = *u;
  return numeroUsuarios++;
}

// Consultar informações do cliente -> nickname
usuario * resgatar_usuario_1_svc(int *id_usuario, struct svc_req *req) {
  static usuario resultado_consulta;
  resultado_consulta = usuarios[*id_usuario];
  return &resultado_consulta;
}

// Envio da mensagem do cliente para o servidor
int * enviar_mensagem_1_svc(msg *mensagem, struct svc_req *req) {
  static int resultadoFinal;

  mensagens[numeroMensagens] = *mensagem;
  resultadoFinal = numeroMensagens++;

  FILE *fileW;
  fileW = fopen("enviado_servidor.serv", "w+");

  writeline(mensagem->conteudo_mensagem, fileW);
  printf("Mensagem enviada pelo id %d: %s\n", mensagem->id_usuario, mensagem->conteudo_mensagem);
  fclose(fileW);

  return &resultadoFinal;
}

// Retorna o número de mensagens a serem recebidas
int * retornar_numero_mensagens_1_svc(void *argp, struct svc_req *req) {
  static int consultaMensagens;
  consultaMensagens = numeroMensagens;
  return &consultaMensagens;
}

// Resgata determinada mensagem
msg * resgatar_mensagem_1_svc(int *idMensagem, struct svc_req *req) {
  static msg mensagem;
  mensagem = mensagens[*idMensagem];
  return &mensagem;
}

