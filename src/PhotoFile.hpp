/*
 * PhotoFile.hpp
 *
 *  Created on: May 21, 2014
 *      Author: Mikhail Kyraha
 */

#ifndef PHOTOFILE_HPP_
#define PHOTOFILE_HPP_

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




#endif /* PHOTOFILE_HPP_ */
