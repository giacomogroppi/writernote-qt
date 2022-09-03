#pragma once

#ifdef CLOUD

#define PORT 8080

#ifdef CLOUD_TESTING
#define URL_SERVER "127.0.0.1"

#else
#define URL_SERVER "writernote-b.ddns.net"
#endif //CLOUD_TESTING

#endif //CLOUD
