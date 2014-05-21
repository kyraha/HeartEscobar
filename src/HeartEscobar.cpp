//============================================================================
// Name        : CygwinTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "PhotoFile.hpp"

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
