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

void loop() {
  // Loop vazio - animação acontece apenas uma vez no setup
}
