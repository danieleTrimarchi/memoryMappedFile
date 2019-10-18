#include <iostream>
#include <fstream>
#include <string>

using std::cout; 
using std::cin; 
using std::string; 

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>

using boost::interprocess::file_mapping;
using boost::interprocess::mapped_region;
using boost::interprocess::read_write;
using boost::interprocess::read_only;

/* Shared memory usage Example : usage of memory mapped files. 
https://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.sharedmemory.windows_shared_memory
*/
int main(int argc, char *argv[]) {
	
		const char *FileName = "Platform_MemoryMapFile";
		std::size_t FileSize = 10000; /// use 2 as the mapped region uses FileSize / 2

	
		if( exists(boost::filesystem::path( FileName ) ) ) {
			cout<<"File exists! The shared memory was already allocated\n"; 
			return 1; 
		}
		
		{   //Create a file
			file_mapping::remove(FileName);
			std::filebuf fbuf;
			fbuf.open(FileName, std::ios_base::in | std::ios_base::out
				  | std::ios_base::trunc | std::ios_base::binary);
			//Set the size
			fbuf.pubseekoff(FileSize - 1, std::ios_base::beg);
			fbuf.sputc(0);
		}
		
		//Remove on exit
		struct file_remove
		{
			file_remove(const char *FileName)
				: FileName_(FileName) {}
			~file_remove() { file_mapping::remove(FileName_); }
			const char *FileName_;
		} remover(FileName);

		file_mapping m_file2(FileName,read_write);

		try
		{
			//Map the second half of the file
			mapped_region region(m_file2,read_write);
			
			//Get the address of the region
			void * addr = region.get_address();
			std::cout << "Region address: " << addr << std::endl;

			//Get the size of the region
			std::size_t size = region.get_size();
			std::cout << "Region size: " << size << std::endl;

		}
		catch ( boost::interprocess::interprocess_exception& e )
		{
			std::cout << "EXCEPTION: " << e.what() << std::endl;
		}
		
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

		return 0; 
}

