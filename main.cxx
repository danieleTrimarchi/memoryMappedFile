#include <iostream>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

using boost::interprocess::file_mapping;
using boost::interprocess::mapped_region;
using boost::interprocess::read_write;

/* Shared memory usage Example : usage of memory mapped files. 
https://www.boost.org/doc/libs/1_55_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.sharedmemory.windows_shared_memory
*/
int main(int argc, char *argv[]) {
	
		const char *FileName = "Platform_MemoryMapFile";
		std::size_t FileSize = 10000; /// use 2 as the mapped region uses FileSize / 2

								  // create a file
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

		file_mapping m_file(FileName,read_write);

		try
		{
			//Map the second half of the file
			mapped_region region(m_file,read_write);
			
			//Flush the whole region
			//region.flush();

			//Get the address of the region
			void * addr = region.get_address();
			std::cout << "Region address: " << addr << std::endl;

			//Get the size of the region
			std::size_t size = region.get_size();
			std::cout << "Region size: " << size << std::endl;

			//Write all the memory to 1
			std::memset(addr, 1, size);
		}
		catch ( boost::interprocess::interprocess_exception& e )
		{
			std::cout << "EXCEPTION: " << e.what() << std::endl;
		}

		return 0; 
}

