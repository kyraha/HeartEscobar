//============================================================================
// Name        : CygwinTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <libexif/exif-data.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

class PhotoFile {
public:
	PhotoFile(directory_entry& p);
	string getIdFromExif();
	string id;
	directory_entry mPath;
	friend std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo);
};

std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo)
{
	stream << "\"" << photo.id << "\" : " << photo.mPath;
	return stream;
}

PhotoFile::PhotoFile(directory_entry& p)
: mPath(p)
{
	id = getIdFromExif();
}

string PhotoFile::getIdFromExif()
{
	string ret("");
	ExifData *exif_data = exif_data_new_from_file(mPath.path().c_str());
	if(!exif_data) return "";

	ExifEntry *entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
    if (entry && entry->format==2) {
    	ret += (const char*)entry->data;
   		trim(ret);
    }
    else {
    	ret += "UNKNOWN";
    }
	ret += "|";
	return ret;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: tut1 path\n";
		return 1;
	}

	path p (argv[1]);

	if (exists(p)) {
		try {
			for(recursive_directory_iterator pPath(p);
					pPath != recursive_directory_iterator();
					++pPath) {
				if(is_regular_file(*pPath)) {
					PhotoFile thePhoto(*pPath);
					if(!thePhoto.id.empty()) {
						//cout << *pPath << "---";
			    		cout << thePhoto;
						cout << endl;
					}
				}
			}
			cout << "All good!" << endl;
		}
		catch (const filesystem_error& ex) {
			cerr << ex.what() << endl;
		}
	}
	else {
		cerr << "File or directory " << p << " not found" << endl;
	}
	return 0;
}
