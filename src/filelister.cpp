#include <boost/filesystem.hpp>
#include <iostream>

void walk( const boost::filesystem::path & dir_path,         // in this directory,
           int maxdep ){
  if ( !(exists( dir_path )&&maxdep) ) return;
  boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
  for ( boost::filesystem::directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
  {
    if ( is_directory(itr->status()) )
    {
      std::cout<<itr->path().leaf();
      walk( itr->path(), maxdep-1);
    }
    else // see below
    {
      std::cout<<itr->path().leaf();
    }
  }
  return;
}


int main()
{
	boost::filesystem::path cur_dir(".");
	walk(cur_dir, 7);
	return 0;
}
