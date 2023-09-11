#ifndef LOCATION_HPP
# define LOCATION_HPP

class location
{
		std::string			_path;
		std::string			_root;
		std::string			_index;
		bool				_listing;
		unsigned long		_client_body_size;

	public:

		Location();
		Location(Location const &copy);
		~Location();

		Location	&operator=(Location const &assign);

		void	setPath(std::string path);
		void	setRoot(std::string path);
		void	setIndex(std::string path);
		void	setListing(std::string option);
};
#endif