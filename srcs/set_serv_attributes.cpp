#include "../includes/IRCserv.hpp"

void	set_serv_attributes(MyServ &serv)
{
	serv.set_hostname(serv.serv_config.hostname);
	serv.set_motd_path(serv.serv_config.motd_path);
	serv.set_admin_path(serv.serv_config.admin_path);
	serv.set_info_path(serv.serv_config.info_path);
	serv.set_listen_limit(serv.serv_config.listen_limit);
	serv.set_oper_name(serv.serv_config.oper_name);
	serv.set_oper_password(serv.serv_config.oper_password);
	serv.set_pass_oper(serv.serv_config.pass_oper);
	serv.set_password(serv.serv_config.password);
	serv.set_need_pass(serv.serv_config.pass_for_connection);
	serv.set_allow_ipv6(serv.serv_config.allow_ipv6);
	serv.set_client_limit(serv.serv_config.client_limit);
	serv.set_client_hostname(serv.serv_config.client_hostname);
	serv.set_ping(serv.serv_config.ping);
	serv.set_t_timeout(serv.serv_config.t_timeout);
	serv.set_timeout_register(serv.serv_config.timeout_register);
	serv.network = serv.serv_config.aNetworks;
	// serv.set_accept_tls(serv.serv_config.accept_tls);
	// serv.sslctx = serv.serv_config.sslctx;
}
