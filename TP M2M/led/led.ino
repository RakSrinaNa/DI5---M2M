#define led 13 // Constante representant le pin de la LED

// Appelée une fois au démarage
void setup() {
  Serial.begin(9600); // Def le débit de transmition de données
  pinMode(led, OUTPUT); // Def que le pin 13 sera une sortie
  digitalWrite(led, LOW); // Ecrit sur un pin digital (0 ou 1)
  Serial.println("Lancement de l'app"); // Log
}

// Exécuter en boucle
void loop() {
  digitalWrite(led, HIGH); // Allume la LED 
  Serial.println("LED allumée"); // Log
  delay(1000); // Attend 1s
  digitalWrite(led, LOW); // Eteind la LED
  Serial.println("LED éteinte"); // Log
  delay(800); // Attend 0.8s
}
