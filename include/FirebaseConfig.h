#ifndef FIREBASE_CONFIG_H
#define FIREBASE_CONFIG_H
#include <Firebase_ESP_Client.h>

#define FIREBASE_PROJECT_ID "https://msib-7-ta-ldre-default-rtdb.asia-southeast1.firebasedatabase.app/" // Ganti dengan ID Proyek Firebase Anda
#define FIREBASE_API_KEY "AIzaSyDmnYvJv7QkXUZc-T3wxQKnjTUKpy8QnJM"                                      // Ganti dengan Web API Key Anda
#define USER_EMAIL "user@example.com"                                                                   // Email pengguna jika menggunakan email/password
#define USER_PASSWORD "user-password"                                                                   // Password pengguna jika menggunakan email/password

// Objek Firebase dan Config
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

if (Firebase.signUp(&config, &auth, "", ""))
{
  Serial.println("Signup successful");
  signupOK = true;
}
else
{
  Serial.printf("Signup failed: %s\n", config.signer.signupError.message.c_str());
}

// Set up the Firebase token status callback function
config.token_status_callback = tokenStatusCallback;

// Initialize Firebase
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);

#endif