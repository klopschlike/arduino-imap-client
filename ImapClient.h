// This is start of the header guard.  IMAPCLIENT_H can be any unique name.  By convention, we use the name of the header file.
#ifndef IMAPCLIENT_H
#define IMAPCLIENT_H

// This is the content of the .h file, which is where the declarations go
class ImapClient
{
    public:
        ImapClient(Client*);
        bool hasNewMails();
        bool init(const char *email, const char *password);
        bool isConnected();
    private:
        Client* client;
        bool login(const char *email, const char *password);
        bool selectMailbox();
        bool startConnection();
        bool startIdling();
        void stopConnection();
        void waitForResponse();
};

// This is the end of the header guard
#endif
