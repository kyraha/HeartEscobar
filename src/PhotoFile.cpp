/*
 * PhotoFile.cpp
 *
 *  Created on: May 21, 2014
 *      Author: Mikhail Kyraha
 */

#include "PhotoFile.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

std::ostream& operator<<(std::ostream& stream, const PhotoFile& photo)
{
	stream << "\"" << photo.id << "\" : " << photo.mPath;
	return stream;
}

PhotoFile::PhotoFile(boost::filesystem::directory_entry& p)
: mPath(p), mX(0), mY(0)
{
	mpExif = exif_data_new_from_file(p.path().c_str());
	if(mpExif) id = getIdFromExif();
}

PhotoFile::~PhotoFile() {
	if(mpExif) exif_data_unref(mpExif);
}


long PhotoFile::exif_fetch_long(ExifTag tag, ExifIfd ifd) {
	int ifd_end = ifd;
	int ifd_i = 0;
	if(ifd < EXIF_IFD_COUNT) { ifd_i = ifd; ifd_end = ifd+1; }
	for(; ifd_i < ifd_end; ++ifd_i) {
		ExifEntry* entry = exif_content_get_entry(mpExif->ifd[ifd_i], tag);
		/*! Return the #ExifEntry in this IFD corresponding to the given tag.
		 * This is a pointer into a member of the #ExifContent array and must NOT be
		 * freed or unrefed by the caller. */
		if(!entry) continue;

		switch(entry->format) {
		case EXIF_FORMAT_LONG:
			return exif_get_long(entry->data, exif_data_get_byte_order(mpExif));
		case EXIF_FORMAT_SHORT:
			return exif_get_short(entry->data, exif_data_get_byte_order(mpExif));
		default:
			continue;
		}
	}
	return 0;
}

std::string PhotoFile::exif_fetch_string(ExifTag tag, ExifIfd ifd) {
	std::string ret;
	int ifd_end = ifd;
	int ifd_i = 0;
	if(ifd<EXIF_IFD_COUNT) { ifd_i = ifd; ifd_end = ifd+1; }
	for(; ifd_i < ifd_end; ++ifd_i) {
		ExifEntry* entry = exif_content_get_entry(mpExif->ifd[ifd_i], tag);
		if(!entry) continue;

		switch(entry->format) {
		case EXIF_FORMAT_ASCII:
			ret = (const char*)entry->data;
			boost::algorithm::trim(ret);
			return ret;
		default:
			continue;
		}
	}
	return "";
}

string PhotoFile::getIdFromExif()
{
	mModel = exif_fetch_string(EXIF_TAG_MODEL);
	mTaken = exif_fetch_string(EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_IFD_EXIF);
	mTakenSub = exif_fetch_string(EXIF_TAG_SUB_SEC_TIME_ORIGINAL, EXIF_IFD_EXIF);
	mX = exif_fetch_long(EXIF_TAG_PIXEL_X_DIMENSION,EXIF_IFD_EXIF);
	mY = exif_fetch_long(EXIF_TAG_PIXEL_Y_DIMENSION,EXIF_IFD_EXIF);

   	if(mModel.empty()) return "";

	return mModel+"|"+mTaken+"|"+(mTakenSub.empty()?"00":mTakenSub)+"|"
			+boost::lexical_cast<std::string>(mX)+"x"
			+boost::lexical_cast<std::string>(mY);
}



