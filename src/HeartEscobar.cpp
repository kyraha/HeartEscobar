//============================================================================
// Name        : CygwinTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "PhotoFile.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: tut1 path\n";
		return 1;
	}

	boost::filesystem::path p (argv[1]);

	if (boost::filesystem::exists(p)) {
		try {
			for(boost::filesystem::recursive_directory_iterator pPath(p);
					pPath != boost::filesystem::recursive_directory_iterator();
					++pPath) {
				if(boost::filesystem::is_regular_file(*pPath)) {
					PhotoFile thePhoto(*pPath);
					if(!thePhoto.id.empty()) {
						//cout << *pPath << "---";
			    		cout << thePhoto;
						cout << endl;
					}
				}
			}
			cerr << "All good!" << endl;
		}
		catch (const boost::filesystem::filesystem_error& ex) {
			cerr << ex.what() << endl;
		}
	}
	else {
		cerr << "File or directory " << p << " not found" << endl;
	}
	return 0;
}
