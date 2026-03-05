#pragma once

enum {
	FLOW_OUTGOING,
	FLOW_INCOMING,
};

enum {
	GENERIC,	   // must be first and is default group
	LOCALPLAYER,   // bytes for local player entity update
	OTHERPLAYERS,  // bytes for other players update
	ENTITIES,	   // all other entity bytes
	SOUNDS,		   // game sounds
	EVENTS,		   // event messages
	USERMESSAGES,  // user messages
	ENTMESSAGES,   // entity messages
	VOICE,		   // voice data
	STRINGTABLE,   // a stringtable update
	MOVE,		   // client move cmds
	STRINGCMD,	   // string command
	SIGNON,		   // various signondata
	TOTAL,		   // must be last and is not a real group
};

struct net_channel_info_t {
	virtual const char* get_name(void) const = 0;
	virtual const char* get_address(void) const = 0;

	virtual float		get_time(void) const = 0;
	virtual float		get_time_connected(void) const = 0;

	virtual int		    get_buffer_size(void) const = 0;
	virtual int		    get_data_rate(void) const = 0;

	virtual bool		is_loop_back(void) const = 0;
	virtual bool		is_timing_out(void) const = 0;
	virtual bool		is_play_back(void) const = 0;

	virtual float		get_latency(int flow) const = 0;
	virtual float		get_average_latency(int flow) const = 0;
	virtual float		get_average_loss(int flow) const = 0;
	virtual float		get_average_choke(int flow) const = 0;
	virtual float		get_average_data(int flow) const = 0;
	virtual float		get_average_packets(int flow) const = 0;

	virtual int		    get_total_data(int flow) const = 0;
	virtual int		    get_sequence_number(int flow) const = 0;

	virtual bool		is_valid_packet(int flow, int frame_number) const = 0;

	virtual float		get_packet_time(int flow, int frame_number) const = 0;

	virtual int		    get_packet_bytes(int flow, int frame_number, int group) const = 0;

	virtual bool		get_stream_progress(int flow, int* received, int* total) const = 0;

	virtual float		get_since_last_time_recieved(void) const = 0;
	virtual	float		get_command_interpolation_ammount(int flow, int frame_number) const = 0;

	virtual void		get_packet_response_latency(int flow, int frame_number, int* latency_msecs, int* choke) const = 0;
	virtual void		get_remote_framerate(float* frame_time, float* frame_time_std_deviation) const = 0;

	virtual float		get_timeout_seconds() const = 0;
};