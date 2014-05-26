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
#include <libexif/exif-data.h>

class PhotoFile {
public:
	PhotoFile(boost::filesystem::directory_entry& p);
	~PhotoFile();
	std::string getIdFromExif();
	long exif_fetch_long(ExifTag tag, ExifIfd ifd=EXIF_IFD_COUNT);
	std::string exif_fetch_string(ExifTag tag, ExifIfd ifd=EXIF_IFD_COUNT);

	std::string id;
	ExifData* mpExif;
	boost::filesystem::directory_entry mPath;
	unsigned long mFileSize;
	std::string mModel;
	std::string mTaken;
	std::string mTakenSub;
	int mX, mY;
	friend std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo);
};




#endif /* PHOTOFILE_HPP_ */
