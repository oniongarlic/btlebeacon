/*
 *  Eddystone Bluetooth LE Beacon setup 
 *
 *  Copyright (C) 2017 Kaj-Michael Lang
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

static const char* eddystone_url_prefix[] = {
    "http://www.",
    "https://www.",
    "http://",
    "https://",
    "urn:uuid:",
    NULL
};

static const char* eddystone_url_suffix[] = {
    ".com/",
    ".org/",
    ".edu/",
    ".net/",
    ".info/",
    ".biz/",
    ".gov/",
    ".com",
    ".org",
    ".edu",
    ".net",
    ".info",
    ".biz",
    ".gov",
    NULL
};

int setup_filter(int dev)
{
struct hci_filter flt;
hci_filter_clear(&flt);
hci_filter_set_ptype(HCI_EVENT_PKT, &flt);
hci_filter_all_events(&flt);
if (setsockopt(dev, SOL_HCI, HCI_FILTER, &flt, sizeof(flt)) < 0) {
	perror("HCI filter setup failed");
	return -1;
}

return 0;
}

int read_event(int dev)
{
unsigned char buf[HCI_MAX_EVENT_SIZE], *ptr = buf;
int len;
hci_event_hdr *hdr;

len = read(dev, buf, sizeof(buf));
if (len < 0) {
	perror("Read failed");
	return -1;
}

hdr = (void *)(buf + 1);
ptr = buf + (1 + HCI_EVENT_HDR_SIZE);
len -= (1 + HCI_EVENT_HDR_SIZE);

printf("> HCI Event: 0x%02x plen %d\n", hdr->evt, hdr->plen);

return 0;
}

#if 0
void lescan(int dev)
{
int err;
uint8_t filter_dup = 1;

err = hci_le_set_scan_enable(dev, 0x01, filter_dup, 1000);

printf("Scanning...\n");

err = hci_le_set_scan_enable(dev, 0x00, filter_dup, 1000);
}
#endif

int beacon(int dev, int tx, char *url)
{
char data[32]; // XXX: Or le_set_advertising_data_cp hci.h
int i;
uint8_t ogf=OGF_LE_CTL; // LE
uint16_t ocf=OCF_LE_SET_ADVERTISING_DATA; // LE_Set_Advertising_Data

memset(data, 0, 32);

data[0]=31; // Total length
data[1]=0x02; // Length, next
data[2]=0x01; // Flags
data[3]=0x06; // Flag data
data[4]=0x03; // Length, next
data[5]=0x03;
data[6]=0xAA; // UUID
data[7]=0xFE; // UUID
data[8]=31-8; // Beacon data length
data[9]=0x16; //
data[10]=0xAA; // UUID
data[11]=0xFE; // UUID
data[12]=0x10; // Eddystone URL Type
data[13]=tx; // TX power
data[14]=0x03; // URL Scheme (https://)

for (i=0;i<strlen(url) && i<19;i++) {
	data[i+15]=url[i];
}

if (i==18) {
	printf("URL too long\n");
	return -2;
}

data[8]=6+i;

printf("Setting beacon to advertise: %d %s\n", i, url);
if (hci_send_cmd(dev, ogf, ocf, 31, data) < 0) {
	perror("Send failed");
	return -1;
}

read_event(dev);

return 0;
}

int enable_advertise(int dev, uint8_t e)
{
uint8_t ogf=0x08; // LE
uint16_t ocf=OCF_LE_SET_ADVERTISE_ENABLE; // LE_Set_Advertising_Data
char data[1];

data[0]=e==0 ? 0 : 1;

if (hci_send_cmd(dev, ogf, ocf, 1, data) < 0) {
	perror("Send failed");
	return -1;
}

read_event(dev);

return 0;
}

int main(int argc, char *argv[])
{
int dev_id, dev;

if (argc!=2) {
	printf("URL argument not given\n");
	return 1;
}
printf("%d %s\n", argc, argv[1]);

dev_id = hci_get_route(NULL);
if (dev_id<0) {
	perror("hci_get_route");
	return 1;
}

dev = hci_open_dev(dev_id);
if (dev<0) {
	perror("hci_open_dev");
	return 1;
}

setup_filter(dev);

if (beacon(dev, 0xed, argv[1])==0)
	enable_advertise(dev, 1);


hci_close_dev(dev);
}
