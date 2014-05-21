/*
 * PhotoFile.cpp
 *
 *  Created on: May 21, 2014
 *      Author: Mikhail Kyraha
 */

#include "PhotoFile.hpp"

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

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



