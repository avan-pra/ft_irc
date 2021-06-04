#ifndef UNREGISTERED_HPP
# define UNREGISTERED_HPP

# include "./Connection.hpp"

class Unregistered : public Connection
{
	private:
		bool			_has_try_password;
		int				_password_state;

		bool			_all_arg_set;
		std::string		_version;
		std::string		_implementation_name;
		std::string		_implementation_option;
		std::string		_link_option;
		std::string		_pass_submited;
	public:
		Unregistered(/* args */)
		{
			_has_try_password = false;
			_password_state = -1;
			_all_arg_set = false;
			_type = UNKNOWN_TYPE;
		}
		virtual ~Unregistered()
		{
			
		}
		bool			get_pass_try() { return _has_try_password; }
		int				get_pass_state() { return _password_state; }
		int				get_arg_set() { return _all_arg_set; }
		std::string		get_version() { return _version; }
		std::string		get_implementation_name() { return _implementation_name; }
		std::string		get_implementation_option() { return _implementation_option; }
		std::string		get_link_option() { return _link_option; }
		std::string		get_pass_submited() { return _pass_submited; };

		void			set_pass_try(bool pass) { _has_try_password = pass; }
		void			set_pass_state(int state) { _password_state = state; }
		void			set_arg_set(bool state) { _all_arg_set = state; }
		void			set_version(std::string version) { _version = version; }
		void			set_implementation_name(std::string implementation_name) { _implementation_name = implementation_name; }
		void			set_implementation_option(std::string implementation_option) { _implementation_option = implementation_option; }
		void			set_link_option(std::string link_option) { _link_option = link_option; }
		void			set_pass_submited(std::string pass_submited) { _pass_submited = pass_submited; }
};

#endif
