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
#include <boost/lexical_cast.hpp>
#include <libexif/exif-data.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

class PhotoFile {
public:
	PhotoFile(directory_entry& p);
	string getIdFromExif();
	int getIntEntry(ExifEntry* entry, ExifData* exif_data) {
   		switch(entry->format) {
   		case EXIF_FORMAT_LONG:
   			return exif_get_long(entry->data,exif_data_get_byte_order (exif_data));
   			break;
   		case EXIF_FORMAT_SHORT:
   			return exif_get_short(entry->data,exif_data_get_byte_order (exif_data));
   			break;
   		default:
   			return 0;
   		}
	}
	string id;
	directory_entry mPath;
	string mModel;
	string mTaken;
	string mTakenSub;
	int mX, mY;
	friend std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo);
};

std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo)
{
	stream << "\"" << photo.id << "\" : " << photo.mPath;
	return stream;
}

PhotoFile::PhotoFile(directory_entry& p)
: mPath(p), mX(0), mY(0)
{
	id = getIdFromExif();
}

string PhotoFile::getIdFromExif()
{
	ExifData *exif_data = exif_data_new_from_file(mPath.path().c_str());
	if(!exif_data) return "";

	ExifEntry *entry;

	entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
    if (entry && entry->format==EXIF_FORMAT_ASCII) {
		mModel = (const char*)entry->data;
		trim(mModel);
		exif_entry_unref(entry);
    }

   	entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL);
   	if(entry && entry->format==EXIF_FORMAT_ASCII) {
		mTaken = (const char*)entry->data;
		trim(mTaken);
		exif_entry_unref(entry);
   	}

   	entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_EXIF], EXIF_TAG_SUB_SEC_TIME_ORIGINAL);
   	if(entry && entry->format==EXIF_FORMAT_ASCII) {
   		mTakenSub = (const char*)entry->data;
   		trim(mTakenSub);
   		exif_entry_unref(entry);
   	}

   	entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_EXIF], EXIF_TAG_PIXEL_X_DIMENSION);
   	if(entry) {
   		mX = getIntEntry(entry,exif_data);
		exif_entry_unref(entry);
   	}

   	entry = exif_content_get_entry(exif_data->ifd[EXIF_IFD_EXIF], EXIF_TAG_PIXEL_Y_DIMENSION);
   	if(entry) {
   		mY = getIntEntry(entry,exif_data);
   		exif_entry_unref(entry);
   	}

   	if(mModel.empty()) return "";

	return mModel+"|"+mTaken+"|"+(mTakenSub.empty()?"00":mTakenSub)+"|"
			+boost::lexical_cast<std::string>(mX)+"x"
			+boost::lexical_cast<std::string>(mY);
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
