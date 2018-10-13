#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ImapClient.h>

// Your IMAP providers server details
const char *host = "imap.example.org";
// Port is propably 143 without SSL or 993 with SSL
const int port = 993;

const char *email = "you@example.org";
const char *password = "yourpassword";

//  your network SSID (name)
const char *ssid = "yourwifissid";
const char *pass = "yourwifipassword";

int status = WL_IDLE_STATUS;
ImapClient *imap;
WiFiClientSecure secureClient;

// SHA1 fingerprint 
// echo -n | openssl s_client -connect imap.example.org:993 -CAfile /usr/share/ca-certificates/mozilla/DigiCert_Assured_ID_Root_CA.crt | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > ./imap.example.org.pem
// openssl x509 -noout -in imap.example.org.pem -fingerprint -sha1
const char *fingerprint = "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";

void setup()
{
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    secureClient.connect(host, port);
    if (secureClient.verify(fingerprint, host))
    {
        Serial.println("certificate matches");
        imap = new ImapClient(&secureClient);
        imap->init(email, password);
    }
    else
    {
        Serial.println("certificate doesn't match");
        secureClient.stop();
    }
}

void loop()
{
    if (secureClient.connected() && imap->hasNewMails())
    {
        // Trigger an Action upon arrival here
        Serial.println("New mails arrived");
    }
}
