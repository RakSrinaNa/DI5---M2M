
// Environnement Arduino 1.0.6
// Mise en oeuvre d'un capteur de pression BMP085
// Communication sur bus I2C

#include <Wire.h>

// Capteur de pression : adressage I2C
// adresse sur le bus I2C du capteur BMP085

// Oversampling Setting pour BMP085

// Coefficients de calibration du capteur de pression BMP085 : déclaration

/*---------------------------------------------------------------------------------*/
// Fonctions de gestion du capteur de pression BMP085

/*-----------------------------------------------*/
// Acquisition de la valeurs des parametres de calibration

void getbmp085Calibration()
{
  // Votre code à écrire
}
/*-----------------------------------------------*/
// Calcul de la température connaissant ut.
// La valeur retournée est exprimée en 1/10 de °C (valeur entiére) puis convertie en °C (valeur réelle)
float bmp085GetTemperature(int ut)
{
  // Votre code à écrire  
}
/*-----------------------------------------------*/
// Calcul de la pression en hPa = 100 Pa
// Les paramètres de calibration doivent être connus
// Le calcul de la valeur du coefficient b5 doit être effectué au préalable.
// La valeur retournée est exprimé en Pa (valeur entiére), puis convertie en hPa (valeur réelle).

float bmp085GetPressure(unsigned long up)
{
  
  // Calcul de B3
 // Votre code à écrire
  
  // Calcul de B4
  // Votre code à écrire
  
  // Calcul de B7
  // Votre code à écrire
  // Votre code à écrire
}

/*-----------------------------------------------*/
// Calcul de l'altitude par rapport au niveau de la mer
// A faire : compensation du calcul
//PARTIE OPTIONNELLE
float estime_altitude (float pression)
{
  // Votre code à écrire
}

/*-----------------------------------------------*/
// Lecture d'un octet depuis le capteur BMP085 
// à l'adresse 'address'
char bmp085Read(unsigned char address)
{
 // Votre code à écrire
}

/*-----------------------------------------------*/
// Lecture de 2 octets depuis le capteur BMP085
// Premier octet : adresse 'address' (octet poids fort)
// Second octet : adresse 'address'+1 (octet poids faible)
int bmp085ReadInt(unsigned char address)
{
  // Votre code à écrire
}
/*-----------------------------------------------*/
// Lecture de la valeur non compensée de la température ut
int bmp085ReadUT()
{
  // Votre code à écrire
}
/*-----------------------------------------------*/
// Lecture de la valeur non compensée de la pression up
long bmp085ReadUP()
{
  // Votre code à écrire
  
  // Attente de la conversion, delay dépend de la valeur de OSS
  delay(2 + (3<<OSS));
  
  // Lecture des registres d'adresses 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  // Votre code à écrire
  
  // attendre que les données soient disponibles
  // Votre code à écrire
 
  // Votre code à écrire
  
}

/*---------------------------------------------------------------------------------*/
void setup(void)
{ 
  // Votre code à écrire
  Serial.println("***** Demarrage de l'application ****");
  
  // Initialidation BMP085 : récupérer les paramètres de calibration du capteur
  // Votre code à écrire
}

void loop (void)
{
  float temperature, pression, altitude;
  
  // Acquisition de la température et de la pression
  // Estimation de l'altitude : optionnel
  // Votre code à écrire

  // Gestion de l'affichage des données BMP085
  // Votre code à écrire

  delay(1000);
}
