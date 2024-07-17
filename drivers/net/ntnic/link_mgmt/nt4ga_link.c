/*
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2023 Napatech A/S
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "ntlog.h"
#include "ntnic_mod_reg.h"

#include "nt4ga_link.h"

/*
 * port: speed capabilities
 * This is actually an adapter capability mapped onto every port
 */
static uint32_t nt4ga_port_get_link_speed_capabilities(struct adapter_info_s *p, int port)
{
	(void)p;
	(void)port;
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	const uint32_t nt_link_speed_capa = p_link->speed_capa;
	return nt_link_speed_capa;
}

/*
 * port: link mode
 */
static void nt4ga_port_set_adm_state(struct adapter_info_s *p, int port, bool adm_state)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	p_link->port_action[port].port_disable = !adm_state;
}

static bool nt4ga_port_get_adm_state(struct adapter_info_s *p, int port)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	const bool adm_state = !p_link->port_action[port].port_disable;
	return adm_state;
}

/*
 * port: link status
 */
static void nt4ga_port_set_link_status(struct adapter_info_s *p, int port, bool link_status)
{
	/* Setting link state/status is (currently) the same as controlling the port adm state */
	nt4ga_port_set_adm_state(p, port, link_status);
}

static bool nt4ga_port_get_link_status(struct adapter_info_s *p, int port)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	bool status = p_link->link_state[port].link_up;
	return status;
}

/*
 * port: link speed
 */
static void nt4ga_port_set_link_speed(struct adapter_info_s *p, int port, nt_link_speed_t speed)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	p_link->port_action[port].port_speed = speed;
	p_link->link_info[port].link_speed = speed;
}

static nt_link_speed_t nt4ga_port_get_link_speed(struct adapter_info_s *p, int port)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	nt_link_speed_t speed = p_link->link_info[port].link_speed;
	return speed;
}

/*
 * port: link autoneg
 * Currently not fully supported by link code
 */
static void nt4ga_port_set_link_autoneg(struct adapter_info_s *p, int port, bool autoneg)
{
	(void)p;
	(void)port;
	(void)autoneg;
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	(void)p_link;
}

static bool nt4ga_port_get_link_autoneg(struct adapter_info_s *p, int port)
{
	(void)p;
	(void)port;
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	(void)p_link;
	return true;
}

/*
 * port: link duplex
 * Currently not fully supported by link code
 */
static void nt4ga_port_set_link_duplex(struct adapter_info_s *p, int port, nt_link_duplex_t duplex)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	p_link->port_action[port].port_duplex = duplex;
}

static nt_link_duplex_t nt4ga_port_get_link_duplex(struct adapter_info_s *p, int port)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	nt_link_duplex_t duplex = p_link->link_info[port].link_duplex;
	return duplex;
}

/*
 * port: loopback mode
 */
static void nt4ga_port_set_loopback_mode(struct adapter_info_s *p, int port, uint32_t mode)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	p_link->port_action[port].port_lpbk_mode = mode;
}

static uint32_t nt4ga_port_get_loopback_mode(struct adapter_info_s *p, int port)
{
	nt4ga_link_t *const p_link = &p->nt4ga_link;
	return p_link->port_action[port].port_lpbk_mode;
}


static const struct port_ops ops = {
	/*
	 * port:s link mode
	 */
	.set_adm_state = nt4ga_port_set_adm_state,
	.get_adm_state = nt4ga_port_get_adm_state,

	/*
	 * port:s link status
	 */
	.set_link_status = nt4ga_port_set_link_status,
	.get_link_status = nt4ga_port_get_link_status,

	/*
	 * port: link autoneg
	 */
	.set_link_autoneg = nt4ga_port_set_link_autoneg,
	.get_link_autoneg = nt4ga_port_get_link_autoneg,

	/*
	 * port: link speed
	 */
	.set_link_speed = nt4ga_port_set_link_speed,
	.get_link_speed = nt4ga_port_get_link_speed,

	/*
	 * port: link duplex
	 */
	.set_link_duplex = nt4ga_port_set_link_duplex,
	.get_link_duplex = nt4ga_port_get_link_duplex,

	/*
	 * port: loopback mode
	 */
	.set_loopback_mode = nt4ga_port_set_loopback_mode,
	.get_loopback_mode = nt4ga_port_get_loopback_mode,

	.get_link_speed_capabilities = nt4ga_port_get_link_speed_capabilities,
};

void port_init(void)
{
	register_port_ops(&ops);
}
