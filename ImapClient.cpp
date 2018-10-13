#include <Arduino.h>
#include <Client.h>
#include <ImapClient.h>

// Comment out to debug
// #define IMAP_DEBUG

Client* client;
ImapClient::ImapClient(Client* cl)
{
    client = cl;
#ifdef IMAP_DEBUG
    Serial.println("construct");
    Serial.println(cl->connected());
#endif
}

bool ImapClient::init(const char* email, const char* password)
{
    waitForResponse();

    while (client->available())
    {
        String line = client->readStringUntil('\r');
#ifdef IMAP_DEBUG
        Serial.println(line);
#endif
    }

    if(login(email, password) == false){
#ifdef IMAP_DEBUG
        Serial.println("login");
#endif
        return false;
    }
    if(selectMailbox() == false){
#ifdef IMAP_DEBUG
        Serial.println("selectMailbox");
#endif
        return false;
    }
    if(startIdling() == false){
#ifdef IMAP_DEBUG
        Serial.println("startIdling");
#endif
        return false;
    }
    return true;
}

/**
 * Checks a possible server response for recent activity
 */
bool ImapClient::hasNewMails(){
    if(isConnected() == false){
#ifdef IMAP_DEBUG
        Serial.println("Not connected");
#endif
        return false;
    }

    bool newMailsAvailable = false;
    // Is new data available?
    while (client->available())
    {
        String line = client->readStringUntil('\r');
        // Does the reponse contain information on recent activity?
        if (line.indexOf("RECENT") != -1)
        {
            int numberEnd = line.indexOf(" ", 3);
            String number = line.substring(2, numberEnd);
            int recentCount = number.toInt();
            if (recentCount >= 0)
            {
#ifdef IMAP_DEBUG
                Serial.println("New mails!");
#endif
                newMailsAvailable = true;
            } else {
#ifdef IMAP_DEBUG
                Serial.println("No new mails :(");
#endif
            }
        }
#ifdef IMAP_DEBUG
        Serial.print(line);
#endif
    }

    return newMailsAvailable;
}

bool ImapClient::selectMailbox(){
    String command = "A select inbox";
#ifdef IMAP_DEBUG
    Serial.println(command);
#endif
    client->println(command);

    waitForResponse();

    while (client->available())
    {
        String line = client->readStringUntil('\r');
#ifdef IMAP_DEBUG
        Serial.println(line);
#endif
    }
    return true;
}

bool ImapClient::startIdling(){
    String command = "A idle";
#ifdef IMAP_DEBUG
    Serial.println(command);
#endif
    client->println(command);
    waitForResponse();
    while (client->available())
    {
        String line = client->readStringUntil('\r');
#ifdef IMAP_DEBUG
        Serial.println(line);
#endif
        return true;
    }
    return false;
}

bool ImapClient::isConnected(){
    return client->connected();
}

void ImapClient::stopConnection(){
    client->stop();
}

bool ImapClient::login(const char *email, const char *password)
{
    // Send login command
    String command = "A login " + String(email) + " " + String(password);
#ifdef IMAP_DEBUG
    Serial.println(command);
#endif
    client->println(command);
    waitForResponse();
    while (client->available())
    {
        String line = client->readStringUntil('\r');
#ifdef IMAP_DEBUG
        Serial.println(line);
#endif
    }
    return true;
}

void ImapClient::waitForResponse(){
    unsigned long timeout = millis();
    while (client->available() == 0)
    {
        if (millis() - timeout > 15000)
        {
#ifdef IMAP_DEBUG
            Serial.println(">>> Client Timeout!");
#endif
            client->stop();
            return;
        }
    }
}
