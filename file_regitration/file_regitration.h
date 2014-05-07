#ifndef FILE_REGISTRATION_H
#define FILE_REGISTRATION_H


class FileRegistration {
public:
    ~FileRegistration() {
    }

    static FileRegistration * instance();
    static void close() {
        delete self;
    }

private:
    FileRegistration() {

    }

    static FileRegistration *self;
};

#endif // FILE_REGISTRATION
