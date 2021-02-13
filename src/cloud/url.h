#ifndef URL_H
#define URL_H

#ifndef NO_CLOUD

#ifdef CLOUD_TESTING
#define URL_SERVER "127.0.0.1"

#else
#define URL_SERVER "writernote-b.ddns.net"
#endif //CLOUD_TESTING

#endif //NO_CLOUD

#endif // URL_H
