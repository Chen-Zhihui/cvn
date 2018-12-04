

#include <rx.hpp>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/DirectoryIterator.h>


// images : dir ->  obs<Poco::Path>
// folders : dir-> obs<Poco::Path>

namespace imageio {


    inline decltype(auto) files(const Poco::Path &dir) {
        Poco::DirectoryIterator it(inputdir);
        Poco::DirectoryIterator end;
        return rx::range(it, end).map([](Poco::DirectoryIterator it) {
            return Poco::Path(it->path());
        })
    }

    inline decltype(auto) images(const Poco::Path &dir) {
        return files(dir).flatMap([](const Poco::Path &p) {
            return files(p)
        })
    }


}