#include "File.h"

#include <sys/types.h>
#include <unistd.h>

File::File(std::string const &s) : filename(s) {
    if(stat(s.c_str(), &_stat) < 0) {
        throw;
    }
}

FileType File::type() const  {
    switch(mode() & S_IFMT) {
        case S_IFSOCK: return SOCKET;
        case S_IFLNK : return SYMBOLIC_LINK;
        case S_IFREG : return REGULAR_FILE;
        case S_IFBLK : return BLOCK_DEVICE;
        case S_IFDIR : return DIRECTORY;
        case S_IFCHR : return CHARACTER_DEVICE;
        case S_IFIFO : return FIFO;
        default: return NOT_A_FILE;
    }

}

perm_t File::permissions() const {
    const perm_t permissions_mask = 07777;
    return mode() & permissions_mask;
}

std::string File::name() const {
    return filename;
}

std::ostream& operator<<(std::ostream& o , FileType const & t) {
    switch (t) {
        case SOCKET: return o << "SOCKET";
        case SYMBOLIC_LINK: return o << "SYMBOLIC_LINK";
        case REGULAR_FILE: return o << "REGULAR_FILE";
        case BLOCK_DEVICE: return o << "BLOCK_DEVICE";
        case DIRECTORY: return o << "DIRECTORY";
        case CHARACTER_DEVICE: return o << "CHARACTER_DEVICE";
        case FIFO: return o << "FIFO";
        default: return o << "NOT_A_FILE";
    }
}