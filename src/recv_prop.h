#pragma once

class d_variant;
class recv_table;
class recv_prop;
class c_recv_proxy_data;

using recv_var_proxy_fn = void(*)(const c_recv_proxy_data* data, void* struct_ptr, void* out_ptr);
using array_length_recv_proxy_fn = void(*)(void* struct_ptr, int object_id, int current_array_length);
using data_table_recv_var_proxy_fn = void(*)(const recv_prop* prop, void** out_ptr, void* data_ptr, int object_id);

class d_variant {
public:
	union {
		float m_float;
		long m_int;
		char* m_string;
		void* m_data;
		float m_vector[3];
		__int64 m_int64;
	};

	int m_type;
};

class c_recv_proxy_data {
public:
	const recv_prop* m_recv_prop;
	d_variant        m_value;
	
	int              m_element_index;
	int              m_object_id;
};

class recv_prop {
public:
	char*                        m_prop_name;
	
	int                          m_prop_type;
	int                          m_prop_flags;
	int                          m_buffer_size;
	int                          m_is_inside_of_array;
	
	const void*                  m_extra_data_ptr;
	recv_prop*                   m_array_prop;
	array_length_recv_proxy_fn   m_array_length_proxy;
	recv_var_proxy_fn            m_proxy_fn;
	data_table_recv_var_proxy_fn m_data_table_proxy_fn;
	
	recv_table*                  m_data_table;
	int                          m_offset;
	int                          m_element_stride;
	int                          m_elements_count;
	
	const char*                  m_parent_array_prop_name;
};

class recv_table {
public:
	recv_prop* m_props;
	int        m_props_count;

	void*      m_decoder_ptr;
	char*      m_table_name;
	
	bool       m_is_initialized;
	bool       m_is_in_main_list;
};