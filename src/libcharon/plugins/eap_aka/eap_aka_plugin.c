/*
 * Copyright (C) 2008-2009 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "eap_aka_plugin.h"

#include "eap_aka_peer.h"
#include "eap_aka_server.h"

#include <daemon.h>

METHOD(plugin_t, get_name, char*,
	eap_aka_plugin_t *this)
{
	return "eap-aka";
}

METHOD(plugin_t, destroy, void,
	eap_aka_plugin_t *this)
{
	charon->eap->remove_method(charon->eap,
							   (eap_constructor_t)eap_aka_server_create);
	charon->eap->remove_method(charon->eap,
							   (eap_constructor_t)eap_aka_peer_create);
	free(this);
}

/*
 * see header file
 */
plugin_t *eap_aka_plugin_create()
{
	eap_aka_plugin_t *this;

	INIT(this,
		.plugin = {
			.get_name = _get_name,
			.reload = (void*)return_false,
			.destroy = _destroy,
		},
	);

	charon->eap->add_method(charon->eap, EAP_AKA, 0, EAP_SERVER,
							(eap_constructor_t)eap_aka_server_create);
	charon->eap->add_method(charon->eap, EAP_AKA, 0, EAP_PEER,
							(eap_constructor_t)eap_aka_peer_create);

	return &this->plugin;
}

