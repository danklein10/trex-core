/*
  Ido Barnea
  Cisco Systems, Inc.
*/

/*
  Copyright (c) 2016-2016 Cisco Systems, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef __PRE_TEST_H__
#define __PRE_TEST_H__

#include <iostream>
#include <common/Network/Packet/Arp.h>
#include "bp_sim.h"
#include "trex_defs.h"

class CPreTestStats {
 public:
    uint32_t m_rx_arp; // how many ARP packets we received
    uint32_t m_tx_arp; // how many ARP packets we sent

 public:
    void clear() {
        m_rx_arp = 0;
        m_tx_arp = 0;
    }
};

class CPretestPortInfo {
    friend class CPretest;
    
 private:
    enum CPretestPortInfoStates {
        INIT_NEEDED,
        RESOLVE_NEEDED,
        RESOLVE_DONE,
        RESOLVE_NOT_NEEDED,
    };

    CPretestPortInfo() {
        m_state = INIT_NEEDED;
        m_is_loopback = false;
        m_stats.clear();
    }
    void dump(FILE *fd);
    uint8_t *create_arp_req(uint16_t &pkt_size, uint8_t port, bool is_grat);
    void set_params(CPerPortIPCfg port_cfg, const uint8_t *src_mac, bool resolve_needed);
    void set_dst_mac(const uint8_t *dst_mac);
    
 private:
    uint32_t m_ip;
    uint32_t m_def_gw;
    uint16_t m_vlan;
    uint8_t m_src_mac[6];
    uint8_t m_dst_mac[6];
    enum CPretestPortInfoStates m_state;
    bool m_is_loopback;
    CPreTestStats m_stats;
};


class CPretest {
 public:
    CPretest(uint16_t max_ports) {
        m_max_ports = max_ports;
    }
    bool get_mac(uint16_t port, uint32_t ip, uint8_t *mac);
    CPreTestStats get_stats(uint16_t port_id);
    bool is_loopback(uint16_t port);
    void set_port_params(uint16_t port_id, const CPerPortIPCfg &port_cfg, const uint8_t *src_mac, bool resolve_needed);
    bool resolve_all();
    void send_arp_req(uint16_t port, bool is_grat);
    void send_grat_arp_all();
    bool is_arp(const uint8_t *p, uint16_t pkt_size, ArpHdr *&arp);
    void dump(FILE *fd);
    void test();
    
 private:
    int handle_rx(int port, int queue_id);

 private:
    CPretestPortInfo m_port_info[TREX_MAX_PORTS];
    uint16_t m_max_ports;
};

#endif
