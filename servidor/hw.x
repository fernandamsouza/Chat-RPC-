
struct usuario {
  char nickname[10];
};

struct msg {
  int id_usuario;
  char conteudo_mensagem[100];
};

program PROG {
  version VERS {
    int     conectar_usuario(usuario u)      = 1;
    usuario resgatar_usuario(int id_usuario) = 2;
    int     enviar_mensagem(msg m)           = 3;
    int     retornar_numero_mensagens()      = 4;
    msg     resgatar_mensagem(int msgid)     = 5;
  } = 1;
} = 0x30009999;
