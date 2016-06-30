//
//  serial_console_sample.ino - Arduinoで簡単なシリアルコンソールを作るひな形サンプル
//
// github:
//     https://github.com/yoggy/serial_console_sample.ino.git
//
// license:
//     Copyright (c) 2016 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php;
//
#include <string.h>

// receive buffer
#define RECV_BUF_SIZE 64
char recv_buf[RECV_BUF_SIZE];
int recv_buf_idx = 0;

// parsed command parameters
char cmd_str[RECV_BUF_SIZE];
int32_t  cmd_param1 = 0;
int32_t  cmd_param2 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("hello serial_console_sample.ino");
  Serial.print("> ");
}

void loop() {
  char buf[2];
  buf[1] = 0;
  
  if (Serial.available() > 0) {
    int c = Serial.read();
    if (c == 10) {  // \n
      Serial.print("\r\n");
      parse_buf();
      clear_buf();
      Serial.print("> ");
    }
    else if (c == 127) {
      backspace_buf();
      buf[0] = (char)c;
      Serial.print(buf);
    }
    else {
      add_buf(c);
      buf[0] = (char)c;
      Serial.print(buf);
    }
  }
}

void clear_buf() {
    memset(recv_buf, 0, RECV_BUF_SIZE);
    recv_buf_idx = 0;
}

void add_buf(int c) {
  if (recv_buf_idx == RECV_BUF_SIZE-1) return; // buffer is full...

  recv_buf[recv_buf_idx] = (char)c;
  recv_buf_idx ++;  
}

void backspace_buf() {
  if (recv_buf_idx > 0) {
    recv_buf_idx --;
    recv_buf[recv_buf_idx] = 0;
  }
}

// 
// command format
//   cmd_str(string) cmd_param1(int) cmd_param2(int)
//
void parse_buf() {
  memset(cmd_str, 0, RECV_BUF_SIZE);
  cmd_param1 = 0;
  cmd_param2 = 0;

  if (recv_buf_idx == 0) return;
  if (strlen(recv_buf) == 0) return;

  char *tp;
  tp = strtok(recv_buf, " "); 

  int idx = 0;
  while (tp != NULL) {
    switch(idx) {
      case 0:
        strncpy(cmd_str, tp, strlen(tp));
        break;
      case 1:
        cmd_param1 = atol(tp);
        break;
      case 2:
        cmd_param2 = atol(tp);
        break;
      default:
        break;
    }
    idx ++;

    tp = strtok(NULL, " ");
  }

  Serial.print("parse result : cmd_str=");
  Serial.print(cmd_str);
  Serial.print(", cmd_param1=");
  Serial.print(cmd_param1);
  Serial.print(", cmd_param2=");
  Serial.print(cmd_param2);
  Serial.println();
}

