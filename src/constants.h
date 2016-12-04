/**
 * Constants
 */
const char http_OK[] PROGMEM =
        "HTTP/1.0 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Pragma: no-cache\r\n\r\n";

const char HTTP_JSON[] PROGMEM =
        "HTTP/1.0 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Connection: close\r\n\r\n";

const char http_Found[] PROGMEM =
        "HTTP/1.0 302 Found\r\n"
                "Location: /\r\n\r\n";

const char http_Unauthorized[] PROGMEM =
        "HTTP/1.0 401 Unauthorized\r\n"
                "Content-Type: text/html\r\n\r\n"
                "<h1>401 Unauthorized</h1>";

static const char *const HEADER_JSON = "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n\r\n";

const char *const HTTP = "HTTP";

/**
 * Ethernet setup
 */
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);

