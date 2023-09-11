#ifndef LOCATION_HPP
# define LOCATION_HPP

class location
{
		std::string	_path;
		std::string	_root;
		std::string	_index;
		bool		_listing;

	public:

		Location();
		Location(Location const &copy);
		~Location();

		Location	&operator=(Location const &assign);
};
#endif