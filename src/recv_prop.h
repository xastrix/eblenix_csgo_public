#pragma once

class c_dvariant;
class c_recv_table;
class c_recv_prop;
class c_recv_proxy_data;

using recv_var_proxy_fn = void(*)(const c_recv_proxy_data* data, void* struct_ptr, void* out_ptr);
using array_length_recv_proxy_fn = void(*)(void* struct_ptr, int object_id, int current_array_length);
using data_table_recv_var_proxy_fn = void(*)(const c_recv_prop* prop, void** out_ptr, void* data_ptr, int object_id);

class c_dvariant {
public:
	union {
		float _float;
		long _int;
		char* _string;
		void* _data;
		float _vector[3];
		__int64 int64;
	};

	int type;
};

class c_recv_proxy_data {
public:
	const c_recv_prop*           recv_prop;
	c_dvariant                   value;
	
	int                          element_index;
	int                          object_id;
};

class c_recv_prop {
public:
	char*                        prop_name;
	
	int                          prop_type;
	int                          prop_flags;
	int                          buffer_size;
	int                          is_inside_of_array;
	
	const void*                  extra_data_ptr;
	c_recv_prop*                 array_prop;
	array_length_recv_proxy_fn   array_length_proxy;
	recv_var_proxy_fn            proxy_fn;
	data_table_recv_var_proxy_fn data_table_proxy_fn;
	
	c_recv_table*                data_table;
	int                          offset;
	int                          element_stride;
	int                          elements_count;
	
	const char*                  parent_array_prop_name;
};

class c_recv_table {
public:
	c_recv_prop*                 props;
	int                          props_count;

	void*                        decoder_ptr;
	char*                        table_name;
	
	bool                         is_initialized;
	bool                         is_in_main_list;
};