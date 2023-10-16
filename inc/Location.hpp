#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "AllHeaders.hpp"

class Location
{
		std::string					_path;
		std::string					_root;
		std::string					_index;
		std::string					_redirection;
		std::string					_proxy;
		bool						_listing;
		unsigned long				_client_body_size;
		std::vector<std::string>	_cgi_extensions;
		std::vector<std::string>	_cgi_paths;
		std::vector<bool>			_methods;
		std::map<std::string, std::string>	_cgimap;

	public:

		Location();
		Location(Location const &copy);
		~Location();

		Location	&operator=(Location const &assign);

		void	setPathL(std::string path);
		void	setRootL(std::string path);
		void	setIndexL(std::string path);
		void	setListingL(std::string option);
		void	setClientBodySizeL(std::string option);
		void	setClientBodySizeL2(unsigned long option);
		void	setRedirectionL(std::string option);
		void	setProxyL(std::string option);
		void	setMethodsL(std::vector<std::string> methods);
		void	setExtensionsL(std::vector<std::string> extensions);
		void	setCgiPathL(std::vector<std::string> paths);
		void	setCgiMap(std::map<std::string, std::string> map);

		std::string					getPathL() const;
		std::string					getRootL() const;
		std::string					getIndexL() const;
		std::string					getRedirectionL() const;
		std::string					getProxyL() const;
		bool						getListingL() const;
		unsigned long				getClientBodySizeL() const;
		const std::vector<std::string>	&getCgiExtensionsL() const;
		const std::vector<std::string>	&getCgiPathsL() const;
		std::vector<bool>			getMethodsL() const;
		std::map<std::string, std::string>	getCgiMap()const;
};

std::ostream	&operator<<(std::ostream &o, Location const &location);

#endif
