#pragma once
#include <inttypes.h>
#include <string>
#include <mutex>
#include <vector>
#include <atomic>
#include <map>
#include <SJMMsgUtils/IMsgQ.h>

enum class RUN_MODE :uint8_t {
	TRADE_MODE = 0, RECORD_MODE, REPLAY_MODE
};

enum class BROKERS : uint8_t {
	IB = 0, CTP, GOOGLE, SINA, PAPER, BTCC, OKCOIN, ZERODHA, UPSTOX
};

const std::string SERIALIZATION_SEPARATOR = "|";

class  SJMTradeConfig
{
public:	
	static SJMTradeConfig& instance();	
	std::string getConfigDir() const;
	std::string getLogDir() const;
	std::string getDataDir() const;
	~SJMTradeConfig();
private:
	SJMTradeConfig();
	void readConfig();

private:
	std::map<std::string, std::string> _configMap;
	std::string _config_dir;
	std::string _log_dir;
	std::string _data_dir;
	static SJMTradeConfig* m_instance;
	static std::mutex m_instanceLock;
	RUN_MODE _mode = RUN_MODE::TRADE_MODE;
	BROKERS _broker = BROKERS::IB;
	MSGQ _msgq = MSGQ::NANOMSG;


public:

	/******************************************* Brokerage ***********************************************/
	// TODO: move to brokerage
	std::string ib_host = "127.0.0.1";
	uint64_t ib_port = 7496;
	std::atomic_int ib_client_id;

	std::string account = "DU448830";
	std::string filetoreplay = "";

	std::string ctp_broker_id = "";
	std::string ctp_user_id = "";
	std::string ctp_password = "";
	std::string ctp_auth_code = "";
	std::string ctp_user_prod_info = "";
	std::string ctp_data_address = "";
	std::string ctp_broker_address = "";

	std::vector<std::string> securities;
	/**************************************** End of Brokeragee ******************************************/

	/******************************************* Message Queue ***********************************************/
	std::string MKT_DATA_PUBSUB_PORT = "55555";				// market/tick data
	std::string BROKERAGE_PAIR_PORT = "55556";				// brokerage order, account, etc
	std::string BAR_AGGREGATOR_PUBSUB_PORT = "55557";		// bar from aggregation service
	std::string API_PORT = "55558";							// client port
	std::string API_ZMQ_DATA_PORT = "55559";					// client port

	std::string tick_msg = "k";
	std::string last_price_msg = "p";
	std::string last_size_msg = "z";
	std::string bar_msg = "b";
	std::string new_order_msg = "o";
	std::string cancel_order_msg = "c";
	std::string order_status_msg = "s";
	std::string fill_msg = "f";				// including partial fill
	std::string close_all_msg = "a";
	std::string position_msg = "n";
	std::string account_msg = "u";		// user
	std::string contract_msg = "r";
	std::string hist_msg = "h";
	std::string general_msg = "m";
	std::string test_msg = "e";		// echo

	/**************************************** End of Message Queue ******************************************/
};

