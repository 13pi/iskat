#include <sys/stat.h>
#include <string>
#include <iostream>

typedef unsigned short perm_t;

enum FileType {
    SOCKET,
    SYMBOLIC_LINK,
    REGULAR_FILE,
    BLOCK_DEVICE,
    DIRECTORY,
    CHARACTER_DEVICE,
    FIFO,
    NOT_A_FILE
};

std::ostream& operator<<(std::ostream&, FileType const &);

struct File {
    File(std::string const &);

  

    ino_t   inode() const {return _stat.st_ino; }
    mode_t  mode() const { return _stat.st_mode; }
    nlink_t nlinks() const { return _stat.st_nlink; }
    uid_t   uid() const { return _stat.st_uid; }
    gid_t   gid() const { return _stat.st_gid; }
    off_t   size() const { return _stat.st_size; }
    time_t  atime() const { return _stat.st_atime; } // access
    time_t  mtime() const { return _stat.st_mtime; } // modification
    time_t  ctime() const { return _stat.st_ctime; } // status change
    FileType type() const;
    perm_t permissions() const;
    std::string name() const;

private:
    struct stat _stat;
    std::string filename;  
};