#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Textos que serão "digitados"
String titulo = "Darel.OS";
String linha1 = "Sistema Operacional";
String linha2 = "Versao 1.0";
String linha3 = "Iniciando...";

unsigned long tempoInicio = 0;
bool carregamentoCompleto = false;
bool logoMostrada = false;
bool menuAtivo = false;
unsigned long tempoLogo = 0;

// Variáveis do menu
int opcaoSelecionada = 0;
const int totalOpcoes = 4;
String opcoes[] = {"Configuracoes", "Aplicativos", "Sistema", "Sobre"};

// Declaração de funções
void digitarTitulo();
void digitarLinha(String texto, int x, int y);
void animarCarregamento();
void mostrarLogo();
void mostrarTitulo();
void mostrarMenu();

void setup() {
  Serial.begin(9600);
  
  // Inicializa o display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha ao inicializar o display OLED!");
    for(;;); // Loop infinito se falhar
  }
  
  display.clearDisplay();
  display.display(); // Mostra tela vazia no início
  
  delay(1000); // Pausa inicial de 1 segundo
  
  // Digita o título
  digitarTitulo();
  
  delay(500); // Pausa entre título e corpo
  
  // Digita as linhas do corpo
  digitarLinha(linha1, 10, 25);
  delay(300);
  digitarLinha(linha2, 10, 35);
  delay(300);
  digitarLinha(linha3, 10, 45);
  
  // Marca o tempo de início do carregamento
  tempoInicio = millis();
}

void loop() {
  if (!carregamentoCompleto) {
    // Animação de carregamento por 5 segundos
    if (millis() - tempoInicio < 5000) {
      animarCarregamento();
    } else {
      carregamentoCompleto = true;
      mostrarLogo();
      tempoLogo = millis(); // Marca o tempo de início da logo
    }
  } else if (logoMostrada && !menuAtivo) {
    // Mostra logo por 3 segundos, depois vai para o menu
    if (millis() - tempoLogo > 3000) {
      menuAtivo = true;
      mostrarMenu();
    }
  } else if (menuAtivo) {
    // Simula navegação automática no menu (apenas para demonstração)
    delay(2000);
    opcaoSelecionada = (opcaoSelecionada + 1) % totalOpcoes;
    mostrarMenu();
  }
}

void digitarTitulo() {
  display.setTextSize(1);
  int x = 35;
  int y = 5;
  
  // Calcula a altura do texto para criar o fundo da linha
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(titulo, 0, 0, &x1, &y1, &w, &h);
  
  // Cria o fundo branco para o título
  display.fillRect(0, y, SCREEN_WIDTH, h, SSD1306_WHITE);
  display.display();
  
  delay(200);
  
  // Digita o título letra por letra
  display.setTextColor(SSD1306_BLACK);
  for (int i = 0; i < titulo.length(); i++) {
    display.setCursor(x, y);
    display.print(titulo.substring(0, i + 1));
    display.display();
    delay(150); // Velocidade da digitação
  }
}

void digitarLinha(String texto, int x, int y) {
  display.setTextColor(SSD1306_WHITE);
  
  // Digita o texto letra por letra
  for (int i = 0; i < texto.length(); i++) {
    display.setCursor(x, y);
    display.print(texto.substring(0, i + 1));
    display.display();
    delay(80); // Velocidade da digitação (mais rápida que o título)
  }
}

void animarCarregamento() {
  int x = 10;
  int y = 45;
  String baseTexto = "Iniciando";
  
  // Ciclo de pontos: ., .., ..., vazio, repete
  String pontos[] = {".", "..", "...", ""};
  
  for (int i = 0; i < 4; i++) {
    // Verifica se o tempo limite foi atingido
    if (millis() - tempoInicio >= 5000) {
      return;
    }
    
    // Limpa apenas a área dos pontos (depois da palavra "Iniciando")
    display.fillRect(x + baseTexto.length() * 6, y, 24, 8, SSD1306_BLACK);
    
    // Reescreve o texto com os pontos atuais
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(x, y);
    display.print(baseTexto + pontos[i]);
    display.display();
    
    delay(500); // Velocidade da animação de carregamento
  }
}

void mostrarTitulo() {
  display.setTextSize(1);
  int x = 35;
  int y = 5;
  
  // Calcula a altura do texto para criar o fundo da linha
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(titulo, 0, 0, &x1, &y1, &w, &h);
  
  display.fillRect(0, y, SCREEN_WIDTH, h, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(x, y);
  display.print(titulo);
}

void mostrarLogo() {
  if (!logoMostrada) {
    display.clearDisplay();
    
    // Mantém o título no topo
    mostrarTitulo();
    
    // Desenha um logo simples (círculo com "D" no centro) - posição mais baixa
    display.drawCircle(64, 32, 15, SSD1306_WHITE);
    display.drawCircle(64, 32, 14, SSD1306_WHITE);
    
    // Desenha a letra "D" estilizada
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(58, 26);
    display.print("D");
    
    // Adiciona alguns elementos decorativos ao redor
    display.drawLine(45, 32, 50, 32, SSD1306_WHITE);
    display.drawLine(78, 32, 83, 32, SSD1306_WHITE);
    display.drawLine(64, 14, 64, 19, SSD1306_WHITE);
    display.drawLine(64, 45, 64, 50, SSD1306_WHITE);
    
    // Escreve "Bem-vindo" embaixo do logo
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    // Centraliza o texto "Bem-vindo"
    String bemVindo = "Bem-vindo";
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(bemVindo, 0, 0, &x1, &y1, &w, &h);
    int xBemVindo = (SCREEN_WIDTH - w) / 2;
    
    display.setCursor(xBemVindo, 55);
    display.print(bemVindo);
    
    display.display();
    logoMostrada = true;
  }
}

void mostrarMenu() {
  display.clearDisplay();
  
  // Mantém o título no topo
  mostrarTitulo();
  
  // Desenha o menu
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  for (int i = 0; i < totalOpcoes; i++) {
    int y = 20 + (i * 10);
    
    // Destaca a opção selecionada
    if (i == opcaoSelecionada) {
      display.fillRect(5, y - 1, SCREEN_WIDTH - 10, 9, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
      display.setCursor(8, y);
      display.print("> " + opcoes[i]);
      display.setTextColor(SSD1306_WHITE);
    } else {
      display.setCursor(8, y);
      display.print("  " + opcoes[i]);
    }
  }
  
  display.display();
}
