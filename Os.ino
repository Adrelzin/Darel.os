#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Estados do sistema
enum SistemaEstado { BOOT, LOGO, MENU_PRINCIPAL };
SistemaEstado estadoAtual = BOOT;

// Variáveis de tempo
unsigned long tempoInicio = 0;
unsigned long ultimoTempoMenu = 0;

// Variáveis para controle de animação
int etapaAnimacao = 0;
unsigned long tempoUltimaEtapa = 0;

// Variáveis para animação de texto
int charAtualTitulo = 0;
int charAtualTexto = 0;
int pontoAtual = 0;
unsigned long ultimoPonto = 0;
unsigned long ultimoChar = 0;

// Menu
const int TOTAL_OPCOES = 4;
const String OPCOES_MENU[TOTAL_OPCOES] = {
  "Calculadora",
  "Relogio",
  "Configuracoes",
  "Sobre"
};
int opcaoSelecionada = 0;

void setup() {
  Serial.begin(9600);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha no display OLED!");
    while(true);
  }
  
  display.clearDisplay();
  display.display();
  tempoInicio = millis();
  tempoUltimaEtapa = millis();
}

void loop() {
  switch(estadoAtual) {
    case BOOT:
      executarBoot();
      break;
    case LOGO:
      executarLogo();
      break;
    case MENU_PRINCIPAL:
      executarMenu();
      break;
  }
}

void executarBoot() {
  unsigned long tempoDecorrido = millis() - tempoUltimaEtapa;
  
  switch(etapaAnimacao) {
    case 0: // Aguarda 1 segundo inicial
      if (tempoDecorrido >= 1000) {
        display.clearDisplay();
        etapaAnimacao = 1;
        tempoUltimaEtapa = millis();
      }
      break;
      
    case 1: // Desenha fundo completo da linha do título
      {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        
        // Desenha fundo branco em toda a largura da tela
        display.fillRect(0, 4, SCREEN_WIDTH, 10, SSD1306_WHITE);
        display.display();
        
        etapaAnimacao = 2;
        tempoUltimaEtapa = millis();
        charAtualTitulo = 0; // Reset contador
        ultimoChar = millis();
      }
      break;
      
    case 2: // Aguarda um pouco antes de digitar
      if (tempoDecorrido >= 300) {
        etapaAnimacao = 3;
        tempoUltimaEtapa = millis();
      }
      break;
      
    case 3: // Digita "Darel.OS" letra por letra
      {
        String titulo = "Darel.OS";
        
        if (millis() - ultimoChar >= 150 && charAtualTitulo <= titulo.length()) {
          // Limpa apenas a área do texto do título
          display.fillRect(35, 5, 60, 8, SSD1306_WHITE);
          
          display.setTextColor(SSD1306_BLACK);
          display.setCursor(35, 5);
          display.print(titulo.substring(0, charAtualTitulo));
          display.display();
          
          charAtualTitulo++;
          ultimoChar = millis();
          
          if (charAtualTitulo > titulo.length()) {
            etapaAnimacao = 4;
            tempoUltimaEtapa = millis();
            charAtualTexto = 0;
            ultimoChar = millis();
          }
        }
      }
      break;
      
    case 4: // Digita "Sistema Operacional"
      {
        String texto = "Sistema Operacional";
        
        if (millis() - ultimoChar >= 80 && charAtualTexto <= texto.length()) {
          display.setTextColor(SSD1306_WHITE);
          display.setCursor(10, 25);
          display.print(texto.substring(0, charAtualTexto));
          display.display();
          
          charAtualTexto++;
          ultimoChar = millis();
          
          if (charAtualTexto > texto.length()) {
            etapaAnimacao = 5;
            tempoUltimaEtapa = millis();
            charAtualTexto = 0;
            ultimoChar = millis();
          }
        }
      }
      break;
      
    case 5: // Digita "Versao 1.0"
      {
        String texto = "Versao 1.0";
        
        if (millis() - ultimoChar >= 80 && charAtualTexto <= texto.length()) {
          display.setCursor(10, 35);
          display.print(texto.substring(0, charAtualTexto));
          display.display();
          
          charAtualTexto++;
          ultimoChar = millis();
          
          if (charAtualTexto > texto.length()) {
            etapaAnimacao = 6;
            tempoUltimaEtapa = millis();
            charAtualTexto = 0;
            ultimoChar = millis();
          }
        }
      }
      break;
      
    case 6: // Digita "Iniciando"
      {
        String texto = "Iniciando";
        
        if (millis() - ultimoChar >= 80 && charAtualTexto <= texto.length()) {
          display.setCursor(10, 45);
          display.print(texto.substring(0, charAtualTexto));
          display.display();
          
          charAtualTexto++;
          ultimoChar = millis();
          
          if (charAtualTexto > texto.length()) {
            etapaAnimacao = 7;
            tempoUltimaEtapa = millis();
          }
        }
      }
      break;
      
    case 7: // Transição para LOGO
      if (tempoDecorrido >= 1000) {
        estadoAtual = LOGO;
        etapaAnimacao = 0;
        tempoUltimaEtapa = millis();
        pontoAtual = 0;
        ultimoPonto = millis();
      }
      break;
  }
}

void executarLogo() {
  unsigned long tempoDecorrido = millis() - tempoUltimaEtapa;
  
  switch(etapaAnimacao) {
    case 0: // Animação dos pontos
      {
        if (millis() - ultimoPonto >= 500) {
          // Limpa área dos pontos
          display.fillRect(65, 45, 25, 10, SSD1306_BLACK);
          display.setCursor(65, 45);
          
          // Mostra pontos
          for (int i = 0; i <= pontoAtual; i++) {
            display.print(".");
          }
          display.display();
          
          pontoAtual = (pontoAtual + 1) % 4;
          ultimoPonto = millis();
        }
        
        // Após 3 segundos, passa para logo
        if (tempoDecorrido >= 3000) {
          etapaAnimacao = 1;
          tempoUltimaEtapa = millis();
        }
      }
      break;
      
    case 1: // Mostra logo completa
      display.clearDisplay();
      
      // Cabeçalho
      display.fillRect(0, 0, SCREEN_WIDTH, 10, SSD1306_WHITE);
      display.setTextSize(1);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(35, 2);
      display.print("Darel.OS");
      
      // Logo central - círculo duplo
      display.drawCircle(64, 32, 15, SSD1306_WHITE);
      display.drawCircle(64, 32, 14, SSD1306_WHITE);
      
      // Letra D central
      display.setTextSize(2);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(60, 25);
      display.print("D");
      
      // Elementos decorativos
      display.drawLine(45, 32, 50, 32, SSD1306_WHITE); // esquerda
      display.drawLine(78, 32, 83, 32, SSD1306_WHITE); // direita
      display.drawLine(64, 14, 64, 19, SSD1306_WHITE); // cima
      display.drawLine(64, 45, 64, 50, SSD1306_WHITE); // baixo
      
      // Rodapé
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(30, 55);
      display.print("Bem-vindo!");
      
      display.display();
      
      etapaAnimacao = 2;
      tempoUltimaEtapa = millis();
      break;
      
    case 2: // Aguarda antes do menu
      if (tempoDecorrido >= 2000) {
        estadoAtual = MENU_PRINCIPAL;
        etapaAnimacao = 0;
        ultimoTempoMenu = millis();
        mostrarMenu();
      }
      break;
  }
}

void mostrarMenu() {
  display.clearDisplay();
  
  // Cabeçalho
  display.fillRect(0, 0, SCREEN_WIDTH, 10, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(35, 2);
  display.print("Darel.OS");
  
  // Opções do menu
  display.setTextSize(1);
  for (int i = 0; i < TOTAL_OPCOES; i++) {
    int yPos = 15 + i * 12;
    
    if (i == opcaoSelecionada) {
      // Destaque da opção selecionada
      display.fillRoundRect(5, yPos, SCREEN_WIDTH - 10, 10, 3, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    
    display.setCursor(10, yPos + 1);
    display.print(OPCOES_MENU[i]);
  }
  
  // Seta indicadora
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH - 15, 16 + opcaoSelecionada * 12);
  display.print(">");
  
  display.display();
}

void executarMenu() {
  // Navegação automática para demonstração
  if (millis() - ultimoTempoMenu >= 2000) {
    ultimoTempoMenu = millis();
    opcaoSelecionada = (opcaoSelecionada + 1) % TOTAL_OPCOES;
    mostrarMenu();
  }
}
