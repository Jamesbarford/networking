#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PREFIX_LEN 16

unsigned int get_ip_integral_equivalent(char *ip_address);
static int get_net_mask(int mm);

static unsigned int get_subnet_cardinality(char mm) {
	// max number of assignable IP addresses i.e: 24 -> 254,  20 -> 4094
	return (~get_net_mask(mm)) - 1;
}

// n_id = 192.168.0.0 ip = 192.168.0.13 -> 0
static int check_ip_subnet_membership(char *network_id, char mask, char *check_ip) {
	unsigned int int_network_id = get_ip_integral_equivalent(network_id);
	unsigned int ip_addr = get_ip_integral_equivalent(check_ip);
	unsigned int cardinality = get_subnet_cardinality(mask);
	int netmask = get_net_mask(mask);

	int same_network = (int_network_id & netmask) == (ip_addr & netmask);
	unsigned int machine_number = ~netmask & ip_addr;

	if (machine_number == 0 || machine_number >= cardinality)
		return -1;

	return same_network == 1 ? 0 : -1;
}

static int get_net_mask(int mm) {
	// assume mask is 10
	// 11111111.11 000000.00000000.00000000
	// what we want -> 0xFFC00000	
	return ~((1 << (32 - mm)) - 1);
}

static void to_human_readable(int addr, char *outbuf) {
	// there are 4 parts to the ip. each number 3 chars wide with 3 full stops;
	int p1 = (addr >> 24) & 0xFF;
	int p2 = (addr >> 16) & 0xFF;
	int p3 = (addr >> 8) & 0xFF;
	int p4 = addr & 0xFF;

	sprintf(outbuf, "%d.%d.%d.%d", p1, p2, p3, p4);
}

static unsigned int get_network_address(char *ip, int mask) {
	return get_ip_integral_equivalent(ip) & get_net_mask(mask);
} 

/**
 *	test case1:
 *	in: 192.168.2.10
 *	mask: 24
 *	out: 192.268.2.255
 *
 *	case2:
 *	in: 10.1.23.10
 *	mask: 20
 *	output: 10.1.31.255
 * */
static unsigned int get_broadcast_address(char *ip, char mask) {
	return get_network_address(ip, mask) | ~get_net_mask(mask);
}

/**
 * test case1:
 * in: 192.168.2.10
 * output: 3232236042
 *
 * case2:
 * in: 10.1.23.10
 * out: 167853594
 * */
unsigned int get_ip_integral_equivalent(char *ip_address) {
	int part = 1;
	int ip_int = 0;

	const int total = 4;
	char buf[total];
	memset(buf, '\0', total);
	char c;

	for (int i = 0, k = 0; (c = ip_address[i]) != '\0'; i++) {
		if (c == '.') {
			ip_int |= atoi(buf) << (8 * (total - part));
			memset(buf, '\0', total);
			part++;
			k = 0;
		} else {
			buf[k++] = c;
		}
	}

	ip_int |= atoi(buf) << (8 * (total - part));

	return ip_int;
}

int main(void) {
	char output_buffer[PREFIX_LEN];
	char network_buf[PREFIX_LEN + 3];
	char netmask_buf[PREFIX_LEN];
	memset(output_buffer, 0, PREFIX_LEN);
	memset(network_buf, 0, PREFIX_LEN);
	memset(netmask_buf, 0, PREFIX_LEN);
	
	char *ip = "192.168.2.10";
	char mask = 20;

	unsigned int network_addr = get_network_address(ip, mask);
	unsigned int broadcast_addr = get_broadcast_address(ip, mask);
	int netmask =  get_net_mask(mask);
	to_human_readable(broadcast_addr, output_buffer);
	to_human_readable(network_addr, network_buf);
	to_human_readable(netmask, netmask_buf);

	printf("Network mask is:   %s\n", netmask_buf);
	printf("Network address:   %s\n", network_buf);
	printf("Broadcast address: %s\n", output_buffer);
	int max_ips = get_subnet_cardinality(mask);

	printf("Max ips: %d \n", max_ips);
	unsigned int int_ip = get_ip_integral_equivalent(ip);
	printf("Integer equivelent for: %s is %u\n", ip, int_ip);
	
	int in_network = check_ip_subnet_membership("192.168.0.0", 20, "192.168.0.13");
	printf("In network: %s\n", in_network == -1 ? "false" : "true");
}
