#include <iostream>
#include <fstream>
#include <string>

using std::cout; 
using std::cin; 
using std::string; 

#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
using boost::interprocess::file_lock;

/* File lock usage Example 
https://www.boost.org/doc/libs/1_57_0/doc/html/interprocess/synchronization_mechanisms.html#interprocess.synchronization_mechanisms.file_lock
*/
int main(int argc, char *argv[]) {
	
		const char *FileName = "Platform_MemoryMapFile";
		std::size_t FileSize = 10000; /// use 2 as the mapped region uses FileSize / 2

	
		if( !exists(boost::filesystem::path( FileName ) ) ) {
			cout<<"File does not exists! let's create one!\n"; 

			//Create a file
			std::filebuf fbuf;
			fbuf.open(FileName, std::ios_base::in | std::ios_base::out
				  | std::ios_base::trunc | std::ios_base::binary);
			//Set the size
			fbuf.pubseekoff(FileSize - 1, std::ios_base::beg);
			fbuf.sputc(0);
		}
		
		
		//This throws if the file does not exist or it can't
		//open it with read-write access!
		try {
			boost::interprocess::file_lock flock(FileName);
			if(flock.try_lock())
				cout<<"Got the lock!\n";
			else
				cout<<"No lock available...!\n";
	
			// Generate a segv and break this code - file remove is not called!
			try {
				int* foo; 
				foo[2]=0; 
			} catch (...) {
				cout<<"Catched segv! \n";
			}
	
			cout<<"Please press a key to exit\n";
			string s; 
			cin >> s;  
		} 
		catch ( boost::interprocess::interprocess_exception& e )
		{
			std::cout << "INTERPROCESS EXCEPTION: " << e.what() << std::endl;
		}
		catch (...) 
		{
			std::cout << "UNKNOWN EXCEPTION: " << std::endl;
		}

	

		return 0; 
}

