#include <sstream>
#include <string>

#define STR(x) (((std::ostringstream&)(std::ostringstream() << x)).str())
#define UID(x) "[" << (x)->getInGameID() << "]"
