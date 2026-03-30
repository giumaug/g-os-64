//#define PROFILE

#ifndef SYSTEM_H                
#define SYSTEM_H

#include "general.h"
#include "data_types/dllist.h"
#include "data_types/primitive_types.h"
#include "data_types/hashtable.h"
#include "synchro_types/spin_lock.h"
#include "synchro_types/semaphore.h"
#include "scheduler/process.h"
#include "console/console.h"
#include "scheduler/scheduler.h"
#include "memory_manager/buddy.h"
#include "network/network.h"
#include "lib/lib.h"
#include "device.h"
#include "timer.h"
#include "drivers/lapic/lapic.h"

#define DEVICE_IDLE 0
#define DEVICE_BUSY 1
#define POOLING_MODE 2

struct s_ext2;
struct s_scheduler_desc;
struct s_buddy_desc;
struct s_device_desc;
struct s_network_desc;

typedef struct s_tcp_fin_status
{
	int status;
	struct s_tcp_conn_desc* conn;
}
t_tcp_fin_status;

typedef struct s_system
{
	unsigned int time;
	struct t_process_info* process_info;
	t_console_desc* active_console_desc;
	struct s_buddy_desc* buddy_desc;
	u64* master_page_pml4;
	struct s_scheduler_desc* scheduler_desc[NUM_CPU];
	struct s_ext2* root_fs;
	struct s_ext2* scnd_fs;
	struct s_device_desc* device_desc;
	unsigned int int_path_count;
	struct s_network_desc* network_desc;
	unsigned int panic;
	t_llist* timer_list;
	u8 force_scheduling;
	u8 flush_network;
	t_hashtable* mount_map;
	//trick to use second device
	u8 device_id;
 }
t_system;

extern t_system system;

#endif
