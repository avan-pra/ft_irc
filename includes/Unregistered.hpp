#ifndef UNREGISTERED_HPP
# define UNREGISTERED_HPP

# include "./Connection.hpp"

class Unregistered : public Connection
{
	private:
		bool			_has_try_password;
		int				_password_state;
	public:
		Unregistered(/* args */)
		{
			_has_try_password = false;
			_password_state = -1;
		}
		virtual ~Unregistered()
		{
			
		}
		bool			get_pass_try() { return _has_try_password; }
		int				get_pass_state() { return _password_state; }

		void			set_pass_try(bool pass) { _has_try_password = pass; }
		void			set_pass_state(bool state) { _password_state = state; }
};

#endif