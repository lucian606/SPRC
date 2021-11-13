build:
	rpcgen -C rpc_db.x
	g++ -o rpc_dp_client -g rpc_db_clnt.cpp -lnsl -g -I/usr/include/tirpc -ltirpc
	g++ -o rpc_db_server -g rpc_db_svc.cpp -lnsl -g -I/usr/include/tirpc -ltirpc
